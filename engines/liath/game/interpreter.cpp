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

#include "liath/game/interpreter.h"

#include "liath/game/action.h"
#include "liath/game/array.h"
#include "liath/game/expression.h"
#include "liath/game/game.h"
#include "liath/game/graphics.h"
#include "liath/game/hero.h"
#include "liath/game/mouse.h"
#include "liath/game/position.h"
#include "liath/game/progress.h"
#include "liath/game/savegame.h"
#include "liath/game/sound.h"
#include "liath/game/text.h"
#include "liath/game/work.h"

#include "liath/liath.h"
#include "liath/helpers.h"

namespace Liath {

Interpreter::Interpreter(LiathEngine *engine) : _engine(engine) {}

Interpreter::~Interpreter() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Interpreter
//////////////////////////////////////////////////////////////////////////
int Interpreter::interpret(ObjectIndex *pIndex, Common::Array<FileData *> *segment) {
	error("Interpreter::interpret: Not implemented!");
}

OpcodeRet Interpreter::run(OpcodeParameters *parameters, ObjectIndex index) {
	switch (parameters->opcode) {
	default:
		return kOpcodeRetDefault;

	case kOpcodeStartHero:
		return getHero()->start(parameters, NULL, NULL);

	case kOpcodeResetHero:
		return getHero()->reset(parameters, NULL);

	case kOpcodeStartHeroExt:
		return getHero()->startExt(parameters);

	case kOpcodeStartAction:
		return getAction()->start(parameters);

	case kOpcodeStartActionGlobal:
		return getAction()->startGlobal(parameters);

	case kOpcodeStartActionHeroVar:
		return getAction()->startHeroVariable(parameters);

	case kOpcodeStartActionVar:
		return getAction()->startVariable(parameters);

	case kOpcodeCheckLimits:
		return getPosition()->checkLimits(parameters);

	case kOpcodeStartObject:
	case kOpcodeStartObject2:
	case kOpcodeStartObject3:
	case kOpcodeStartObject4:
		return getWork()->startObj(index);

	case kOpcodeCommandNumberBox:
		return getMouse()->commandNumberBox(parameters);

	case kOpcodePassVar:
		return getHero()->passVariable(parameters);

	case kOpcodeExitGame:
		return getAction()->exitGame(parameters);

	case kOpcodeRestart:
		return getWork()->restart();

	case kOpcodeExitHero:
		return getHero()->exit(parameters);

	case kOpcodeCel:
		return getWork()->cel(parameters);

	case kOpcodeCelExt:
		return getWork()->celExt(parameters);

	case kOpcodeMyRandom:
		return getGame()->rnd(parameters);

	case kOpcodeCash:
		return getSound()->cash(parameters);

	case kOpcodeSaveGame:
		return getSave()->save(parameters);

	case kOpcodeLoadGame:
		return getSave()->load(parameters);

	case kOpcodeSaveHero:
		return getHero()->save(parameters);

	case kOpcodeLoadHero:
		return getHero()->load();

	case kOpcodeNumberAct:
		return getAction()->number(parameters);

	case kOpcodeRedraw:
		return getGraphics()->redraw(parameters);

	case kOpcodeAddMouseBox:
		return getMouse()->addBox(parameters);

	case kOpcodeDelMouseBox:
		return getMouse()->removeBox(parameters);

	case kOpcodePushMouse:
		return getMouse()->push();

	case kOpcodePopMouse:
		return getMouse()->pop();

	case kOpcodePopHerous:
		return getWork()->popHerous();

	case kOpcodePushHerous:
		return getWork()->pushHerous();

	case kOpcodeTypeMouse:
		return getMouse()->type(parameters);

	case kOpcodeResetMouse:
		return getMouse()->reset(parameters);

	case kOpcodeKeyMouse:
		return getMouse()->key(parameters);

	case kOpcodeMousePress:
		return getMouse()->press(parameters);

	case kOpcodeTextOut:
		return getText()->out(parameters);

	case kOpcodeTextQuit:
		return getText()->quit(parameters);

	case kOpcodeTextExit:
		return getText()->exit(parameters);

	case kOpcodeTextCheck:
		return getText()->check(parameters);

	case kOpcodeTextChange:
		return getText()->change(parameters);

	case kOpcodeArrayInit:
		return getArray()->init(parameters);

	case kOpcodeArrayImg:
		return getArray()->img(parameters);

	case kOpcodeArrayAdd:
		return getArray()->add(parameters);

	case kOpcodeArrayDel:
		return getArray()->del(parameters);

	case kOpcodeArrayKill:
		return getArray()->kill(parameters);

	case kOpcodeArrayClear:
		return getArray()->clear(parameters);

	case kOpcodeArrayGet:
		return getArray()->get(parameters);

	case kOpcodeArrayGetNumber:
		return getArray()->getNumber(parameters);

	case kOpcodeArraySize:
		return getArray()->size(parameters);

	case kOpcodeArrayTGet:
		return getArray()->tget(parameters);

	case kOpcodeArrayCur:
		return getArray()->cur(parameters);

	case kOpcodeArraySetCur:
		return getArray()->setcur(parameters);

	case kOpcodeHTime:
		return getWork()->htime(parameters);

	case kOpcodeHDark:
		return getWork()->hdark(parameters);

	case kOpcodeXScroll:
		return getGraphics()->xscroll(parameters);

	case kOpcodeYScroll:
		return getGraphics()->yscroll(parameters);

	case kOpcodePlayAvsStopMusic:
		return getGraphics()->playAvs(parameters, false);

	case kOpcodePlayAvs:
		return getGraphics()->playAvs(parameters, true);

	case kOpcodeFreeze:
	case kOpcodeFreeze2:
		return getHero()->freeze(parameters);

	case kOpcodeFreezeGlobal:
		return getHero()->freezeGlobal(parameters);

	case kOpcodeFreezeHeroVar:
		return getHero()->freezeHeroVar(parameters);

	case kOpcodeFreezeVar:
		return getHero()->freezeVar(parameters);

	case kOpcodeUnfreeze:
	case kOpcodeUnfreeze2:
		return getHero()->unfreeze(parameters);

	case kOpcodeUnfreezeGlobal:
		return getHero()->unfreezeGlobal(parameters);

	case kOpcodeUnfreezeHeroVar:
		return getHero()->unfreezeHeroVar(parameters);

	case kOpcodeUnfreezeVar:
		return getHero()->unfreezeVar(parameters);

	case kOpcodeStop:
	case kOpcodeStop2:
		return getWork()->stop(parameters);

	case kOpcodeStopGlobal:
		return getWork()->stopGlobal(parameters);

	case kOpcodeStopHeroVar:
		return getWork()->stopHeroVar(parameters);

	case kOpcodeStopVar:
		return getWork()->stopVar(parameters);

	case kOpcodeContinue:
	case kOpcodeContinue2:
		return getWork()->cont(parameters);

	case kOpcodeContinueGlobal:
		return getWork()->contGlobal(parameters);

	case kOpcodeContinueHeroVar:
		return getWork()->contHeroVar(parameters);

	case kOpcodeContinueVar:
		return getWork()->contVar(parameters);

	case kOpcodeKey:
		return getGame()->key(parameters);

	case kOpcodeBkey:
		return getGame()->bkey(parameters);

	case kOpcodeCurAction:
		return getAction()->curAction(parameters);

	case kOpcodeHeroVarGlobal:
		return getHero()->herovarGlobal(parameters);

	case kOpcodeHeroVarHeroVar:
		return getHero()->herovarHeroVar(parameters);

	case kOpcodeHeroVarVar:
		return getHero()->herovarVar(parameters);

	case kOpcodeHeroVar:
		return getHero()->herovar(parameters);

	case kOpcodeHeroSys:
		return getHero()->heroSys(parameters);

	case kOpcodeVar:
		return getHero()->var(parameters);

	case kOpcodeSys:
		return getHero()->sys(parameters);

	case kOpcodeGlobal:
		return getGame()->global(parameters);

	case kOpcodeExpression:
		return getExpression()->expression(parameters);

	case kOpcodeQuitHero:
		return getHero()->quitHero(parameters);

	case kOpcodeQuitGlobal:
		return getHero()->quitGlobal(parameters);

	case kOpcodeQuitHeroVar:
		return getHero()->quitHeroVar(parameters);

	case kOpcodeQuitVar:
		return getHero()->quitVar(parameters);

	case kOpcodeCheckMouse:
		return getMouse()->check(parameters);

	case kOpcodeLook:
		return getGraphics()->look(parameters);

	case kOpcodePers:
		return getPosition()->pers(parameters);

	case kOpcodeHPers:
		return getPosition()->hpers(parameters);

	case kOpcodeDist:
		return getPosition()->dist(parameters);

	case kOpcodeFeel:
		return getPosition()->feel(parameters);

	case kOpcodeGlassIncrement:
		return getWork()->glass(parameters, true);

	case kOpcodeGlassDecrement:
		return getWork()->glass(parameters, false);

	case kOpcodePlayWave:
		return getSound()->playWave(parameters);

	case kOpcodePlayMusic:
		return getSound()->playMusic(parameters, false);

	case kOpcodePlayMusicEffects:
		return getSound()->playMusic(parameters, true);

	case kOpcodeStopMusic:
		return getSound()->stopMusic(parameters);

	case kOpcodeGStopMusic:
		return getSound()->gstopMusic(parameters);

	case kOpcodePlayMidi:
		return getSound()->playMidi(parameters, false);

	case kOpcodePlayMidiLoop:
		return getSound()->playMidi(parameters, true);

	case kOpcodeStopMidi:
		return getSound()->stopMidi();

	case kOpcodeAutoStart:
		return getPosition()->autoStart(parameters);

	case kOpcodeAutoCheck:
		return getPosition()->autoCheck(parameters);

	case kOpcodeAutoStop:
		return getPosition()->autoStop(parameters);

	case kOpcodeSetBg:
		return getGraphics()->setBackground(parameters);

	case kOpcodeSetHeroRGB:
		return getGraphics()->setHeroRGB(parameters);

	case kOpcodeProgressInit:
		return getProgress()->init(parameters);

	case kOpcodeProgressSet:
		return getProgress()->set(parameters);

	case kOpcodeProgressGet:
		return getProgress()->get(parameters);

	case kOpcodeProgressHelp:
		return getProgress()->help(parameters);

	case kOpcodeAuto2Hero:
		return getHero()->auto2hero(parameters);

	case kOpcodeAuto2HeroGlobal:
		return getHero()->auto2heroGlobal(parameters);

	case kOpcodeAuto2HeroHeroVar:
		return getHero()->auto2heroHeroVar(parameters);

	case kOpcodeAuto2HeroVar:
		return getHero()->auto2heroVar(parameters);

	case kOpcodeBarrier:
	case kOpcodeBarrier2:
	case kOpcodeBarrier3:
	case kOpcodeBarrier4:
		return getPosition()->barrier(parameters);

	case kOpcodeHearGlobal:
		return getHero()->hearGlobal(parameters);

	case kOpcodeHearHeroVar:
		return getHero()->hearHeroVar(parameters);

	case kOpcodeHearVar:
		return getHero()->hearVar(parameters);

	case kOpcodeArrayGetScroll:
		return getArray()->getScroll(parameters);

	case kOpcodeFace2Hero:
		return getHero()->face2hero(parameters);

	case kOpcodeFace2HeroGlobal:
		return getHero()->face2heroGlobal(parameters);

	case kOpcodeFace2HeroHeroVar:
		return getHero()->face2heroHeroVar(parameters);

	case kOpcodeFace2HeroVar:
		return getHero()->face2heroVar(parameters);

	case kOpcodeSaveTop:
		return getWork()->savetop(parameters);

	case kOpcodeEndTop:
		return getWork()->endtop();

	case kOpcodeMouseScroll:
		return getMouse()->scroll(parameters);

	case kOpcodeVolMusic:
		return getSound()->volume(parameters);
	}
}

} // End of namespace Liath
