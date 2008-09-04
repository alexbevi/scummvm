#ifndef INNOCENT_GRAPH_DAT_H
#define INNOCENT_GRAPH_DAT_H

#include "common/stream.h"

#include "innocent/datafile.h"

namespace Innocent {

class Resources;

class GraphicsMap : public Datafile {
public:
	GraphicsMap(Resources *resources) : Datafile(resources) {}
	const char *filename() const { return "iuc_graf.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	uint32 offsetOfImage(uint16 index);

private:
	uint32 _data[1200/4];
	uint16 _imgCount;
};

} // End of namespace Innocent

#endif // INNOCENT_GRAPH_DAT_H
