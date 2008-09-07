#include "resources.h"

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

Common::ReadStream *Resources::imageStream(uint16 index) {
	debug(kAck, "seeking image %04x", index);
	uint16 file_index = _main->fileIndexOfImage(index);
	debug(kDataRead, "file index is %d", file_index);
	uint32 offset = _graphicsMap->offsetOfImage(index);
	debug(kDataRead, "offset is %04d", offset);

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

void Resources::loadImage(uint16 index, byte *target, uint16 size, byte *palette) {
	Common::ReadStream *file = imageStream(index);
	(void) file->readUint16LE();
	(void) file->readUint16LE(); // we know size alright

	decodeImage(file, target, size);

	if (!palette)
		return;

	file->readByte(); // skip zero

	readPalette(file, palette);
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

Program *Resources::getRoomScript(uint16 room) {
	uint16 program_id = _main->getRoomScriptId(room);
	return _progDat->getScript(program_id);
}

void Resources::descramble(byte *data, uint32 len) {
	for (uint32 i = 0; i < len; i++)
		data[i] ^= 0x6f;
}

byte *Resources::initialCode() const {
	return _main->getEntryPoint();
}

::Graphics::Surface *Resources::loadBackdrop(uint16 index, byte *palette) {
	Common::ReadStream *stream = imageStream(index);

	uint16 width = stream->readUint16LE();
	uint16 height = stream->readUint16LE();

	debug(2, "loading backdrop of size %dx%d", width, height);

	Graphics::Surface *backdrop = new Graphics::Surface;
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


} // End of namespace Innocent
