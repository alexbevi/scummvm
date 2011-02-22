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
class Debugger;
class DialogHandler;
class DragControl;
class EventHandler;
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
class ScreenManager;
class SoundHandler;
class SoundManager;
class TimerHandler;
class Var;

class Application {
public:
	Application(RingEngine *engine);
	virtual ~Application();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	void init();
	virtual void setup() = 0;
	virtual void initZones() = 0;

	void exitZone();
	void exitToMenu(uint32 a1);

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen() = 0;
	virtual void startMenu(bool savegame) = 0;
	virtual void showMenu(Zone zone, uint32 a2) = 0;
	virtual void showCredits() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Event handling
	void onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed);
	void onMouseLeftButtonDown(Common::Event &evt);
	void onMouseRightButtonUp(Common::Event &evt);
	void onKeyDown(Common::Event &evt);
	void onTimer(TimerId id);
	void onSound(Id id, SoundType type, uint32 a3);

	void update(const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw() = 0;

	void playMovie(Common::String filename, float frameDivider = 0.0f);
	void playMovieChannel(Common::String filename, uint32 channel);

	static Common::String getFileExtension(ImageType imageType);

	//////////////////////////////////////////////////////////////////////////
	// Language, Font, Cursor and Subtitle
	void languageAdd(LanguageId id, Common::String name, Common::String folder, uint channel);
	LanguageId languageGetCurrent();
	Common::String languageGetFolder();
	uint32 languageGetChannel();

	void fontAdd(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId);

	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte frameCount, LoadFrom loadFrom, ArchiveType archiveType);
	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte frameCount, uint32 a4, float a5, byte a6, LoadFrom loadFrom, ArchiveType archiveType);
	void cursorSetOffset(CursorId id, const Common::Point &offset);

	void subtitleSetColor(const Color &color);
	void subtitleSetBackgroundColor(const Color &color);

	//////////////////////////////////////////////////////////////////////////
	// Bag
	void bagAdd(ObjectId id);
	void bagRemove(ObjectId id);
	void bagRemoveAll();
	bool bagHas(ObjectId id);
	void bagSetClickedObject(ObjectId objectId);
	bool bagHasClickedObject();
	ObjectId bagGetClickedObject();
	void bagOpen(const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Cursor
	void cursorSelect(CursorId id);
	void cursorDelete();

	//////////////////////////////////////////////////////////////////////////
	// Message
	virtual void messageInsertCd(Zone zone) {}
	virtual void messageShowWarning(uint32 accelerationIndex) {}
	virtual void messageHideWarning(uint32 accelerationIndex) {}
	virtual void messageShowQuestion(uint32 accelerationIndex) {}
	virtual void messageHideQuestion(uint32 accelerationIndex) {}

	bool messageGet(Common::String messageId);
	void messageFormat(Common::String messageId, Common::String argument);

	//////////////////////////////////////////////////////////////////////////
	// Zone name, short string and readFrom
	virtual Common::String getZoneString(Zone zone) const = 0;
	virtual Common::String getZoneLongName(Zone zone) const = 0;
	virtual ArchiveType getReadFrom(Zone zone) const = 0;

	//////////////////////////////////////////////////////////////////////////
	// Puzzle
	void puzzleAdd(PuzzleId id);

	void puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive);
	void puzzleAddMovabilityToPuzzle(PuzzleId puzzleIdFrom, PuzzleId puzzleIdTo, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void puzzleAddMovabilityToRotation(PuzzleId puzzleIdFrom, Id rotationIdTo, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void puzzleSetMovabilityToRotation(PuzzleId puzzleId, uint32 movabilityIndex, float a3, float a4, float a5);

	void puzzleSetMovabilityOnOrOff(PuzzleId puzzleId, bool enableHotspot);
	void puzzleSetMovabilityOnOrOff(PuzzleId puzzleId, bool enableHotspot, uint32 fromMovability, uint32 toMovability);
	void puzzleSetMovabilityOn(const PuzzleId &puzzleId);
	void puzzleSetMovabilityOff(const PuzzleId & puzzleId);
	void puzzleSetMovabilityOn(const PuzzleId &puzzleId, uint32 fromMovability, uint32 toMovability);
	void puzzleSetMovabilityOff(const PuzzleId &puzzleId, uint32 fromMovability, uint32 toMovability);

	bool puzzleSetMod(PuzzleId puzzleId, uint32 a2, uint32 a3);

	void puzzleAddAmbientSound(PuzzleId puzzleId, Id soundId, uint32 volume, int32 pan, uint32 fadeFrames, uint32 a6, uint32 a7);
	void puzzleSetAmbientSoundOn(PuzzleId puzzleId, Id soundId);
	void puzzleSetAmbientSoundOff(PuzzleId puzzleId, Id soundId);
	void puzzleAdd3DSound(PuzzleId puzzleId, Id soundId, uint32 a3, uint32 a4, uint32 fadeFrames, uint32 volume, float a7, uint32 a8);
	void puzzleSet3DSoundOn(const PuzzleId &puzzleId, Id soundId);
	void puzzleSet3DSoundOff(const PuzzleId &puzzleId, Id soundId);
	void puzzleSet3DSoundVolume(PuzzleId puzzleId, Id soundId, int32 volume);

	void puzzleSetActive(PuzzleId id, bool a2 = true, bool a3 = true);

	void puzzleReset();

	bool hasCurrentPuzzle() { return _puzzle != NULL; }
	PuzzleId getCurrentPuzzleId();

	//////////////////////////////////////////////////////////////////////////
	// Object
	void objectAdd(ObjectId objectId, Common::String description, Common::String name, byte a5);
	void objectRemove(ObjectId objectId);
	Object *objectGet(ObjectId objectId);

	void objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void objectSetPuzzleAccessibilityKey(ObjectId objectId, uint32 accessibilityIndex, Common::KeyCode key);

	void objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot);
	void objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration);
	void objectSetAccessibilityOn(const ObjectId &objectId);
	void objectSetAccessibilityOff(const ObjectId &objectId);
	void objectSetAccessibilityOn(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration);
	void objectSetAccessibilityOff(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration);

	void objectAddRotationAccessibility(ObjectId objectId, Id rotationId, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);

	void objectSetActiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetPassiveCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetActiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);
	void objectSetPassiveDrawCursor(ObjectId objectId, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8);

	void objectAddPresentation(ObjectId objectId);
	void objectAddBagAnimation(ObjectId objectId, ImageType imageType, uint32 a3, uint32 a4, float a5, uint32 a6);

	void objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, const Common::Point &point, FontId fontId, Color foreground, Color background);
	void objectPresentationSetTextToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, Common::String text);
	void objectPresentationSetTextCoordinatesToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, const Common::Point &point);
	uint32 objectPresentationGetTextWidth(ObjectId objectId, uint32 presentationIndex, uint32 textIndex);

	void objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive, uint32 a8, uint32 priority);
	void objectPresentationAddImageToRotation(ObjectId objectId, uint32 presentationIndex, Id rotationId, uint32 layer);
	void objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, const Common::Point &point);
	void objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex, const Common::Point &point);
	void objectPresentationSetImageOriginalCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex);
	Common::Point objectPresentationGetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex);

	void objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, ImageType imageType, const Common::Point &point, uint32 a8, uint32 a9, uint32 a10, float a11, uint32 a12);
	void objectPresentationAddAnimationToRotation(ObjectId, uint32 presentationIndex, Id rotationId, uint32 a4, uint32 a5, float a6, uint32 a7);
	void objectPresentationSetAnimationOnPuzzle(ObjectId id, uint32 presentationIndex, uint32 animationIndex, const ObjectId &targetId);
	void objectPresentationSetAnimationOnRotation(ObjectId id, uint32 presentationIndex, uint32 animationIndex, const ObjectId &targetId);
	void objectPresentationSetAnimationCoordinatesOnPuzzle(ObjectId id, uint32 presentationIndex, const Common::Point &point);
	void objectPresentationSetAnimationStartFrame(ObjectId objectId, uint32 presentationIndex, uint32 startFrame);
	void objectPresentationSetAnimationActiveFrame(ObjectId objectId, uint32 presentationIndex, uint32 activeFrame);
	void objectPresentationPauseAnimation(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationPauseAnimationFrame(ObjectId objectId, uint32 presentationIndex, uint32 frame, uint32 a4, uint32 a5);
	void objectPresentationUnpauseAnimation(ObjectId objectId, uint32 presentationIndex);

	void objectPresentationShow(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationShow(ObjectId objectId);
	void objectPresentationHide(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationHide(ObjectId objectId);
	void objectPresentationHideAndRemove(ObjectId objectId, uint32 presentationIndex);
	void objectPresentationHideAndRemove(ObjectId objectId);

	//////////////////////////////////////////////////////////////////////////
	// Rotation
	void rotationAdd(Id rotationId, Common::String name, byte a3, uint32 nodeCount);
	void rotationSetComBufferLength(Id rotationId, uint32 length);
	void rotationAddMovabilityToPuzzle(Id rotationId, PuzzleId puzzleId, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void rotationSetMovabilityToPuzzle(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7);
	void rotationAddMovabilityToRotation(Id fromRotationId, Id toRotationId, Common::String name, const Common::Rect &rect, bool enabled, uint32 a9, uint32 a10);
	void rotationSetMovabilityToRotation(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7, float a8, float a9, float a10);

	void rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot);
	void rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot, uint32 fromMovability, uint32 toMovability);
	void rotationSetMovabilityOff(Id rotationId);
	void rotationSetMovabilityOn(Id rotationId, uint32 fromMovability, uint32 toMovability);
	void rotationSetMovabilityOff(Id rotationId, uint32 fromMovability, uint32 toMovability);
	void rotationSetMovabilityRideName(Id rotationId, uint32 movabilityIndex, Common::String name);

	void rotationSetAlp(Id rotationId, float alp);
	void rotationSetBet(Id rotationId, float bet);
	void rotationSetRan(Id rotationId, float ran);
	float rotationGetAlp(Id rotationId);
	float rotationGetBet(Id rotationId);
	float rotationGetRan(Id rotationId);
	void rotationSetRolTo(Id rotationId, float a2, float a3, float a4);
	void rotationSetJugOn(Id rotationId, float amplitude, float speed);

	void rotationAddAmbientSound(Id rotationId, Id soundId, uint32 volume, int32 pan, uint32 fadeFrames, uint32 a6, uint32 a7);
	void rotationSetAmbientSoundOn(Id rotationId, Id soundId);
	void rotationSetAmbientSoundOff(Id rotationId, Id soundId);
	void rotationAdd3DSound(Id rotationId, uint32 soundId, uint32 a3, uint32 a4, uint32 fadeFrames, uint32 volume, float a7, uint32 a8);
	void rotationSet3DSoundOn(Id rotationId, Id soundId);
	void rotationSet3DSoundOff(Id rotationId, Id soundId);

	void rotationSetFreOn(Id rotationId);
	void rotationSetFreOff(Id rotationId);

	void rotationSetActive(Id rotationId, bool a2 = true, bool a3 = true);

	// Current rotation
	bool hasCurrentRotation() { return _rotation != NULL; }
	Id getCurrentRotationId();
	Rotation *getCurrentRotation() { return _rotation; }
	void setupCurrentRotation();
	void setFreOffCurrentRotation();

	//////////////////////////////////////////////////////////////////////////
	// Sound
	void soundAdd(Id soundId, SoundType soundType, Common::String filename, LoadFrom loadFrom);
	void soundAdd(Id soundId, SoundType soundType, Common::String filename, LoadFrom loadFrom, uint32 a4, int soundChunk);
	void soundPlay(Id soundId, int a2);
	void soundStop(Id soundId, uint32 a2);
	void soundSetVolume(Id soundId, uint32 volume);
	void soundStopType(SoundType soundType, uint32 a2);
	void soundSetMultiplier(SoundType soundType, uint32 a2);
	void soundSetMultiplierIfNotType(SoundType soundType, int32 multiplier);
	void soundStopAll(uint32 a1);
	void soundSetPan(Id soundId, int32 pan);
	bool soundIsPlaying(Id soundId);
	bool soundIsPlayingType(SoundType soundType);

	//////////////////////////////////////////////////////////////////////////
	// Timer
	void timerStart(TimerId id, uint32 elapseTime);
	void timerStop(TimerId id);
	void timerStopAll();
	bool timerHas(TimerId id);

	//////////////////////////////////////////////////////////////////////////
	// Var
#define DEFINE_VAR_FUNCTIONS(name, type) \
	void varDefine##name(Id id, type value); \
	type varGet##name(Id id); \
	void varSet##name(Id id, type value);

	DEFINE_VAR_FUNCTIONS(Byte,   byte);
	DEFINE_VAR_FUNCTIONS(Word,   int16);
	DEFINE_VAR_FUNCTIONS(Dword,  int32);
	DEFINE_VAR_FUNCTIONS(String, Common::String);
	DEFINE_VAR_FUNCTIONS(Float,  float);

#undef DEFINE_VAR_FUNCTIONS

	//////////////////////////////////////////////////////////////////////////
	// Visual
	void visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 width, uint32 progressMultiplier, uint32 progressColor);
	void visualListAddToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                           Common::String imageDir, Common::String iconDir, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                           Common::String filename11, Common::String filename12, Common::String filename13,
							   uint32 a17, uint32 a18, uint32 a19, uint32 a20, uint32 a21, uint32 a22, uint32 a23, uint32 a24, uint32 a25, uint32 a26,
							   uint32 a27, uint32 a28, uint32 a29, uint32 a30, uint32 a31, uint32 a32, uint32 a33, uint32 a34, uint32 a35, uint32 a36,
							   uint32 a37, uint32 a38, uint32 a39, uint32 a40, uint32 a41, ImageType imageType, uint32 a43, uint32 a44, uint32 a45, uint32 a46,
							   Color foreground1, Color foreground2, Color background, FontId fontId,
							   ArchiveType archiveType);
	void visualListAdd(Id visualId, PuzzleId puzzleId, const ObjectId &objectId);
	void visualListRemove(Id visualId, PuzzleId puzzleId, const ObjectId &objectId, bool removeObject);
	void visualListRemove(Id visualId, PuzzleId puzzleId, bool removeObject);
	uint32 visualListGetItemCount(Id visualId, PuzzleId puzzleId);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Zone getCurrentZone() { return _zone; }
	Common::String getCurrentZoneString() { return getZoneString(_zone); }
	ArchiveType getArchiveType() { return _archiveType; }
	void setField78(bool state) { _field_78 = state; }

	//////////////////////////////////////////////////////////////////////////
	// Handlers and shared data
	//CursorHandler *getCursorHandler() { return _cursorHandler; }
	ArtHandler    *getArtHandler()    { return _artHandler; }
	Bag           *getBag()           { return _bag; }
	DialogHandler *getDialogHandler() { return _dialogHandler; }
	FontHandler   *getFontHandler()   { return _fontHandler; }
	SaveManager   *getSaveManager()   { return _saveManager; }
	SoundHandler  *getSoundHandler()  { return _soundHandler; }
	SoundManager  *getSoundManager()  { return _soundManager; }

protected:
	RingEngine *_vm;

	// Application objects
	ScreenManager                *_screenManager;
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
	State                         _state;
	char                          _field_6A;
	Common::String                _zoneString;
	Zone                          _zone;          // original uses byte
	char                          _field_6F;
	uint32                        _field_70;
	char                          _field_74;
	char                          _field_75;
	char                          _field_76;
	char                          _field_77;
	bool                          _field_78;
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
	//bool                          _controlNotPressed; // Moved to EventHandler

	// Save / Load games
	SaveManager                     *_saveManager;

	// Sound (playing & managing sound data)
	SoundManager                    *_soundManager;

	// Event Handling
	EventHandler                    *_eventHandler;

	// Configuration
	void loadConfiguration();

	// Display and movies
	void showImage(Common::String filename, Common::Point point, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);
	bool scrollImage(Common::String filename, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);
	void displayFade(Common::String filenameFrom, Common::String filenameTo, uint32 frameCount, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);
	void waitForEscape(uint32 ticksWait);

	// Current rotation
	Rotation *_currentRotation;

	// Current message
	Common::String _messageType;
	Common::String _message;

	friend Debugger;
};

} // End of namespace Ring

#endif // RING_APPLICATION_H
