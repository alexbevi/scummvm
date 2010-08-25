#include "unity.h"
#include "sprite.h"
#include "common/rect.h"

namespace Unity {

class SpritePlayer;

struct Image {
	unsigned int width;
	unsigned int height;
	byte *data;
};

class Graphics {
public:
	Graphics(UnityEngine *engine);
	~Graphics();

	void init();

	void setCursor(unsigned int id, bool wait);
	void setBackgroundImage(Common::String filename);

	void drawString(unsigned int x, unsigned int y, Common::String text, unsigned int font);
	void drawMRG(Common::String filename, unsigned int entry);
	void drawBackgroundImage();
	void drawSprite(SpritePlayer *sprite, int x, int y, unsigned int scale = 256);
	void drawBackgroundPolys(Common::Array<struct ScreenPolygon> &polys);
	void renderPolygonEdge(Common::Array<Common::Point> &points, byte colour);

protected:
	void loadPalette();
	void loadCursors();
	void loadFonts();

	void blit(byte *data, int x, int y, unsigned int width, unsigned int height, byte transparent = COLOUR_BLANK);

	UnityEngine *_vm;
	byte *basePalette, *palette;
	Image background;
	Common::Array<Image> cursors;
	Common::Array<Image> wait_cursors;
};

}

