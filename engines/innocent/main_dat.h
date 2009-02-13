/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef INNOCENT_MAIN_DAT_H
#define INNOCENT_MAIN_DAT_H

#include <list>

#include "common/endian.h"
#include "common/hashmap.h"
#include "common/list.h"
#include "common/str.h"

#include "innocent/actor.h"
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
	uint16 getBubbleId(SpeechBubblePart part) const;

	Puppeteer getPuppeteer(uint16 actorId) const;

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

	void parsePuppeteers() const;
	mutable Common::HashMap<uint16, Puppeteer> _puppeteers;
};

} // End of namespace Innocent

#endif
