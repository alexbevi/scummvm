#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

#include "common/stream.h"
#include "graphics/surface.h"

#include "innocent/main_dat.h"

namespace Innocent {

enum StringSpecial {
	kStringCountSpacesTerminate = 2,
	kStringGlobalWord = 6,
	kStringSetColour = 7,
	kStringDefaultColour = 8,
	kStringCountSpacesIf0 = 0x0a,
	kStringCountSpacesIf1 = 0x0b
};

class Sprite : public ::Graphics::Surface {
public:
	void recolour(byte colour);
};

class Image : public ::Graphics::Surface {
public:
	Sprite *cut(Common::Rect rect) const;
};

class GraphicsMap;
class ProgDat;
class Program;

class Resources {
public:
	Resources();
	~Resources();
	void load();
	void init() { load(); }

	/**
	 * Load an image. Automatically consult maps to choose the right file.
	 * @param index image index,
	 * @param target buffer,
	 * @param size of the image,
	 * @param palette optional buffer to read the palette to (size 0x400).
	 */
	void loadImage(uint16 index, byte *target, uint16 size, byte *palette = 0) const;
	Image *loadImage(uint16 index) const;

	void loadInterfaceImage(byte *target, byte *palette = 0) {
		loadImage(_main->interfaceImageIndex(), target, 0x3c00, palette);
	}

	uint16 blockOfRoom(uint16 room) const;

	Program *loadCodeBlock(uint16 block) const;

	static void descramble(byte *data, uint32 length);

	byte *getGlobalByteVariable(uint16 var) const;
	byte *getGlobalWordVariable(uint16 var) const;

	::Graphics::Surface *loadBackdrop(uint16 index, byte *palette);

	/* pointer to the base of the main code */
	byte *mainBase() const;
	/* initial entry point offset */
	uint16 mainEntryPoint() const;

	friend class GraphicsMap;
	friend class ProgDat;

	Sprite *getGlyph(byte character) const;

private:
	Sprite *loadSprite(uint16 id) const;
	std::auto_ptr<MainDat> _main;
	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }
	ProgDat *progDat() const { return _progDat.get(); }

	static void readPalette(Common::ReadStream *stream, byte *palette);
	Common::ReadStream *imageStream(uint16 index) const;
	void loadGraphicFiles();

	static void decodeImage(Common::ReadStream *stream, byte *target, uint16 size);

	std::auto_ptr<GraphicsMap> _graphicsMap;
	std::auto_ptr<ProgDat> _progDat;

	std::auto_ptr<Common::SeekableReadStream> *_graphicFiles;
};

} // End of namespace Innocent

#endif
