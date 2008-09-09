#include "innocent/debug.h"

namespace Innocent {

const char *operator+(const Common::Rect &r) {
	static char buf[20];
	snprintf(buf, 20, "%d:%d %d:%d", r.top, r.left, r.bottom, r.right);
	return buf;
}

}
