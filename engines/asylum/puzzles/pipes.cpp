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

#include "asylum/puzzles/pipes.h"

#include "asylum/resources/worldstats.h"

#include "asylum/system/cursor.h"
#include "asylum/system/graphics.h"
#include "asylum/system/screen.h"

#include "asylum/views/scene.h"

#include "asylum/asylum.h"

#define PT Common::Point
#define DRAW(resourceId, frameIndex, point, priority) \
getScreen()->addGraphicToQueue(getWorld()->graphicResourceIds[resourceId], frameIndex, point, 0, 0, priority)

namespace Asylum {

const PT connectorPoints[] = {
		PT(158,  59),
		PT(202,  59),
		PT(271,  60),
		PT(380,  72),
		PT(205, 132),
		PT(272, 131),
		PT(469, 119),
		PT(163, 172),
		PT(206, 172),
		PT(318, 169),
		PT(360, 171),
		PT(428, 172),
		PT(466, 171),
		PT(319, 206),
		PT(360, 206),
		PT(168, 272),
		PT(273, 262),
		PT(318, 261),
		PT(401, 242),
		PT(399, 295),
		PT(460, 294)
};

const PT peepholePoints[] = {
		PT(140,  65),
		PT(311,  44),
		PT(387,  48),
		PT(475,  72),
		PT(189,  67),
		PT(246,  66),
		PT(169, 113),
		PT(215, 106),
		PT(280, 105),
		PT(336,  95),
		PT(434,  80),
		PT(248, 136),
		PT(303, 154),
		PT(407, 125),
		PT(470, 151),
		PT(193, 180),
		PT(347, 176),
		PT(401, 177),
		PT(245, 201),
		PT(325, 196),
		PT(347, 212),
		PT(406, 213),
		PT(431, 218),
		PT(174, 228),
		PT(217, 234),
		PT(280, 227),
		PT(325, 239),
		PT(370, 244),
		PT(467, 239),
		PT(303, 267),
		PT(405, 273),
		PT(356, 293),
		PT(436, 294),
		PT(182, 317),
		PT(277, 299),
		PT(324, 291),
		PT(461, 323)
};

const uint32 peepholeResources[] = {15, 15, 15, 15, 32, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 32, 32, 15,
							15, 32, 32, 15, 15, 15, 15,15, 15, 15, 15, 32, 15, 15, 15, 15, 15, 15, 15};

bool PuzzlePipes::Peephole::marks[peepholesCount];

#include "asylum/puzzles/pipesclasses.cpp"

PuzzlePipes::PuzzlePipes(AsylumEngine *engine) : Puzzle(engine) {
	_previousMusicVolume = 0;
	_rectIndex = -2;

	_frameIndex = _frameIndexLever = 0;
	memset(&_levelFlags, false, sizeof(_levelFlags));
	_levelFlags[4] = true;
}

PuzzlePipes::~PuzzlePipes() {
}

void PuzzlePipes::reset() {
	warning("[PuzzlePipes::reset] Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Event Handling
//////////////////////////////////////////////////////////////////////////
bool PuzzlePipes::init(const AsylumEvent &evt) {
	_previousMusicVolume = getSound()->getMusicVolume();

	if (_previousMusicVolume >= -1000)
		getSound()->setMusicVolume(-1000);

	getSound()->playSound(getWorld()->graphicResourceIds[41], true, Config.ambientVolume);
	getScreen()->setPalette(getWorld()->graphicResourceIds[0]);
	getScreen()->setGammaLevel(getWorld()->graphicResourceIds[0], 0);

	_rectIndex = -2;

	//updateCursor();
	initResources();
	setup(true);

	return true;
}

bool PuzzlePipes::update(const AsylumEvent &evt) {
	getScreen()->clear();
	getScreen()->clearGraphicsInQueue();
	DRAW(1, 0, PT(0, 0), 4);

	for (uint32 i = 0; i < ARRAYSIZE(_connectors); ++i)
		DRAW(_connectorResources[_connectors[i].getState()], 0, connectorPoints[i], 1);

	DRAW(18, 0, PT(210, 444 - int(_levelValues[0] * 52)), 3);
	DRAW(18, 0, PT(276, 455 - int(_levelValues[1] * 52)), 3);
	DRAW(18, 0, PT(376, 448 - int(_levelValues[2] * 52)), 3);
	DRAW(18, 0, PT(458, 442 - int(_levelValues[3] * 52)), 3);

	DRAW(33, 0, PT(204, 377), 1);

	_frameIndex = (_frameIndex + 1) % GraphicResource::getFrameCount(_vm, getWorld()->graphicResourceIds[15]);
	for (uint32 i = 0; i < ARRAYSIZE(_peepholes); ++i)
		if (_peepholes[i].isConnected())
			DRAW(peepholeResources[i], _frameIndex, peepholePoints[i], 1);

	DRAW(2, _frameIndexLever, PT(540, 90), 1);
	_isLeverReady = false;	
	if (_frameIndexLever) {
		_frameIndexLever = (_frameIndexLever + 1) % GraphicResource::getFrameCount(_vm, getWorld()->graphicResourceIds[2]);
		if (!_frameIndexLever)
			_isLeverReady = true;
	}

	if (_levelFlags[0])
		DRAW(40, 0, PT(233, 416), 1);
	else if (_levelFlags[1])
		DRAW(40, 0, PT(299, 431), 1);
	else if (_levelFlags[2])
		DRAW(40, 0, PT(398, 421), 1);
	else if (_levelFlags[3])
		DRAW(40, 0, PT(481, 417), 1);
	 if (!_levelFlags[4])
		 DRAW(45, 0, PT(518, 108), 2);

	getScreen()->drawGraphicsInQueue();
	getScreen()->copyBackBufferToScreen();
	updateCursor();

	if (_isLeverReady && !_levelFlags[4]) {
		getScreen()->clear();
		getSound()->stop(getWorld()->graphicResourceIds[41]);
		getSound()->setMusicVolume(_previousMusicVolume);

		_vm->switchEventHandler(getScene());
	}

	return true;
}

bool PuzzlePipes::mouseLeftDown(const AsylumEvent &evt) {
	PT mousePos = getCursor()->position();

	if (Common::Rect(540, 90, 590, 250).contains(mousePos)) {
		if (!_frameIndexLever)
			++_frameIndexLever;
		getSound()->playSound(getWorld()->graphicResourceIds[43], false, Config.sfxVolume - 10);
	} else {
		if (_rectIndex != -1) {
			getSound()->playSound(getWorld()->graphicResourceIds[42], false, Config.sfxVolume - 10);

			_connectors[_rectIndex].turn();
			startUpWater();
			memset(_levelFlags, false, sizeof(_levelFlags));
			_levelFlags[checkFlags()] = true;
		}
	}

	return true;
}

bool PuzzlePipes::mouseRightDown(const AsylumEvent &evt) {
	getScreen()->clear();
	getSound()->stop(getWorld()->graphicResourceIds[41]);
	getSound()->setMusicVolume(_previousMusicVolume);

	_vm->switchEventHandler(getScene());

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////
void PuzzlePipes::initResources() {
	_connectorResources[k0011] =  4;
	_connectorResources[k0110] =  3;
	_connectorResources[k1100] =  6;
	_connectorResources[k1001] =  5;

	_connectorResources[k0111] =  7;
	_connectorResources[k1110] = 10;
	_connectorResources[k1101] =  9;
	_connectorResources[k1011] =  8;

	_connectorResources[k0101] = 11;
	_connectorResources[k1010] = 12;
}

void PuzzlePipes::setup(bool val) {
	uint32 i;

	memset(&_levelValues, 0.0, sizeof(_levelValues));

	for (i = 0; i < peepholesCount; ++i)
		_peepholes[i].setId(i);

	for (i = 0; i < connectorsCount; ++i)
		_connectors[i].setId(i);

	for (i = 0; i < 4; ++i) {
		_sinks[i] = &_peepholes[peepholesCount - 4 + i];
		_sources[i] = &_peepholes[i];
		memset(&_sources[i]->_flowValues, 0, sizeof(_sources[i]->_flowValues));
		_sources[i]->_flowValues[i] = 1;
	}

	_connectors[ 0].init(NULL,	      _peepholes +  4, 	_peepholes +  6, _peepholes +  0, k0110);
	_connectors[ 1].init(NULL,	      _peepholes +  5, 	_peepholes +  7, _peepholes +  4, k0110);
	_connectors[ 2].init(NULL,	      _peepholes +  1, 	_peepholes +  8, _peepholes +  5, k0110);
	_connectors[ 3].init(_peepholes +  2, _peepholes + 10, 	NULL,		 _peepholes +  9, k0011);
	_connectors[ 4].init(_peepholes +  7, _peepholes + 11, 	NULL,		 NULL,		  k0011, _connectors +  8, kS);
	_connectors[ 5].init(_peepholes +  8, _peepholes + 12, 	_peepholes + 25, _peepholes + 11, k0111);
	_connectors[ 6].init(_peepholes +  3, NULL,		_peepholes + 14, _peepholes + 13, k1100);
	_connectors[ 7].init(_peepholes +  6, _peepholes + 15, 	_peepholes + 23, NULL,		  k0110);
	_connectors[ 8].init(NULL,	      _peepholes + 18, 	_peepholes + 24, _peepholes + 15, k0111, _connectors +  4, kN);
	_connectors[ 9].init(_peepholes +  9, _peepholes + 16, 	_peepholes + 19, _peepholes + 12, k1110);
	_connectors[10].init(_peepholes + 13, _peepholes + 17, 	NULL,		 _peepholes + 16, k0111, _connectors + 14, kS);
	_connectors[11].init(_peepholes + 10, NULL, 		_peepholes + 22, _peepholes + 17, k0101, _connectors + 12, kE);
	_connectors[12].init(_peepholes + 14, NULL, 		_peepholes + 28, NULL,		  k1001, _connectors + 11, kW);
	_connectors[13].init(_peepholes + 19, _peepholes + 20, 	_peepholes + 26, NULL,		  k0011);
	_connectors[14].init(NULL,	      _peepholes + 21, 	_peepholes + 27, _peepholes + 20, k1110, _connectors + 10, kN);
	_connectors[15].init(_peepholes + 23, _peepholes + 24, 	_peepholes + 33, NULL,		  k0011);
	_connectors[16].init(_peepholes + 25, _peepholes + 29, 	_peepholes + 34, _peepholes + 18, k1011);
	_connectors[17].init(_peepholes + 26, _peepholes + 31, 	_peepholes + 35, _peepholes + 29, k1011);
	_connectors[18].init(_peepholes + 21, _peepholes + 22, 	_peepholes + 30, _peepholes + 27, k1011);
	_connectors[19].init(_peepholes + 30, _peepholes + 32, 	NULL,		 _peepholes + 31, k0011);
	_connectors[20].init(_peepholes + 28, NULL,		_peepholes + 36, _peepholes + 32, k1001);

	_connectors[ 4].initGroup();
	_connectors[10].initGroup();
	_connectors[11].initGroup();

	startUpWater();
}

void PuzzlePipes::updateCursor() {
	int32 index = findRect();

	if (_rectIndex == index)
		return;

	_rectIndex = index;
	// FIXME
	if (index > -1 || Common::Rect(540, 90, 590, 250).contains(getCursor()->position()))
		getCursor()->set(getWorld()->graphicResourceIds[16]);
	else
		getCursor()->set(getWorld()->graphicResourceIds[16], 0, kCursorAnimationNone);
}

int32 PuzzlePipes::findRect() {
	for (uint32 i = 0; i < ARRAYSIZE(connectorPoints); ++i)
		if (Common::Rect(connectorPoints[i].x - 5, connectorPoints[i].y - 5, connectorPoints[i].x + 30, connectorPoints[i].y + 30).contains(getCursor()->position()))
			return i;

	return -1;
}

uint32 PuzzlePipes::checkFlags() {
	uint32 total = _sinks[0]->getLevel1() + _sinks[1]->getLevel1() +_sinks[2]->getLevel1() + _sinks[3]->getLevel1();
	float temp;

	if (total) 
		for (uint32 i = 0; i < 4; ++i) {
			temp = _sinks[i]->getLevel1() / float(total);
			_levelValues[i] = temp * _sinks[i]->getLevel() / 4;
			if (_levelValues[i] == 1.0)
				return i;
		}
	else
		memset(_levelValues, 0, sizeof(_levelValues));

	return 4;
}

uint32 PuzzlePipes::log2(uint32 n) {
	uint32 i;
	for (i = 0; n; n >>= 1, ++i) ;
	return i - 1;
}

void PuzzlePipes::startUpWater() {
	for (uint32 i = 4; i < peepholesCount; ++i)
		memset(_peepholes[i]._flowValues, 0, sizeof(_peepholes[i]._flowValues));

	_sources[0]->startUpWater(true);
	_sources[1]->startUpWater(true);
	_sources[2]->startUpWater(true);
	_sources[3]->startUpWater(true);
}
} // End of namespace Asylum