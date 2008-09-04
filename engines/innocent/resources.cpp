#include "resources.h"

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

enum DebugLevels {
	kAcks = 2,
	kDataRead = 1
};

Resources::Resources() : _main(new MainDat(this)), _graphicsMap(new GraphicsMap(this)) {
}

void Resources::load() {
	_main->load();
	_graphicsMap->load();
}

void Datafile::load() {
	File file;

	debug(kAcks, "Resources::DataFile::load()ing %s", filename());
	if (!file.open(filename()))
		error("could not open %s", filename());

	readFile(file);
	file.close();
	debug(kAcks, "Resources::DataFile::load()ed %s", filename());
}

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

void GraphicsMap::readFile(SeekableReadStream &stream) {
	uint32 actually_read = stream.read(_data, 1200);

	_imgCount = actually_read / 4;
	debug(kDataRead, "loaded %d image entries from iuc_graf.dat", _imgCount);

	if (_imgCount != _resources->mainDat()->imagesCount())
		error("ILL Error: Incorrect graphics for current logic.");
}

} // End of namespace Innocent
