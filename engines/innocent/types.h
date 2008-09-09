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
