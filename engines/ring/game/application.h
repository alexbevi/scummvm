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

#include "common/events.h"

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
class SaveManager;
class SoundHandler;
class SoundManager;
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
	virtual void setup() = 0;
	virtual void initZones() = 0;

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
	// Bag
	void bagAdd(ObjectId id);
	void bagRemove(ObjectId id);
	void bagRemoveAll();
	bool bagIsIn(ObjectId id);

	//////////////////////////////////////////////////////////////////////////
	// Zone name, short string and readFrom
	virtual Common::String getZoneString(Zone zone) const = 0;
	virtual Common::String getZoneLongName(Zone zone) const = 0;
	virtual ArchiveType getReadFrom(Zone zone) const = 0;

	//////////////////////////////////////////////////////////////////////////
	// Puzzle
	void puzzleAdd(PuzzleId id);

	void puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, uint32 a3, uint32 a4, bool isActive);
	void puzzleAddMovabilityToPuzzle(PuzzleId puzzleIdFrom, PuzzleId puzzleIdTo, Common::String name, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void puzzleAddMovabilityToRotation(PuzzleId puzzleIdFrom, Id rotationIdTo, Common::String name, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void puzzleSetMovabilityToRotation(PuzzleId puzzleId, uint32 movabilityIndex, float a3, float a4, float a5);

	void puzzleAddAmbientSound(PuzzleId puzzleId, Id soundId, uint32 volume, uint32 a4, uint32 fadeFrames, uint32 a6, uint32 a7);
	void puzzleSetAmbientSoundOff(PuzzleId puzzleId, Id soundId);
	void puzzleAdd3DSound(PuzzleId puzzleId, Id soundId, uint32 a3, uint32 a4, uint32 fadeFrames, uint32 volume, float a7, uint32 a8);
	void puzzleSet3DSoundOff(PuzzleId puzzleId, Id soundId);

	void puzzleReset();

	bool hasCurrentPuzzle() { return _puzzle != NULL; }
	PuzzleId getCurrentPuzzleId();

	bool hasCurrentRotation() { return _rotation != NULL; }
	Id getCurrentRotationId();

	//////////////////////////////////////////////////////////////////////////
	// Object
	void objectAdd(ObjectId objectId, Common::String language, Common::String name, byte a5);
	void objectRemove(ObjectId objectId);

	void objectSetAccessibilityOnOrOff(ObjectId objectId, uint32 a2, uint32 a3);

	void objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void objectSetPuzzleAccessibilityKey(ObjectId objectId, uint32 accessibilityIndex, Common::KeyCode key);

	void objectAddRotationAccessibility(ObjectId objectId, Id rotationId, Common::Rect rect, uint32 a8, uint32 a9, uint32 a10);

	void objectSetActiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetPassiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetActiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetPassiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);

	void objectAddPresentation(ObjectId objectId);
	void objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, uint32 a5, uint32 a6, FontId fontId, uint32 a8, int32 a9, int32 a10, int32 a11, int32 a12, int32 a13);
	void objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, uint32 a6, bool isActive, uint32 a8, uint32 priority);
	void objectPresentationAddImageToRotation(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4);
	void objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, float a11, uint32 a12);
	void objectPresentationAddAnimationToRotation(ObjectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7);
	void objectPresentationSetAnimationOnPuzzle(ObjectId id, uint32 a2, uint32 a3, uint32 a4);
	void objectPresentationSetAnimationOnRotation(ObjectId id, uint32 a2, uint32 a3, uint32 a4);
	void objectPresentationSetAnimationCoordinatesOnPuzzle(ObjectId id, uint32 presentationIndex, Common::Point point);
	void objectPresentationAnimationSetStartFrame(ObjectId objectId, uint32 animationIdex, uint32 startFrame);
	void objectPresentationShow(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationShow(ObjectId objectId);
	void objectPresentationHide(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationHide(ObjectId objectId);
	void objectPresentationPauseAnimation(ObjectId objectId, uint32 presentationIndex);

	void objectAddBagAnimation(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6);

	//////////////////////////////////////////////////////////////////////////
	// Rotation
	void rotationAdd(Id rotationId, Common::String name, uint32 a3, uint32 nodeCount);
	void rotationSetComBufferLength(Id rotationId, uint32 length);
	void rotationSetMovabilityOnOrOff(Id rotationId, uint32 a2, uint32 a3);
	void rotationAddMovabilityToPuzzle(Id rotationId, PuzzleId puzzleId, Common::String name, uint32 a4, int32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10);
	void rotationSetMovabilityToPuzzle(Id rotationId, uint32 movabilityIndex, uint32 a3, int32 a4, uint32 a5, uint32 a6, uint32 a7);
	void rotationAddMovabilityToRotation(Id rotationId1, Id rotationId2, Common::String name, uint32 a4, int32 a5, uint32 a6, int32 a7, uint32 a8, uint32 a9, uint32 a10);
	void rotationSetMovabilityToRotation(Id rotationId, uint32 movabilityIndex, uint32 a3, int32 a4, uint32 a5,  uint32 a6, uint32 a7, int32 a8, int32 a9, uint32 a10);

	void rotationAddAmbientSound(Id rotationId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7);
	void rotationSetAmbiantSoundOff(Id rotationId, Id soundId);
	void rotationAdd3DSound(Id rotationId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, float a7, uint32 a8);
	void rotationSet3DSoundOff(Id rotationId, Id soundId);
	void rotationSetJugOn(Id rotationId, float amplitude, float speed);

	//////////////////////////////////////////////////////////////////////////
	// Sound
	void soundAdd(Id soundId, SoundType soundType, Common::String filename, LoadFrom loadFrom);
	void soundAdd(Id soundId, SoundType soundType, Common::String filename, LoadFrom loadFrom, uint32 a4, int soundChunk);
	void soundSetVolume(Id soundId, uint32 volume);

	void noiceIdPlay(Id noiceId, bool a2);

	//////////////////////////////////////////////////////////////////////////
	// Timer
	void timerStart(TimerId id, uint32 elapseTime);
	void timerStop(TimerId id);
	void timerStopAll();

	//////////////////////////////////////////////////////////////////////////
	// Var
	void varDefineByte(Id id, byte val);
	void varSetByte(Id id, byte val);
	byte varGetByte(Id id);
	void varDefineWord(Id id, int16 val);
	void varSetWord(Id id, int16 val);
	int16 varGetWord(Id id);
	void varDefineDword(Id id, int32 val);
	void varSetDword(Id id, uint32 val);
	uint32 varGetDword(Id id);
	void varDefineString(Id id, Common::String val);
	void varSetString(Id id, Common::String val);
	Common::String varGetString(Id id);
	void varDefineFloat(Id id, float val);
	void varSetFloat(Id id, float val);
	float varGetFloat(Id id);

	//////////////////////////////////////////////////////////////////////////
	// Visual
	void visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10);
	void visualAddListToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                           Common::String filename1, Common::String filename2, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                           Common::String filename11, Common::String filename12, Common::String filename13,
							   uint32 a16, uint32 a17, uint32 a18, uint32 a19, uint32 a20, uint32 a21, uint32 a22, uint32 a23, uint32 a24, uint32 a25,
	                           uint32 a26, uint32 a27, uint32 a28, uint32 a29, uint32 a30, uint32 a31, uint32 a32, uint32 a33, uint32 a34, uint32 a35,
	                           uint32 a36, uint32 a37, uint32 a38, uint32 a39, uint32 a40, uint32 a41, uint32 a42, uint32 a43, uint32 a44, uint32 a45,
							   uint32 a46, uint32 a47, uint32 a48, uint32 a49, uint32 a50, uint32 a51, int32 a52, int32 a53, int32 a54, int32 a55,
							   ArchiveType archiveType);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Zone getCurrentZone() { return _zone; }
	ArchiveType getArchiveType() { return _archiveType; }

	//////////////////////////////////////////////////////////////////////////
	// Handlers and shared data
	//CursorHandler *getCursorHandler() { return _cursorHandler; }
	ArtHandler    *getArtHandler()    { return _artHandler; }
	Bag           *getBag()           { return _bag; }
	DialogHandler *getDialogHandler() { return _dialogHandler; }
	FontHandler   *getFontHandler()   { return _fontHandler; }
	SoundHandler  *getSoundHandler()  { return _soundHandler; }

protected:
	RingEngine *_vm;

	// Application objects
	Video                        *_video;
	ArtHandler                   *_artHandler;
	FontHandler                  *_fontHandler;
	DialogHandler                *_dialogHandler;
	LanguageHandler              *_languageHandler;
	//Common::String                _cdPath;
	//Common::String                _currentDirectory
	Configuration                 _configuration;
	uint32                        _field_54;
	ArchiveType                   _archiveType;
	CursorHandler                *_cursorHandler;
	LoadFrom                      _loadFrom;
	uint32                        _field_5E;
	SoundHandler                 *_soundHandler;
	uint32                        _field_66;
	char                          _field_6A;
	Common::String                _zoneString;
	Zone                          _zone;          // original uses byte
	char                          _field_6F;
	uint32                        _field_70;
	char                          _field_74;
	char                          _field_75;
	char                          _field_76;
	char                          _field_77;
	char                          _field_78;
	AssociativeArray<Object *>    _objects;
	AssociativeArray<Puzzle *>    _puzzles;
	Puzzle                       *_puzzle;
	AssociativeArray<Rotation *>  _rotations;
	Rotation                     *_rotation;
	Bag                          *_bag;
	TimerHandler                 *_timerHandler;
	Var                          *_var;
	DragControl                  *_dragControl;
	ObjectHandler                *_objectHandler;
	PreferenceHandler            *_preferenceHandler;
	bool                          _controlNotPressed;

	// Save / Load games
	SaveManager                     *_saveManager;

	// Sound (playing & managing sound data)
	SoundManager                    *_soundManager;

	// Configuration
	void loadConfiguration();

	// Event handling
	void onMouseLeftButtonUp(Common::Event &evt);

	// Timer
	virtual void onZoneTimer(TimerId id) = 0;
};

} // End of namespace Ring

#endif // RING_APPLICATION_H
