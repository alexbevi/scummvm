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

#include "liath/data/segment.h"

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

// Name of all opcodes
const char *g_opcodes[] = {
	"None",
	"1",
	"Key",
	"MyRandom",
	"4",
	"5",                         // 5
	"6",
	"7",
	"8",
	"9",
	"10",                        // 10
	"11",
	"12",
	"13",
	"14",
	"15",                        // 15
	"16",
	"HeroVar",
	"HeroSys",
	"Var",
	"Sys",                       // 20
	"Global",
	"Expression",
	"Cel",
	"CelExt",
	"25",                        // 25
	"26",
	"27",
	"28",
	"29",
	"Look",                      // 30
	"Pers",
	"Dist",
	"PassVar",
	"ExitGame",
	"ExitHero",                  // 35
	"36",
	"37",
	"38",
	"39",
	"40",                        // 40
	"41",
	"42",
	"43",
	"CurAction",
	"StartAction",               // 45
	"StartActionGlobal",
	"StartActionHeroVar",
	"StartActionVar",
	"StartObject",
	"StartObject2",              // 50
	"StartObject3",
	"StartObject4",
	"StartHero",
	"StartHeroExt",
	"ResetHero",                 // 55
	"56",
	"57",
	"58",
	"59",
	"60",                        // 60
	"61",
	"62",
	"63",
	"64",
	"65",                        // 65
	"66",
	"67",
	"68",
	"69",
	"70",                        // 70
	"71",
	"72",
	"73",
	"74",
	"75",                        // 75
	"76",
	"77",
	"78",
	"79",
	"80",                        // 80
	"81",
	"Unfreeze",
	"Unfreeze2",
	"UnfreezeGlobal",
	"UnfreezeHeroVar",           // 85
	"UnfreezeVar",
	"Freeze",
	"Freeze2",
	"FreezeGlobal",
	"FreezeHeroVar",             // 90
	"FreezeVar",
	"Stop",
	"Stop2",
	"StopGlobal",
	"StopHeroVar",              // 95
	"StopVar",
	"Continue",
	"Continue2",
	"ContinueGlobal",
	"ContinueHeroVar",           // 100
	"ContinueVar",
	"102",
	"103",
	"104",
	"105",                        // 105
	"106",
	"107",
	"108",
	"109",
	"110",                        // 110
	"111",
	"112",
	"113",
	"114",
	"115",                        // 115
	"116",
	"117",
	"118",
	"119",
	"120",                        // 120
	"121",
	"122",
	"123",
	"124",
	"125",                        // 125
	"126",
	"127",
	"128",
	"129",
	"130",                        // 130
	"131",
	"132",
	"133",
	"134",
	"135",                        // 135
	"QuitHero",
	"QuitGlobal",
	"QuitHeroVar",
	"QuitVar",
	"HeroVarGlobal",             // 140
	"HeroVarHeroVar",
	"HeroVarVar",
	"PlayWave",
	"PlayMidi",
	"PlayMidiLoop",              // 145
	"StopMidi",
	"AutoStart",
	"AutoStop",
	"149",
	"150",                       // 150
	"Auto2Hero",
	"152",
	"Auto2HeroGlobal",
	"Auto2HeroHeroVar",
	"Auto2HeroVar",              // 155
	"Barrier",
	"157",
	"HearGlobal",
	"HearHeroVar",
	"HearVar",                   // 160
	"161",
	"Face2Hero",
	"Face2HeroGlobal",
	"Face2HeroHeroVar",
	"Face2HeroVar",              // 165
	"HPers",
	"Feel",
	"Barrier2",
	"Barrier3",
	"170",                       // 170
	"171",
	"SaveTop",
	"EndTop",
	"GlassIncrement",
	"GlassDecrement",            // 175
	"TypeMouse",
	"KeyMouse",
	"Bkey",
	"AutoCheck",
	"Barrier4",                  // 180
	"AddMouseBox",
	"PushMouse",
	"PopMouse",
	"Redraw",
	"PushHerous",                // 185
	"PopHerous",
	"HTime",
	"PlayAvsStopMusic",
	"DelMouseBox",
	"PlayMusic",                 // 190
	"StopMusic",
	"CommandNumberBox",
	"Cash",
	"194",
	"SaveGame",                  // 195
	"LoadGame",
	"TextOut",
	"TextQuit",
	"TextExit",
	"TextChange",                // 200
	"PlayAvs",
	"SaveHero",
	"LoadHero",
	"NumberAct",
	"TextCheck",                 // 205
	"ArrayInit",
	"ArrayAdd",
	"ArrayDel",
	"ArrayKill",
	"ArrayClear",                // 210
	"ArrayGet",
	"ArraySize",
	"ArrayTGet",
	"ArrayCur",
	"ArraySetCur",               // 215
	"ArrayGetScroll",
	"HDark",
	"ArrayImg",
	"CheckLimits",
	"XScroll",                   // 220
	"YScroll",
	"CheckMouse",
	"MousePress",
	"SetBg",
	"SetHeroRGB",                // 225
	"ArrayGetNumber",
	"ResetMouse",
	"MouseScroll",
	"VolMusic",
	"GStopMusic",                // 230
	"PlayMusicEffects",
	"ProgressInit ",
	"ProgressSet",
	"ProgressGet",
	"ProgressHelp",              // 235
	"Restart"
};

#define OPCODE_NAME(opcode) (opcode > 237 ? Common::String::format("%d", opcode).c_str() : g_opcodes[opcode])

Interpreter::Interpreter(LiathEngine *engine) : _engine(engine) {}

Interpreter::~Interpreter() {
	// Zero-out passed pointers
	_engine = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Interpreter
//////////////////////////////////////////////////////////////////////////
int Interpreter::interpret(ObjectIndex *pObjectIndex, Segment segment) const {
	getSegment()->set(kSegmentExpression, segment);

	while (pObjectIndex) {

		ObjectIndex objectIndex = *pObjectIndex;
		OpcodeParameters *parameters = (objectIndex ? (OpcodeParameters *)getSegment()->getData(kSegmentExpression, objectIndex) : NULL);

		if (parameters) {

			for (;;) {
				switch (run(parameters, objectIndex)) {
				default:
				case kOpcodeRetDefault:
					objectIndex = parameters->objectIndex;
					parameters = (objectIndex ? (OpcodeParameters *)getSegment()->getData(kSegmentExpression, objectIndex) : NULL);

					if (!parameters)
						goto update_index;
					break;

				case kOpcodeRetNext:
					if (pObjectIndex[2])
						pObjectIndex = (pObjectIndex[2] ? (ObjectIndex *)getSegment()->getData(kSegmentExpression, pObjectIndex[2]) : NULL);
					else
						goto update_index;
					break;

				case kOpcodeRetExit:
					return 0;

				case kOpcodeRetExitSuccess:
				case kOpcodeRetExitSuccess2:
					return 2;
				}
			}
		} else {
update_index:
			if (pObjectIndex[3])
				pObjectIndex = (ObjectIndex *)getSegment()->getData(kSegmentExpression, pObjectIndex[3]);
			else if (pObjectIndex[1])
				pObjectIndex = (ObjectIndex *)getSegment()->getData(kSegmentExpression, pObjectIndex[1]);
			else
				pObjectIndex = NULL;
		}
	}

	return 1;
}

OpcodeRet Interpreter::run(OpcodeParameters *parameters, ObjectIndex index) const {
	debugC(kLiathDebugInterpreter, "Executing opcode: %s (%02d)", OPCODE_NAME(parameters->opcode), parameters->opcode);

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
		return getHero()->exitHero(parameters);

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
		return getText()->exitText(parameters);

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
		return getArray()->setCur(parameters);

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
