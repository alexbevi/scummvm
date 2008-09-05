#include "innocent/main_dat.h"

#include "common/endian.h"
#include "common/util.h"

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
	kImagesCount		= 0x1C,
	kImageDirectory		= 0x1E,
	kGraphicFileCount	= 0x20,
	kGraphicFileNames	= 0x22,
	kWordVars			= 0x3A,
	kByteVars			= 0x3E,
	kEntryPoint			= 0x42,
	kInterfaceImgIdx	= 0xB4
};

void MainDat::readFile(SeekableReadStream &stream) {
	_dataLen = stream.readUint16LE();
	debug(kDataRead, "length of main data is %d", _dataLen);

	_data = new byte[_dataLen];
	stream.seek(0);
	stream.read(_data, _dataLen);
	descramble();

	stream.read(_footer, kFooterLen);

	_imageDirectory = _data + READ_LE_UINT16(_footer + kImageDirectory);
	debug(kDataRead, "image directory offset is %04x", _imageDirectory - _data);
}

void MainDat::descramble() {
	for (int i = 2; i < _dataLen; i++)
		_data[i] ^= 0x6f;
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
	debug(kAck, "finding file index of image 0x%x at offset 0x%x of directory", index, offset);
	uint16 fst = READ_LE_UINT16(_imageDirectory + offset);
	uint16 snd = READ_LE_UINT16(_imageDirectory + offset + 2);
	debug(kDataRead, "read 0x%04x 0x%04x", fst, snd);
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

byte *MainDat::getByteVar(uint16 index) {
	uint16 offset = READ_LE_UINT16(_footer + kByteVars);
	return _data + offset + index;
}

byte *MainDat::getWordVar(uint16 index) {
	uint16 offset = READ_LE_UINT16(_footer + kWordVars);
	return _data + offset + index * 2;
}

uint16 MainDat::interfaceImageIndex() const {
	return READ_LE_UINT16(_footer + kInterfaceImgIdx);
}

byte *MainDat::getEntryPoint() const {
	return _data + READ_LE_UINT16(_footer + kEntryPoint);
}

} // End of namespace Innocent
