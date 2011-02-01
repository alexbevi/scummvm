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

struct Configuration;

class ArtHandler;
class Bag;
class CursorHandler;
class DialogHandler;
class DragControl;
class LanguageHandler;
class FontHandler;
class Game;
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
	void initZones();

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
	// Language, Font, Cursor and Subtitle
	void languageAdd(LanguageId id, Common::String name, Common::String folder, uint channel);
	LanguageId languageGetCurrent();
	Common::String languageGetFolder();
	uint32 languageGetChannel();

	void fontAdd(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId);

	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, uint32 a3, ImageType imageType, ArchiveType archiveType);
	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, uint32 a3, uint32 a4, uint32 a5, uint32 a6, ImageType imageType, ArchiveType archiveType);
	void cursorSetOffset(CursorId id, Common::Point offset);

	void subtitleSetColor(Color color);
	void subtitleSetBackgroundColor(Color color);

	//////////////////////////////////////////////////////////////////////////
	// Zone
	void setArchiveType(ArchiveType type) { _archiveType = type; }
	ArchiveType getArchiveType() { return _archiveType; }

	Common::String getZoneString(Zone zone) const;
	Common::String getZoneLongName(Zone zone) const;

	void setCurrentZone(Zone zone) { _zone = zone; _zoneName = getZoneString(zone); }
	Zone getCurrentZone() { return _zone; }
	Common::String getCurrentZoneName() { return _zoneName; }

	uint32 getReadFrom(Zone zone) const;

	//////////////////////////////////////////////////////////////////////////
	// Puzzle
	void puzzleAdd(PuzzleId id);
	void puzzleReset();
	bool hasPuzzle() { return _puzzle != NULL; }
	PuzzleId getPuzzleId();

	//////////////////////////////////////////////////////////////////////////
	// Object
	void objectAdd(ObjectId objectId, Common::String language, Common::String name, byte a5);
	void objectAddPresentation(ObjectId objectId);
	void objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10);
	void objectSetPuzzleAccessibilityKey(ObjectId id, uint32 accessiblityIndex, uint32 a3);
	void objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, uint32 a5, uint32 a6, uint32 a7, uint32 a8, int32 a9, int32 a10, int32 a11, int32 a12, int32 a13);
	void objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9);
	void objectRemove(ObjectId objectId);

	//////////////////////////////////////////////////////////////////////////
	// Bag
	void bagAdd(ObjectId id);
	void bagRemove(ObjectId id);
	void bagRemoveAll();
	bool bagIsIn(ObjectId id);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Configuration getConfiguration() { return _configuration; }
	void setField5D(uint32 val) { _field_5D = val; }
	void setField66(uint32 val) { _field_66 = val; }

	//////////////////////////////////////////////////////////////////////////
	// Handlers and shared data
	//CursorHandler *getCursorHandler() { return _cursorHandler; }
	ArtHandler    *getArtHandler()    { return _artHandler; }
	Bag           *getBag()           { return _bag; }
	DialogHandler *getDialogHandler() { return _dialogHandler; }
	FontHandler   *getFontHandler()   { return _fontHandler; }
	SoundHandler  *getSoundHandler()  { return _soundHandler; }

private:
	RingEngine *_vm;

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
	AssociativeArray<Puzzle*>   _puzzleList;
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

	// Game specific
	Game                       *_game;

	// Configuration
	void loadConfiguration();

	// Event handling
	void onMouseLeftButtonUp(Common::Event &evt);
	void onZoneTimer(TimerId id);
};

} // End of namespace Ring

#endif // RING_APPLICATION_H
