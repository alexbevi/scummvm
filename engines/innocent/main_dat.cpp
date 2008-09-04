#include "innocent/main_dat.h"

#include "common/endian.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

MainDat::MainDat(Resources *res) : Datafile(res), _data(0) {}

MainDat::~MainDat() {
	if (_data)
		delete[] _data;
}

void MainDat::readFile(SeekableReadStream &stream) {
	_dataLen = stream.readUint16LE();
	debug(kDataRead, "length of main data is %d", _dataLen);

	_data = new byte[_dataLen];
	stream.seek(0);
	stream.read(_data, _dataLen);
	descramble();

	stream.read(_footer, kFooterLen);
}

void MainDat::descramble() {
	for (int i = 2; i < _dataLen; i++)
		_data[i] ^= 0x6f;
}

uint16 MainDat::imagesCount() const {
	return READ_LE_UINT16(_footer + kImagesCount);
}

} // End of namespace Innocent
