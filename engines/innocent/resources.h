#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

#include "common/singleton.h"
#include "common/stream.h"
#include "graphics/surface.h"

#include "innocent/main_dat.h"
#include "innocent/types.h"

namespace Innocent {
//

class Engine;

class Surface : public ::Graphics::Surface { // this surface autodestructs properly
public:
	~Surface() { free(); }
	void blit(const Surface *s, int transparent = -1) {
		blit(s, Common::Point(0, 0), transparent);
	}
	void blit(const Surface *s, Common::Point p, int transparent = -1) {
		blit(s, Common::Rect(p.x, p.y, s->w, s->h), transparent);
	}
	void blit(const Surface *s, Common::Rect r, int transparent = -1);
};

class Sprite : public Surface {
public:
	void recolour(byte colour);
	Common::Point _hotPoint;
};

class Image : public Surface {
public:
	Sprite *cut(Common::Rect rect) const;
};

class MapFile;
class ProgDat;
class Program;

class Resources : public Common::Singleton<Resources> {
public:
	Resources() {}
	~Resources();
	void setEngine(Engine *e);
	void load();
	void init();

	/**
	 * Load an image. Automatically consult maps to choose the right file.
	 * @param index image index,
	 * @param target buffer,
	 * @param size of the image,
	 * @param palette optional buffer to read the palette to (size 0x400).
	 */
	void loadImage(uint16 index, byte *target, uint16 size, byte *palette = 0) const;

	/**
	 * Loads an image given index.
	 * @returns pointer to the image. Please don't delete it, Resources owns it.
	 */
	Image *loadImage(uint16 index) const;
	void loadTune(uint16 index, byte *target) const;

	Common::ReadStream *tuneStream(uint16 index) const;

	void loadInterfaceImage(byte *target, byte *palette = 0) {
		loadImage(_main->interfaceImageIndex(), target, 0x3c00, palette);
	}

	uint16 blockOfRoom(uint16 room) const;

	Program *loadCodeBlock(uint16 block) const;

	static void descramble(byte *data, uint32 length);

	byte *getGlobalByteVariable(uint16 var) const;
	byte *getGlobalWordVariable(uint16 var) const;

	Surface *loadBackdrop(uint16 index, byte *palette);

	/* pointer to the base of the main code */
	byte *mainBase() const;
	/* initial entry point offset */
	uint16 mainEntryPoint() const;

	friend class ProgDat;

	SpriteInfo getSpriteInfo(uint16 id) const;
	Sprite *getGlyph(byte character) const;
	Sprite *loadSprite(uint16 id) const;
	Sprite *getCursor() const;
	Sprite **frames() { return _frames; }

	void loadActors();
	void loadFrames();

	MainDat *mainDat() const { return _main.get(); }

	static void decodeImage(Common::ReadStream *stream, byte *target, uint16 size);
	static void readPalette(Common::ReadStream *stream, byte *palette);

private:
	MapFile *graphicsMap() const { return _graphicsMap.get(); }
	MapFile *tuneMap() const { return _tuneMap.get(); }
	ProgDat *progDat() const { return _progDat.get(); }

	Common::ReadStream *imageStream(uint16 index) const;
	void loadGraphicFiles();
	void loadMusicFiles();

	Engine *_vm;

	std::auto_ptr<MainDat> _main;
	std::auto_ptr<MapFile> _graphicsMap;
	std::auto_ptr<MapFile> _tuneMap;
	std::auto_ptr<ProgDat> _progDat;

	std::auto_ptr<Common::SeekableReadStream> *_graphicFiles;
	std::auto_ptr<Common::SeekableReadStream> *_musicFiles;

	Sprite *_frames[9];
};

#define Res Resources::instance()

} // End of namespace Innocent

#endif
