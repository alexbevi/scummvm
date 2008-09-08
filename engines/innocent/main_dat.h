#ifndef INNOCENT_MAIN_DAT_H
#define INNOCENT_MAIN_DAT_H

#include <list>

#include "common/endian.h"
#include "common/str.h"

#include "innocent/datafile.h"

namespace Innocent {

struct SpriteInfo {
	uint16 left, top, width, height, image;
	int8 hotLeft, hotTop;
};

class MainDat : public Datafile {
public:
	MainDat(Resources *resources);
	~MainDat();

	const char *filename() const { return "iuc_main.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	uint16 imagesCount() const;

	struct GraphicFile {
		uint16 data_set;
		Common::String filename;
	};

	std::list<struct GraphicFile> graphicFiles() const;

	uint16 progEntriesCount0() const;
	uint16 progEntriesCount1() const;

	/**
	 * Find an image in the file contents directory.
	 * @param index of the image (start counting from 1),
	 * @returns index of graphics data file containing the image.
	 */
	uint16 fileIndexOfImage(uint16 index) const;

	uint16 interfaceImageIndex() const;

	byte *getEntryPoint() const;
	
	byte *_data;

	byte *getByteVariable(uint16 index);
	byte *getWordVariable(uint16 index);
	uint16 getRoomScriptId(uint16 room) const;
	uint16 getGlyphSpriteId(byte character) const;

	SpriteInfo getSpriteInfo(uint16 index) const;

private:
	enum {
		kFooterLen = 0xB6
	};

	uint16 _dataLen;
	byte _footer[kFooterLen];
	byte *_imageDirectory;
	uint16 _programsCount;
	byte *_programsMap;
};

} // End of namespace Innocent

#endif
