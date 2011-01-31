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

#ifndef RING_APPLICATION_H
#define RING_APPLICATION_H

#include "ring/shared.h"

namespace Ring {

class ArtHandler;
class Bag;
class CursorHandler;
class DialogHandler;
class DragControl;
class LanguageHandler;
class FontHandler;
class ObjectHandler;
class Object;
class PreferenceHandler;
class Puzzle;
class PuzzleInfo;
class RingEngine;
class Rotation;
class SoundHandler;
class TimerHandler;
class Var;
class Video;

class Application {
public:
	Application(RingEngine *engine);
	~Application();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	void init();
	void setup();
	void setupZones();

	//////////////////////////////////////////////////////////////////////////
	// Startup
	void showStartupScreen();
	void startMenu();

	//////////////////////////////////////////////////////////////////////////
	// Event handling
	void onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed);
	void onMouseLeftButtonDown(Common::Event &evt);
	void onMouseRightButtonUp(Common::Event &evt);
	void onKeyDown(Common::Event &evt);
	void onTimer(TimerId id);

	//////////////////////////////////////////////////////////////////////////
	// Zone
	void setupZone(Zone zone, uint32 a2);
	void setZone(Zone zone, uint32 a2);
	void setZoneAndEnableBag(Zone zone);
	Common::String getZone(Zone zone) const;
	Common::String getZoneName(Zone zone) const;
	uint32 getReadFrom(Zone zone) const;

	//////////////////////////////////////////////////////////////////////////
	// Puzzle
	void resetPuzzle();
	bool hasPuzzle() { return _puzzle != NULL; }
	PuzzleId getPuzzleId();

	//////////////////////////////////////////////////////////////////////////
	// Object
	void addObject(ObjectId id, Common::String language, Common::String name, byte a5);
	void removeObject(ObjectId id);

	//////////////////////////////////////////////////////////////////////////
	// Bag
	void addToBag(ObjectId id);
	void removeFromBag(ObjectId id);
	void removeAllFromBag();
	bool isInBag(ObjectId id);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Zone getZone() { return _zone; }
	Common::String getZoneName() { return _zoneName; }
	LanguageId getLanguage();
	Common::String getLanguageName();

	DialogHandler *getDialogHandler() { return _dialogHandler; }
	FontHandler   *getFontHandler()   { return _fontHandler; }
	SoundHandler  *getSoundHandler()  { return _soundHandler; }

private:
	RingEngine *_vm;

	// Configuration data
	struct SoundConfiguration {
		int soundChunck;
		LoadFrom loadFrom;

		SoundConfiguration() {
			soundChunck = 9;
			loadFrom = kLoadFromCd;
		}
	};

	struct Configuration {
		bool runningFromDisk;
		int checkCD;
		SoundConfiguration backgroundMusic;
		SoundConfiguration ambientMusic;
		SoundConfiguration ambientEffect;
		SoundConfiguration effect;
		SoundConfiguration dialog;
		bool artBAG;
		bool artCURSOR;
		bool artSY;
		bool artAS;
		bool artNI;
		bool artN2;
		bool artRO;
		bool artRH;
		bool artWA;
		bool artFO;
		bool checkLoadSave;

		Configuration() {
			runningFromDisk = false;
			checkCD = 0;
			artBAG = false;
			artCURSOR = false;
			artSY = false;
			artAS = false;
			artNI = false;
			artN2 = false;
			artRO = false;
			artRH = false;
			artWA = false;
			artFO = false;
			checkLoadSave = false;
		}
	};

	// Application objects
	Video                      *_video;
	ArtHandler                 *_artHandler;
	FontHandler                *_fontHandler;
	DialogHandler              *_dialogHandler;
	LanguageHandler            *_languageHandler;
	//Common::String              _cdPath;
	//Common::String              _currentDirectory
	Configuration               _configuration;
	uint32                      _field_54;
	ArchiveType                 _archiveType;
	CursorHandler              *_cursorHandler;
	char                        _field_5D;
	uint32                      _field_5E;
	SoundHandler               *_soundHandler;
	uint32                      _field_66;
	char                        _field_6A;
	Common::String              _zoneName;
	Zone                        _zone;          // original uses byte
	char                        _field_6F;
	uint32                      _field_70;
	char                        _field_74;
	char                        _field_75;
	char                        _field_76;
	char                        _field_77;
	char                        _field_78;
	AssociativeArray<Object*>   _objectList;
	Common::Array<PuzzleInfo*>  _puzzleList;
	Puzzle                     *_puzzle;
	Common::Array<Rotation*>    _rotationList;
	void                       *_field_89;
	Bag                        *_bag;
	TimerHandler               *_timerHandler;
	Var                        *_var;
	DragControl                *_dragControl;
	ObjectHandler              *_objectHandler;
	PreferenceHandler          *_preferenceHandler;
	bool                        _controlNotPressed;

	// Configuration
	void loadConfiguration();

	// Event handling
	void onMouseLeftButtonUp(Common::Event &evt);
	void onZoneTimer(TimerId id);

	// Accessors
	uint32 getLanguageChannel();
};

} // End of namespace Ring

#endif // RING_APPLICATION_H
