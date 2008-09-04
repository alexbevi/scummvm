#ifndef INNOCENT_MAIN_DAT_H
#define INNOCENT_MAIN_DAT_H

#include <list>

#include "innocent/datafile.h"

namespace Innocent {

class MainDat : public Datafile {
public:
	MainDat(Resources *resources);
	~MainDat();

	const char *filename() const { return "iuc_main.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	uint16 imagesCount() const;

	struct GraphicFile {
		uint16 data_set;
		const byte *filename;
	};

	std::list<struct GraphicFile> graphicFiles() const;

	uint16 progEntriesCount0() const;
	uint16 progEntriesCount1() const;

private:
	enum {
		kFooterLen = 0xB6
	};

	enum Offsets {
		kProgEntriesCount0 = 	0x06,
		kProgEntriesCount1 = 	0x08,
		kImagesCount = 			0x1C,
		kGraphicFileCount = 	0x20,
		kGraphicFileNames = 	0x22
	};

	uint16 _dataLen;
	byte *_data;
	byte _footer[kFooterLen];

	void descramble();
};

} // End of namespace Innocent

#endif
