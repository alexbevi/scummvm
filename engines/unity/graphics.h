#include "unity.h"
#include "sprite.h"

namespace Unity {

class SpritePlayer;

class Graphics {
public:
	Graphics(UnityEngine *engine);
	~Graphics();

	void init();
	void drawMRG(Common::String filename, unsigned int entry);
	void setBackgroundImage(Common::String filename);
	void drawBackgroundImage();
	void drawSprite(SpritePlayer *sprite, int x, int y);

protected:
	void blit(byte *data, int x, int y, unsigned int width, unsigned int height, byte transparent = COLOUR_BLANK);

	UnityEngine *_vm;
	byte *basePalette, *palette;
	unsigned int backgroundWidth, backgroundHeight;
	byte *backgroundPixels;
};

}

