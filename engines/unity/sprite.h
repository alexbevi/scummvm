#include "common/stream.h"

namespace Unity {

class Sprite {
public:
	Sprite(Common::SeekableReadStream *_str);
	~Sprite();

protected:
	Common::SeekableReadStream *_stream;

	void readBlock();
	void parseBlock(char blockType[4], uint32 size);

	bool _isSprite;
};

} // Unity

