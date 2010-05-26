#include "unity.h"

namespace Unity {

class Graphics {
public:
	Graphics(UnityEngine *engine);
	~Graphics();

	void init();
	void drawMRG(Common::String filename, unsigned int entry);
	void setBackgroundImage(Common::String filename);
	void drawBackgroundImage();

protected:
	UnityEngine *_vm;
	byte *basePalette, *palette;
	unsigned int backgroundWidth, backgroundHeight;
	byte *backgroundPixels;
};

}

