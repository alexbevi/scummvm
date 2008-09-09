#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

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

enum StringSpecial {
	kStringCountSpacesTerminate = 2,
	kStringGlobalWord = 6,
	kStringSetColour = 7,
	kStringDefaultColour = 8,
	kStringCountSpacesIf0 = 0x0a,
	kStringCountSpacesIf1 = 0x0b
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

class GraphicsMap;
class ProgDat;
class Program;

class Resources {
public:
	Resources(Engine *vm);
	~Resources();
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

	friend class GraphicsMap;
	friend class ProgDat;

	SpriteInfo getSpriteInfo(uint16 id) const;
	Sprite *getGlyph(byte character) const;
	Sprite *loadSprite(uint16 id) const;
	Sprite *getCursor() const;
	Sprite **frames() { return _frames; }

	void loadActors();
	void loadFrames();

private:
	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }
	ProgDat *progDat() const { return _progDat.get(); }

	static void readPalette(Common::ReadStream *stream, byte *palette);
	Common::ReadStream *imageStream(uint16 index) const;
	void loadGraphicFiles();

	static void decodeImage(Common::ReadStream *stream, byte *target, uint16 size);

	Engine *_vm;

	std::auto_ptr<MainDat> _main;
	std::auto_ptr<GraphicsMap> _graphicsMap;
	std::auto_ptr<ProgDat> _progDat;

	std::auto_ptr<Common::SeekableReadStream> *_graphicFiles;

	Sprite *_frames[9];
};

} // End of namespace Innocent

#endif
