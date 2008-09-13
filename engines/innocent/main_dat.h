#ifndef INNOCENT_MAIN_DAT_H
#define INNOCENT_MAIN_DAT_H

#include <list>

#include "common/endian.h"
#include "common/list.h"
#include "common/str.h"

#include "innocent/datafile.h"
#include "innocent/sprite.h"
#include "innocent/types.h"

namespace Innocent {
//

class Actor;
class Interpreter;

class MainDat : public Datafile {
public:
	MainDat(Resources *resources);
	~MainDat();

	const char *filename() const { return "iuc_main.dat"; }
	void readFile(Common::SeekableReadStream &stream);

	uint16 imagesCount() const;
	uint16 tunesCount() const;

	struct GraphicFile {
		uint16 data_set;
		Common::String filename;
	};

	std::list<struct GraphicFile> graphicFiles() const;
	std::list<Common::String> musicFiles() const;

	uint16 progEntriesCount0() const;
	uint16 progEntriesCount1() const;

	/**
	 * Find an image in the file contents directory.
	 * @param index of the image (start counting from 1),
	 * @returns index of graphics data file containing the image.
	 */
	uint16 fileIndexOfImage(uint16 index) const;
	uint16 fileIndexOfTune(uint16 index) const;

	uint16 interfaceImageIndex() const;

	byte *getEntryPoint() const;
	
	byte *_data;

	Actor *actor(uint16 index) const;
	uint16 actorsCount() const { return _actorsCount; }

	byte *getByteVariable(uint16 index);
	byte *getWordVariable(uint16 index);
	uint16 getRoomScriptId(uint16 room) const;
	uint16 getGlyphSpriteId(byte character) const;

	SpriteInfo getSpriteInfo(uint16 index) const;
	uint16 spriteCount() const;

	uint16 getCursorSpriteId() const;
	uint16 getFrameId(FramePart part) const;

	friend class Resources;
private:
	void loadActors(Interpreter *);
	enum {
		kFooterLen = 0xB6
	};

	uint16 _dataLen;
	byte _footer[kFooterLen];
	byte *_imageDirectory;
	byte *_tunesDirectory;
	uint16 _programsCount;
	byte *_programsMap;
	Actor **_actors;
	uint16 _actorsCount;
};

} // End of namespace Innocent

#endif
