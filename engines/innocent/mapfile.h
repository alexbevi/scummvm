#ifndef INNOCENT_MAPFILE_H
#define INNOCENT_MAPFILE_H

#include "common/stream.h"
#include "common/str.h"

#include "innocent/datafile.h"

namespace Innocent {

class Resources;

class MapFile : public Datafile {
public:
	MapFile(const char *name) : Datafile(0), _filename(name) {}
	const char *filename() const { return _filename.c_str(); }
	void readFile(Common::SeekableReadStream &stream);

	uint32 offsetOfEntry(uint16 index);

private:
	byte _data[1200];
	uint16 _imgCount;
	Common::String _filename;
};

} // End of namespace Innocent

#endif // INNOCENT_MAPFILE_H
