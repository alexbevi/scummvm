#include "unity.h"
#include "sprite.h"
#include "common/rect.h"

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
	void drawBackgroundPolys(Common::String filename);
	void renderPolygonEdge(Common::Array<Common::Point> &points, byte colour);

protected:
	void blit(byte *data, int x, int y, unsigned int width, unsigned int height, byte transparent = COLOUR_BLANK);

	UnityEngine *_vm;
	byte *basePalette, *palette;
	unsigned int backgroundWidth, backgroundHeight;
	byte *backgroundPixels;
};

}

