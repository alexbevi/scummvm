#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include "common/stream.h"

namespace Innocent {

class MainDat;
class GraphicsMap;

class Resources {
public:
	Resources();
	void load();

private:
	MainDat *_main;
	GraphicsMap *_graphicsMap;
};

class Datafile {
public:
	Datafile(Resources *resources) : _resources(resources) {}
	virtual ~Datafile() {}

	virtual void load();

	virtual const char *filename() const = 0;
	virtual void readFile(Common::SeekableReadStream &stream) = 0;

private:
	Resources *_resources;
};

class MainDat : public Datafile {
public:
	MainDat(Resources *resources);
	~MainDat();

	const char *filename() const { return "iuc_main.dat"; }
	void readFile(Common::SeekableReadStream &stream);

private:
	enum {
		kFooterLen = 0xB6
	};

	uint16 _dataLen;
	byte *_data;
	byte _footer[kFooterLen];

	void descramble();
};


} // End of namespace Innocent

#endif
