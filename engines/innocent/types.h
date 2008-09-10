#ifndef INNOCENT_TYPES_H
#define INNOCENT_TYPES_H

namespace Innocent {
//
class Graphics;

class Paintable {
public:
	virtual ~Paintable() {}
	virtual void paint(Graphics *g) const = 0;
	virtual byte zIndex() const = 0;
};

enum StringSpecial {
	kStringCountSpacesTerminate = 2,
	kStringMove = 3,
	kStringMenuOption = 5,
	kStringGlobalWord = 6,
	kStringSetColour = 7,
	kStringDefaultColour = 8,
	kStringAdvance = 9,
	kStringCountSpacesIf0 = 0x0a,
	kStringCountSpacesIf1 = 0x0b,
	kStringCenter = 12
};


enum FramePart {
	kFrameTopLeft,
	kFrameTop,
	kFrameTopRight,
	kFrameLeft,
	kFrameFill,
	kFrameRight,
	kFrameBottomLeft,
	kFrameBottom,
	kFrameBottomRight,
	kFrameNum
};

}

#endif
