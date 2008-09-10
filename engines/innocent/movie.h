#ifndef INNOCENT_MOVIE_H
#define INNOCENT_MOVIE_H

#include "common/stream.h"

#include "innocent/resources.h"

namespace Innocent {
//
class Movie {
public:
	virtual ~Movie();
	static Movie *fromFile(const char *name);
	void setFrameDelay(uint delay);
	void play();

protected:
	virtual void showFrame();
	virtual void setPalette();
	virtual void delay();

	Surface _s;
	byte _pal[0x400];
private:
	Movie();
	Movie(Common::ReadStream *);
	Movie(const Movie &);
	Movie &operator=(const Movie &);

	bool findKeyFrame();
	void loadKeyFrame();
	void loadIFrame();

	int _delay, _iFrames;
	Common::ReadStream *_f;
};

}

#endif
