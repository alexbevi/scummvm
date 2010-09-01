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

#ifndef LIATH_H
#define LIATH_H

#include "liath/debug.h"

#include "common/random.h"

#include "engines/advancedDetector.h"
#include "engines/engine.h"

#include "graphics/pixelformat.h"

/**
 * This is the namespace of the Liath engine.
 *
 * Status of this engine:
 *
 * Maintainers:
 *  Littleboy
 *
 * Supported games:
 *  - Liath
 */
namespace Liath {

class Debugger;

class ActionManager;
class ArrayManager;
class ExpressionManager;
class GameManager;
class GraphicsManager;
class HeroManager;
class MouseManager;
class PositionManager;
class ProgressManager;
class ResourceManager;
class SavegameManager;
class SegmentManager;
class SoundManager;
class TextManager;
class WorkManager;

class LiathEngine : public Engine {
protected:
	// Engine APIs
	Common::Error run();
	virtual void errorString(const char *buf_input, char *buf_output, int buf_output_size);
	virtual bool hasFeature(EngineFeature f) const;
	virtual Debugger *getDebugger() { return _debugger; }

public:
	LiathEngine(OSystem *syst, const ADGameDescription *gd);
	~LiathEngine();

	// Misc
	Common::RandomSource getRandom() const {return _random; }

	// Managers
	ActionManager     *getActionManager()     const { return _actionMan; }
	ArrayManager      *getArrayManager()      const { return _arrayMan; }
	ExpressionManager *getExpressionManager() const { return _expressionMan; }
	GameManager       *getGameManager()       const { return _gameMan; }
	GraphicsManager   *getGraphicsManager()   const { return _graphicsMan; }
	HeroManager       *getHeroManager()       const { return _heroMan; }
	MouseManager      *getMouseManager()      const { return _mouseMan; }
	ProgressManager   *getProgressManager()   const { return _progressMan; }
	PositionManager   *getPositionManager()   const { return _positionMan; }
	ResourceManager   *getResourceManager()   const { return _resMan; }
	SavegameManager   *getSavegameManager()   const { return _saveMan; }
	SegmentManager    *getSegmentManager()    const { return _segmentMan; }
	SoundManager      *getSoundManager()      const { return _soundMan; }
	TextManager       *getTextManager()       const { return _textMan; }
	WorkManager       *getWorkManager()       const { return _workMan; }

private:
	const ADGameDescription *_gameDescription;
	Graphics::PixelFormat _pixelFormat;

	// Misc
	Debugger *_debugger;
	Common::RandomSource _random;

	// Managers
	ActionManager     *_actionMan;
	ArrayManager      *_arrayMan;
	ExpressionManager *_expressionMan;
	GameManager       *_gameMan;
	GraphicsManager   *_graphicsMan;
	HeroManager       *_heroMan;
	MouseManager      *_mouseMan;
	PositionManager   *_positionMan;
	ProgressManager   *_progressMan;
	ResourceManager   *_resMan;
	SavegameManager   *_saveMan;
	SegmentManager    *_segmentMan;
	SoundManager      *_soundMan;
	TextManager       *_textMan;
	WorkManager       *_workMan;
};

} // End of namespace Liath

#endif // LIATH_H
