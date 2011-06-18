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

#include "ring/game/faust/faust_animation.h"

#include "ring/base/saveload.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventAnimationFaust::EventAnimationFaust(ApplicationFaust *application) : _app(application) {
}

EventAnimationFaust::~EventAnimationFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventAnimationFaust::onAnimationNextFrameZoneSY(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 2 && frame == _app->getSaveManager()->getData()->progress)
		_app->objectPresentationPauseAnimation(kObject99090, 0);
}

void EventAnimationFaust::onAnimationNextFrameZone3(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone3] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone4(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone4] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone5(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone5] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone6(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	if (animationId == 31000 && frame == 10) {
		_app->objectPresentationShow(kObject31002, 0);
		_app->varSetByte(31001, 1);
	}
}

void EventAnimationFaust::onAnimationNextFrameZone7(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone7] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone8(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone8] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone9(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 61300:
		if (frame == frameCount) {
			_app->objectPresentationShow((ObjectId)(kObject61302 + _app->varGetByte(61909)), _app->varGetByte(61901 + _app->varGetByte(61909)));
			_app->objectSetAccessibilityOn(kObjectWhip);

			if (_app->varGetByte(61901) == 2 && _app->varGetByte(61902) == 2)
				_app->timerStart(kTimer2, 1000);
		}
		break;

	case 61301:
		if (frame == frameCount) {
			_app->objectPresentationShow(kObject61303, _app->varGetByte(61902));
			_app->objectSetAccessibilityOn(kObjectWhip);
		}
		break;
	}
}

void EventAnimationFaust::onAnimationNextFrameZone10(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone10] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone11(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone11] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone12(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone12] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone13(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	switch (animationId) {
	default:
		break;

	case 91001:
		if (frame == 15) {
			_app->objectPresentationHide(kObject91002);
			_app->puzzleSetActive(kPuzzle91001);
		}
		break;

	case 91002:
		if (frame == 1) {
			_app->soundPlay(91103);
			_app->objectSetAccessibilityOn(kObject91002, 0, 0);
		}
		break;

	case 91003:
		if (frame == 10) {
			_app->rotationSetAlp(91007, 320.0f);
			_app->rotationSetBet(91007, 0.0f);
			_app->rotationSetRan(91007, 87.0f);
			_app->rotationSetActive(91007);
			_app->objectPresentationHide(kObject91001, 0);
			_app->objectSetAccessibilityOn(kObject91001, 0, 0);
			_app->soundPlay(91121);
		}
		break;
	}
}

void EventAnimationFaust::onAnimationNextFrameZone14(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone14] Not implemented");
}

void EventAnimationFaust::onAnimationNextFrameZone15(Id animationId, const Common::String &name, uint32 frame, uint32 frameCount) {
	error("[EventAnimationFaust::onAnimationNextFrameZone15] Not implemented");
}

} // End of namespace Ring
