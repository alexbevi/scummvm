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

#include "ring/game/faust/faust_visual.h"

#include "ring/base/object.h"

#include "ring/game/faust/faust_application.h"
#include "ring/game/faust/faust_shared.h"

using namespace FaustGame;

namespace Ring {

EventVisualFaust::EventVisualFaust(ApplicationFaust *application) : _app(application) {
}

EventVisualFaust::~EventVisualFaust() {
	// Zero-out passed pointers
	_app = NULL;
}

void EventVisualFaust::onVisualListZoneSY(Id id, uint32 type, const Common::Point &) {
	switch (id) {
	default:
		break;

	case 1:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));
		}
		break;

	case 3:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzlePreferences);
			_app->objectPresentationSetTextToPuzzle(kObject99501, 0, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99501, 0, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99501, 0, Common::Point(width + 233, 334));
		}
		break;

	case 4:
		if (type == 3) {
			ObjectId objectId = _app->visualListGetObjectIdClicked(1, kPuzzleGeneralMenu);
			_app->objectPresentationSetTextToPuzzle(kObject99601, 1, 0, _app->getObject(objectId)->getName());

			uint32 width = _app->objectPresentationGetTextWidth(kObject99601, 1, 0);
			_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObject99601, 0, Common::Point(width + 233, 384));

			_app->objectSetAccessibilityOn(kObject99023);
			_app->objectPresentationHide(kObject99023, 1);
			_app->objectPresentationShow(kObject99023, 2);
		}
		break;
	}
}

} // End of namespace Ring
