#ifndef INNOCENT_DATAFILE_H
#define INNOCENT_DATAFILE_H

#include "common/stream.h"

namespace Innocent {

class Resources;

enum DebugLevels {
	kAck = 2,
	kDataRead = 1
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

} // End of namespace Innocent

#endif // INNOCENT_DATAFILE_H
