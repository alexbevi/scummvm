#include "innocent/program.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/resources.h"

namespace Innocent {

enum FooterOffsets {
	kEntryPointOffset = 0x0E
};

Program::Program(Common::ReadStream &file) {
	uint16 length = file.readUint16LE() - 2; // for this length
	if (length > 25000)
		error("too large a program (%d)", length);

	_code = new byte[length];

	file.read(_code, length);
	Resources::descramble(_code, length);

	file.read(_footer, 0x10);
}

Program::~Program() {
	delete[] _code;
}

byte *Program::begin() {
	return _code + entryPointOffset();
}

uint16 Program::entryPointOffset() {
	return READ_LE_UINT16(_footer + kEntryPointOffset);
}

} // End of namespace Innocent
