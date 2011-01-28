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
class ObjectInfo;
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

	// Initialization
	void init();
	void setupCursors();
	void setupZones();

	// Startup
	void showStartupScreen();
	void startMenu();

private:
	RingEngine *_vm;

	// Configuration data
	struct SoundConfiguration {
		int soundChunck;
		LoadFrom loadFrom;
	};

	struct Configuration {
		bool runningFromDisk;
		int checkCD;
		SoundConfiguration backgroundMusic;
		SoundConfiguration ambiantMusic;
		SoundConfiguration ambiantEffect;
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
	bool                        _field_58;
	CursorHandler              *_cursorHandler;
	bool                        _field_5D;
	uint32                      _field_5E;
	SoundHandler               *_soundHandler;
	uint32                      _field_66;
	bool                        _field_6A;
	Common::String              _zoneName;
	Zone                        _zone;          // original uses byte
	bool                        _field_6F;
	uint32                      _field_70;
	bool                        _field_74;
	bool                        _field_75;
	bool                        _field_76;
	bool                        _field_77;
	bool                        _field_78;
	Common::Array<ObjectInfo*>  _objectList;
	Common::Array<PuzzleInfo*>  _puzzleList;
	Puzzle                     *_puzzle;
	Common::Array<Rotation*>    _rotationList;
	uint32                      _field_89;
	Bag                        *_bag;
	TimerHandler               *_timerHandler;
	Var                        *_var;
	DragControl                *_dragControl;
	ObjectHandler              *_objectHandler;
	bool                        _field_A5;

};

} // End of namespace Ring

#endif // RING_APPLICATION_H
