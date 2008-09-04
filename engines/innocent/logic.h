#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

#include "common/ptr.h"
#include "common/stream.h"

namespace Innocent {

class Logic {
public:
	Logic();
	~Logic();

	void load();

	uint16 graphicsCount() const;

private:
	std::auto_ptr<Common::MemoryReadStream> _mainData;

	static void descramble(byte *data, uint16 size);

	enum DescriptorOffsets {
		kDescriptorGraphicsCount = 0x1C,

		kDescriptorSize = 0xB6
	};

	std::auto_ptr<Common::MemoryReadStream> _descriptor;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
