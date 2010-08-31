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

/**
 * A black and white SCI-style arrow cursor (11x16).
 * 3 = Transparent.
 * 0 = Black (#000000 in 24-bit RGB).
 * 1 = White (#FFFFFF in 24-bit RGB).
 */
static const byte sciMouseCursor[] = {
	0,0,3,3,3,3,3,3,3,3,3,
	0,1,0,3,3,3,3,3,3,3,3,
	0,1,1,0,3,3,3,3,3,3,3,
	0,1,1,1,0,3,3,3,3,3,3,
	0,1,1,1,1,0,3,3,3,3,3,
	0,1,1,1,1,1,0,3,3,3,3,
	0,1,1,1,1,1,1,0,3,3,3,
	0,1,1,1,1,1,1,1,0,3,3,
	0,1,1,1,1,1,1,1,1,0,3,
	0,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,0,3,3,3,3,
	0,1,0,3,0,1,1,0,3,3,3,
	0,0,3,3,0,1,1,0,3,3,3,
	3,3,3,3,3,0,1,1,0,3,3,
	3,3,3,3,3,0,1,1,0,3,3,
	3,3,3,3,3,3,0,1,1,0,3
};

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst), data(this) {
	in_dialog = false;
	icon = NULL;
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

	// TODO: what do we do with all the objects?
	// for now, we just delete all but the away team
	for (unsigned int i = 4; i < data.current_screen.objects.size(); i++) {
		delete data.current_screen.objects[i]->sprite;
		data.current_screen.objects[i]->sprite = NULL;
	}
	// TODO: delete everything, re-copy
	data.current_screen.objects.resize(4);

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
		if (!objects[i]->active) continue;
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
	const char *bridge_sprites[10] = {
		"brdgldor.spr", // Left Door (conference room)
		"brdgdoor.spr", // Door
		"brdgworf.spr", // Worf
		"brdgtroi.spr", // Troi
		"brdgdata.spr", // Data
		"brdgrikr.spr", // Riker
		"brdgpica.spr", // Picard
		"brdgtitl.spr", // Episode Title
		"advice.spr", // advice button in UI :(
		0 };

	for (unsigned int i = 0; bridge_sprites[i] != 0; i++) {
		Object *obj = new Object(this);
		obj->x = obj->y = 0;
		obj->z_adjust = 0xffff;
		obj->active = true;
		obj->scaled = false;
		obj->sprite = new SpritePlayer(new Sprite(data.openFile(bridge_sprites[i])), obj, this);;
		obj->sprite->startAnim(0);
		data.current_screen.objects.push_back(obj);
	}

	_gfx->setBackgroundImage("bridge.rm");
}

void UnityEngine::startAwayTeam(unsigned int world, unsigned int screen) {
	// beam in an away team
	for (unsigned int i = 0; i < 4; i++) {
		objectID objid(0, 0, i);
		Object *obj = data.getObject(objid);
		obj->loadSprite();
		data.current_screen.objects.push_back(obj);
	}

	openLocation(world, screen);
	for (unsigned int i = 0; i < 4; i++) {
		data.current_screen.objects[i]->x = data.current_screen.entrypoints[0][i].x;
		data.current_screen.objects[i]->y = data.current_screen.entrypoints[0][i].y;
	}

	// draw UI
	MRGFile mrg;
	_gfx->loadMRG("awayteam.mrg", &mrg);
	_gfx->drawMRG(&mrg, 0, 0, 400);
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
			target->use_entries.execute(this);
			break;
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
	icon->startAnim(2); // speaking
}

void UnityEngine::handleLook(Object *obj) {
	// TODO: this is very wrong, of course :)

	// use only Picard's entry for now
	unsigned int i = 0;
	while (i < obj->descriptions.size() &&
			obj->descriptions[i].entry_id != 0) i++;
	if (i == obj->descriptions.size()) return;

	Description &desc = obj->descriptions[i];
	in_dialog = true;
	dialog_text = desc.text;
	setSpeaker(objectID(0, 0, 0));

	// TODO: this is broken
	// for these in-area files, it's sometimes %02x%c%1x%02x%02x.vac ?!
	// where %c is 'l' or 't' - i haven't worked out why yet

	Common::String file;
	file = Common::String::printf("%02x%c%1x%02x%02x.vac",
		desc.voice_group, 'l', desc.voice_subgroup,
		desc.entry_id, desc.voice_id);

	if (!SearchMan.hasFile(file)) {
		file = Common::String::printf("%02x%c%1x%02x%02x.vac",
			desc.voice_group, 't', desc.voice_subgroup,
			desc.entry_id, desc.voice_id);

		if (!SearchMan.hasFile(file)) {
			file = Common::String::printf("%02x%02x%02x%02x.vac",
				desc.voice_group, desc.entry_id,
				desc.voice_subgroup, desc.voice_id);
		}
	}

	_snd->playSpeech(file);
}

// TODO
unsigned int curr_loc = 4;
unsigned int curr_screen = 1;
unsigned int anim = 26;

void UnityEngine::checkEvents() {
	Common::Array<Object *> &objects = data.current_screen.objects;

	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		switch (event.type) {
			case Common::EVENT_QUIT:
				_system->quit();
				break;

			case Common::EVENT_KEYUP:
				printf("trying anim %d\n", anim);
				anim++;
				anim %= objects[0]->sprite->numAnims();
				for (unsigned int i = 0; i < 4; i++)
					objects[i]->sprite->startAnim(anim);
				break;

			case Common::EVENT_RBUTTONUP:
				curr_screen++;
				openLocation(curr_loc, curr_screen);
				for (unsigned int i = 0; i < 4; i++) {
					objects[i]->x = data.current_screen.entrypoints[0][i].x;
					objects[i]->y = data.current_screen.entrypoints[0][i].y;
				}
				break;

			case Common::EVENT_MOUSEMOVE:
				/*if (objectAt(event.mouse.x, event.mouse.y)) {
					_gfx->setCursor(1, false);
				} else {
					_gfx->setCursor(0, false);
				}*/
				break;

			case Common::EVENT_LBUTTONUP: {
				if (in_dialog) {
					in_dialog = false;
					break;
				}

				Object *obj = objectAt(event.mouse.x, event.mouse.y);
				if (!obj) break;

				handleLook(obj);
			} break;

			default:
				break;
		}
	}
}

void UnityEngine::drawObjects() {
	Common::Array<Object *> &objects = data.current_screen.objects;

	Common::Array<Object *> to_draw;
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->sprite && objects[i]->active) {
			objects[i]->sprite->update();

			// TODO
			if (!objects[i]->sprite->valid()) { /*warning("invalid sprite?");*/ continue; }

			to_draw.push_back(objects[i]);
		}
	}

	Common::sort(to_draw.begin(), to_draw.end(), DrawOrderComparison());
	for (unsigned int i = 0; i < to_draw.size(); i++) {
		// XXX: this is obviously a temporary hack
		unsigned int scale = 256;
		if (to_draw[i]->scaled) {
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
				warning("couldn't find poly for walkable at (%d, %d)", x, y);
		}
		_gfx->drawSprite(to_draw[i]->sprite, to_draw[i]->x, to_draw[i]->y, scale);
	}
}

void UnityEngine::drawDialogWindow() {
	// dialog.mrg

	MRGFile mrg;
	Common::Array<uint16> &widths = mrg.widths;
	Common::Array<uint16> &heights = mrg.heights;
	_gfx->loadMRG("dialog.mrg",  &mrg);
	assert(widths.size() == 31);

	bool use_thick_frame = false;
	unsigned int base = (use_thick_frame ? 17 : 0);

	// TODO: de-hardcode
	unsigned int x = 100;
	unsigned int y = 280;

	// calculate required bounding box
	// TODO: some kind of scrolling
	Common::Array<unsigned int> strwidths, starts;
	unsigned int height;
	_gfx->calculateStringBoundary(320, strwidths, starts, height, dialog_text, 2);
	unsigned int width = 0;
	for (unsigned int i = 0; i < strwidths.size(); i++) {
		if (strwidths[i] > width)
			width = strwidths[i];
	}

	unsigned int real_x1 = x - 10;
	unsigned int real_x2 = x + width + 10;
	unsigned int real_y1 = y - 20;
	unsigned int real_y2 = y + height + 20;

	real_y1 -= widths[base+4];
	real_y2 += widths[base+5];
	real_x1 -= widths[base+6];
	real_x2 += widths[base+7];

	// icon frame goes on LEFT side, up/down buttons on RIGHT side, both in centre
	// TODO: version without icon
	// text border is pretty small if there's no icon frame, otherwise includes that space..
	real_x1 -= widths[8] / 2;
	real_x1 += 8;

	// black background inside the border
	_gfx->fillRect(0, real_x1 + widths[base+0], real_y1 + heights[base+0], real_x2 - widths[base+3], real_y2 - heights[base+3]);

	for (unsigned int border_x = real_x1; border_x + widths[base+4] < real_x2; border_x += widths[base+4])
		_gfx->drawMRG(&mrg, base+4, border_x, real_y1);
	for (unsigned int border_x = real_x1; border_x + widths[base+5] < real_x2; border_x += widths[base+5])
		_gfx->drawMRG(&mrg, base+5, border_x, real_y2 - widths[base+5]);
	for (unsigned int border_y = real_y1; border_y + heights[base+6] < real_y2; border_y += heights[base+6])
		_gfx->drawMRG(&mrg, base+6, real_x1, border_y);
	for (unsigned int border_y = real_y1; border_y + heights[base+7] < real_y2; border_y += heights[base+7])
		_gfx->drawMRG(&mrg, base+7, real_x2 - widths[base+7], border_y);
	_gfx->drawMRG(&mrg, base+0, real_x1, real_y1);
	_gfx->drawMRG(&mrg, base+1, real_x2 - widths[base+7], real_y1);
	_gfx->drawMRG(&mrg, base+2, real_x1, real_y2 - heights[base+5]);
	_gfx->drawMRG(&mrg, base+3, real_x2 - widths[base+7], real_y2 - heights[base+5]);

	_gfx->drawMRG(&mrg, 8, real_x1 - (widths[8] / 2) + (widths[base+6]/2), (real_y1+real_y2)/2 - (heights[8]/2));

	if (icon) {
		icon->update();
		_gfx->drawSprite(icon, real_x1 + (widths[base+6]/2) + 1, (real_y1+real_y2)/2 + (heights[8]/2) - 4);
	}

	// font 2 is normal, font 3 is highlighted
	_gfx->drawString(x, y, width, height, dialog_text, 2);

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

	Common::String sector_name = data.getSectorName(90, 90, 90); // TODO
	snprintf(buffer, 30, "SECTOR: %s", sector_name.c_str());
	_gfx->drawString(9, 395, 9999, 9999, buffer, 2);

	unsigned int warp_hi = 0, warp_lo = 0; // TODO
	snprintf(buffer, 30, "WARP: %d.%d", warp_hi, warp_lo);
	_gfx->drawString(168, 395, 9999, 9999, buffer, 2);
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

	startupScreen();

	// XXX: this mouse cursor is borrowed from SCI
	CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 3);
	CursorMan.showMouse(true);

	// and we stomp over it anyway, but this only good for some situations :)
	//_gfx->setCursor(0, false);

	//startAwayTeam(curr_loc, curr_screen);
	//for (unsigned int i = 0; i < 4; i++) data.current_screen.objects[i]->sprite->startAnim(anim);

	startBridge();

	while (!shouldQuit()) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data.current_screen.polygons);

		drawObjects();
		drawBridgeUI();

		assert(!in_dialog);

		processTriggers();

		_system->updateScreen();
	}

	return Common::kNoError;
}

void UnityEngine::runDialogChoice() {
	assert(dialog_choice_responses.size() > 1);

	dialog_text.clear();
	for (unsigned int i = 0; i < dialog_choice_responses.size(); i++) {
		Response *r = current_conversation.getResponse(dialog_choice_responses[i],
			dialog_choice_states[i]);
		dialog_text += r->text + "\n\n";
	}
	runDialog();

	// TODO: don't always run the first choice, actually offer a choice? :)
	Object *speakerobj = data.getObject(objectID(0, 0, 0)); // TODO: this is starting to be crazy
	current_conversation.execute(this, speakerobj, dialog_choice_responses[0], dialog_choice_states[0]);
}

void UnityEngine::runDialog() {
	in_dialog = true;

	while (in_dialog) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data.current_screen.polygons);

		drawObjects();
		drawBridgeUI();

		drawDialogWindow();

		_system->updateScreen();
	}

	// TODO: stupid hack until we actually keep the sound handle
	_mixer->stopAll();
}

} // Unity

