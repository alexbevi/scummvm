#include "innocent/main_dat.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/actor.h"
#include "innocent/resources.h"
#include "innocent/util.h"

using namespace Common;
using namespace std;

namespace Innocent {

MainDat::MainDat(Resources *res) : Datafile(res), _data(0), _actors(0) {}

MainDat::~MainDat() {
	if (_data)
		delete[] _data;

	if (_actors) {
		for (int i = 0; i < _actorsCount; i++)
			delete _actors[i];
		delete[] _actors;
	}
}

enum Offsets {
	kProgEntriesCount0	= 0x06,
	kProgEntriesCount1	= 0x08,
	kProgramsMap		= 0x0A,
	kActorsCount		= 0x10,
	kActors 			= 0x12,
	kSpriteCount		= 0x18,
	kSpriteMap			= 0x1A,
	kImagesCount		= 0x1C,
	kImageDirectory		= 0x1E,
	kGraphicFileCount	= 0x20,
	kGraphicFileNames	= 0x22,
	kTunesCount			= 0x2A,
	kTunesDirectory		= 0x2C,
	kMusicFileCount		= 0x2E,
	kMusicFileNames		= 0x30,
	kWordVars			= 0x3A,
	kByteVars			= 0x3E,
	kEntryPoint			= 0x42,
	kCharacterMap		= 0x48,
//	kCursors			= 0x54, another set
	kCursors			= 0x58,
	kInterfaceImgIdx	= 0xB4,

	kFrameTopLeftOffset = 0x76,
	kFrameTopOffset = 0x7e,
	kFrameTopRightOffset = 0x84,
	kFrameLeftOffset = 0x7a,
	kFrameFillOffset = 0x80,
	kFrameRightOffset = 0x88,
	kFrameBottomLeftOffset = 0x7c,
	kFrameBottomOffset = 0x82,
	kFrameBottomRightOffset = 0x8a
};

void MainDat::readFile(SeekableReadStream &stream) {
	_dataLen = stream.readUint16LE();

	_data = new byte[_dataLen];
	stream.seek(0);
	stream.read(_data, _dataLen);
	Resources::descramble(_data + 2, _dataLen - 2);

	stream.read(_footer, kFooterLen);

	_imageDirectory = _data + READ_LE_UINT16(_footer + kImageDirectory);
	_tunesDirectory = _data + READ_LE_UINT16(_footer + kTunesDirectory);

	_programsCount = READ_LE_UINT16(_footer + kProgEntriesCount1);

	_programsMap = _data + READ_LE_UINT16(_footer + kProgramsMap);
}

void MainDat::loadActors(Interpreter *in) {
	uint16 nactors = _actorsCount = READ_LE_UINT16(_footer + kActorsCount);
	uint16 actors = READ_LE_UINT16(_footer + kActors);
	assert(!_actors);
	_actors = new Actor *[nactors];
	for (int i = 0; i < nactors; ++i) {
		_actors[i] = new Actor(CodePointer(actors, in));
		actors += Actor::Size;
	}
}

uint16 MainDat::imagesCount() const {
	return READ_LE_UINT16(_footer + kImagesCount);
}

uint16 MainDat::tunesCount() const {
	return READ_LE_UINT16(_footer + kTunesCount);
}

uint16 MainDat::progEntriesCount0() const {
	return READ_LE_UINT16(_footer + kProgEntriesCount0);
}

uint16 MainDat::progEntriesCount1() const {
	return READ_LE_UINT16(_footer + kProgEntriesCount1);
}

uint16 MainDat::fileIndexOfImage(uint16 index) const {
	uint32 offset = (index - 1) * 4;
	(void) READ_LE_UINT16(_imageDirectory + offset);
	uint16 snd = READ_LE_UINT16(_imageDirectory + offset + 2);
	return snd;
}

uint16 MainDat::fileIndexOfTune(uint16 index) const {
	uint32 offset = (index - 1) * 2;
	return READ_LE_UINT16(_tunesDirectory + offset);
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

list<Common::String> MainDat::musicFiles() const {
	uint16 file_count = READ_LE_UINT16(_footer + kMusicFileCount);
	uint16 names_offset = READ_LE_UINT16(_footer + kMusicFileNames);

	byte *data = _data + names_offset;
	list<Common::String> files;
	for (; file_count > 0; file_count--) {
		data += 2; // data set id
		data++; // music type (1 - adlib, 4 - roland)
		Common::String file(reinterpret_cast<char *>(data));
		files.push_back(file);
		while (*data)
			data++;
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

Actor *MainDat::actor(uint16 index) const {
	return _actors[index];
}

uint16 MainDat::getRoomScriptId(uint16 room) const {

	byte *programInfo = _programsMap;
	for (int i = 1; i <= _programsCount; i++) {
		programInfo += 2;

		uint16 this_room;
		while ((this_room = READ_LE_UINT16(programInfo)) != 0xffff) {
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
	return id;
}

uint16 MainDat::spriteCount() const {
	return READ_LE_UINT16(_footer + kSpriteCount);
}

SpriteInfo MainDat::getSpriteInfo(uint16 index) const {
	byte *spritemap = _data + READ_LE_UINT16(_footer + kSpriteMap);
	if (index >= spriteCount())
		error("local sprite index given (index: 0x%04x)", index);

	return SpriteInfo(spritemap, index);
}

uint16 MainDat::getCursorSpriteId() const {
//	uint16 offset = READ_LE_UINT16(_footer + kCursors);
	uint16 sprite = 0x6c;
	debugC(1, kDebugLevelGraphics | kDebugLevelFiles, "loading cursor STUB, sprite %d", sprite);
	return sprite;
}


uint16 MainDat::getFrameId(FramePart part) const {
	switch (part) {
	#define PART(p) case p: return READ_LE_UINT16(_footer + p##Offset)
	PART(kFrameBottom);
	PART(kFrameBottomLeft);
	PART(kFrameBottomRight);
	PART(kFrameFill);
	PART(kFrameLeft);
	PART(kFrameRight);
	PART(kFrameTop);
	PART(kFrameTopLeft);
	PART(kFrameTopRight);
	#undef PART
	default: assert(false);
	}
}


} // End of namespace Innocent
