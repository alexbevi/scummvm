/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "unity.h"
#include "bridge.h"
#include "computer.h"
#include "conversation.h"
#include "graphics.h"
#include "sound.h"
#include "sprite_player.h"
#include "object.h"
#include "trigger.h"
#include "viewscreen.h"

#include "common/fs.h"
#include "common/config-manager.h"
#include "common/error.h"
#include "common/textconsole.h"
#include "engines/util.h"
#include "common/events.h"
#include "common/stream.h"
#include "common/system.h"
#include "common/unzip.h"
#include "common/file.h"
#include "common/archive.h"
#include "graphics/cursorman.h"
#include "graphics/font.h"
#include "common/debug-channels.h"
#include "common/EventRecorder.h"

namespace Unity {

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst), data(this) {
	DebugMan.addDebugChannel(kDebugResource, "Resource", "Resource Debug Flag");
	DebugMan.addDebugChannel(kDebugSaveLoad, "Saveload", "Saveload Debug Flag");
	DebugMan.addDebugChannel(kDebugScript, "Script", "Script Debug Flag");
	DebugMan.addDebugChannel(kDebugGraphics, "Graphics", "Graphics Debug Flag");
	DebugMan.addDebugChannel(kDebugSound, "Sound", "Sound Debug Flag");

	_in_dialog = false;
	_dialog_choosing = false;
	_icon = NULL;
	_next_conversation = NULL;
	_next_situation = 0xffffffff;
	_beam_world = 0;
	_mode = mode_Look;
	_current_away_team_member = NULL;
	_current_away_team_icon = NULL;
	_inventory_index = 0;
	_viewscreen_sprite_id = ~0; // should be set by startup scripts

	// TODO: de-hardcode
	_dialog_x = 100;
	_dialog_y = 280;

	_bridgeScreen = new BridgeScreen(this);
	_computerScreen = new ComputerScreen(this);
	_viewscreenScreen = new ViewscreenScreen(this);
	_currScreen = NULL;
	_currScreenType = NoScreenType;
}

UnityEngine::~UnityEngine() {
	if (_currScreen)
		_currScreen->shutdown();
	delete _bridgeScreen;
	delete _computerScreen;
	delete _viewscreenScreen;

	delete _snd;
	delete _console;
	delete _gfx;
	// FIXME: Segfaults if deletion is done
	//delete data.data;
	delete _icon;
	endAwayTeam();

	DebugMan.clearAllDebugChannels();
	delete _rnd;
}

Common::Error UnityEngine::init() {
	_rnd = new Common::RandomSource("unity");

	_gfx = new Graphics(this);
	_console = new UnityConsole(this);
	_snd = new Sound(this);

	data._data = Common::makeZipArchive("STTNG.ZIP");
	if (!data._data) {
		error("couldn't open data file");
	}
	/*Common::ArchiveMemberList list;
	data->listMembers(list);
	for (Common::ArchiveMemberList::const_iterator file = list.begin(); file != list.end(); ++file) {
		Common::String filename = (*file)->getName();
		filename.toLowercase();
		if (filename.hasSuffix(".spr") || filename.hasSuffix(".spt")) {
			Common::SeekableReadStream *ourstr = (*file)->createReadStream();
			debugN("trying '%s'\n", filename.c_str());
			Sprite spr(ourstr);
		}
	}*/
	SearchMan.add("sttngzip", data._data);

	// DOS version only
	data._instData = Common::makeZipArchive("STTNGINS.ZIP");
	if (data._instData) {
		SearchMan.add("sttnginszip", data._instData);
	}

	// Mac version only
	const Common::FSNode gameDataDir(ConfMan.get("path"));
	SearchMan.addDirectory(".movies", gameDataDir.getPath() + "/.movies");

	return Common::kNoError;
}

void UnityEngine::openLocation(unsigned int world, unsigned int screen) {
	_snd->stopMusic(); // FIXME: find a better place

	Common::String filename = Common::String::format("sl%03d.scr", world);
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

	data._currentScreen.entrypoints.clear();
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
		data._currentScreen.entrypoints.push_back(entrypoints);
	}

	delete locstream;

	filename = Common::String::format("w%02x%02xobj.bst", world, screen);
	locstream = data.openFile(filename);

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
		debug(6, "reading obj '%s' (%s)", _name, _desc);

		Object *obj = data.getObject(id);
		obj->loadSprite();
		data._currentScreen.objects.push_back(obj);
	}

	delete locstream;

	filename = Common::String::format("w_%02dstrt.bst", world);
	locstream = data.openFile(filename);

	locstream->seek(45 * screen);
	if (locstream->eos()) error("no startup entry for screen %x (world %x)", screen, world);

	// TODO: use these
	uint16 advice_id = locstream->readUint16LE();
	uint16 advice_timer = locstream->readUint16LE();
	debug(6, "openLocation() advice_id: %d advice_timer: %d", advice_id, advice_timer);

	byte unknown1 = locstream->readByte();
	byte unknown2 = locstream->readByte();
	debug(6, "openLocation() unknown1: %d unknown2: %d", unknown1, unknown2);

	uint16 startup_screen = locstream->readUint16LE();
	objectID target = readObjectID(locstream);
	byte action_type = locstream->readByte();
	objectID who = readObjectID(locstream);
	objectID other = readObjectID(locstream);
	uint32 unknown3 = locstream->readUint32LE();
	uint16 unknown4 = locstream->readUint16LE();
	uint16 unknown5 = locstream->readUint16LE();
	byte unknown6 = locstream->readByte();
	debug(6, "openLocation() unknown3: %d unknown4: %d", unknown3, unknown4);
	debug(6, "openLocation() unknown5: %d unknown6: %d", unknown5, unknown6);

	if (startup_screen != 0xffff) {
		assert(startup_screen == screen);
		if (target.id != 0xff) {
			// TODO: only fire this on first load
			ActionType action_id;
			switch (action_type) {
			case 0: action_id = ACTION_USE; break;
			case 1: action_id = ACTION_GET; break;
			case 2: action_id = ACTION_LOOK; break;
			case 3: action_id = ACTION_GET; break;
			case 4: action_id = ACTION_WALK; break;
			case 5: action_id = ACTION_TALK; break;
			case 6: action_id = ACTION_USE; break;
			default: error("unknown action type %x when starting screen %x of world %x",
				action_type, screen, world);
			}
			// TODO: run delayed
			performAction(action_id, data.getObject(target), who, other);
		}
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
		if (a->y_adjust != -1) ay += a->y_adjust;
		if (b->y_adjust != -1) by += b->y_adjust;
		return ay < by;
	}
};

Object *UnityEngine::objectAt(unsigned int x, unsigned int y) {
	Common::Array<Object *> &objects = data._currentScreen.objects;

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

void UnityEngine::clearObjects() {
	data._currentScreen.objects.clear();
}

void UnityEngine::removeObject(Object *obj) {
	Common::Array<Object *> &objects = data._currentScreen.objects;

	for (uint i = 0; i < objects.size(); i++) {
		if (objects[i] != obj)
			continue;
		objects.remove_at(i);
		return;
	}

	error("removeObject failed to find object");
}

void UnityEngine::startBridge() {
	endAwayTeam();
	changeToScreen(BridgeScreenType);
}

void UnityEngine::endAwayTeam() {
	_current_away_team_member = NULL;
	delete _current_away_team_icon;
	_current_away_team_icon = NULL;
	_away_team_members.clear();
	_inventory_items.clear();
	for (unsigned int i = 0; i < _inventory_icons.size(); i++) {
		delete _inventory_icons[i];
	}
	_inventory_icons.clear();

	_on_away_team = false;
}

void UnityEngine::addToInventory(Object *obj) {
	assert(_inventory_items.size() == _inventory_icons.size());
	if (Common::find(_inventory_items.begin(), _inventory_items.end(), obj) != _inventory_items.end()) {
		error("trying to add %s to inventory but it's already there", obj->identify().c_str());
	}

	obj->flags |= OBJFLAG_INVENTORY;

	_inventory_items.push_back(obj);

	Common::String icon_sprite = data.getIconSprite(obj->id);
	if (!icon_sprite.size()) {
		error("couldn't find icon sprite for inventory item %s", obj->identify().c_str());
	}
	SpritePlayer *icon = NULL;
	icon = new SpritePlayer(icon_sprite.c_str(), NULL, this);
	icon->startAnim(0); // static
	_inventory_icons.push_back(icon);
}

void UnityEngine::removeFromInventory(Object *obj) {
	obj->flags &= ~OBJFLAG_INVENTORY;

	unsigned int i;
	for (i = 0; i < _inventory_items.size(); i++) {
		if (_inventory_items[i] == obj) {
			_inventory_items.remove_at(i);
			break;
		}
	}
	if (i == _inventory_items.size()) {
		error("sync error while trying to remove %s from inventory", obj->identify().c_str());
	}
	delete _inventory_icons[i];
	_inventory_icons.remove_at(i);

	assert(_inventory_items.size() == _inventory_icons.size());
}

void UnityEngine::startAwayTeam(unsigned int world, unsigned int screen, byte entrance) {
	if (_currScreen)
		_currScreen->shutdown();
	_currScreen = NULL;
	_currScreenType = NoScreenType;

	clearObjects();
	data._currentScreen.world = world;
	data._currentScreen.screen = screen;

	if (!_away_team_members.size()) {
		// TODO: move this somewhere sensible!
		for (unsigned int i = 0; i < 4; i++) {
			objectID objid(i, 0, 0);
			Object *obj = data.getObject(objid);
			obj->loadSprite();
			_away_team_members.push_back(obj);
		}
		_current_away_team_member = _away_team_members[0];

		Common::String icon_sprite = data.getIconSprite(_current_away_team_member->id);
		if (!icon_sprite.size()) {
			error("couldn't find icon sprite for away team member %s",
				_current_away_team_member->identify().c_str());
		}
		_current_away_team_icon = new SpritePlayer(icon_sprite.c_str(), NULL, this);
		_current_away_team_icon->startAnim(0); // static

		for (unsigned int i = 0; i < 5; i++) {
			if (i == 2) continue; // TODO: handle communicator
			objectID objid(i, 0x70, 0);
			Object *obj = data.getObject(objid);
			if (!(obj->flags & OBJFLAG_ACTIVE)) continue;
			addToInventory(obj);
		}
	} else {
		assert(_current_away_team_member);
	}

	// beam in the away team
	for (unsigned int i = 0; i < 9; i++) {
		objectID objid(i, 0, 0);
		Object *obj = data.getObject(objid);
		if (Common::find(_away_team_members.begin(), _away_team_members.end(), obj)
			== _away_team_members.end()) {
			// XXX: stupid hack to make sure the rest of the away team is off-screen
			obj->curr_screen = 0;
		} else {
			obj->curr_screen = screen;
			obj->loadSprite();
			obj->sprite->startAnim(26);
			data._currentScreen.objects.push_back(obj);
		}
	}

	openLocation(world, screen);
	if (entrance == 0xff) {
		// TODO: don't display sprites?
		entrance = 0;
	}
	if (entrance >= data._currentScreen.entrypoints.size()) {
		error("entrance %d beyond the %d entrances to screen %x/%x", entrance,
			data._currentScreen.entrypoints.size(), world, screen);
	}
	for (unsigned int i = 0; i < 4; i++) {
		data._currentScreen.objects[i]->x = data._currentScreen.entrypoints[entrance][i].x;
		data._currentScreen.objects[i]->y = data._currentScreen.entrypoints[entrance][i].y;
	}

	_on_away_team = true;
	handleAwayTeamMouseMove(Common::Point());
}

void UnityEngine::startupScreen() {
	// play two animations (both logo anim followed by text) from one file
	SpritePlayer *p = new SpritePlayer("legaleze.spr", 0, this);
	unsigned int anim = 0;
	p->startAnim(anim);
	uint32 waiting = 0;
	while (!shouldQuit()) {
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
				// arbitary 4 second wait, original waits until background
				// load is done (i.e. 0s on modern systems)
				waiting = g_system->getMillis() + 2000;
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
	for (unsigned int i = 0; i < data._triggers.size(); i++) {
		if (data._triggers[i]->tick(this)) {
			Object *target = data.getObject(data._triggers[i]->target);
			debug(1, "running trigger %x (target %s)", data._triggers[i]->id, target->identify().c_str());
			// TODO: should trigger be who?
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

	Common::Array<Object *> &objects = data._currentScreen.objects;
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (!(objects[i]->flags & OBJFLAG_ACTIVE)) continue;
		if (objects[i]->timer == 0xffff) continue;
		if (objects[i]->timer == 0) continue;

		objects[i]->timer--;
		if (objects[i]->timer == 0) {
			debug(1, "running timer on %s", objects[i]->identify().c_str());
			// TODO: should who be the timer?
			performAction(ACTION_TIMER, objects[i]);
		}
	}
}

void UnityEngine::setSpeaker(objectID s) {
	_speaker = s;

	if (_icon) {
		delete _icon;
		_icon = NULL;
	}

	Common::String icon_sprite = data.getIconSprite(_speaker);
	if (!icon_sprite.size()) {
		warning("couldn't find icon sprite for %02x%02x%02x", _speaker.world,
			_speaker.screen, _speaker.id);
		return;
	}

	_icon = new SpritePlayer(icon_sprite.c_str(), NULL, this);
	if (_icon->getNumAnims() < 3) {
		_icon->startAnim(0); // static
	} else {
		_icon->startAnim(2); // speaking
	}
}

void UnityEngine::handleLook(Object *obj) {
	// TODO: correct?
	if (performAction(ACTION_LOOK, obj, _current_away_team_member->id) & RESULT_DIDSOMETHING) return;

	playDescriptionFor(obj);
}

void UnityEngine::playDescriptionFor(Object *obj) {
	// TODO: this is very wrong, of course :)

	unsigned int i = 0;
	while (i < obj->descriptions.size() &&
			obj->descriptions[i].entry_id != _current_away_team_member->id.id) i++;
	if (i == obj->descriptions.size()) {
		// TODO: should we just run this directly?
		// TODO: hard-coded :( @95,34,xy...
		_next_conversation = data.getConversation(0x5f, 34);
		_next_situation = 0x1; // default to failed look
		return;
	}

	Description &desc = obj->descriptions[i];
	_dialog_text = desc.text;
	setSpeaker(_current_away_team_member->id);

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
		return Common::String::format("%02x%02x%02x%02x.vac",
			voice_group, speaker.id,
			voice_subgroup, voice_id);
	} else if (type) {
		return Common::String::format("%02x%c%1x%02x%02x.vac",
			voice_group, type, voice_subgroup,
			speaker.id, voice_id);
	} else {
		return Common::String::format("%02x%02x%02x%02x.vac",
			voice_group, voice_subgroup,
			speaker.id, voice_id);
	}
}

void UnityEngine::changeToScreen(ScreenType screenType) {
	if (screenType == _currScreenType)
		return;
	_currScreenType = screenType;

	if (_currScreen)
		_currScreen->shutdown();
	_currScreen = NULL;

	switch (screenType) {
	case BridgeScreenType:
		_currScreen = _bridgeScreen;
		break;
	case ComputerScreenType:
		_currScreen = _computerScreen;
		break;
	case ViewscreenScreenType:
		_currScreen = _viewscreenScreen;
		break;
	default:
		error("changeToScreen for unimplemented screen type");
	}

	_snd->stopMusic();
	_currScreen->start();
}

void UnityEngine::handleUse(Object *obj) {
	// TODO: correct?
	if (performAction(ACTION_USE, obj, _current_away_team_member->id) & RESULT_DIDSOMETHING) return;

	// TODO: should we just run this directly?
	// TODO: hard-coded :( @95,34,xy...
	_next_conversation = data.getConversation(0x5f, 34);
	_next_situation = 0x2; // failed use?
}

void UnityEngine::handleTalk(Object *obj) {
	// TODO: correct?
	if (performAction(ACTION_TALK, obj, _current_away_team_member->id) & RESULT_DIDSOMETHING) return;

	// TODO: should we just run this directly?
	// TODO: hard-coded :( @95,34,xy...
	_next_conversation = data.getConversation(0x5f, 34);
	_next_situation = 0x3; // failed talk?
}

void UnityEngine::handleWalk(Object *obj) {
	performAction(ACTION_WALK, obj, _current_away_team_member->id);
}

// TODO
unsigned int anim = 26;

void UnityEngine::DebugNextScreen() {
	unsigned int &curr_loc = data._currentScreen.world;
	unsigned int &curr_screen = data._currentScreen.screen;

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
	debugN("moving to %d/%d\n", curr_loc, curr_screen);
	startAwayTeam(curr_loc, curr_screen);
}

void UnityEngine::checkEvents() {
	Common::Array<Object *> &objects = data._currentScreen.objects;

	Common::Event event;
	while (_eventMan->pollEvent(event)) {
		switch (event.type) {
			case Common::EVENT_QUIT:
				return;

			case Common::EVENT_KEYUP:
				switch (event.kbd.keycode) {
				case Common::KEYCODE_n:
					if (_on_away_team) {
						debugN("trying anim %d\n", anim);
						anim++;
						anim %= objects[0]->sprite->getNumAnims();
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

			case Common::EVENT_KEYDOWN:
				switch (event.kbd.keycode) {
                        	case Common::KEYCODE_d:
                                	if (event.kbd.hasFlags(Common::KBD_CTRL)) {
                                	        this->getDebugger()->attach();
                                	        this->getDebugger()->onFrame();
                                	}
                                	break;
				default:
					break;
				}
				break;			

			case Common::EVENT_RBUTTONUP:
				if (!_on_away_team)
					break;

				// cycle through away team modes
				switch (_mode) {
				case mode_Look:
					_mode = mode_Use;
					break;
				case mode_Use:
					_mode = mode_Walk;
					break;
				case mode_Walk:
					_mode = mode_Talk;
					break;
				case mode_Talk:
					_mode = mode_Look;
					break;
				}
				handleAwayTeamMouseMove(Common::Point()); // TODO
				break;

			case Common::EVENT_MOUSEMOVE:
				if (_in_dialog) {
					dialogMouseMove(event.mouse);
					break;
				}

				if (_on_away_team) {
					handleAwayTeamMouseMove(event.mouse);
				} else {
					_currScreen->mouseMove(event.mouse);
				}
				break;

			case Common::EVENT_LBUTTONUP:
				if (_in_dialog) {
					dialogMouseClick(event.mouse);
					break;
				}

				if (_on_away_team) {
					handleAwayTeamMouseClick(event.mouse);
				} else {
					_currScreen->mouseClick(event.mouse);
				}
				break;

			default:
				break;
		}
	}

	_snd->updateMusic(); // TODO
}

void UnityEngine::handleAwayTeamMouseMove(const Common::Point &pos) {
	Object *obj = objectAt(pos.x, pos.y);
	if (obj) {
		switch (_mode) {
		case mode_Look:
			_status_text = "Look at " + obj->name;
			_gfx->setCursor(1, false);
			break;
		case mode_Use:
			_status_text = "Use " + obj->name;
			if (obj->talk_string.size() && obj->talk_string[obj->talk_string.size() - 1] == '-') {
				// handle custom USE strings
				// TODO: this doesn't really belong here (and isn't right anyway?)
				_status_text.clear();
				unsigned int i = obj->talk_string.size() - 1;
				while (obj->talk_string[i] == '-' && i > 0) {
					i--;
				}
				while (obj->talk_string[i] != '-' && obj->talk_string[i] != ' ' && i > 0) {
					_status_text = obj->talk_string[i] + _status_text; i--;
				}
				_status_text = _status_text + " " + obj->name;
			}
			_gfx->setCursor(3, false);
			break;
		case mode_Walk:
			_status_text = "Walk to " + obj->name;
			if (obj->cursor_flag == 1) // TODO
				_gfx->setCursor(8 + obj->cursor_id, false);
			else
				_gfx->setCursor(7, false);
			break;
		case mode_Talk:
			_status_text = "Talk to " + obj->name;
			_gfx->setCursor(5, false);
			break;
		}
	} else {
		_status_text.clear();
		switch (_mode) {
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

void UnityEngine::handleAwayTeamMouseClick(const Common::Point &pos) {
	Object *obj = objectAt(pos.x, pos.y);
	if (!obj)
		return;

	switch (_mode) {
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
	Common::Array<Object *> &objects = data._currentScreen.objects;

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
			for (j = 0; j < data._currentScreen.polygons.size(); j++) {
				ScreenPolygon &poly = data._currentScreen.polygons[j];
				if (poly.type != 1) continue;

				unsigned int triangle;
				if (poly.insideTriangle(x, y, triangle)) {
					// TODO: interpolation
					scale = poly.triangles[triangle].distances[0];
					break;
				}
			}
			if (j == data._currentScreen.polygons.size())
				debug(2, "couldn't find poly for walkable at (%d, %d)", x, y);
		}
		_gfx->drawSprite(to_draw[i]->sprite, to_draw[i]->x, to_draw[i]->y, scale);
	}
}

void UnityEngine::drawDialogFrameAround(unsigned int x, unsigned int y, unsigned int width,
	unsigned int height, bool use_thick_frame, bool with_icon, bool with_buttons) {
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
	} else if (with_buttons) {
		// spacing for up/down buttons
		real_x2 += widths[11] / 2;
		real_x2 -= 8;
	}

	// black background inside the border
	_gfx->fillRect(0, real_x1 + widths[base+4], real_y1 + heights[base+6], real_x2 - widths[base+5], real_y2 - heights[base+7]);

	// top
	for (unsigned int border_x = real_x1 + widths[base+0]; border_x + widths[base+4] < real_x2 - widths[base+1]; border_x += widths[base+4])
		_gfx->drawMRG(&mrg, base+4, border_x, real_y1);
	_gfx->drawMRG(&mrg, base+4, real_x2 - widths[base+1] - widths[base+4], real_y1);
	// bottom
	for (unsigned int border_x = real_x1 + widths[base+2]; border_x + widths[base+5] < real_x2 - widths[base+3]; border_x += widths[base+5])
		_gfx->drawMRG(&mrg, base+5, border_x, real_y2 - heights[base+5]);
	_gfx->drawMRG(&mrg, base+5, real_x2 - widths[base+3] - widths[base+5], real_y2 - heights[base+5]);
	// left
	for (unsigned int border_y = real_y1 + heights[base+0]; border_y + heights[base+6] < real_y2 - heights[base+2]; border_y += heights[base+6])
		_gfx->drawMRG(&mrg, base+6, real_x1, border_y);
	_gfx->drawMRG(&mrg, base+6, real_x1, real_y2 - heights[base+2] - heights[base+6]);
	// right
	for (unsigned int border_y = real_y1 + heights[base+1]; border_y + heights[base+7] < real_y2 - heights[base+3]; border_y += heights[base+7])
		_gfx->drawMRG(&mrg, base+7, real_x2 - widths[base+7], border_y);
	_gfx->drawMRG(&mrg, base+7, real_x2 - widths[base+7], real_y2 - heights[base+3] - heights[base+7]);
	// corners
	_gfx->drawMRG(&mrg, base+0, real_x1, real_y1);
	_gfx->drawMRG(&mrg, base+1, real_x2 - widths[base+1], real_y1);
	_gfx->drawMRG(&mrg, base+2, real_x1, real_y2 - heights[base+2]);
	_gfx->drawMRG(&mrg, base+3, real_x2 - widths[base+3], real_y2 - heights[base+3]);

	if (with_icon && _icon) {
		_gfx->drawMRG(&mrg, 8, real_x1 - (widths[8] / 2) + (widths[base+6]/2), (real_y1+real_y2)/2 - (heights[8]/2));

		_icon->update();
		_gfx->drawSprite(_icon, real_x1 + (widths[base+6]/2) + 1, (real_y1+real_y2)/2 + (heights[8]/2) - 4);
	} else if (with_buttons) {
		// TODO: 11/12/13 up buttons (normal/highlight/grayed), 14/15/16 down buttons
		uint up_button = 13;
		_gfx->drawMRG(&mrg, up_button, real_x2 - (widths[up_button] / 2) - (widths[base+7]/2), (real_y1+real_y2)/2 - heights[up_button]);
		uint down_button = 16;
		_gfx->drawMRG(&mrg, down_button, real_x2 - (widths[down_button] / 2) - (widths[base+7]/2), (real_y1+real_y2)/2);
	}
}

void UnityEngine::initDialog() {
	const uint maxWidth = 313; // TODO: probably should be calculated

	_dialogSelected = (uint)-1;
	_dialogStartLine = 0;
	_dialogLines.clear();
	_dialogWidth = 0;

	::Graphics::Font *font = _gfx->getFont(2);

	if (!_choice_list.size()) {
		_dialogLines.resize(1);
		_dialogWidth = font->wordWrapText(_dialog_text, maxWidth, _dialogLines[0]);
	} else {
		_dialogLines.resize(_choice_list.size());
		for (uint i = 0; i < _choice_list.size(); i++) {
			Common::String our_str = _choice_list[i];
			Common::Array<Common::String> &lines = _dialogLines[i];
			uint newWidth = font->wordWrapText(our_str, maxWidth, lines);
			if (newWidth > _dialogWidth)
				_dialogWidth = newWidth;

			// add an empty spacing line if necessary
			if (i == _choice_list.size() - 1)
				continue;
			if (lines.empty() || !lines[lines.size() - 1].empty())
				lines.push_back(Common::String());
		}
	}
}

void UnityEngine::dialogMouseMove(const Common::Point &pos) {
	_dialogSelected = (uint)-1;

	for (uint i = 0; i < _dialogRects.size(); i++)
		if (_dialogRects[i].contains(pos))
			_dialogSelected = i;
}

void UnityEngine::dialogMouseClick(const Common::Point &pos) {
	// if there are no choices to be made, or a choice has been made, or we don't need a choice
	if (_choice_list.empty() || _dialogSelected != (uint)-1 || !_dialog_choosing) {
		_in_dialog = false;
		return;
	}
}

void UnityEngine::drawDialogWindow() {
	uint y = _dialog_y;

	const uint fontSpacing = 8;
	::Graphics::Font *font = _gfx->getFont(2);
	const uint fontHeight = font->getFontHeight();

	uint width = _dialogWidth;
	uint height = 0;

	for (uint i = 0; i < _dialogLines.size(); i++) {
		uint lineSize = _dialogLines[i].size();
		height += lineSize * fontHeight;
		if (lineSize)
			height += (lineSize - 1) * fontSpacing;
	}
	if (height > 160)
		height = 160;

	bool show_icon = _choice_list.size() == 0;
	bool show_buttons = _dialog_choosing;
	bool thick_frame = _choice_list.size() != 0 && (!_dialog_choosing);
	drawDialogFrameAround(_dialog_x, y, width, height, thick_frame, show_icon, show_buttons);

	_dialogRects.clear();

	::Graphics::Surface *surf = _system->lockScreen();
	if (!_choice_list.size()) {
		for (uint i = _dialogStartLine; i < _dialogLines[0].size(); i++) {
			if ((y - _dialog_y) + fontHeight > height)
				break;

			font->drawString(surf, _dialogLines[0][i], _dialog_x, y, width, 0);
			y += fontHeight + fontSpacing;
		}
	} else {
		uint linesSeen = 0;
		for (uint j = 0; j < _dialogLines.size(); j++) {
			uint beginLine = 0;
			if (linesSeen < _dialogStartLine)
				beginLine = _dialogStartLine - linesSeen;
			linesSeen += j;

			Common::Array<Common::String> &lines = _dialogLines[j];
			uint oldY = y;
			for (uint i = beginLine; i < lines.size(); i++) {
				if ((y - _dialog_y) + fontHeight > height) {
					// done drawing
					j = _dialogLines.size();
					break;
				}

				::Graphics::Font *thisFont = font;
				bool selected = (j == _dialogSelected);
				// font 2 is normal, font 3 is highlighted - but have identical metrics
				if (selected)
					thisFont = _gfx->getFont(3);

				thisFont->drawString(surf, lines[i], _dialog_x, y, width, 0);
				y += fontHeight;
				if (i != lines.size() - 1)
					y += fontSpacing;
			}
			_dialogRects.push_back(Common::Rect(_dialog_x, oldY, _dialog_x + width, y));
		}
	}
	_system->unlockScreen();

	// dialog window FRAME:
	// 0 is top left, 1 is top right, 2 is bottom left, 3 is bottom right
	// 4 is top, 5 is bottom, 6 is left, 7 is right?
	// 8 is icon frame (for left side), 9 is hilight box, 10 is not-hilight box
	// 11/12/13 up buttons (normal/highlight/grayed), 14/15/16 down buttons

	// options (lift, conference room, etc) window FRAME:
	// 17 top left, 18 top right, 19 bottom left, 20 bottom right, 21 top padding, 22 bottom padding,
	// 23 left padding, 24 right padding? then the buttons again, but for this mode: 25-30

	/*for (uint i = 0; i < 31; i++) {
		_gfx->drawMRG("dialog.mrg", i, 50 * (1 + i/10), 30 * (i%10));
	}*/
}

void UnityEngine::drawAwayTeamUI() {
	// draw UI
	MRGFile mrg;
	_gfx->loadMRG("awayteam.mrg", &mrg);
	_gfx->drawMRG(&mrg, 0, 0, 400);

	// notes on the UI elements not used here:
	// 1 has the normal admin menu, 2 the selected one
	// 22 is an empty phaser bar and 12 has an empty grey panel

	// away team member icon
	assert(_current_away_team_icon);
	_current_away_team_icon->update();
	_gfx->drawSprite(_current_away_team_icon, 75, 476);

	// away team member health bar
	_gfx->drawMRG(&mrg, 23, 100, 426);

	// mode icons
	_gfx->drawMRG(&mrg, 13 + (_mode == mode_Look ? 1 : 0), 108, 424);
	_gfx->drawMRG(&mrg, 19 + (_mode == mode_Use ? 1 : 0), 149, 424);
	_gfx->drawMRG(&mrg, 17 + (_mode == mode_Talk ? 1 : 0), 108, 451);
	// TODO: 21 is disabled walk?
	_gfx->drawMRG(&mrg, 15 + (_mode == mode_Walk ? 1 : 0), 149, 451);

	// status text
	_gfx->drawString(0, 403, _status_text.c_str(), 2);

	// inventory
	for (unsigned int i = 0; i < 6; i++) {
		unsigned int index = _inventory_index + i;
		if (index >= _inventory_icons.size()) break;

		// TODO: unverified coords
		unsigned int x;
		switch (i) {
		case 0: x = 267; break;
		case 1: x = 320; break;
		case 2: x = 373; break;
		case 3: x = 499; break;
		case 4: x = 552; break;
		case 5: x = 605; break;
		}
		unsigned int y = 475;

		SpritePlayer *icon = _inventory_icons[index];
		icon->update();
		_gfx->drawSprite(icon, x, y);
	}

	// TODO: inventory scrolling
	// 6/7/8 is left (normal/hilight/greyed)
	// 9/10/11 is right (normal/hilight/greyed)

	// TODO: phaser selection
	// 3/4/5 is the low/medium/high phaser selection
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
	data.loadExecutableData();
	data.loadMovieInfo();
	data.loadComputerDatabase();

	startupScreen();

	// TODO: are the indexes identical in all versions?
	_gfx->playMovie(data._movieFilenames[0]);
	_gfx->playMovie(data._movieFilenames[4]);
	// TODO: add #3 to the 'already played' list

	_gfx->setCursor(0xffffffff, false);

	startBridge();

	while (!shouldQuit()) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data._currentScreen.polygons);

		drawObjects();
		if (_on_away_team) {
			drawAwayTeamUI();
		} else {
			_currScreen->draw();
		}

		assert(!_in_dialog);

		while (_next_situation != 0xffffffff && !shouldQuit()) {
			assert(_next_conversation);
			unsigned int situation = _next_situation;
			_next_situation = 0xffffffff;

			// TODO: not always Picard! :(
			objectID speaker = objectID(0, 0, 0);
			if (_current_away_team_member) speaker = _current_away_team_member->id;
			_next_conversation->execute(this, data.getObject(speaker), situation);
		}

		processTriggers();
		processTimers();

		_system->updateScreen();
	}

	return Common::kNoError;
}

unsigned int UnityEngine::runDialogChoice(Conversation *conversation) {
	assert(conversation);
	assert(_dialog_choice_states.size() > 1);

	_dialog_text.clear();
	assert(!_choice_list.size());
	_choice_list.clear();
	_dialog_choosing = true;

	for (unsigned int i = 0; i < _dialog_choice_states.size(); i++) {
		Response *r = conversation->getResponse(_dialog_choice_situation,
			_dialog_choice_states[i]);
		assert(r);
		_choice_list.push_back(r->text + "\n");
	}
	runDialog();

	_choice_list.clear();
	_dialog_choosing = false;

	if (_dialogSelected == (uint)-1)
		return (uint)-1;
	return _dialog_choice_states[_dialogSelected];
}

void UnityEngine::runDialog() {
	initDialog();
	_in_dialog = true;
	_gfx->setCursor(0xffffffff, false);

	while (_in_dialog) {
		checkEvents();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(data._currentScreen.polygons);

		drawObjects();
		if (_on_away_team) {
			drawAwayTeamUI();
		} else {
			_currScreen->draw();
		}

		drawDialogWindow();
		if (_icon && _icon->playing() && !_snd->speechPlaying()) {
			_icon->startAnim(0); // static
		}

		_system->updateScreen();
	}

	// TODO: reset cursor
	_snd->stopSpeech();
}

ResultType UnityEngine::performAction(ActionType action_type, Object *target, objectID who, objectID other,
	unsigned int target_x, unsigned int target_y) {
	Action context;
	context.action_type = action_type;
	context.target = target;
	context.who = who;
	context.other = other;
	context.x = target_x;
	context.y = target_y;

	switch (action_type) {
		case ACTION_USE:
			if (!target) error("USE requires a target");

			// TODO..
			debug(1, "performAction: USE (on %s)", target->identify().c_str());
			return target->use_entries.execute(this, &context);
			break;

		case ACTION_GET:
			if (!target) error("GET requires a target");

			debug(1, "performAction: GET (on %s)", target->identify().c_str());
			return target->get_entries.execute(this, &context);
			break;

		case ACTION_LOOK:
			if (target) {
				debug(1, "performAction: LOOK (on %s)", target->identify().c_str());
				return target->look_entries.execute(this, &context);
				break;
			} else {
				warning("unimplemented: performAction: LOOK");
				// TODO
			}
			break;

		case ACTION_TIMER:
			if (!target) error("TIMER requires a target");

			debug(1, "performAction: TIMER (on %s)", target->identify().c_str());
			return target->timer_entries.execute(this, &context);
			break;

		case ACTION_WALK:
			if (target) {
				debug(1, "performAction: WALK (on %s)", target->identify().c_str());
				if (target->transition.world != 0xff) {
					Object *obj = data.getObject(target->transition);
					if (!obj) error("couldn't find transition object");

					// TODO: this is a silly hack
					return performAction(ACTION_USE, obj);
				}

				warning("unimplemented: performAction: WALK (on %s)", target->identify().c_str());

				// TODO
			} else {
				warning("unimplemented: performAction: WALK");
				// TODO
			}
			break;

		case ACTION_TALK:
			if (!target) error("we don't handle TALK without a valid target, should we?");
			// target is target (e.g. Pentara), other is source (e.g. Picard)
			// TODO..
			debug(1, "performAction: TALK (on %s)", target->identify().c_str());
			if (!target->talk_string.size()) {
				return RESULT_EMPTYTALK;
			} else {
				target->runHail(target->talk_string);
				return RESULT_DIDSOMETHING; // TODO: ??
			}
			break;

		default:
			error("performAction: unknown action type %x", action_type);
	}

	return 0;
}


} // Unity

