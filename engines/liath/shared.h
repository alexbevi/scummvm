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

#ifndef LIATH_SHARED_H
#define LIATH_SHARED_H

#include "common/array.h"
#include "common/rect.h"
#include "common/scummsys.h"

namespace Liath {

//////////////////////////////////////////////////////////////////////////
// Types
//////////////////////////////////////////////////////////////////////////
typedef uint32 HeroIndex;
typedef uint32 ObjectIndex;

//////////////////////////////////////////////////////////////////////////
// Enumerations
//////////////////////////////////////////////////////////////////////////
enum SegmentType {
	kSegmentGame,
	kSegmentAction,
	kSegmentHero,
	kSegmentExpression
};

enum ActionIndex {
	kActionNone = 0
};

enum ParamValue {
	kParamClearAvi = 0xFFFFFFEA
};

enum ParamOrigin {
	kOriginGlobal = 1,
	kOriginHero = 2,
	kOriginHeroWork = 5,
	kOriginParam = 7
};

enum OpcodeRet {
	kOpcodeRetNextOffset   = -1,
	kOpcodeRetExit         = 0,
	kOpcodeRetDefault      = 1,
	kOpcodeRetExitSuccess  = 2,
	kOpcodeRetExitSuccess2 = 3
};

enum Opcode {
	kOpcodeNone                 = 0,

	kOpcodeKey                  = 2,
	kOpcodeMyRandom             = 3,

	kOpcodeHeroVar              = 17,
	kOpcodeHeroSys              = 18,
	kOpcodeVar                  = 19,
	kOpcodeSys                  = 20,
	kOpcodeGlobal               = 21,
	kOpcodeExpression           = 22,
	kOpcodeCel                  = 23,
	kOpcodeCelExt               = 24,

	kOpcodeLook                 = 30,
	kOpcodePers                 = 31,
	kOpcodeDist                 = 32,
	kOpcodePassVar              = 33,
	kOpcodeExitGame             = 34,
	kOpcodeExitHero             = 35,

	kOpcodeCurAction            = 44,
	kOpcodeStartAction          = 45,
	kOpcodeStartActionGlobal    = 46,
	kOpcodeStartActionHeroVar   = 47,
	kOpcodeStartActionVar       = 48,
	kOpcodeStartObject          = 49,
	kOpcodeStartObject2         = 50,
	kOpcodeStartObject3         = 51,
	kOpcodeStartObject4         = 52,
	kOpcodeStartHero            = 53,
	kOpcodeStartHeroExt         = 54,
	kOpcodeResetHero            = 55,

	kOpcodeUnfreeze             = 82,
	kOpcodeUnfreeze2            = 83,
	kOpcodeUnfreezeGlobal       = 84,
	kOpcodeUnfreezeHeroVar      = 85,
	kOpcodeUnfreezeVar          = 86,
	kOpcodeFreeze               = 87,
	kOpcodeFreeze2              = 88,
	kOpcodeFreezeGlobal         = 89,
	kOpcodeFreezeHeroVar        = 90,
	kOpcodeFreezeVar            = 91,
	kOpcodeStop                 = 92,
	kOpcodeStop2                = 93,
	kOpcodeStopGlobal           = 94,
	kOpcodeStopHeroVar          = 95,
	kOpcodeStopVar              = 96,
	kOpcodeContinue             = 97,
	kOpcodeContinue2            = 98,
	kOpcodeContinueGlobal       = 99,
	kOpcodeContinueHeroVar      = 100,
	kOpcodeContinueVar          = 101,

	kOpcodeQuitHero             = 136,
	kOpcodeQuitGlobal           = 137,
	kOpcodeQuitHeroVar          = 138,
	kOpcodeQuitVar              = 139,
	kOpcodeHeroVarGlobal        = 140,
	kOpcodeHeroVarHeroVar       = 141,
	kOpcodeHeroVarVar           = 142,
	kOpcodePlayWave             = 143,
	kOpcodePlayMidi             = 144,
	kOpcodePlayMidiLoop         = 145,
	kOpcodeStopMidi             = 146,
	kOpcodeAutoStart            = 147,
	kOpcodeAutoStop             = 148,

	kOpcodeAuto2Hero            = 151,

	kOpcodeAuto2HeroGlobal      = 153,
	kOpcodeAuto2HeroHeroVar     = 154,
	kOpcodeAuto2HeroVar         = 155,
	kOpcodeBarrier              = 156,

	kOpcodeHearGlobal           = 158,
	kOpcodeHearHeroVar          = 159,
	kOpcodeHearVar              = 160,

	kOpcodeFace2Hero            = 162,
	kOpcodeFace2HeroGlobal      = 163,
	kOpcodeFace2HeroHeroVar     = 164,
	kOpcodeFace2HeroVar         = 165,
	kOpcodeHPers                = 166,
	kOpcodeFeel                 = 167,
	kOpcodeBarrier2             = 168,
	kOpcodeBarrier3             = 169,

	kOpcodeSaveTop              = 172,
	kOpcodeEndTop               = 173,
	kOpcodeGlassIncrement       = 174,
	kOpcodeGlassDecrement       = 175,
	kOpcodeTypeMouse            = 176,
	kOpcodeKeyMouse             = 177,
	kOpcodeBkey                 = 178,
	kOpcodeAutoCheck            = 179,
	kOpcodeBarrier4             = 180,
	kOpcodeAddMouseBox          = 181,
	kOpcodePushMouse            = 182,
	kOpcodePopMouse             = 183,
	kOpcodeRedraw               = 184,
	kOpcodePushHerous           = 185,
	kOpcodePopHerous            = 186,
	kOpcodeHTime                = 187,
	kOpcodePlayAvsStopMusic     = 188,
	kOpcodeDelMouseBox          = 189,
	kOpcodePlayMusic            = 190,
	kOpcodeStopMusic            = 191,
	kOpcodeCommandNumberBox     = 192,
	kOpcodeCash                 = 193,

	kOpcodeSaveGame             = 195,
	kOpcodeLoadGame             = 196,
	kOpcodeTextOut              = 197,
	kOpcodeTextQuit             = 198,
	kOpcodeTextExit             = 199,
	kOpcodeTextChange           = 200,
	kOpcodePlayAvs              = 201,
	kOpcodeSaveHero             = 202,
	kOpcodeLoadHero             = 203,
	kOpcodeNumberAct            = 204,
	kOpcodeTextCheck            = 205,
	kOpcodeArrayInit            = 206,
	kOpcodeArrayAdd             = 207,
	kOpcodeArrayDel             = 208,
	kOpcodeArrayKill            = 209,
	kOpcodeArrayClear           = 210,
	kOpcodeArrayGet             = 211,
	kOpcodeArraySize            = 212,
	kOpcodeArrayTGet            = 213,
	kOpcodeArrayCur             = 214,
	kOpcodeArraySetCur          = 215,
	kOpcodeArrayGetScroll       = 216,
	kOpcodeHDark                = 217,
	kOpcodeArrayImg             = 218,
	kOpcodeCheckLimits          = 219,
	kOpcodeXScroll              = 220,
	kOpcodeYScroll              = 221,
	kOpcodeCheckMouse           = 222,
	kOpcodeMousePress           = 223,
	kOpcodeSetBg                = 224,
	kOpcodeSetHeroRGB           = 225,
	kOpcodeArrayGetNumber       = 226,
	kOpcodeResetMouse           = 227,
	kOpcodeMouseScroll          = 228,
	kOpcodeVolMusic             = 229,
	kOpcodeGStopMusic           = 230,
	kOpcodePlayMusicEffects     = 231,
	kOpcodeProgressInit         = 232,
	kOpcodeProgressSet          = 233,
	kOpcodeProgressGet          = 234,
	kOpcodeProgressHelp         = 235,
	kOpcodeRestart              = 236
};

//////////////////////////////////////////////////////////////////////////
// Segment
//////////////////////////////////////////////////////////////////////////
struct SegmentData {
	void *data;
	size_t size;

	SegmentData() {
		data = NULL;
		size = 0;
	}

	~SegmentData() {
		if (data)
			free(data);

		data = NULL;
	}
};

typedef Common::Array<SegmentData *> SegmentDef;
typedef Common::Array<SegmentData *> *Segment;

#include "common/pack-start.h"
//////////////////////////////////////////////////////////////////////////
// Game data
//////////////////////////////////////////////////////////////////////////
struct GameData {
	uint32 gIntOffset;
	uint32 gQuitOffset;
	uint32 objectIndexOffset;
	uint32 countHero;
	uint32 param;
	uint32 gParam;
	uint32 action;
	uint32 timer;
	uint32 paletteOffset;
} PACKED_STRUCT;

//////////////////////////////////////////////////////////////////////////
// Parameters
//////////////////////////////////////////////////////////////////////////
struct OpcodeParameters {
	uint32 objectIndex;
	byte opcode;
	byte field_5;
} PACKED_STRUCT;

struct OpcodeParametersDBDD : OpcodeParameters {
	uint32 param1;
	byte   param2;
	uint32 param3;
	uint32 param4;
	byte   param5;
	uint32 param6;
	uint32 param7;
} PACKED_STRUCT;

struct OpcodeParametersDWDD : OpcodeParameters {
	uint32 param1;
	int16  param2;
	int16  param3;
} PACKED_STRUCT;

struct OpcodeParametersBDDD : OpcodeParameters {
	byte   param1;
	uint32 param2;
	uint32 param3;
	uint32 param4;
	uint32 param5;
} PACKED_STRUCT;

struct OpcodeParametersDefault : OpcodeParameters {
	uint32 param1;
	uint32 param2;
	uint32 param3;
	uint32 param4;
	byte   field_16;
	uint32 field_17;
	uint32 field_1B;
	byte   field_1F;
	uint32 field_20;
	uint32 field_24;
	byte   field_28;
	byte   field_29;
	byte   field_2A;
	int16  field_2B;
	int16  field_2D;
} PACKED_STRUCT;

//////////////////////////////////////////////////////////////////////////
// Structures
//////////////////////////////////////////////////////////////////////////
struct Hero;

struct Object {
	uint32 field_0;

	uint16 field_41;
	uint16 field_43;
} PACKED_STRUCT;

struct Work {
	// Work *next;
	uint32 field_35;
	uint32 field_39;
	uint32 field_3D;
	uint32 field_41;
	uint32 field_45;
	byte field_57;
	bool isObjectIndexSet;
	bool field_59;
	ObjectIndex objectIndex;
	HeroIndex heroIndex;
	uint16 field_60;
	uint16 field_62;
	Object *object;
	uint16 field_68;
	//Sprite *sprite;

	uint16 field_76;

	byte status;
	uint32 time;

	uint16 field_A9;
	uint16 field_AB;
	uint16 field_AD;
	uint16 field_AF;
	uint16 field_B1;
	uint16 field_B3;
	uint16 field_B5;
	uint16 field_B7;


	uint32 field_DC;
	uint32 field_FE;


	byte field_579;

	uint32 xScroll;
	uint32 yScroll;
} PACKED_STRUCT;

struct HeroObject {
	uint32 field_0;
	uint32 field_4;
	uint32 field_8;
	uint32 field_C;
	uint32 field_10;
	uint32 field_14;
	uint32 field_18;
	uint32 field_1C;
	uint32 field_20;
	uint32 field_24;
} PACKED_STRUCT;

struct Hero {
	HeroObject *object;
	uint32 field_4;
	uint32 field_8;
	Segment *segment;
	uint32 field_10;
	uint32 field_14;
	uint32 field_18;
	HeroIndex index;
	Work *work;
	uint16 colorRed;
	uint16 colorGreen;
	uint16 colorBlue;

	uint32 *getData(uint32 offset) {
		return (uint32 *)((byte *)object + offset);
	}
} PACKED_STRUCT;

struct MouseBox {
	uint32 field_0;
	uint16 field_4;
	uint32 field_6;
	Common::Point *vertices;
	uint32 field_E;
	uint32 field_12;
	uint32 field_16;
	uint32 field_1A;
	uint32 field_1E;
	uint32 field_22;
	uint32 field_26;
	uint32 field_2A;
	uint32 field_2E;
	uint32 field_32;
	uint32 field_36;
	uint32 field_3A;
	uint32 field_3E;
	uint32 pointsCount;
	uint32 field_46;
	uint32 field_4A;
	uint32 count;
	uint32 field_52;
	//uint32 hGlobal;
	uint32 field_5A;
	uint32 field_5E;
	uint32 field_62;
	uint32 field_66;
	//Segment *segment;
} PACKED_STRUCT;

struct Array {

} PACKED_STRUCT;

#include "common/pack-end.h"

} // End of namespace Liath

#endif // LIATH_SHARED_H
