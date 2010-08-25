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
 * 0 = Transparent.
 * 1 = Black (#000000 in 24-bit RGB).
 * 2 = White (#FFFFFF in 24-bit RGB).
 */
static const byte sciMouseCursor[] = {
	1,1,0,0,0,0,0,0,0,0,0,
	1,2,1,0,0,0,0,0,0,0,0,
	1,2,2,1,0,0,0,0,0,0,0,
	1,2,2,2,1,0,0,0,0,0,0,
	1,2,2,2,2,1,0,0,0,0,0,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,2,2,2,2,2,1,0,0,0,
	1,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,1,0,
	1,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,1,0,0,0,0,
	1,2,1,0,1,2,2,1,0,0,0,
	1,1,0,0,1,2,2,1,0,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,1,2,2,1,0,0,
	0,0,0,0,0,0,1,2,2,1,0
};

UnityEngine::UnityEngine(OSystem *syst) : Engine(syst) {
}

UnityEngine::~UnityEngine() {
	delete _snd;
	delete _gfx;
	delete data.data;
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

	current_screen.entrypoints.clear();
	byte num_entrances = locstream->readByte();
	for (unsigned int e = 0; e < num_entrances; e++) {
		Common::Array<Common::Point> entrypoints;
		// XXX: read this properly
		uint16 unknown = locstream->readUint16BE(); // 0001, 0101, 0201, ...
		entrypoints.resize(4);
		for (unsigned int i = 0; i < 4; i++) {
			entrypoints[i].x = locstream->readUint16LE();
			entrypoints[i].y = locstream->readUint16LE();
		}
		current_screen.entrypoints.push_back(entrypoints);
	}

	delete locstream;

	// TODO: what do we do with all the objects?
	// for now, we just delete all but the away team
	for (unsigned int i = 4; i < data.objects.size(); i++) {
		delete data.objects[i]->sprite;
		delete data.objects[i];
	}
	data.objects.resize(4);

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

		Object *obj = new Object;
		obj->loadObject(this, id.world, id.screen, id.id);
		data.objects.push_back(obj);
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
	Common::Array<Object *> &objects = data.objects;

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
	const char *bridge_sprites[9] = {
		"brdgldor.spr", // Left Door (conference room)
		"brdgdoor.spr", // Door
		"brdgworf.spr", // Worf
		"brdgtroi.spr", // Troi
		"brdgdata.spr", // Data
		"brdgrikr.spr", // Riker
		"brdgpica.spr", // Picard
		"brdgtitl.spr", // Episode Title
		0 };

	for (unsigned int i = 0; bridge_sprites[i] != 0; i++) {
		Object *obj = new Object;
		obj->x = obj->y = 0;
		obj->z_adjust = 0xffff;
		obj->active = true;
		obj->scaled = false;
		obj->sprite = new SpritePlayer(new Sprite(data.openFile(bridge_sprites[i])), obj, this);;
		obj->sprite->startAnim(0);
		data.objects.push_back(obj);
	}

	_gfx->setBackgroundImage("bridge.rm");
}

void UnityEngine::startAwayTeam(unsigned int world, unsigned int screen) {
	// beam in an away team
	for (unsigned int i = 0; i < 4; i++) {
		Object *obj = new Object;
		obj->loadObject(this, 0, 0, i);
		data.objects.push_back(obj);
	}

	openLocation(world, screen);
	for (unsigned int i = 0; i < 4; i++) {
		data.objects[i]->x = current_screen.entrypoints[0][i].x;
		data.objects[i]->y = current_screen.entrypoints[0][i].y;
	}

	// draw UI
	_gfx->drawMRG("awayteam.mrg", 0);
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
	delete p;
}

void UnityEngine::processTriggers() {
	for (unsigned int i = 0; i < data.triggers.size(); i++) {
		if (data.triggers[i]->tick()) {
			printf("should run trigger %x\n", data.triggers[i]->id);
		}
	}
}

Common::Error UnityEngine::run() {
	Common::Array<Object *> &objects = data.objects;

	init();

	initGraphics(640, 480, true);
	_gfx->init();
	_snd->init();

	data.loadTriggers();
	data.loadSpriteFilenames();

	startupScreen();

	// XXX: this mouse cursor is borrowed from SCI
	CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 0);
	CursorMan.showMouse(true);

	// and we stomp over it anyway, but this only good for some situations :)
	_gfx->setCursor(0, false);

	unsigned int curr_loc = 4;
	unsigned int curr_screen = 1;
	startAwayTeam(curr_loc, curr_screen);

	unsigned int anim = 26;
	for (unsigned int i = 0; i < 4; i++) objects[i]->sprite->startAnim(anim);

	Common::Event event;
	while (!shouldQuit()) {
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
						objects[i]->x = current_screen.entrypoints[0][i].x;
						objects[i]->y = current_screen.entrypoints[0][i].y;
					}
					break;

				case Common::EVENT_MOUSEMOVE:
					if (objectAt(event.mouse.x, event.mouse.y)) {
						_gfx->setCursor(1, false);
					} else {
						_gfx->setCursor(0, false);
					}
					break;

				case Common::EVENT_LBUTTONUP: {
					Object *obj = objectAt(event.mouse.x, event.mouse.y);
					if (!obj) break;

					// TODO: this is very wrong, of course :)

					// use only Picard's entry for now
					unsigned int i = 0;
					while (i < obj->descriptions.size() &&
						obj->descriptions[i].entry_id != 0) i++;
					if (i == obj->descriptions.size()) break;

					Description &desc = obj->descriptions[i];

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
					} break;

				default:
					break;
			}
		}

		processTriggers();

		_gfx->drawBackgroundImage();
		_gfx->drawBackgroundPolys(current_screen.polygons);

		Common::Array<Object *> to_draw;
		for (unsigned int i = 0; i < objects.size(); i++) {
			if (objects[i]->sprite && objects[i]->active) {
				objects[i]->sprite->update();

				// TODO
				if (!objects[i]->sprite->valid()) { warning("invalid sprite?"); continue; }

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
				for (j = 0; j < current_screen.polygons.size(); j++) {
					ScreenPolygon &poly = current_screen.polygons[j];
					if (poly.type != 1) continue;

					unsigned int triangle;
					if (poly.insideTriangle(x, y, triangle)) {
						// TODO: interpolation
						scale = poly.triangles[triangle].distances[0];
						break;
					}
				}
				if (j == current_screen.polygons.size())
					warning("couldn't find poly for walkable at (%d, %d)", x, y);
			}
			_gfx->drawSprite(to_draw[i]->sprite, to_draw[i]->x, to_draw[i]->y, scale);
		}

		_system->updateScreen();
	}

	return Common::kNoError;
}

} // Unity

