#include "common/stream.h"

#include "common/array.h" // XXX

namespace Unity {

class Sprite {
public:
	Sprite(Common::SeekableReadStream *_str);
	~Sprite();

	Common::Array<byte *> sprites;
	Common::Array<uint32> widths;
	Common::Array<uint32> heights;

protected:
	Common::SeekableReadStream *_stream;

	void readBlock();
	void parseBlock(char blockType[4], uint32 size);

	void readCompressedImage(uint32 size);
	void decodeSpriteTypeOne(byte *buf, unsigned int size, byte *data, unsigned int width, unsigned int height);
	void decodeSpriteTypeTwo(byte *buf, unsigned int size, byte *data, unsigned int targetsize);

	bool _isSprite;
};

} // Unity

