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

#include "ring/game/ring/ring_visual.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/graphics/screen.h"

#include "ring/helpers.h"
#include "ring/ring.h"

using namespace RingGame;

namespace Ring {

#pragma region VisualElement

VisualElementRing::VisualElementRing(Id id) : Visual(id) {
	_field_D  = 0;
	_field_11 = 0;
	_left = 0;
	_top = 0;
	_offsetY = 0;
	_height = 0;
	_progressMultiplier = 0;
	_progressColor = 0;
	_initialized = false;
	_progress1 = 0.0f;
	_progress2 = 0.0f;
	_progress3 = 0.0f;
	_progress4 = 0.0f;
}

VisualElementRing::~VisualElementRing() {
}

void VisualElementRing::alloc() {
	if (_initialized)
		return;

	setupProgress(90005, 0, &_progress1);
	setupProgress(90006, 1, &_progress2);
	setupProgress(90007, 2, &_progress3);
	setupProgress(90008, 3, &_progress4);

	_initialized = true;
}

void VisualElementRing::setupProgress(Id progressId, uint32 textIndex, float *width) const {
	float progress = getApp()->varGetFloat(progressId);
	if (progress > 100.0f)
		progress = 100.0f;

	if (progress < 0.0f)
		progress = 0.0f;

	getApp()->objectPresentationSetTextToPuzzle(kObjectStatusProgress, 0, textIndex, Common::String::format("%3.1f", progress));

	*width = (float)ceil(_progressMultiplier * progress * 0.01);
}

void VisualElementRing::dealloc() {
	_initialized = true;
}

void VisualElementRing::init(uint32 a1, uint32 a2, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor) {
	_field_D  = a1;
	_field_11 = a2;
	_left = left;
	_top = top;
	_offsetY = offsetY;
	_height = height;
	_progressMultiplier = progressMultiplier;
	_progressColor = progressColor;
}

void VisualElementRing::draw() {
	if (!_visible)
		return;

	getApp()->getScreenManager()->drawRectangle(Common::Rect((int16)_left, (int16)(_top),                    (int16)(_left + _progress1), (int16)(_top + _height)),                    _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect((int16)_left, (int16)(_top + _offsetY + 1),     (int16)(_left + _progress2), (int16)(_top + _offsetY + _height + 1)),     _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect((int16)_left, (int16)(_top + 2 * _offsetY + 1), (int16)(_left + _progress3), (int16)(_top + 2 * _offsetY + _height + 1)), _progressColor);
	getApp()->getScreenManager()->drawRectangle(Common::Rect((int16)_left, (int16)(_top + 3 * _offsetY - 1), (int16)(_left + _progress4), (int16)(_top + 3 * _offsetY + _height - 1)), _progressColor);
}

#pragma endregion

#pragma region Event

EventVisualRing::EventVisualRing(ApplicationRing *application) : _app(application) {}

EventVisualRing::~EventVisualRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventVisualRing::onVisualListZoneSY(Id id, uint32 type, const Common::Point &) {
	if (id == 1) {
		if (type > 0 && type <= 3) {
			_app->objectPresentationHide(kObjectLoadOk, 0);
			_app->objectPresentationHide(kObjectLoadCancel, 0);
		}
	}
}

#pragma endregion

} // End of namespace Ring
