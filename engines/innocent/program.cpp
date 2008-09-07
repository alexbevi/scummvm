#include "innocent/program.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/resources.h"

namespace Innocent {

enum FooterOffsets {
	kEntryPointOffset = 0x0E
};

Program::Program(Common::ReadStream &file) {
	uint16 length = file.readUint16LE(); // for this length
	if (length > 25000)
		error("too large a program (%d)", length);

	_code = new byte[length];

	file.read(_code + 2, length - 2);
	Resources::descramble(_code + 2, length - 2);

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

byte *Program::localVariable(uint16 offset) {
	return _code + offset;
}

byte *Program::roomHandler(uint16 room) {
	byte *index = _code + 2;

	uint16 r;
	while ((r = READ_LE_UINT16(index)) != 0xffff)
		if (r == room) {
			index += 2;
			uint16 offset = READ_LE_UINT16(index);
			return _code + offset;
		}

	return 0;
}

} // End of namespace Innocent
