#include "resources.h"

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"

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

void Resources::loadImage(uint16 index, byte *target, uint16 size, byte *palette) {
	debug(kAck, "loading image %04x", index);
	uint16 file_index = _main->fileIndexOfImage(index);
	debug(kDataRead, "file index is %d", file_index);
	uint32 offset = _graphicsMap->offsetOfImage(index);
	debug(kDataRead, "offset is %04d", offset);

	SeekableReadStream *file = _graphicFiles[file_index].get();
	file->seek(offset + 4);

	decodeImage(file, target, size);

	if (!palette)
		return;

	file->readByte(); // skip zero
	for(int i = 256; i; i--) { // convert to rgba
		file->read(palette, 3);
		palette += 4;
	}
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

} // End of namespace Innocent
