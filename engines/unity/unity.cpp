#include "unity.h"
#include "graphics.h"
#include "sound.h"
#include "sprite.h"
#include "object.h"
#include "trigger.h"

#include "common/fs.h"
#include "common/config-manager.h"
#include "engines/util.h"
#include "common/events.h"
#include "common/system.h"
#include "common/unzip.h"
#include "common/file.h"
#include "common/archive.h"
#include "graphics/cursorman.h"
#include "common/EventRecorder.h"

namespace Unity {

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst), data(this) {
	in_dialog = false;
	dialog_choosing = false;
	icon = NULL;
	current_conversation = NULL;
	next_situation = 0xffffffff;
	next_state = 0xffffffff;
	beam_world = 0;
	mode = mode_Look;
}

UnityEngine::~UnityEngine() {
	delete _snd;
	delete _gfx;
	delete data.data;
	delete icon;
}

Common::Error UnityEngine::init() {
	g_eventRec.registerRandomSource(_rnd, "unity");

	_gfx = new Graphics(this);
	_snd = new Sound(this);

	data.data = Common::makeZipArchive("STTNG.ZIP");
	if (!data.data) {
		error("couldn't open data file");
	}
	/*Common::ArchiveMemberList list;
	data->listMembers(list);
	for (Common::ArchiveMemberList::const_iterator file = list.begin(); file != list.end(); ++file) {
		Common::String filename = (*file)->getName();
		filename.toLowercase();
		if (filename.hasSuffix(".spr") || filename.hasSuffix(".spt")) {
			Common::SeekableReadStream *ourstr = (*file)->createReadStream();
			printf("trying '%s'\n", filename.c_str());
			Sprite spr(ourstr);
		}
	}*/
	SearchMan.add("sttngzip", data.data);

	data.instdata = Common::makeZipArchive("STTNGINS.ZIP");
	if (!data.instdata) {
		error("couldn't open data file");
	}
	SearchMan.add("sttnginszip", data.instdata);

	return Common::kNoError;
}

void UnityEngine::openLocation(unsigned int world, unsigned int screen) {
	Common::String filename = Common::String::printf("sl%03d.scr", world);
	Common::SeekableReadStream *locstream = data.openFile(filename);

	uint16 num_entries = locstream->readUint16LE();
	Common::HashMap<uint32, uint32> offsets;
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = locstream->readUint32LE();
		uint32 offset = locstream->readUint32LE();
		offsets[id] = offset;
	}

	if (screen > num_entries) {
		error("no such screen %d in world %d (only %d screens)", screen, world, num_entries);
	}

	locstream->seek(offsets[screen]);

	// XXX: null-terminated in file?
	byte length = locstream->readByte();
	char *background = (char *)alloca(length + 1);
	locstream->read(background, length);
	background[length] = 0;

	_gfx->setBackgroundImage(background);

	// XXX: null-terminated in file?
	length = locstream->readByte();
	char *polygons = (char *)alloca(length + 1);
	locstream->read(polygons, length);
	polygons[length] = 0;

	data.loadScreenPolys(polygons);

	data.current_screen.entrypoints.clear();
	byte num_entrances = locstream->readByte();
	for (unsigned int e = 0; e < num_entrances; e++) {
		Common::Array<Common::Point> entrypoints;
		// XXX: read this properly
		uint16 unknown = locstream->readUint16BE(); // 0001, 0101, 0201, ...
		(void)unknown;
		entrypoints.resize(4);
		for (unsigned int i = 0; i < 4; i++) {
			entrypoints[i].x = locstream->readUint16LE();
			entrypoints[i].y = locstream->readUint16LE();
		}
		data.current_screen.entrypoints.push_back(entrypoints);
	}

	delete locstream;

	filename = Common::String::printf("w%02x%02xobj.bst", world, screen);
	locstream = data.openFile(filename);

	// TODO: is there data we need here in w_XXstrt.bst?
	// (seems to be only trigger activations)
	while (true) {
		uint16 counter = locstream->readUint16LE();
		if (locstream->eos()) break;

		objectID id = readObjectID(locstream);
		assert(id.id == counter);
		assert(id.screen == screen);
		assert(id.world == world);

		char _name[30], _desc[260];
		locstream->read(_name, 30);
		locstream->read(_desc, 260);
		//printf("reading obj '%s' (%s)\n", _name, _desc);

		Object *obj = data.getObject(id);
		obj->loadSprite();
		data.current_screen.objects.push_back(obj);
	}

	delete locstream;
}

#define DIRECTION(x, y, x1, y1, x2, y2) (((int)y - y1)*(x2 - x1) - ((int)x - x1)*(y2 - y1))

bool ScreenPolygon::insideTriangle(unsigned int x, unsigned int y, unsigned int &triangle) {
	for (unsigned int i = 0; i < triangles.size(); i++) {
		int direction = DIRECTION(x, y, triangles[i].points[0].x, triangles[i].points[0].y, triangles[i].points[1].x, triangles[i].points[1].y);
		bool neg = direction < 0;
		direction = DIRECTION(x, y, triangles[i].points[1].x, triangles[i].points[1].y, triangles[i].points[2].x, triangles[i].points[2].y);
		if ((direction < 0) != neg) continue;
		direction = DIRECTION(x, y, triangles[i].points[2].x, triangles[i].points[2].y, triangles[i].points[0].x, triangles[i].points[0].y);
		if ((direction < 0) != neg) continue;
		triangle = i;
		return true;
	}
	return false;
}

struct DrawOrderComparison {
	bool operator() (const Object *a, const Object *b) {
		int ay = a->y, by = b->y;
		if (a->z_adjust != 0xffff) ay += a->z_adjust;
		if (b->z_adjust != 0xffff) by += b->z_adjust;
		return ay < by;
	}
};

Object *UnityEngine::objectAt(unsigned int x, unsigned int y) {
	Common::Array<Object *> &objects = data.current_screen.objects;

	for (unsigned int i = 0; i < objects.size(); i++) {
		if (!(objects[i]->flags & OBJFLAG_ACTIVE)) continue;
		if (objects[i]->flags & OBJFLAG_INVENTORY) continue;
		if (objects[i]->width == (unsigned int)~0) continue;

		// TODO: should we be doing this like this, or keeping track of original coords, or what?
		if (objects[i]->x - objects[i]->width/2 > x) continue;
		if (objects[i]->x + objects[i]->width/2 < x) continue;
		if (objects[i]->y - objects[i]->height > y) continue;
		if (objects[i]->y < y) continue;
		return objects[i];
	}

	return 0;
}

void UnityEngine::startBridge() {
	data.current_screen.objects.clear();
	data.current_screen.polygons.clear();
	data.current_screen.world = 0x5f;
	data.current_screen.screen = 0xff;

	const char *bridge_sprites[5] = {
		"brdgldor.spr", // Left Door (conference room)
		"brdgdoor.spr", // Door
		"brdgtitl.spr", // Episode Title
		"advice.spr", // advice button in UI :(
		0 };

	for (unsigned int i = 0; bridge_sprites[i] != 0; i++) {
		Object *obj = new Object(this);
		obj->x = obj->y = 0;
		obj->z_adjust = 0xffff;
		obj->flags = OBJFLAG_ACTIVE;
		obj->objwalktype = OBJWALKTYPE_NORMAL;
		obj->sprite = new SpritePlayer(new Sprite(data.openFile(bridge_sprites[i])), obj, this);
		obj->sprite->startAnim(0);
		data.current_screen.objects.push_back(obj);
	}

	for (unsigned int i = 0; i < data.bridge_objects.size(); i++) {
		Object *obj = new Object(this);
		// TODO: correct?
		obj->x = data.bridge_objects[i].x;
		obj->y = data.bridge_objects[i].y;
		obj->z_adjust = 0xffff;
		obj->flags = OBJFLAG_ACTIVE;
		obj->objwalktype = OBJWALKTYPE_NORMAL;
		obj->sprite = new SpritePlayer(new Sprite(data.openFile(data.bridge_objects[i].filename)), obj, this);
		obj->sprite->startAnim(0);
		/*printf("%s: %d, %d\n", data.bridge_objects[i].filename.c_str(),
			data.bridge_objects[i].unknown1,
			data.bridge_objects[i].unknown2);*/
		data.current_screen.objects.push_back(obj);
	}

	_gfx->setBackgroundImage("bridge.rm");
	on_bridge = true;
	handleBridgeMouseMove(0, 0);
}

void UnityEngine::startAwayTeam(unsigned int world, unsigned int screen) {
	data.current_screen.objects.clear();
	data.current_screen.world = world;
	data.current_screen.screen = screen;

	// beam in an away team
	for (unsigned int i = 0; i < 4; i++) {
		objectID objid(i, 0, 0);
		Object *obj = data.getObject(objid);
		obj->loadSprite();
		obj->sprite->startAnim(26);
		data.current_screen.objects.push_back(obj);
	}

	openLocation(world, screen);
	for (unsigned int i = 0; i < 4; i++) {
		data.current_screen.objects[i]->x = data.current_screen.entrypoints[0][i].x;
		data.current_screen.objects[i]->y = data.current_screen.entrypoints[0][i].y;
	}

	on_bridge = false;
	handleAwayTeamMouseMove(0, 0);
}

void UnityEngine::startupScreen() {
	// play two animations (both logo anim followed by text) from one file
	SpritePlayer *p = new SpritePlayer(new Sprite(data.openFile("legaleze.spr")), 0, this);
	unsigned int anim = 0;
	p->startAnim(anim);
	uint32 waiting = 0;
	while (true) {
		Common::Event event;
		bool escape = false;
		while (_eventMan->pollEvent(event)) {
			if (event.type == Common::EVENT_KEYUP) {
				if (event.kbd.keycode == Common::KEYCODE_ESCAPE) {
					// note that the original game didn't allow escape
					// until the first animation was done
					escape = true;
				}
			}
		}
		if (escape) break;

		p->update();
		if (!p->playing()) {
			if (!waiting) {
				// arbitary 4 second wait, not sure how long the original waits
				waiting = g_system->getMillis() + 4000;
			} else if (g_system->getMillis() < waiting) {
				// delay for 1/10th sec while waiting
				g_system->delayMillis(100);
			} else if (anim == 0) {
				waiting = 0;
				// play second animation
				anim = 1;
				p->startAnim(anim);
			} else {
				// all done!
				break;
			}
		}
		_gfx->drawSprite(p, 0, 0, 256);
		_system->updateScreen();
	}
	_mixer->stopAll();
	delete p;
}

void UnityEngine::processTriggers() {
	for (unsigned int i = 0; i < data.triggers.size(); i++) {
		if (data.triggers[i]->tick(this)) {
			Object *target = data.getObject(data.triggers[i]->target);
			debug(1, "running trigger %x (target %s)", data.triggers[i]->id, target->identify().c_str());
			performAction(ACTION_USE, target);
			break;
		}
	}
}

void UnityEngine::processTimers() {
	// TODO: stupid hack to only run this so often
	// TODO: is it correct to do this 0x123 thing here?
	static uint32 last_time = 0;
	if (last_time && ((last_time + 0x123) > g_system->getMillis()))
		return;
	last_time = g_system->getMillis();

	Common::Array<Object *> &objects = data.current_screen.objects;
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (!(objects[i]->flags & OBJFLAG_ACTIVE)) continue;
		if (objects[i]->timer == 0xffff) continue;
		if (objects[i]->timer == 0) continue;

		objects[i]->timer--;
		if (objects[i]->timer == 0) {
			debug(1, "running timer on %s", objects[i]->identify().c_str());
			performAction(ACTION_TIMER, objects[i]);
		}
	}
}

void UnityEngine::setSpeaker(objectID s) {
	speaker = s;

	if (icon) {
		delete icon;
		icon = NULL;
	}

	Common::String icon_sprite = data.getIconSprite(speaker);
	if (!icon_sprite.size()) {
		warning("couldn't find icon sprite for %02x%02x%02x", speaker.world,
			speaker.screen, speaker.id);
		return;
	}

	icon = new SpritePlayer(new Sprite(data.openFile(icon_sprite)), NULL, this);
	if (icon->numAnims() < 3) {
		icon->startAnim(0); // static
	} else {
		icon->startAnim(2); // speaking
	}
}

void UnityEngine::handleLook(Object *obj) {
	if (obj->flags & OBJFLAG_LOOK) {
		obj->look_entries.execute(this);
	}

	// TODO: this is very wrong, of course :)

	// use only Picard's entry for now
	unsigned int i = 0;
	while (i < obj->descriptions.size() &&
			obj->descriptions[i].entry_id != 0) i++;
	if (i == obj->descriptions.size()) {
		// TODO: bad place?
		if (next_situation == 0xffffffff) {
			// TODO: hard-coded :( @95,34,xy...
			current_conversation = data.getConversation(0x5f, 34);
			next_situation = 0x1; // default to failed look
		}
		return;
	}

	Description &desc = obj->descriptions[i];
	dialog_text = desc.text;
	setSpeaker(objectID(0, 0, 0));

	// 'l' for look :)
	Common::String file = voiceFileFor(desc.voice_group, desc.voice_subgroup, objectID(desc.entry_id, 0, 0), desc.voice_id, 'l');
	_snd->playSpeech(file);

	runDialog();
}

Common::String UnityEngine::voiceFileFor(byte voice_group, byte voice_subgroup, objectID speaker, byte voice_id, char type) {
	if (speaker.world != 0 || speaker.screen != 0 || speaker.id > 9) {
		speaker.id = 0xff;
	}

	if (voice_group == 0xfe) {
		return Common::String::printf("%02x%02x%02x%02x.vac",
			voice_group, speaker.id,
			voice_subgroup, voice_id);
	} else if (type) {
		return Common::String::printf("%02x%c%1x%02x%02x.vac",
			voice_group, type, voice_subgroup,
			speaker.id, voice_id);
	} else {
		return Common::String::printf("%02x%02x%02x%02x.vac",
			voice_group, voice_subgroup,
			speaker.id, voice_id);
	}
}

void UnityEngine::handleUse(Object *obj) {
	if (obj->flags & OBJFLAG_GET) {
		performAction(ACTION_GET, obj);
	} else if (obj->flags & OBJFLAG_USE) {
		performAction(ACTION_USE, obj);
	} else {
		if (next_situation == 0xffffffff) {
			// TODO: hard-coded :( @95,34,xy...
			current_conversation = data.getConversation(0x5f, 34);
			next_situation = 0x2; // failed use?
		}
	}
}

void UnityEngine::handleTalk(Object *obj) {
	if (obj->flags & OBJFLAG_TALK) {
		obj->runHail(obj->talk_string);
	} else {
		if (next_situation == 0xffffffff) {
			// TODO: hard-coded :( @95,34,xy...
			current_conversation = data.getConversation(0x5f, 34);
			next_situation = 0x3; // failed talk?
		}
	}
}

void UnityEngine::handleWalk(Object *obj) {
	performAction(ACTION_WALK, obj);
}

// TODO
unsigned int anim = 26;

void UnityEngine::DebugNextScreen() {
	unsigned int &curr_loc = data.current_screen.world;
	unsigned int &curr_screen = data.current_screen.screen;

	// start in first screen
	if (curr_loc == 0x5f) { curr_loc = 2; curr_screen = 0; }

	curr_screen++;
	switch (curr_loc) {
	case 2: if (curr_screen == 2) curr_screen++;
		else if (curr_screen == 14) curr_screen++;
		else if (curr_screen == 21) { curr_loc++; curr_screen = 1; }
		break;
	case 3:	if (curr_screen == 11) { curr_loc++; curr_screen = 1; }
		break;
	case 4: if (curr_screen == 13) { curr_loc++; curr_screen = 1; }
		break;
	case 5: if (curr_screen == 11) curr_screen++;
		else if (curr_screen == 14) { curr_loc++; curr_screen = 1; }
		break;
	case 6: if (curr_screen == 16) { curr_loc++; curr_screen = 1; }
		break;
	case 7: if (curr_screen == 5) { curr_loc = 2; curr_screen = 1; }
		break;
	default:error("huh?");
	}
	printf("moving to %d/%d\n", curr_loc, curr_screen);
	startAwayTeam(curr_loc, curr_screen);
}

void UnityEngine::checkEvents() {
	Common::Array<Object *> &objects = data.current_screen.objects;

	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		switch (event.type) {
			case Common::EVENT_QUIT:
				_system->quit();
				break;

			case Common::EVENT_KEYUP:
				switch (event.kbd.keycode) {
				case Common::KEYCODE_n:
					if (!on_bridge) {
						printf("trying anim %d\n", anim);
						anim++;
						anim %= objects[0]->sprite->numAnims();
						for (unsigned int i = 0; i < 4; i++)
							objects[i]->sprite->startAnim(anim);
					}
					break;
				case Common::KEYCODE_SPACE:
					DebugNextScreen();
					break;
				default:
					break;
				}
				break;

			case Common::EVENT_RBUTTONUP:
				if (on_bridge) break;

				// cycle through away team modes
				switch (mode) {
				case mode_Look:
					mode = mode_Use;
					break;
				case mode_Use:
					mode = mode_Walk;
					break;
				case mode_Walk:
					mode = mode_Talk;
					break;
				case mode_Talk:
					mode = mode_Look;
					break;
				}
				handleAwayTeamMouseMove(0, 0); // TODO
				break;

			case Common::EVENT_MOUSEMOVE:
				if (in_dialog) {
					break;
				}

				if (on_bridge) {
					handleBridgeMouseMove(event.mouse.x, event.mouse.y);
				} else {
					handleAwayTeamMouseMove(event.mouse.x, event.mouse.y);
				}
				break;

			case Common::EVENT_LBUTTONUP:
				if (in_dialog) {
					in_dialog = false;
					break;
				}

				if (on_bridge) {
					handleBridgeMouseClick(event.mouse.x, event.mouse.y);
				} else {
					handleAwayTeamMouseClick(event.mouse.x, event.mouse.y);
				}
				break;

			default:
				break;
		}
	}
}

void UnityEngine::handleBridgeMouseMove(unsigned int x, unsigned int y) {
	status_text.clear();

	for (unsigned int i = 0; i < data.bridge_items.size(); i++) {
		BridgeItem &item = data.bridge_items[i];
		if (x < item.x) continue;
		if (y < item.y) continue;
		if (x > item.x + item.width) continue;
		if (y > item.y + item.height) continue;

		status_text = item.description;

		// TODO: this is kinda random :)
		if (item.unknown1 < 0x32)
			_gfx->setCursor(3, false);
		else
			_gfx->setCursor(5, false);

		return;
	}

	_gfx->setCursor(0xffffffff, false);
}

void UnityEngine::handleAwayTeamMouseMove(unsigned int x, unsigned int y) {
	Object *obj = objectAt(x, y);
	if (obj) {
		switch (mode) {
		case mode_Look:
			status_text = "Look at " + obj->name;
			_gfx->setCursor(1, false);
			break;
		case mode_Use:
			status_text = "Use " + obj->name;
			if (obj->talk_string.size() && obj->talk_string[obj->talk_string.size() - 1] == '-') {
				// handle custom USE strings
				// TODO: this doesn't really belong here (and isn't right anyway?)
				status_text.clear();
				unsigned int i = obj->talk_string.size() - 1;
				while (obj->talk_string[i] == '-' && i > 0) {
					i--;
				}
				while (obj->talk_string[i] != '-' && obj->talk_string[i] != ' ' && i > 0) {
					status_text = obj->talk_string[i] + status_text; i--;
				}
				status_text = status_text + " " + obj->name;
			}
			_gfx->setCursor(3, false);
			break;
		case mode_Walk:
			status_text = "Walk to " + obj->name;
			if (obj->cursor_flag == 1) // TODO
				_gfx->setCursor(8 + obj->cursor_id, false);
			else
				_gfx->setCursor(7, false);
			break;
		case mode_Talk:
			status_text = "Talk to " + obj->name;
			_gfx->setCursor(5, false);
			break;
		}
	} else {
		status_text.clear();
		switch (mode) {
		case mode_Look:
			_gfx->setCursor(0, false);
			break;
		case mode_Use:
			_gfx->setCursor(2, false);
			break;
		case mode_Walk:
			_gfx->setCursor(6, false);
			break;
		case mode_Talk:
			_gfx->setCursor(4, false);
			break;
		}
	}
}

void UnityEngine::handleBridgeMouseClick(unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < data.bridge_items.size(); i++) {
		BridgeItem &item = data.bridge_items[i];
		if (x < item.x) continue;
		if (y < item.y) continue;
		if (x > item.x + item.width) continue;
		if (y > item.y + item.height) continue;

		debug(1, "user clicked bridge item %d", i);
		if (item.id.world != 0) {
			// bridge crew member
			Object *obj = data.getObject(item.id);
			obj->runHail(obj->talk_string);
		} else {
			switch (i) {
				case 0: // conference lounge
					// TODO
					break;
				case 1: // turbolift (menu)
					dialog_text.clear();
					assert(!choice_list.size());
					choice_list.clear();
					assert(!dialog_choosing);

					for (unsigned int j = 0; j < data.bridge_screen_entries.size(); j++) {
						choice_list.push_back(data.bridge_screen_entries[j].text);
					}
					runDialog();

					// TODO: do this properly
					if (beam_world != 0) {
						startAwayTeam(beam_world, beam_screen);
					}

					choice_list.clear();
					break;
				case 2: // comms
					// TODO
					break;
				case 3: // tactical
					// TODO
					break;
				case 4: // astrogation
					// TODO
					break;
				case 5: // computer
					// TODO
					break;
				case 10: // replay conversation
					// TODO
					break;
				default:
					error("unknown bridge item");
			}
		}

		return;
	}
}

void UnityEngine::handleAwayTeamMouseClick(unsigned int x, unsigned int y) {
	Object *obj = objectAt(x, y);
	if (!obj) return;

	switch (mode) {
	case mode_Look:
		handleLook(obj);
		break;
	case mode_Use:
		handleUse(obj);
		break;
	case mode_Walk:
		handleWalk(obj);
		break;
	case mode_Talk:
		handleTalk(obj);
		break;
	}
}

void UnityEngine::drawObjects() {
	Common::Array<Object *> &objects = data.current_screen.objects;

	Common::Array<Object *> to_draw;
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->sprite) {
			if (!(objects[i]->flags & OBJFLAG_ACTIVE)) continue;
			if (objects[i]->flags & OBJFLAG_INVENTORY) continue;

			objects[i]->sprite->update();

			// TODO
			if (!objects[i]->sprite->valid()) { debug(2, "invalid sprite?"); continue; }

			to_draw.push_back(objects[i]);
		}
	}

	Common::sort(to_draw.begin(), to_draw.end(), DrawOrderComparison());
	for (unsigned int i = 0; i < to_draw.size(); i++) {
		// XXX: this is obviously a temporary hack
		unsigned int scale = 256;
		if (to_draw[i]->objwalktype == OBJWALKTYPE_SCALED) {
			unsigned int j;
			unsigned int x = to_draw[i]->x, y = to_draw[i]->y;
			for (j = 0; j < data.current_screen.polygons.size(); j++) {
				ScreenPolygon &poly = data.current_screen.polygons[j];
				if (poly.type != 1) continue;

				unsigned int triangle;
				if (poly.insideTriangle(x, y, triangle)) {
					// TODO: interpolation
					scale = poly.triangles[triangle].distances[0];
					break;
				}
			}
			if (j == data.current_screen.polygons.size())
				debug(2, "couldn't find poly for walkable at (%d, %d)", x, y);
		}
		_gfx->drawSprite(to_draw[i]->sprite, to_draw[i]->x, to_draw[i]->y, scale);
	}
}

void UnityEngine::drawDialogFrameAround(unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, bool use_thick_frame, bool with_icon) {
	// dialog.mrg
	MRGFile mrg;
	Common::Array<uint16> &widths = mrg.widths;
	Common::Array<uint16> &heights = mrg.heights;
	_gfx->loadMRG("dialog.mrg",  &mrg);
	assert(widths.size() == 31);

	unsigned int base = (use_thick_frame ? 17 : 0);

	unsigned int real_x1 = x - 10;
	unsigned int real_x2 = x + width + 10;
	unsigned int real_y1 = y - 20;
	unsigned int real_y2 = y + height + 20;

	real_y1 -= widths[base+4];
	real_y2 += widths[base+5];
	real_x1 -= widths[base+6];
	real_x2 += widths[base+7];

	// icon frame goes on LEFT side, up/down buttons on RIGHT side, both in centre

	if (with_icon) {
		// text border is pretty small if there's no icon frame, otherwise includes that space..
		real_x1 -= widths[8] / 2;
		real_x1 += 8;
	}

	// black background inside the border
	_gfx->fillRect(0, real_x1 + widths[base+4], real_y1 + heights[base+6], real_x2 - widths[base+5], real_y2 - heights[base+7]);

	for (unsigned int border_x = real_x1; border_x + widths[base+4] < real_x2; border_x += widths[base+4])
		_gfx->drawMRG(&mrg, base+4, border_x, real_y1);
	for (unsigned int border_x = real_x1; border_x + widths[base+5] < real_x2; border_x += widths[base+5])
		_gfx->drawMRG(&mrg, base+5, border_x, real_y2 - heights[base+5]);
	for (unsigned int border_y = real_y1; border_y + heights[base+6] < real_y2; border_y += heights[base+6])
		_gfx->drawMRG(&mrg, base+6, real_x1, border_y);
	for (unsigned int border_y = real_y1; border_y + heights[base+7] < real_y2; border_y += heights[base+7])
		_gfx->drawMRG(&mrg, base+7, real_x2 - widths[base+7], border_y);
	_gfx->drawMRG(&mrg, base+0, real_x1, real_y1);
	_gfx->drawMRG(&mrg, base+1, real_x2 - widths[base+1], real_y1);
	_gfx->drawMRG(&mrg, base+2, real_x1, real_y2 - heights[base+2]);
	_gfx->drawMRG(&mrg, base+3, real_x2 - widths[base+3], real_y2 - heights[base+3]);

	if (with_icon && icon) {
		_gfx->drawMRG(&mrg, 8, real_x1 - (widths[8] / 2) + (widths[base+6]/2), (real_y1+real_y2)/2 - (heights[8]/2));

		icon->update();
		_gfx->drawSprite(icon, real_x1 + (widths[base+6]/2) + 1, (real_y1+real_y2)/2 + (heights[8]/2) - 4);
	}
}

void UnityEngine::drawDialogWindow() {
	// TODO: de-hardcode
	unsigned int x = 100;
	unsigned int y = 280;

	// calculate required bounding box
	// TODO: some kind of scrolling
	unsigned int width = 0;
	unsigned int height = 0;

	Common::Array<unsigned int> heights;
	if (!choice_list.size()) {
		unsigned int newheight = 0;
		_gfx->calculateStringMaxBoundary(width, newheight, dialog_text, 2);
		height += newheight;
	} else {
		for (unsigned int i = 0; i < choice_list.size(); i++) {
			Common::String our_str = choice_list[i];
			if (i != choice_list.size() - 1) our_str += "\n";
			unsigned int newheight = 0;
			_gfx->calculateStringMaxBoundary(width, newheight, our_str, 2);
			heights.push_back(newheight);
			height += newheight;
		}
	}
	if (height > 160) height = 160;

	bool show_icon = choice_list.size() == 0;
	bool thick_frame = choice_list.size() != 0 && (!dialog_choosing);
	drawDialogFrameAround(x, y, width, height, thick_frame, show_icon);

	if (!choice_list.size()) {
		_gfx->drawString(x, y, width, height, dialog_text, 2);
	} else {
		// note this code changes y/height as it goes..
		for (unsigned int i = 0; i < choice_list.size(); i++) {
			// font 2 is normal, font 3 is highlighted
			bool selected = (i == 0); // TODO

			Common::String our_str = choice_list[i];
			if (i != choice_list.size() - 1) our_str += "\n";
			_gfx->drawString(x, y, width, height, our_str, selected ? 3 : 2);
			if (heights[i] > height) break;
			y += heights[i];
			height -= heights[i];
		}
	}

	// dialog window FRAME:
	// 0 is top left, 1 is top right, 2 is bottom left, 3 is bottom right
	// 4 is top, 5 is bottom, 6 is left, 7 is right?
	// 8 is icon frame (for left side), 9 is hilight box, 10 is not-hilight box
	// 11/12/13 up buttons (normal/highlight/grayed), 14/15/16 down buttons

	// options (lift, conference room, etc) window FRAME:
	// 17 top left, 18 top right, 19 bottom left, 20 bottom right, 21 top padding, 22 bottom padding,
	// 23 left padding, 24 right padding? then the buttons again, but for this mode: 25-30

	/*for (unsigned int i = 0; i < 31; i++) {
		_gfx->drawMRG("dialog.mrg", i, 50 * (1 + i/10), 30 * (i%10));
	}*/
}

void UnityEngine::drawBridgeUI() {
	// the advice button is drawn as a sprite (see startBridge), sigh
	// TODO: draw icons there if there's no subtitles

	// sensor.mrg
	// 4 sprites: "bridge" in/out and "viewscreen" in/out

	// draw viewscreen button
	MRGFile mrg;
	_gfx->loadMRG("sensor.mrg", &mrg);
	_gfx->drawMRG(&mrg, 3, 484, 426);

	// transp.mrg
	// 0-5: up arrows (normal, hilight, grayed) + down arrows
	// 5-11: seek arrows (normal, hilight, grayed): 3 left then 3 right
	// 11-17: 6 transporter room stuff?
	// 18-19: normal admin menu, hilighted admin menu
	// 20: some grey thing

	// draw grayed up/down arrows
	MRGFile tmrg;
	_gfx->loadMRG("transp.mrg", &tmrg);
	_gfx->drawMRG(&tmrg, 2, 117, 426);
	_gfx->drawMRG(&tmrg, 5, 117, 450);

	// display text (TODO: list of visited sectors)
	char buffer[30];

	// TODO: sane max width/heights

	// TODO: the location doesn't actually seem to come from this object
	Object *ship = data.getObject(objectID(0x00, 0x01, 0x5f));

	// TODO: updates while warping, etc
	Common::String sector_name = data.getSectorName(ship->universe_x, ship->universe_y, ship->universe_z);
	snprintf(buffer, 30, "SECTOR: %s", sector_name.c_str());
	_gfx->drawString(9, 395, 9999, 9999, buffer, 2);

	unsigned int warp_hi = 0, warp_lo = 0; // TODO
	snprintf(buffer, 30, "WARP: %d.%d", warp_hi, warp_lo);
	_gfx->drawString(168, 395, 9999, 9999, buffer, 2);

	snprintf(buffer, 30, "%s", status_text.c_str());
	Common::Array<unsigned int> strwidths, starts; unsigned int height;
	_gfx->calculateStringBoundary(110, strwidths, starts, height, buffer, 2);
	// draw centered, with 544 being the centre
	_gfx->drawString(544 - strwidths[0]/2, 393, strwidths[0], 9999, buffer, 2);
}

void UnityEngine::drawAwayTeamUI() {
	// draw UI
	MRGFile mrg;
	_gfx->loadMRG("awayteam.mrg", &mrg);
	_gfx->drawMRG(&mrg, 0, 0, 400);

	_gfx->drawString(0, 403, 9999, 9999, status_text.c_str(), 2);

	// TODO
}

Common::Error UnityEngine::run() {
	init();

	initGraphics(640, 480, true);
	_gfx->init();
	_snd->init();

	data.loadTriggers();
	data.loadSpriteFilenames();
	data.loadSectorNames();
	data.loadIconSprites();
	data.loadBridgeData();
	data.loadMovieInfo();

	startupScreen();

	// TODO: are the indexes identical in all versions?
	_gfx->playMovie(data.movie_filenames[0]);
	_gfx->playMovie(data.movie_filenames[4]);
	// TODO: add #3 to the 'already played' list

	_gfx->setCursor(0xffffffff, false);

	startBridge();

	while (!shouldQuit()) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data.current_screen.polygons);

		drawObjects();
		if (on_bridge) {
			drawBridgeUI();
		} else {
			drawAwayTeamUI();
		}

		assert(!in_dialog);

		while (next_situation != 0xffffffff) {
			assert(current_conversation);
			Response *resp;
			if (next_state != 0xffffffff)
				resp = current_conversation->getResponse(next_situation, next_state);
			else
				resp = current_conversation->getEnabledResponse(next_situation);
			next_situation = 0xffffffff;
			next_state = 0xffffffff;
			if (!resp) {
				warning("failed to find a next situation!");
				break;
			}
			// TODO: not always Picard! :(
			resp->execute(this, data.getObject(objectID(0, 0, 0)));
		}

		processTriggers();
		processTimers();

		_system->updateScreen();
	}

	return Common::kNoError;
}

void UnityEngine::runDialogChoice() {
	assert(current_conversation);
	assert(dialog_choice_responses.size() > 1);

	dialog_text.clear();
	assert(!choice_list.size());
	choice_list.clear();
	dialog_choosing = true;

	for (unsigned int i = 0; i < dialog_choice_responses.size(); i++) {
		Response *r = current_conversation->getResponse(dialog_choice_responses[i],
			dialog_choice_states[i]);
		choice_list.push_back(r->text + "\n");
	}
	runDialog();

	choice_list.clear();
	dialog_choosing = false;

	// TODO: don't always run the first choice, actually offer a choice? :)
	next_situation = dialog_choice_responses[0];
	next_state = dialog_choice_states[0];
}

void UnityEngine::runDialog() {
	in_dialog = true;
	_gfx->setCursor(0xffffffff, false);

	while (in_dialog) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data.current_screen.polygons);

		drawObjects();
		if (on_bridge) {
			drawBridgeUI();
		} else {
			drawAwayTeamUI();
		}

		drawDialogWindow();
		if (icon && icon->playing() && !_snd->speechPlaying()) {
			icon->startAnim(0); // static
		}

		_system->updateScreen();
	}

	// TODO: reset cursor
	_snd->stopSpeech();
}

void UnityEngine::performAction(ActionType action_id, Object *target, objectID other, objectID who) {
	switch (action_id) {
		case ACTION_USE: {
			if (!target) error("USE requires a target");

			// TODO..
			debug(1, "CommandBlock::execute: USE (on %s)", target->identify().c_str());
			target->use_entries.execute(this);
			}
			break;

		case ACTION_GET:
			if (!target) error("GET requires a target");

			debug(1, "CommandBlock::execute: GET (on %s)", target->identify().c_str());
			target->get_entries.execute(this);
			break;

		case ACTION_LOOK:
			if (target)
				warning("unimplemented: CommandBlock::execute: LOOK (on %s)", target->identify().c_str());
			else
				warning("unimplemented: CommandBlock::execute: LOOK");
			// TODO
			break;

		case ACTION_TIMER:
			if (!target) error("TIMER requires a target");

			debug(1, "CommandBlock::execute: TIMER (on %s)", target->identify().c_str());
			target->timer_entries.execute(this);
			break;

		case ACTION_WALK:
			if (target) {
				debug(1, "CommandBlock::execute: WALK (on %s)", target->identify().c_str());
				if (target->transition.world != 0xff) {
					Object *obj = data.getObject(target->transition);
					if (!obj) error("couldn't find transition object");

					// TODO: this is a silly hack
					performAction(ACTION_USE, obj);
					break;
				}

				warning("unimplemented: CommandBlock::execute: WALK (on %s)", target->identify().c_str());

				// TODO
			} else {
				warning("unimplemented: CommandBlock::execute: WALK");
				// TODO
			}
			break;

		case ACTION_TALK:
			{
			if (!target) error("we don't handle TALK without a valid target, should we?");
			// target is target (e.g. Pentara), other is source (e.g. Picard)
			// TODO..
			debug(1, "CommandBlock::execute: TALK (on %s)", target->identify().c_str());
			target->runHail(target->talk_string);
			}
			break;

		default:
			error("unknown action type %x", action_id);
	}
}


} // Unity

