/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1995 Presto Studios, Inc.
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
 */

#include "buried/buried.h"
#include "buried/gameui.h"
#include "buried/graphics.h"
#include "buried/inventory_window.h"
#include "buried/resources.h"
#include "buried/scene_view.h"
#include "buried/sound.h"
#include "buried/environ/scene_base.h"
#include "buried/environ/scene_common.h"

namespace Buried {

class ClickZoomToyShelf : public SceneBase {
public:
	ClickZoomToyShelf(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation);

	int mouseUp(Window *viewWindow, const Common::Point &pointLocation);
	int specifyCursor(Window *viewWindow, const Common::Point &pointLocation);

private:
	Common::Rect _toyZooms[4];
};

ClickZoomToyShelf::ClickZoomToyShelf(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation) : SceneBase(vm, viewWindow, sceneStaticData, priorLocation) {
	_toyZooms[0] = Common::Rect(102, 8, 170, 108);
	_toyZooms[1] = Common::Rect(188, 24, 228, 108);
	_toyZooms[2] = Common::Rect(282, 24, 330, 108);
	_toyZooms[3] = Common::Rect(330, 28, 366, 108);
}

int ClickZoomToyShelf::mouseUp(Window *viewWindow, const Common::Point &pointLocation) {
	for (int i = 0; i < 4; i++) {
		if (_toyZooms[i].contains(pointLocation)) {
			DestinationScene newScene;
			newScene.destinationScene = _staticData.location;
			newScene.destinationScene.depth = i + 1;
			newScene.transitionType = TRANSITION_VIDEO;
			newScene.transitionStartFrame = -1;
			newScene.transitionLength = -1;

			static const int transitionData[4] = { 13, 16, 19, 22 };
			newScene.transitionData = transitionData[i];

			((SceneViewWindow *)viewWindow)->moveToDestination(newScene);

			return SC_TRUE;
		}
	}

	return SC_FALSE;
}

int ClickZoomToyShelf::specifyCursor(Window *viewWindow, const Common::Point &pointLocation) {
	for (int i = 0; i < 4; i++)
		if (_toyZooms[i].contains(pointLocation))
			return kCursorMagnifyingGlass;

	return kCursorArrow;
}

class ToyClick : public SceneBase {
public:
	ToyClick(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation,
			int left = -1, int top = -1, int right = -1, int bottom = -1, int returnDepth = -1, int clickAnimation = -1, int returnAnimation = -1); 

	int mouseUp(Window *viewWindow, const Common::Point &pointLocation);
	int specifyCursor(Window *viewWindow, const Common::Point &pointLocation);

private:
	Common::Rect _clickRect;
	int _returnDepth;
	int _clickAnimation;
	int _returnAnimation;
};

ToyClick::ToyClick(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation,
		int left, int top, int right, int bottom, int returnDepth, int clickAnimation, int returnAnimation) :
		SceneBase(vm, viewWindow, sceneStaticData, priorLocation) {
	_clickRect = Common::Rect(left, top, right, bottom);
	_returnDepth = returnDepth;
	_clickAnimation = clickAnimation;
	_returnAnimation = returnAnimation;
}

int ToyClick::mouseUp(Window *viewWindow, const Common::Point &pointLocation) {
	if (_clickRect.contains(pointLocation)) {
		((SceneViewWindow *)viewWindow)->playSynchronousAnimation(_clickAnimation);

		if (_clickAnimation == 17) {
			((SceneViewWindow *)viewWindow)->getGlobalFlags().faHeardAgentFigure = 1;

			if (((SceneViewWindow *)viewWindow)->getGlobalFlags().generalWalkthroughMode == 1 && !((GameUIWindow *)viewWindow->getParent())->_inventoryWindow->isItemInInventory(kItemBioChipAI))
				((SceneViewWindow *)viewWindow)->displayLiveText(_vm->getString(IDS_WM_AI_LAB_TEXT));
		}

		return SC_TRUE;
	}

	DestinationScene newScene;
	newScene.destinationScene = _staticData.location;
	newScene.destinationScene.depth = _returnDepth;
	newScene.transitionType = TRANSITION_VIDEO;
	newScene.transitionData = _returnAnimation;
	newScene.transitionStartFrame = -1;
	newScene.transitionLength = -1;
	((SceneViewWindow *)viewWindow)->moveToDestination(newScene);

	return SC_TRUE;
}

int ToyClick::specifyCursor(Window *viewWindow, const Common::Point &pointLocation) {
	if (_clickRect.contains(pointLocation))
		return kCursorFinger;

	return kCursorPutDown;
}

class MainEnvironDoorDown : public SceneBase {
public:
	MainEnvironDoorDown(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation);
	int postEnterRoom(Window *viewWindow, const Location &priorLocation);
	int mouseUp(Window *viewWindow, const Common::Point &pointLocation);
	int specifyCursor(Window *viewWindow, const Common::Point &pointLocation);

private:
	bool _doorOpen;
	Common::Rect _doorRect;
};

MainEnvironDoorDown::MainEnvironDoorDown(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation) :
		SceneBase(vm, viewWindow, sceneStaticData, priorLocation) {
	_doorOpen = false;

	if (priorLocation.timeZone == _staticData.location.timeZone &&
			priorLocation.environment == _staticData.location.environment &&
			priorLocation.node == _staticData.location.node &&
			priorLocation.facing == _staticData.location.facing &&
			priorLocation.orientation == 1) {
		if (((SceneViewWindow *)viewWindow)->getGlobalFlags().faMNEnvironDoor == 1) {
			_doorOpen = true;
			_staticData.navFrameIndex = 220;
		}
	} else {
		((SceneViewWindow *)viewWindow)->getGlobalFlags().faMNEnvironDoor = 0;
	}

	_doorRect = Common::Rect(0, 0, 432, 189);
}

int MainEnvironDoorDown::postEnterRoom(Window *viewWindow, const Location &priorLocation) {
	// Play the door open movie
	DestinationScene newScene;
	newScene.destinationScene = _staticData.location;
	newScene.destinationScene.depth = 1;
	newScene.transitionType = TRANSITION_VIDEO;
	newScene.transitionData = 0;
	newScene.transitionStartFrame = -1;
	newScene.transitionLength = -1;
	((SceneViewWindow *)viewWindow)->moveToDestination(newScene);
	return SC_FALSE;
}

int MainEnvironDoorDown::mouseUp(Window *viewWindow, const Common::Point &pointLocation) {
	if (_doorRect.contains(pointLocation)) {
		_staticData.navFrameIndex = 220;
		_doorOpen = true;
		((SceneViewWindow *)viewWindow)->getGlobalFlags().faMNEnvironDoor = 1;

		// Play the door open movie
		DestinationScene newScene;
		newScene.destinationScene = _staticData.location;
		newScene.destinationScene.depth = 1;
		newScene.transitionType = TRANSITION_VIDEO;
		newScene.transitionData = 0;
		newScene.transitionStartFrame = -1;
		newScene.transitionLength = -1;
		((SceneViewWindow *)viewWindow)->moveToDestination(newScene);

		return SC_TRUE;
	}

	return SC_FALSE;
}

int MainEnvironDoorDown::specifyCursor(Window *viewWindow, const Common::Point &pointLocation) {
	if (_doorRect.contains(pointLocation))
		return kCursorFinger;

	return kCursorPutDown;
}

class MainEnvironDoorExit : public SceneBase {
public:
	MainEnvironDoorExit(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation);
	int postEnterRoom(Window *viewWindow, const Location &priorLocation);
};

MainEnvironDoorExit::MainEnvironDoorExit(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation) :
		SceneBase(vm, viewWindow, sceneStaticData, priorLocation) {
}

int MainEnvironDoorExit::postEnterRoom(Window *viewWindow, const Location &priorLocation) {
	// Play the door open movie
	DestinationScene newScene;
	newScene.destinationScene = _staticData.location;
	newScene.destinationScene.depth = 1;
	newScene.transitionType = TRANSITION_VIDEO;
	newScene.transitionData = 14;
	newScene.transitionStartFrame = -1;
	newScene.transitionLength = -1;
	((SceneViewWindow *)viewWindow)->moveToDestination(newScene);
	return SC_FALSE;
}

class EnvironDoorExitSound : public SceneBase {
public:
	EnvironDoorExitSound(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation);
	int postExitRoom(Window *viewWindow, const Location &newLocation);
};

EnvironDoorExitSound::EnvironDoorExitSound(BuriedEngine *vm, Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation) :
		SceneBase(vm, viewWindow, sceneStaticData, priorLocation) {
}

int EnvironDoorExitSound::postExitRoom(Window *viewWindow, const Location &newLocation) {
	if (_staticData.location.timeZone == newLocation.timeZone)
		_vm->_sound->playSoundEffect(_vm->getFilePath(IDS_FUTAPT_ENVIRON_DOOR_CLOSE));

	return SC_TRUE;
}

bool SceneViewWindow::startFutureApartmentAmbient(int oldTimeZone, int oldEnvironment, int environment, bool fade) {
	_vm->_sound->setAmbientSound(_vm->getFilePath(4, environment, SF_AMBIENT), fade, 64);
	return true;
}

SceneBase *SceneViewWindow::constructFutureApartmentSceneObject(Window *viewWindow, const LocationStaticData &sceneStaticData, const Location &priorLocation) {
	// TODO

	switch (sceneStaticData.classID) {
	case 11:
		return new GenericItemAcquire(_vm, viewWindow, sceneStaticData, priorLocation, 200, 83, 230, 116, kItemBioChipTranslate, 61, offsetof(GlobalFlags, faKITakenPostboxItem));
	case 12:
		return new GenericItemAcquire(_vm, viewWindow, sceneStaticData, priorLocation, 202, 80, 227, 155, kItemCheeseGirl, 59, offsetof(GlobalFlags, faKITakenPostboxItem));
	case 13:
		return new GenericItemAcquire(_vm, viewWindow, sceneStaticData, priorLocation, 203, 111, 225, 129, kItemGenoSingleCart, 63, offsetof(GlobalFlags, faKITakenPostboxItem));
	case 23:
		return new GenericItemAcquire(_vm, viewWindow, sceneStaticData, priorLocation, 81, 146, 134, 189, kItemRemoteControl, 45, offsetof(GlobalFlags, faERTakenRemoteControl));
	case 30:
		return new PlayStingers(_vm, viewWindow, sceneStaticData, priorLocation, 128, offsetof(GlobalFlags, faStingerID), offsetof(GlobalFlags, faStingerChannelID), 10, 14);
	case 37:
		return new ClickZoomToyShelf(_vm, viewWindow, sceneStaticData, priorLocation);
	case 38:
		return new ToyClick(_vm, viewWindow, sceneStaticData, priorLocation, 82, 0, 358, 189, 0, 14, 15);
	case 39:
		return new ToyClick(_vm, viewWindow, sceneStaticData, priorLocation, 104, 0, 320, 189, 0, 17, 18);
	case 40:
		return new ToyClick(_vm, viewWindow, sceneStaticData, priorLocation, 104, 10, 270, 189, 0, 20, 21);
	case 41:
		return new ToyClick(_vm, viewWindow, sceneStaticData, priorLocation, 128, 0, 332, 189, 0, 23, 24);
	case 54:
		return new ClickPlayVideo(_vm, viewWindow, sceneStaticData, priorLocation, 36, kCursorFinger, 0, 0, 432, 189);
	case 56:
		return new MainEnvironDoorDown(_vm, viewWindow, sceneStaticData, priorLocation);
	case 57:
		return new MainEnvironDoorExit(_vm, viewWindow, sceneStaticData, priorLocation);
	case 59:
		return new EnvironDoorExitSound(_vm, viewWindow, sceneStaticData, priorLocation);
	}

	warning("TODO: Future apartment scene object %d", sceneStaticData.classID);

	return new SceneBase(_vm, viewWindow, sceneStaticData, priorLocation);
}

} // End of namespace Buried