#include "resources.h"

#include "common/file.h"
#include "common/util.h"

using namespace Common;

namespace Innocent {

Resources::Resources() : _main(new MainDat(this)) {
}

void Resources::load() {
	_main->load();
}

void Datafile::load() {
	File file;

	debug(2, "Resources::DataFile::load()ing %s", filename());
	if (!file.open(filename()))
		error("could not open %s", filename());

	readFile(file);
	file.close();
	debug(2, "Resources::DataFile::load()ed %s", filename());
}

MainDat::MainDat(Resources *res) : Datafile(res), _data(0) {}

MainDat::~MainDat() {
	if (_data)
		delete[] _data;
}

void MainDat::readFile(SeekableReadStream &stream) {
	_dataLen = stream.readUint16LE();
	debug(1, "length of main data is %d", _dataLen);

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

} // End of namespace Innocent
