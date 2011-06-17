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
 */

#ifndef RING_APPLICATION_H
#define RING_APPLICATION_H

#include "ring/shared.h"

#include "ring/base/event.h"

#include "common/events.h"

namespace Ring {

struct Configuration;

class ArtHandler;
class Bag;
class CursorHandler;
class Debugger;
class DialogHandler;
class DragControl;
class LanguageHandler;
class FontHandler;
class Game;
class Image;
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
class Visual;
class Var;
class ZoneHandler;

class Application : public Common::Serializable, public EventHandler {
public:
	Application(RingEngine *engine);
	virtual ~Application();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	void init();
	virtual void initLanguages();
	virtual void initFont() = 0;
	virtual void setup() = 0;
	virtual void initZones() = 0;
	virtual void setupZone(ZoneId zone, SetupType type);
	virtual void setZone(ZoneId zone, SetupType type);
	virtual void initBag() = 0;
	virtual void initData();

	void exitZone();
	void exitToMenu(MenuAction a1);

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen() = 0;
	virtual void startMenu(bool savegame) = 0;
	virtual void showMenu(ZoneId zone, MenuAction menuAction) = 0;
	virtual void showCredits() = 0;
	virtual void loadPreferences() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Timer & update
	void processTimer(TimerId id);
	void update(const Common::Point &point);
	void updateBag(const Common::Point &point);

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw() = 0;
	void drawZoneName(ZoneId zone);

	void playMovie(Common::String filename, float frameDivider = 0.0f);
	void playMovieChannel(Common::String filename, uint32 channel);

	void displayFade(Common::String filenameFrom, Common::String filenameTo, uint32 frameCount, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);
	void fadeOut(uint32 frameCount, const Color &colorTo, uint32 ticksWait);

	//////////////////////////////////////////////////////////////////////////
	// Misc
	static Common::String getFileExtension(ImageType imageType);
	void waitForEscape(uint32 ticksWait) const;

	//////////////////////////////////////////////////////////////////////////
	// Language, Font, Cursor and Subtitle
	void languageAdd(LanguageId id, Common::String name, Common::String folder, uint channel);
	LanguageId getCurrentLanguage();
	Common::String getLanguageFolder();
	uint32 getLanguageChannel();
	bool hasLanguagePack();

	void fontAdd(FontId id, Common::String filename, Common::String facename, uint32 height, bool smallWeight, bool underline, bool italic, bool strikeout, LanguageId langId);

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

	//////////////////////////////////////////////////////////////////////////
	// Cursor
	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte imageCount, LoadFrom loadFrom, ArchiveType archiveType);
	void cursorAdd(CursorId id, Common::String name, CursorType cursorType, byte imageCount, uint32 frameCount, float frameRate, byte a6, LoadFrom loadFrom, ArchiveType archiveType);
	void cursorSetOffset(CursorId id, const Common::Point &offset);
	void cursorSelect(CursorId id);
	void cursorDelete();
	void cursorRemoveByType(CursorType type);

	//////////////////////////////////////////////////////////////////////////
	// Message
	virtual void messageInsertCd(ZoneId zone) {}
	virtual void messageShowWarning(uint32 accelerationIndex);
	virtual void messageHideWarning(uint32 accelerationIndex);
	virtual void messageShowQuestion(uint32 accelerationIndex);
	virtual void messageHideQuestion(uint32 accelerationIndex);

	bool messageGet(Common::String messageId);
	void messageFormat(Common::String messageId, Common::String argument);

	//////////////////////////////////////////////////////////////////////////
	// Puzzle
	void puzzleAdd(PuzzleId puzzleId);
	Puzzle *getPuzzle(PuzzleId puzzleId);

	void puzzleAddBackgroundImage(PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive);
	void puzzleAddMovabilityToPuzzle(PuzzleId puzzleIdFrom, PuzzleId puzzleIdTo, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);
	void puzzleAddMovabilityToRotation(PuzzleId puzzleIdFrom, Id rotationIdTo, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);
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

	void reset();

	bool hasCurrentPuzzle() { return _puzzle != NULL; }
	Puzzle *getCurrentPuzzle() { return _puzzle; }
	PuzzleId getCurrentPuzzleId();

	//////////////////////////////////////////////////////////////////////////
	// Object
	void objectAdd(ObjectId objectId, Common::String description, Common::String name, byte a5);
	void objectRemove(ObjectId objectId);
	Object *getObject(ObjectId objectId);

	void objectAddPuzzleAccessibility(ObjectId objectId, PuzzleId puzzleId, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);
	void objectSetPuzzleAccessibilityKey(ObjectId objectId, uint32 accessibilityIndex, Common::KeyCode key);

	void objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot);
	void objectSetAccessibilityOnOrOff(ObjectId objectId, bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration);
	void objectSetAccessibilityOn(const ObjectId &objectId);
	void objectSetAccessibilityOff(const ObjectId &objectId);
	void objectSetAccessibilityOn(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration);
	void objectSetAccessibilityOff(const ObjectId &objectId, uint32 fromAcceleration, uint32 toAcceleration);

	void objectAddRotationAccessibility(ObjectId objectId, Id rotationId, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);

	void objectSetActiveCursor(ObjectId objectId,      const Common::Point &point, uint32 framecount, CursorType type, float framerate, byte a7, LoadFrom loadFrom);
	void objectSetPassiveCursor(ObjectId objectId,     const Common::Point &point, uint32 framecount, CursorType type, float framerate, byte a7, LoadFrom loadFrom);
	void objectSetActiveDrawCursor(ObjectId objectId,  const Common::Point &point, uint32 framecount, CursorType type, float framerate, byte a7, LoadFrom loadFrom);
	void objectSetPassiveDrawCursor(ObjectId objectId, const Common::Point &point, uint32 framecount, CursorType type, float framerate, byte a7, LoadFrom loadFrom);

	void objectAddPresentation(ObjectId objectId);
	void objectAddBagAnimation(ObjectId objectId, ImageType imageType, DrawType drawType, uint32 frameCount, float framerate, byte a6);

	void objectPresentationAddTextToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String text, const Common::Point &point, FontId fontId, const Color &foreground, const Color &background);
	void objectPresentationSetTextToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, Common::String text);
	void objectPresentationSetTextCoordinatesToPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 textIndex, const Common::Point &point);
	uint32 objectPresentationGetTextWidth(ObjectId objectId, uint32 presentationIndex, uint32 textIndex);

	void objectPresentationAddImageToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, const Common::Point &point, bool isActive, DrawType drawType, uint32 priority);
	void objectPresentationAddImageToRotation(ObjectId objectId, uint32 presentationIndex, Id rotationId, uint32 layer);
	void objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, const Common::Point &point);
	void objectPresentationSetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex, const Common::Point &point);
	void objectPresentationSetImageOriginalCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex);
	Common::Point objectPresentationGetImageCoordinatesOnPuzzle(ObjectId objectId, uint32 presentationIndex, uint32 imageIndex);

	void objectPresentationAddAnimationToPuzzle(ObjectId objectId, uint32 presentationIndex, PuzzleId puzzleId, Common::String filename, ImageType imageType, const Common::Point &point, DrawType drawType, uint32 priority, uint32 a10, float a11, byte a12);
	void objectPresentationAddAnimationToRotation(ObjectId, uint32 presentationIndex, Id rotationId, uint32 a4, uint32 a5, float a6, byte a7);
	void objectPresentationSetAnimationOnPuzzle(ObjectId id, uint32 presentationIndex, uint32 animationIndex, Id targetId);
	void objectPresentationSetAnimationOnRotation(ObjectId id, uint32 presentationIndex, uint32 animationIndex, Id targetId);
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

	void initObjectCursors(ObjectId objectId);
	void initObjectDrawCursors(ObjectId objectId);

	//////////////////////////////////////////////////////////////////////////
	// Rotation
	Rotation *getRotation(Id rotationId);
	void rotationAdd(Id rotationId, Common::String name, byte a3, uint32 nodeCount);
	void rotationSetCompressionBufferLength(Id rotationId, uint32 length);
	void rotationAddMovabilityToPuzzle(Id rotationId, PuzzleId puzzleId, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);
	void rotationSetMovabilityToPuzzle(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7);
	void rotationAddMovabilityToRotation(Id fromRotationId, Id toRotationId, Common::String name, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10);
	void rotationSetMovabilityToRotation(Id rotationId, uint32 movabilityIndex, float a3, float a4, float a5, float a6, byte a7, float a8, float a9, float a10);

	void rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot);
	void rotationSetMovabilityOnOrOff(Id rotationId, bool enableHotspot, uint32 fromMovability, uint32 toMovability);
	void rotationSetMovabilityOn(Id rotationId);
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
	void soundAdd(Id soundId, SoundType soundType, Common::String filename, LoadFrom loadFrom, uint32 a4, uint32 soundChunk);
	void soundPlay(Id soundId, SoundLoopType loop = kSoundOnce);
	void soundStop(Id soundId, uint32 a2);
	void soundSetVolume(Id soundId, uint32 volume);
	void soundStopType(SoundType soundType, uint32 a2);
	void soundSetMultiplier(SoundType soundType, int32 multiplier);
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
	bool hasTimer(TimerId id);

	//////////////////////////////////////////////////////////////////////////
	// Var
#define DEFINE_VAR_FUNCTIONS(name, type) \
	void varDefine##name(Id id, type value); \
	type varGet##name(Id id); \
	void varSet##name(Id id, type value);

	DEFINE_VAR_FUNCTIONS(Byte,   int8)
	DEFINE_VAR_FUNCTIONS(Word,   int16)
	DEFINE_VAR_FUNCTIONS(Dword,  int32)
	DEFINE_VAR_FUNCTIONS(String, Common::String)
	DEFINE_VAR_FUNCTIONS(Float,  float)

#undef DEFINE_VAR_FUNCTIONS

	void varRemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// Visual
	void visualAddShowToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 width, uint32 progressMultiplier, uint32 progressColor);
	void visualListAddToPuzzle(Id visualId, PuzzleId puzzleId, uint32 a3,
	                           Common::String imageDir, Common::String iconDir, Common::String filename3, Common::String filename4, Common::String filename5, Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	                           Common::String filename11, Common::String filename12, Common::String filename13,
	                           DrawType drawType, const Common::Point &origin, const Common::Point &backgroundOffset, uint32 a22, uint32 a23, uint32 a24, uint32 a25, uint32 a26,
	                           uint32 a27, const Common::Point &upOffset, uint32 a30, uint32 a31, uint32 a32, uint32 a33, const Common::Point &downOffset, uint32 a36,
	                           uint32 a37, uint32 a38, uint32 a39, const Common::Point &imageCoords, ImageType imageType, DrawType imageDrawType, uint32 a44, uint32 a45, uint32 a46,
	                           const Color &foreground, const Color &foregroundSelected, const Color &background, FontId fontId,
	                           ArchiveType archiveType);
	void visualListAdd(Id visualId, PuzzleId puzzleId, const ObjectId &objectId);
	void visualListRemove(Id visualId, PuzzleId puzzleId, const ObjectId &objectId, bool removeObject);
	void visualListRemove(Id visualId, PuzzleId puzzleId, bool removeObject);
	bool visualListIsIn(Id visualId, PuzzleId puzzleId, const Common::String &description);
	void visualListSetOn(Id visualId, PuzzleId puzzleId);
	void visualListSetOff(Id visualId, PuzzleId puzzleId);
	void visualListSetIconDirectory(Id visualId, PuzzleId puzzleId, const Common::String &directory);
	uint32 visualListGetItemCount(Id visualId, PuzzleId puzzleId);
	int32 visualListGetImageIndexClicked(Id visualId, PuzzleId puzzleId);
	ObjectId visualListGetObjectIdClicked(Id visualId, PuzzleId puzzleId);
	int32 visualListGetObjectIndexClicked(Id visualId, PuzzleId puzzleId);
	void visualListResetObjectClicked(Id visualId, PuzzleId puzzleId);


	virtual Visual *createVisual(Id visualId, uint32 a3, uint32 a4, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) = 0;

	//////////////////////////////////////////////////////////////////////////
	// DragControl
	void dragControlSetField45(uint32 a1);
	void dragControlSetHotspot(const Common::Rect &rect);
	void dragControlSetCoords1(const Common::Point &point);
	bool dragControlYLower();
	bool dragControlYEqual();
	bool dragControlYHigher();
	bool dragControlXLower();
	bool dragControlXEqual();
	bool dragControlXHigher();
	bool dragControlYLower0();
	bool dragControlYHigher0();
	bool dragControlXLower0();
	bool dragControlXHigher0();
	bool dragControlYLower1();
	bool dragControlYHigher1();
	bool dragControlXLower1();
	bool dragControlXHigher1();

	uint32 dragControlGetOffsetX() const;
	uint32 dragControlGetOffsetY() const;
	uint32 dragControlGetDistance() const;

	uint32 dragControlGetOffsetX0();
	uint32 dragControlGetOffsetY0();
	uint32 dragControlGetOffsetX1();
	uint32 dragControlGetOffsetY1();

	//////////////////////////////////////////////////////////////////////////
	// Zone
	void addEpisode(ZoneId id, Common::String name, uint32 cd);
	void setCurrentEpisode(ZoneId id);
	ZoneId getCurrentEpisode();
	uint32 getEpisodeCd(ZoneId id);

	void addZone(ZoneId id, Common::String name, Common::String folder, ArchiveType archiveType, LoadFrom loadFrom);
	void setSpace(ZoneId id);
	void setCurrentZone(ZoneId id);
	ZoneId getCurrentZone();
	ZoneId getSystemZone() { return _systemZone; }
	Common::String getCurrentZoneFolder();
	Common::String getZoneFolder(ZoneId zone) const;
	Common::String getZoneName(ZoneId zone) const;
	ArchiveType    getZoneArchiveType(ZoneId zone) const;
	LoadFrom       getZoneLoadFrom(ZoneId zone) const;

	//////////////////////////////////////////////////////////////////////////
	// User
	void startLogin();
	void setUser(Common::String name) { _username = name; }
	Common::String getUser() { return _username; }

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ArchiveType getArchiveType(ZoneId zone) const;
	Image *getThumbnail() { return _thumbnail; }
	bool getField6A() { return _field_6A; }
	LoadFrom getLoadFrom() { return _loadFrom; }
	bool isRotationCompressed() { return _isRotationCompressed; }
	ArchiveType getArchiveType() { return _archiveType; }
	ZoneId getCurrentGameZone() { return _currentGameZone; }
	State getState() { return _state; }
	bool getField74() { return _field_74; }
	bool getField75() { return _field_75; }
	bool getField76() { return _field_76; }
	bool getField77() { return _field_77; }
	bool getField78() { return _field_78; }

	void setArchiveType(ArchiveType type) { _archiveType = type; }
	void setLoadFrom(LoadFrom from) { _loadFrom = from; }
	void setState(State state) { _state = state; }
	void setField6A(bool state) { _field_6A = state; }
	void setField74(bool state) { _field_74 = state; }
	void setField75(bool state) { _field_75 = state; }
	void setField76(bool state) { _field_76 = state; }
	void setField77(bool state) { _field_77 = state; }
	void setField78(bool state) { _field_78 = state; }

	//////////////////////////////////////////////////////////////////////////
	// Handlers and shared data
	ArtHandler        *getArtHandler()        { return _artHandler; }
	Bag               *getBag()               { return _bag; }
	DialogHandler     *getDialogHandler()     { return _dialogHandler; }
	DragControl       *getDragControl()       { return _dragControl; }
	FontHandler       *getFontHandler()       { return _fontHandler; }
	PreferenceHandler *getPreferenceHandler() { return _preferenceHandler; }
	SaveManager       *getSaveManager()       { return _saveManager; }
	ScreenManager     *getScreenManager()     { return _screenManager; }
	SoundHandler      *getSoundHandler()      { return _soundHandler; }
	SoundManager      *getSoundManager()      { return _soundManager; }
	TimerHandler      *getTimerHandler()      { return _timerHandler; }
	Configuration      getConfiguration()     { return _configuration; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

protected:
	RingEngine *_vm;

	// Earlier games use 1 as system zone, later ones use 100 (defaults to 1)
	ZoneId                        _systemZone;

	// Game screen (when showing menu)
	Image                        *_thumbnail;

	// Application objects
	ScreenManager                *_screenManager;
	ArtHandler                   *_artHandler;
	FontHandler                  *_fontHandler;
	DialogHandler                *_dialogHandler;
	LanguageHandler              *_languageHandler;
	Configuration                 _configuration;
	bool                          _isRotationCompressed;
	ArchiveType                   _archiveType;
	CursorHandler                *_cursorHandler;
	LoadFrom                      _loadFrom;
	uint32                        _field_5E;
	SoundHandler                 *_soundHandler;
	State                         _state;
	bool                          _field_6A;
	ZoneId                        _currentGameZone;
	MenuAction                    _menuAction;
	bool                          _field_74;
	bool                          _field_75;
	bool                          _field_76;
	bool                          _field_77;
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
	ZoneHandler                  *_zoneHandler;
	Common::String                _username;

	// Save / Load games
	SaveManager                     *_saveManager;

	// Sound (playing & managing sound data)
	SoundManager                    *_soundManager;

	// Configuration
	void loadConfiguration();

	// Display and movies
	void showImage(Common::String filename, const Common::Point &point, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);
	bool scrollImage(Common::String filename, uint32 ticksWait, LoadFrom loadFrom, ArchiveType archiveType);

	// Current rotation
	Rotation *_currentRotation;

	// Current message
	Common::String _messageType;
	Common::String _message;

	friend class Debugger;
};

} // End of namespace Ring

#endif // RING_APPLICATION_H
