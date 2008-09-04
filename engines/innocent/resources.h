#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

#include "common/stream.h"

#include "innocent/main_dat.h"

namespace Innocent {

class GraphicsMap;
class ProgDat;

class Resources {
public:
	Resources();
	~Resources();
	void load();
	void init() { load(); }

	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }
	ProgDat *progDat() const { return _progDat.get(); }

	/**
	 * Load an image. Automatically consult maps to choose the right file.
	 * @param index image index,
	 * @param target buffer,
	 * @param size of the image,
	 * @param palette optional buffer to read the palette to (size 0x400).
	 */
	void loadImage(uint16 index, byte *target, uint16 size, byte *palette = 0);

	void loadInterfaceImage(byte *target, byte *palette = 0) {
		loadImage(_main->interfaceImageIndex(), target, 0x3c00, palette);
	}

	std::auto_ptr<MainDat> _main;

private:
	void loadGraphicFiles();

	static void decodeImage(Common::ReadStream *stream, byte *target, uint16 size);

	std::auto_ptr<GraphicsMap> _graphicsMap;
	std::auto_ptr<ProgDat> _progDat;

	std::auto_ptr<Common::SeekableReadStream> *_graphicFiles;
};

} // End of namespace Innocent

#endif
