#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

#include "common/stream.h"

namespace Innocent {

class MainDat;
class GraphicsMap;

class Resources {
public:
	Resources();
	void load();

	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }

private:
	std::auto_ptr<MainDat> _main;
	std::auto_ptr<GraphicsMap> _graphicsMap;
};

class Datafile {
public:
	Datafile(Resources *resources) : _resources(resources) {}
	virtual ~Datafile() {}

	virtual void load();

	virtual const char *filename() const = 0;
	virtual void readFile(Common::SeekableReadStream &stream) = 0;

protected:
	Resources *_resources;
};

class MainDat : public Datafile {
public:
	MainDat(Resources *resources);
	~MainDat();

	const char *filename() const { return "iuc_main.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	uint16 imagesCount() const;

private:
	enum {
		kFooterLen = 0xB6
	};

	enum Offsets {
		kImagesCount = 0x1C
	};

	uint16 _dataLen;
	byte *_data;
	byte _footer[kFooterLen];

	void descramble();
};

class GraphicsMap : public Datafile {
public:
	GraphicsMap(Resources *resources) : Datafile(resources) {}
	const char *filename() const { return "iuc_graf.dat"; }
	void readFile(Common::SeekableReadStream &stream);

private:
	byte _data[1200];
	uint16 _imgCount;
};


} // End of namespace Innocent

#endif
