#include "innocent/main_dat.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/resources.h"

using namespace Common;
using namespace std;

namespace Innocent {

MainDat::MainDat(Resources *res) : Datafile(res), _data(0) {}

MainDat::~MainDat() {
	if (_data)
		delete[] _data;
}

enum Offsets {
	kProgEntriesCount0	= 0x06,
	kProgEntriesCount1	= 0x08,
	kProgramsMap		= 0x0A,
	kSpriteCount		= 0x18,
	kSpriteMap			= 0x1A,
	kImagesCount		= 0x1C,
	kImageDirectory		= 0x1E,
	kGraphicFileCount	= 0x20,
	kGraphicFileNames	= 0x22,
	kWordVars			= 0x3A,
	kByteVars			= 0x3E,
	kEntryPoint			= 0x42,
	kCharacterMap		= 0x48,
	kInterfaceImgIdx	= 0xB4
};

void MainDat::readFile(SeekableReadStream &stream) {
	_dataLen = stream.readUint16LE();
	debug(2, "length of main data is %d", _dataLen);

	_data = new byte[_dataLen];
	stream.seek(0);
	stream.read(_data, _dataLen);
	Resources::descramble(_data + 2, _dataLen - 2);

	stream.read(_footer, kFooterLen);

	_imageDirectory = _data + READ_LE_UINT16(_footer + kImageDirectory);
	debug(2, "image directory offset is %04x", _imageDirectory - _data);

	_programsCount = READ_LE_UINT16(_footer + kProgEntriesCount1);

	_programsMap = _data + READ_LE_UINT16(_footer + kProgramsMap);
}

uint16 MainDat::imagesCount() const {
	return READ_LE_UINT16(_footer + kImagesCount);
}

uint16 MainDat::progEntriesCount0() const {
	return READ_LE_UINT16(_footer + kProgEntriesCount0);
}

uint16 MainDat::progEntriesCount1() const {
	return READ_LE_UINT16(_footer + kProgEntriesCount1);
}

uint16 MainDat::fileIndexOfImage(uint16 index) const {
	uint32 offset = (index - 1) * 4;
	debug(3, "finding file index of image 0x%x at offset 0x%x of directory", index, offset);
	uint16 fst = READ_LE_UINT16(_imageDirectory + offset);
	uint16 snd = READ_LE_UINT16(_imageDirectory + offset + 2);
	debug(3, "read 0x%04x 0x%04x", fst, snd);
	return snd;
}

list<MainDat::GraphicFile> MainDat::graphicFiles() const {
	uint16 file_count = READ_LE_UINT16(_footer + kGraphicFileCount);
	uint16 names_offset = READ_LE_UINT16(_footer + kGraphicFileNames);

	byte *data = _data + names_offset;
	list<GraphicFile> files;
	for (; file_count > 0; file_count--) {
		GraphicFile file;
		file.data_set = READ_LE_UINT16(data);
		data += 2;
		file.filename = reinterpret_cast<char *>(data);
		files.push_back(file);
		while (*data)
			data++;
		while (!*data)
			data++;
	}

	return files;
}

byte *MainDat::getByteVariable(uint16 index) {
	uint16 offset = READ_LE_UINT16(_footer + kByteVars);
	return _data + offset + index;
}

byte *MainDat::getWordVariable(uint16 index) {
	uint16 offset = READ_LE_UINT16(_footer + kWordVars);
	return _data + offset + index * 2;
}

uint16 MainDat::interfaceImageIndex() const {
	return READ_LE_UINT16(_footer + kInterfaceImgIdx);
}

byte *MainDat::getEntryPoint() const {
	return _data + READ_LE_UINT16(_footer + kEntryPoint);
}

uint16 MainDat::getRoomScriptId(uint16 room) const {
	debug(4, "looking for script for room 0x%04x", room);

	byte *programInfo = _programsMap;
	for (int i = 1; i <= _programsCount; i++) {
		debug(4, "trying dataset 0x%04x", READ_LE_UINT16(programInfo));
		programInfo += 2;

		uint16 this_room;
		while ((this_room = READ_LE_UINT16(programInfo)) != 0xffff) {
			debug(4, "trying 0x%04x", this_room);
			if (this_room == room)
				return i;
			else
				programInfo += 2;
		}
	}

	return 0;
}

uint16 MainDat::getGlyphSpriteId(byte character) const {
	byte *charmap = _data + READ_LE_UINT16(_footer + kCharacterMap);
	charmap += (character - ' ') * 2;
	uint16 id = READ_LE_UINT16(charmap);
	debug(4, "searching for sprite of glyph '%c', %d found at 0x%04x", character, id, charmap - _data);
	return id;
}

enum SpriteMap {
	kSpriteMapImage = 0,
	kSpriteMapLeft = 2,
	kSpriteMapTop = 4,
	kSpriteMapWidth = 6,
	kSpriteMapHeight = 7,
	kSpriteMapSize = 0xa
};

SpriteInfo MainDat::getSpriteInfo(uint16 index) const {
	SpriteInfo si;
	byte *spritemap = _data + READ_LE_UINT16(_footer + kSpriteMap);
	uint16 nsprites = READ_LE_UINT16(_footer + kSpriteCount);
	if (index >= nsprites)
		error("local sprites not handled yet (index: 0x%04x)", index);

	spritemap += index * kSpriteMapSize;
	si.top = READ_LE_UINT16(spritemap + kSpriteMapTop);
	si.left = READ_LE_UINT16(spritemap + kSpriteMapLeft);
	si.width = spritemap[kSpriteMapWidth];
	si.height = spritemap[kSpriteMapHeight];
	si.image = READ_LE_UINT16(spritemap + kSpriteMapImage);
	debug(3, "found sprite info at 0x%04x for %d: %s", spritemap - _data, index, si.inspect());

	return si;
}

char SpriteInfo::_debugBuf[100];

const char *SpriteInfo::inspect() const {
	snprintf(_debugBuf, 100, "%d#%d:%d:%dx%d", image, left, top, width, height);
	return _debugBuf;
}

} // End of namespace Innocent
