#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include "common/stream.h"

namespace Innocent {

class MainDat {
public:
	MainDat();
	~MainDat();

	void open(const char *filename);
	void load(Common::SeekableReadStream &sourceS);

private:
	byte *_data;

	static void descramble(byte *data, uint16 size);

	struct Footer {
		byte dummy[0xB6];
	};

	struct Footer _footer;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
