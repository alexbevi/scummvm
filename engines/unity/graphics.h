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

struct MRGFile {
	Common::Array<uint16> widths, heights;
	Common::Array<byte *> data;

	~MRGFile() {
		for (unsigned int i = 0; i < data.size(); i++)
			delete[] data[i];
	}
};

class Graphics {
public:
	Graphics(UnityEngine *engine);
	~Graphics();

	void init();

	void setCursor(unsigned int id, bool wait);
	void setBackgroundImage(Common::String filename);

	void loadMRG(Common::String filename, MRGFile *mrg);

	void drawString(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Common::String text, unsigned int font);
	void drawMRG(MRGFile *mrg, unsigned int entry, unsigned int x, unsigned int y);
	void drawBackgroundImage();
	void drawSprite(SpritePlayer *sprite, int x, int y, unsigned int scale = 256);
	void drawBackgroundPolys(Common::Array<struct ScreenPolygon> &polys);
	void renderPolygonEdge(Common::Array<Common::Point> &points, byte colour);

	void fillRect(byte colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

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

