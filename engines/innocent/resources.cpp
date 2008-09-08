#include "resources.h"

#include <algorithm>
#include <ext/algorithm>

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"
#include "graphics/surface.h"

#include "innocent/main_dat.h"
#include "innocent/graph_dat.h"
#include "innocent/prog_dat.h"

using namespace Common;
using namespace std;

namespace Innocent {

Resources::Resources() :
	_main(new MainDat(this)),
	_graphicsMap(new GraphicsMap(this)),
	_progDat(new ProgDat(this)),
	_graphicFiles(0) {}

Resources::~Resources() {
	if (_graphicFiles)
		delete[] _graphicFiles;
}

void Resources::load() {
	_main->load();
	_graphicsMap->load();

	loadGraphicFiles();

	_progDat->load();
}

byte *Resources::getGlobalByteVariable(uint16 var) const {
	return _main->getByteVariable(var);
}

byte *Resources::getGlobalWordVariable(uint16 var) const {
	return _main->getWordVariable(var);
}

void Resources::loadGraphicFiles() {
	const list<MainDat::GraphicFile> files(_main->graphicFiles());

	_graphicFiles = new auto_ptr<SeekableReadStream>[files.size()];

	auto_ptr<SeekableReadStream> *ptr = _graphicFiles;
	for (list<MainDat::GraphicFile>::const_iterator it = files.begin(); it != files.end(); ++it) {
		File *file = new File();
		file->open(String(it->filename));
		auto_ptr<SeekableReadStream> pointer(file);
		*(ptr++) = pointer;
		debug(kAck, "opened %s", it->filename.c_str());
	}
}

Common::ReadStream *Resources::imageStream(uint16 index) const {
	debug(4, "seeking image %04x", index);
	uint16 file_index = _main->fileIndexOfImage(index);
	debug(4, "file index is %d", file_index);
	uint32 offset = _graphicsMap->offsetOfImage(index);
	debug(4, "offset is %04d", offset);

	SeekableReadStream *file = _graphicFiles[file_index].get();
	file->seek(offset);

	return file;
}

void Resources::readPalette(Common::ReadStream *stream, byte *palette) {
	for(int i = 256; i; i--) { // convert to rgba
		stream->read(palette, 3);
		palette[3] = 0;
		palette += 4;
	}
}

void Resources::loadImage(uint16 index, byte *target, uint16 size, byte *palette) const {
	Common::ReadStream *file = imageStream(index);
	(void) file->readUint16LE();
	(void) file->readUint16LE(); // we know size alright

	decodeImage(file, target, size);

	if (!palette)
		return;

	file->readByte(); // skip zero

	readPalette(file, palette);
}

Image *Resources::loadImage(uint16 index) const {
	Image * img = new Image;
	img->create(320, 200, 8);
	debug(4, "image pitch is %d", img->pitch/8);
	assert(img->pitch == 320*8);
	loadImage(index, reinterpret_cast<byte *>(img->pixels), 320*200);
	return img;
}

void Resources::decodeImage(Common::ReadStream *stream, byte *target, uint16 size) {
	enum {
		kRunFlag = 0xc0
	};

	while (size) {
		byte color = stream->readByte();

		uint8 runLength = 1;
		if ((color & kRunFlag) == kRunFlag) {
			runLength = color & (~kRunFlag);
			color = stream->readByte();
		}

		for (; runLength; runLength--) {
			*(target++) = color;
			if (!--size)
				return;
		}
	}
}

uint16 Resources::blockOfRoom(uint16 room) const {
	return _main->getRoomScriptId(room);
}

Program *Resources::loadCodeBlock(uint16 block) const {
	return _progDat->getScript(block);
}

void Resources::descramble(byte *data, uint32 len) {
	for (uint32 i = 0; i < len; i++)
		data[i] ^= 0x6f;
}

byte *Resources::mainBase() const {
	return _main->_data;
}

uint16 Resources::mainEntryPoint() const {
	return _main->getEntryPoint() - mainBase();
}

Surface *Resources::loadBackdrop(uint16 index, byte *palette) {
	Common::ReadStream *stream = imageStream(index);

	uint16 width = stream->readUint16LE();
	uint16 height = stream->readUint16LE();

	debug(2, "loading backdrop of size %dx%d", width, height);

	Surface *backdrop = new Surface;
	backdrop->create(width, height, 8);
	debug(3, "surface created");

	decodeImage(stream, reinterpret_cast<byte *>(backdrop->pixels), width * height);
	debug(3, "stream decoded");

	stream->readByte(); // skip zero

	debug(3, "reading palette");
	readPalette(stream, palette);
	debug(3, "palette read");

	return backdrop;
}

Sprite *Resources::getGlyph(byte ch) const {
	if (ch <= ' ' || ch > '~')
		return 0;
	debug(4, "looking for glyph '%c'", ch);
	uint16 id = _main->getGlyphSpriteId(ch);
	Sprite *s = loadSprite(id);
	debug(4, "got glyph for '%c', %dx%d", ch, s->w, s->h);
	return s;
}

Sprite *Resources::loadSprite(uint16 id) const {
	SpriteInfo info = _main->getSpriteInfo(id);
	Image *image = loadImage(info.image);
	Sprite *sprite = image->cut(Common::Rect(info.left, info.top, info.left + info.width, info.top + info.height));
	delete image;
	return sprite;
}

Sprite *Image::cut(Common::Rect rect) const {
	Sprite *sprite = new Sprite;
	sprite->create(rect.width(), rect.height(), 8);

	const byte *src = reinterpret_cast<const byte *>(getBasePtr(rect.left, rect.top));
	byte *dest = reinterpret_cast<byte *>(sprite->pixels);
	for (uint16 y = 0; y < rect.height(); y++) {
		// TODO make it portable across compilers (probably replacing with std::copy is ok)
		__gnu_cxx::copy_n(src, rect.width(), dest);
		src += pitch / 8;
		dest += sprite->pitch / 8;
	}
	return sprite;
}

enum {
	kChangeableColour = 235
};

void Sprite::recolour(byte colour) {
	byte *data = reinterpret_cast<byte *>(pixels);
	std::replace(data, data + h * pitch/8, byte(kChangeableColour), colour);
}

} // End of namespace Innocent
