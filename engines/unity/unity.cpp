#include "unity.h"
#include "graphics.h"
#include "sound.h"
#include "sprite.h"
#include "object.h"

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
	delete data;
}

Common::Error UnityEngine::init() {
	g_eventRec.registerRandomSource(_rnd, "unity");

	_gfx = new Graphics(this);
	_snd = new Sound(this);

	data = Common::makeZipArchive("STTNG.ZIP");
	if (!data) {
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
	SearchMan.add("sttngzip", data);

	return Common::kNoError;
}

void UnityEngine::openLocation(unsigned int world, unsigned int screen) {
	Common::String filename = Common::String::printf("sl%03d.scr", world);
	Common::SeekableReadStream *locstream = openFile(filename);

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

	loadScreenPolys(polygons);

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
	for (unsigned int i = 4; i < objects.size(); i++) {
		delete objects[i]->sprite;
		delete objects[i];
	}
	objects.resize(4);

	filename = Common::String::printf("w%02x%02xobj.bst", world, screen);
	locstream = openFile(filename);

	// TODO: is there data we need here in w_XXstrt.bst?
	// (seems to be only trigger activations)
	while (true) {
		uint16 counter = locstream->readUint16LE();
		if (locstream->eos()) break;

		byte id = locstream->readByte();
		assert(id == counter);
		byte _screen = locstream->readByte();
		assert(_screen == screen);
		byte _world = locstream->readByte();
		assert(_world == world);

		byte unknown4 = locstream->readByte();
		assert(unknown4 == 0);

		char _name[30], _desc[260];
		locstream->read(_name, 30);
		locstream->read(_desc, 260);
		//printf("reading obj '%s' (%s)\n", _name, _desc);

		Object *obj = new Object;
		obj->loadObject(this, world, screen, id);
		objects.push_back(obj);
	}

	delete locstream;
}

void UnityEngine::loadScreenPolys(Common::String filename) {
	Common::SeekableReadStream *mrgStream = openFile(filename);

	uint16 num_entries = mrgStream->readUint16LE();
	Common::Array<uint32> offsets;
	Common::Array<uint32> ids;
	offsets.reserve(num_entries);
	for (unsigned int i = 0; i < num_entries; i++) {
		uint32 id = mrgStream->readUint32LE();
		ids.push_back(id);
		uint32 offset = mrgStream->readUint32LE();
		offsets.push_back(offset);
	}

	current_screen.polygons.clear();
	current_screen.polygons.reserve(num_entries);
	for (unsigned int i = 0; i < num_entries; i++) {
		ScreenPolygon poly;

		bool r = mrgStream->seek(offsets[i]);
		assert(r);

		poly.id = ids[i];
		poly.type = mrgStream->readByte();
		assert(poly.type == 0 || poly.type == 1 || poly.type == 3 || poly.type == 4);

		uint16 something2 = mrgStream->readUint16LE();
		assert(something2 == 0);

		byte count = mrgStream->readByte();
		for (unsigned int j = 0; j < count; j++) {
			uint16 x = mrgStream->readUint16LE();
			uint16 y = mrgStream->readUint16LE();

			// 0-256, higher is nearer (larger characters);
			// (maybe 0 means not shown at all?)
			uint16 distance = mrgStream->readUint16LE();
			assert(distance <= 0x100);

			poly.distances.push_back(distance);
			poly.points.push_back(Common::Point(x, y));
		}

		for (unsigned int p = 2; p < poly.points.size(); p++) {
			// make a list of triangle vertices (0, p - 1, p)
			// this makes the code easier to understand for now
			Triangle tri;
			tri.points[0] = poly.points[0];
			tri.distances[0] = poly.distances[0];
			tri.points[1] = poly.points[p - 1];
			tri.distances[1] = poly.distances[p - 1];
			tri.points[2] = poly.points[p];
			tri.distances[2] = poly.distances[p];
			poly.triangles.push_back(tri);
		}

		current_screen.polygons.push_back(poly);
	}

	delete mrgStream;
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

Common::Error UnityEngine::run() {
	init();

	initGraphics(640, 480, true);
	_gfx->init();
	_snd->init();

	loadSpriteFilenames();

	// XXX: this mouse cursor is borrowed from SCI
	CursorMan.replaceCursor(sciMouseCursor, 11, 16, 1, 1, 0);
	CursorMan.showMouse(true);

	// and we stomp over it anyway, but this only good for some situations :)
	_gfx->setCursor(0, false);

	//_snd->playSpeech("02140000.vac");

	// beam in an away team
	for (unsigned int i = 0; i < 4; i++) {
		Object *obj = new Object;
		obj->loadObject(this, 0, 0, i);
		objects.push_back(obj);
	}
	unsigned int anim = 26;
	for (unsigned int i = 0; i < 4; i++) objects[i]->sprite->startAnim(anim);

	unsigned int curr_loc = 4;
	unsigned int curr_screen = 1;
	openLocation(curr_loc, curr_screen);
	for (unsigned int i = 0; i < 4; i++) {
		objects[i]->x = current_screen.entrypoints[0][i].x;
		objects[i]->y = current_screen.entrypoints[0][i].y;
	}

	// draw UI
	_gfx->drawMRG("awayteam.mrg", 0);

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

Common::SeekableReadStream *UnityEngine::openFile(Common::String filename) {
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(filename);
	if (!stream) error("couldn't open '%s'", filename.c_str());
	return stream;
}

void UnityEngine::loadSpriteFilenames() {
	Common::SeekableReadStream *stream = openFile("sprite.lst");

	uint32 num_sprites = stream->readUint32LE();
	sprite_filenames.reserve(num_sprites);

	for (unsigned int i = 0; i < num_sprites; i++) {
		char buf[9]; // DOS filenames, so 9 should really be enough
		for (unsigned int j = 0; j < 9; j++) {
			char c = stream->readByte();
			buf[j] = c;
			if (c == 0) break;
			assert (j != 8);
		}
		sprite_filenames.push_back(buf);
	}

	delete stream;
}

Common::String UnityEngine::getSpriteFilename(unsigned int id) {
	assert(id != 0xffff && id != 0xfffe);
	assert(id < sprite_filenames.size());
	return sprite_filenames[id] + ".spr";
}

} // Unity

