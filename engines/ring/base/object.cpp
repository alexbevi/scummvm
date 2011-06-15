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

#include "ring/base/object.h"

#include "ring/base/accessibility.h"
#include "ring/base/application.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/text.h"

#include "ring/graphics/animation.h"
#include "ring/graphics/image.h"
#include "ring/graphics/hotspot.h"

#include "ring/ring.h"
#include "ring/helpers.h"

#include "common/tokenizer.h"

namespace Ring {

#pragma region ObjectCursor

void ObjectCursor::saveLoadWithSerializer(Common::Serializer &s) {
	s.syncAsSint16LE(offset.x);
	s.syncAsSint16LE(offset.y);
	s.syncAsUint32LE(frameCount);
	s.syncAsUint32LE(type);
	s.syncAsUint32LE(frameRate);
	s.syncAsUint32LE(field_14);
	s.syncAsUint32LE(loadFrom);
	s.syncAsUint32LE(archiveType);
}

#pragma endregion

#pragma region ObjectPresentation

ObjectPresentation::ObjectPresentation(Object *object) : _object(object) {
	_isShown = false;
}

ObjectPresentation::~ObjectPresentation() {
	CLEAR_ARRAY(ImageHandle, _imagePuzzle);
	// _imagePuzzlePtr are pointers to existing objects
	CLEAR_ARRAY(AnimationImage, _animationPuzzle);
	// _animationPuzzlePtr are pointers to existing objects
	CLEAR_ARRAY(BaseId, _layerImagePtr);
	// _layImageRotationPtr are pointers to existing objects
	CLEAR_ARRAY(BaseId, _layerAnimationRotation);
	// _layerAnimationRotationPtr are pointers to existing objects
	CLEAR_ARRAY(Animation, _layerAnimationRotationAnimation);
	CLEAR_ARRAY(Text, _textPuzzle);
	// _textPuzzlePtr are pointers to existing objects
	CLEAR_ARRAY(Text, _textRotation);
	// _textRotationPtr are pointers to existing objects

	// Zero-out passed pointers
	_object = NULL;
}

void ObjectPresentation::addTextToPuzzle(Puzzle *puzzle, Common::String text, const Common::Point &point, FontId fontId, const Color &foreground, const Color &background) {
	Text *textObject = new Text();
	textObject->init(text, point, fontId, foreground, background);
	textObject->setObjectPresentation(this);

	_textPuzzle.push_back(textObject);
	_textPuzzlePtr.push_back(puzzle);

	puzzle->addPresentationText(textObject);
}

void ObjectPresentation::setTextToPuzzle(uint32 textIndex, Common::String text) {
	if (textIndex >= _textPuzzle.size())
		error("[ObjectPresentation::setTextToPuzzle] Invalid text index (was:%d, max:%d)", textIndex, _textPuzzle.size() - 1);

	_textPuzzle[textIndex]->set(text);
}

void ObjectPresentation::setTextCoordinatesToPuzzle(uint32 textIndex, const Common::Point &point) {
	if (textIndex >= _textPuzzle.size())
		error("[ObjectPresentation::setTextCoordinatesToPuzzle] Invalid text index (was:%d, max:%d)", textIndex, _textPuzzle.size() - 1);

	_textPuzzle[textIndex]->setCoordinates(point);
}

uint32 ObjectPresentation::getTextWidth(uint32 textIndex) {
	if (textIndex >= _textPuzzle.size())
		error("[ObjectPresentation::getTextWidth] Invalid text index (was:%d, max:%d)", textIndex, _textPuzzle.size() - 1);

	return _textPuzzle[textIndex]->getWidth();
}

void ObjectPresentation::addImageToPuzzle(Puzzle *puzzle, Common::String filename, const Common::Point &point, bool isActive, DrawType drawType, uint32 priority, byte a9, LoadFrom loadFrom) {
	ImageHandle *image = new ImageHandle(filename, point, isActive, drawType, priority, a9, getApp()->getCurrentZone(), loadFrom, kImageTypeBackground, getApp()->getArchiveType());
	image->setObjectPresentation(this);

	_imagePuzzle.push_back(image);
	_imagePuzzlePtr.push_back(puzzle);

	puzzle->addPresentationImage(image);
}

void ObjectPresentation::addImageToRotation(Rotation *rotation, uint32 layer) {
	_layerImagePtr.push_back(new BaseId(layer));
	_layImageRotationPtr.push_back(rotation);
}

void ObjectPresentation::setImageCoordinatesOnPuzzle(const Common::Point &point) {
	for (Common::Array<ImageHandle *>::iterator it = _imagePuzzle.begin(); it != _imagePuzzle.end(); it++)
		(*it)->setCoordinates(point);
}

void ObjectPresentation::setImageCoordinatesOnPuzzle(uint32 imageIndex, const Common::Point &point) {
	if (imageIndex >= _imagePuzzle.size())
		error("[ObjectPresentation::setImageCoordinatesOnPuzzle] Invalid image index (was:%d, max:%d)", imageIndex, _imagePuzzle.size() - 1);

	_imagePuzzle[imageIndex]->setCoordinates(point);
}

void ObjectPresentation::setImageOriginalCoordinatesOnPuzzle() {
	for (Common::Array<ImageHandle *>::iterator it = _imagePuzzle.begin(); it != _imagePuzzle.end(); it++)
		(*it)->setCoordinates((*it)->getOriginalCoordinates());
}

Common::Point ObjectPresentation::getImageCoordinatesOnPuzzle(uint32 imageIndex) {
	if (imageIndex >= _imagePuzzle.size())
		error("[ObjectPresentation::getImageCoordinatesOnPuzzle] Invalid image index (was:%d, max:%d)", imageIndex, _imagePuzzle.size() - 1);

	return _imagePuzzle[imageIndex]->getCoordinates();
}

void ObjectPresentation::addAnimationToPuzzle(Puzzle *puzzle, Common::String filename, ImageType imageType, const Common::Point &point, uint32, DrawType drawType, uint32 priority, byte imageCount, uint32 frameCount, float frameRate, byte a13, LoadFrom loadFrom) {
	AnimationImage *animation = new AnimationImage();
	animation->init(filename, imageType, point, 0, drawType, frameCount, frameRate, 1, a13, imageCount, priority, loadFrom, getApp()->getArchiveType());
	animation->updatePresentation(this);
	if (!(a13 & 2)) {
		animation->setField20(0);
		animation->setField89();
	}

	_animationPuzzle.push_back(animation);
	_animationPuzzlePtr.push_back(puzzle);

	animation->updateCurrentImage();

	puzzle->addPresentationImage(animation->getCurrentImage());
	puzzle->addPresentationAnimation(this);
}

void ObjectPresentation::addAnimationToRotation(Rotation *rotation, uint32 layer, uint32 a3, float a4, byte a5) {
	Animation *animation = rotation->addPresentationAnimation(this, layer, a3, a4, a5);

	if (!(a5 & 2))
		animation->setField20(0);

	_layerAnimationRotation.push_back(new BaseId(layer));
	_layerAnimationRotationPtr.push_back(rotation);
	_layerAnimationRotationAnimation.push_back(animation);
}

void ObjectPresentation::setAnimationOnPuzzle(uint32 animationIndex, Id id) {
	if (animationIndex >= _animationPuzzle.size())
		error("[ObjectPresentation::setAnimationOnPuzzle] Invalid animation index (was: %d, max: %d)", animationIndex, _animationPuzzle.size() - 1);

	_animationPuzzle[animationIndex]->setId(id);
}

void ObjectPresentation::setAnimationOnRotation(uint32 animationIndex, Id id) {
	if (animationIndex >= _layerAnimationRotationAnimation.size())
		error("[ObjectPresentation::setAnimationOnPuzzle] Invalid animation index (was: %d, max: %d)", animationIndex, _layerAnimationRotationAnimation.size() - 1);

	_layerAnimationRotationAnimation[animationIndex]->setId(id);
}

void ObjectPresentation::setAnimationStartFrame(uint32 startFrame) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setStartFrame(startFrame);

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->setStartFrame(startFrame);
}

void ObjectPresentation::setAnimationActiveFrame(uint32 activeFrame) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setActiveFrame(activeFrame);

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->setActiveFrame(activeFrame);
}

void ObjectPresentation::setAnimationCoordinatesOnPuzzle(const Common::Point &point) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setCoordinates(point);
}

void ObjectPresentation::pauseAnimation() {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->pause();

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->pause();
}

void ObjectPresentation::unpauseAnimation() {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->unpause();

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->unpause();
}

void ObjectPresentation::pauseFrameAnimation(uint32 frame, uint32 a3, uint32 a4) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->pauseOnFrame(frame, a3, a4);

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->pauseOnFrame(frame, a3, a4);
}

void ObjectPresentation::playPuzzleAnimations(uint32 ticks) {
	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->computeCurrentFrame(ticks);
}

void ObjectPresentation::playRotationAnimations(uint32 ticks) {
	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->computeCurrentFrame(ticks);
}

void ObjectPresentation::show() {
	_isShown = true;

	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->setTicks(g_system->getMillis());

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->setTicks(g_system->getMillis());

	for (uint32 i = 0; i < _layerImagePtr.size(); i++)
		_layImageRotationPtr[i]->setChannel((uint32)*(_layerImagePtr[i]), 1);

	for (uint32 i = 0; i < _layerAnimationRotation.size(); i++)
		_layerAnimationRotationPtr[i]->setChannel((uint32)_layerAnimationRotation[i]->id(), 1);
}

void ObjectPresentation::hide() {
	_isShown = false;

	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->sub_416710();

	for (Common::Array<Animation *>::iterator it = _layerAnimationRotationAnimation.begin(); it != _layerAnimationRotationAnimation.end(); it++)
		(*it)->sub_416710();

	for (uint32 i = 0; i < _layerImagePtr.size(); i++)
		_layImageRotationPtr[i]->setChannel((uint32)*(_layerImagePtr[i]), 0);

	for (uint32 i = 0; i < _layerAnimationRotation.size(); i++)
		_layerAnimationRotationPtr[i]->setChannel((uint32)_layerAnimationRotation[i]->id(), 0);
}

void ObjectPresentation::hideAndRemove() {
	hide();

	for (Common::Array<ImageHandle *>::iterator it = _imagePuzzle.begin(); it != _imagePuzzle.end(); it++) {
		ImageHandle *image = (*it);

		if (!image->isInitialized())
			continue;

		if (image->getField6C() == 1) {
			image->destroy();
		} else if (image->getAnimation()) {
			image->getAnimation()->dealloc();
		} else {
			error("[ObjectPresentation::hideAndRemove] Invalid animation in ImageHandle!");
		}
	}

	for (Common::Array<AnimationImage *>::iterator it = _animationPuzzle.begin(); it != _animationPuzzle.end(); it++)
		(*it)->dealloc();
}

#pragma region Serializable

void ObjectPresentation::saveLoadWithSerializer(Common::Serializer &s) {
	SaveManager::syncArray(s, &_imagePuzzle);
	SaveManager::syncArray(s, &_animationPuzzle);
	SaveManager::syncArray(s, &_textPuzzle);
	SaveManager::syncArray(s, &_textRotation);

	s.syncAsByte(_isShown);
}

#pragma endregion

#pragma endregion

#pragma region Object

Object::Object(Application *application, ObjectId id, Common::String description, Common::String name, byte a5) : BaseObject(id), _application(application) {
	_description = description;
	_name = name;
	_field_C  = a5;
	_animationImage = NULL;
}

Object::~Object() {
	CLEAR_ARRAY(Accessibility, _accessibilities);
	CLEAR_ARRAY(ObjectPresentation, _presentations);
	SAFE_DELETE(_animationImage);

	// Zero-out passed pointers
	_application = NULL;
}

#pragma region Presentation

void Object::addPresentation() {
	_presentations.push_back(new ObjectPresentation(this));
}

void Object::addTextToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String text, const Common::Point &point, FontId fontId, const Color &foreground, const Color &background) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addTextToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addTextToPuzzle(puzzle, text, point, fontId, foreground, background);
}

void Object::setTextToPuzzle(uint32 presentationIndex, uint32 textIndex, Common::String text) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setTextToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setTextToPuzzle(textIndex, text);
}

void Object::setTextCoordinatesToPuzzle(uint32 presentationIndex, uint32 textIndex, const Common::Point &point) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setTextCoordinatesToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setTextCoordinatesToPuzzle(textIndex, point);
}

uint32 Object::getTextWidth(uint32 presentationIndex, uint32 textIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::getTextWidth] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	return _presentations[presentationIndex]->getTextWidth(textIndex);
}

void Object::addImageToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String filename, const Common::Point &point, bool isActive, DrawType drawType, uint32 priority, byte a10, LoadFrom loadFrom) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addTextToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addImageToPuzzle(puzzle, filename, point, isActive, drawType, priority, a10, loadFrom);
}

void Object::addImageToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addImageToRotation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addImageToRotation(rotation, layer);
}

void Object::setImageCoordinatesOnPuzzle(uint32 presentationIndex, const Common::Point &point) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setImageCoordinatesOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setImageCoordinatesOnPuzzle(point);
}

void Object::setImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex, const Common::Point &point) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setImageCoordinatesOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setImageCoordinatesOnPuzzle(imageIndex, point);
}

void Object::setImageOriginalCoordinatesOnPuzzle(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setImageOriginalCoordinatesOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setImageOriginalCoordinatesOnPuzzle();
}

Common::Point Object::getImageCoordinatesOnPuzzle(uint32 presentationIndex, uint32 imageIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::getImageCoordinatesOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	return _presentations[presentationIndex]->getImageCoordinatesOnPuzzle(imageIndex);
}

void Object::show(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::showPresentation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->show();
}
void Object::show() {
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++)
		(*it)->show();
}

void Object::hide(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::hidePresentation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->hide();
}

void Object::hide() {
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++)
		(*it)->hide();
}

void Object::hideAndRemove(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::hidePresentation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->hideAndRemove();
}

void Object::hideAndRemove() {
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++)
		(*it)->hideAndRemove();
}

#pragma endregion

#pragma region Accessibility

void Object::addPuzzleAccessibility(Puzzle *puzzle, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	Accessibility *accessibility = new Accessibility(this);
	accessibility->setHotspot(rect, enabled, cursorId, a10);

	_accessibilities.push_back(accessibility);

	puzzle->addAccessibility(accessibility);
}

void Object::addRotationAccessibility(Rotation *rotation, const Common::Rect &rect, bool enabled, CursorId cursorId, uint32 a10) {
	Accessibility *accessibility = new Accessibility(this);
	accessibility->setHotspot(rect, enabled, cursorId, a10);

	_accessibilities.push_back(accessibility);

	rotation->addAccessibility(accessibility);
}

void Object::setAccessibilityKey(uint32 accessibilityIndex, Common::KeyCode key) {
	if (accessibilityIndex >= _accessibilities.size())
		error("[Object::setAccessibilityKey] Invalid accessibility index (was: %d, max: %d)", accessibilityIndex, _accessibilities.size() - 1);

	_accessibilities[accessibilityIndex]->getHotspot()->setKey(key);
}

void Object::setAccessibilityOnOrOff(bool enableHotspot) {
	for (Common::Array<Accessibility *>::iterator it = _accessibilities.begin(); it != _accessibilities.end(); it++) {
		if (enableHotspot)
			(*it)->enableHotspot();
		else
			(*it)->disableHotspot();
	}
}

void Object::setAccessibilityOnOrOff(bool enableHotspot, uint32 fromAcceleration, uint32 toAcceleration) {
	// Check from/to accelerations
	if (toAcceleration < fromAcceleration)
		error("[Object::setAccessibilityOnOrOff] From acceleration (%d) is greater than To acceleration (%d)", fromAcceleration, toAcceleration);

	if (fromAcceleration >= _accessibilities.size())
		error("[Object::setAccessibilityOnOrOff] From acceleration is not in range (was:%d, max:%d)", fromAcceleration, _accessibilities.size() - 1);

	if (toAcceleration >= _accessibilities.size())
		error("[Object::setAccessibilityOnOrOff] To acceleration is not in range (was:%d, max:%d)", fromAcceleration, _accessibilities.size() - 1);


	for (uint32 i = fromAcceleration; i <= toAcceleration; i++) {
		if (enableHotspot)
			_accessibilities[i]->enableHotspot();
		else
			_accessibilities[i]->disableHotspot();
	}
}

#pragma endregion

#pragma region Animation

void Object::addAnimationToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String name, ImageType imageType, const Common::Point &point, uint32 a8, DrawType drawType, uint32 priority, byte frameCount, uint32 a12, float a13, byte a14, LoadFrom loadFrom) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addAnimationToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addAnimationToPuzzle(puzzle, name, imageType, point, a8, drawType, priority, frameCount, a12, a13, a14, loadFrom);
}

void Object::addAnimationToRotation(uint32 presentationIndex, Rotation *rotation, uint32 layer, uint32 a5, float a6, byte a7) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addAnimationToRotation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addAnimationToRotation(rotation, layer, a5, a6, a7);
}

void Object::setAnimationOnPuzzle(uint32 presentationIndex, uint32 animationIndex, Id id) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setAnimationOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setAnimationOnPuzzle(animationIndex, id);
}

void Object::setAnimationOnRotation(uint32 presentationIndex, uint32 animationIndex, Id id) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setAnimationOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setAnimationOnRotation(animationIndex, id);
}

void Object::setAnimationStartFrame(uint32 presentationIndex, uint32 startFrame) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setAnimationStartFrame] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setAnimationStartFrame(startFrame);
}

void Object::setAnimationActiveFrame(uint32 presentationIndex, uint32 activeFrame) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setAnimationActiveFrame] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setAnimationActiveFrame(activeFrame);
}

void Object::setAnimationCoordinatesOnPuzzle(uint32 presentationIndex, const Common::Point &point) {
	if (presentationIndex >= _presentations.size())
		error("[Object::setAnimationCoordinatesOnPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->setAnimationCoordinatesOnPuzzle(point);
}

void Object::pauseAnimation(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::pauseAnimation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->pauseAnimation();
}

void Object::unpauseAnimation(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::unpauseAnimation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->unpauseAnimation();
}

void Object::pauseFrameAnimation(uint32 presentationIndex, uint32 frame, uint32 a3, uint32 a4) {
	if (presentationIndex >= _presentations.size())
		error("[Object::pauseAnimation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->pauseFrameAnimation(frame, a3, a4);
}

#pragma endregion

#pragma region Cursor

void Object::setCursor(ObjectCursor *cursor, const Common::Point &point, uint32 frameCount, CursorType type, float frameRate, byte a7, LoadFrom loadFrom, ArchiveType archiveType) const{
	cursor->offset      = point;
	cursor->frameCount  = frameCount;
	cursor->type        = type;
	cursor->frameRate   = frameRate;
	cursor->field_14    = a7;
	cursor->loadFrom    = loadFrom;
	cursor->archiveType = archiveType;
}

void Object::setActiveCursor(const Common::Point &point, uint32 frameCount, CursorType type, float frameRate, byte a7, LoadFrom loadFrom, ArchiveType archiveType) {
	setCursor(&_activeCursor, point, frameCount, type, frameRate, a7, loadFrom, archiveType);
}

void Object::setPassiveCursor(const Common::Point &point, uint32 frameCount, CursorType type, float frameRate, byte a7, LoadFrom loadFrom, ArchiveType archiveType) {
	setCursor(&_passiveCursor, point, frameCount, type, frameRate, a7, loadFrom, archiveType);
}

void Object::setActiveDrawCursor(const Common::Point &point, uint32 frameCount, CursorType type, float frameRate, byte a7, LoadFrom loadFrom, ArchiveType archiveType) {
	setCursor(&_activeDrawCursor, point, frameCount, type, frameRate, a7, loadFrom, archiveType);
}

void Object::setPassiveDrawCursor(const Common::Point &point, uint32 frameCount, CursorType type, float frameRate, byte a7, LoadFrom loadFrom, ArchiveType archiveType) {
	setCursor(&_passiveDrawCursor, point, frameCount, type, frameRate, a7, loadFrom, archiveType);
}

#pragma endregion

#pragma region Serializable

void Object::saveLoadWithSerializer(Common::Serializer &s) {
	_passiveCursor.saveLoadWithSerializer(s);
	_activeCursor.saveLoadWithSerializer(s);
	_passiveDrawCursor.saveLoadWithSerializer(s);
	_activeDrawCursor.saveLoadWithSerializer(s);

	SaveManager::syncArray(s, &_accessibilities);
	SaveManager::syncArray(s, &_presentations);

	SaveManager::syncWithFlag(s, _animationImage);
}

#pragma endregion

#pragma endregion

#pragma region ObjectInfo

ObjectInfo::ObjectInfo(ObjectId id, Common::String description, Common::String name) :
	BaseObject(id), _description(description), _name(name) {
}

#pragma endregion

#pragma region ObjectHandler

ObjectHandler::ObjectHandler() {
}

ObjectHandler::~ObjectHandler() {
	CLEAR_ARRAY(ObjectInfo, _objects);
}

void ObjectHandler::addFromFile(Common::String filename, Common::String language) {
	// Open a stream to the configuration file
	Common::SeekableReadStream *archive = SearchMan.createReadStreamForMember(filename);
	if (!archive)
		error("[ObjectHandler::addFromFile] Error opening objects file (%s)", filename.c_str());

	// Read each object info
	ObjectId id = kObjectInvalid;
	while (!archive->eos() && !archive->err()) {

		Common::String line = archive->readLine();
		if (archive->eos() || archive->err())
			break;

		// Check object id
		if (sscanf(line.c_str(), "%d", (int *)&id) == 1)
			continue;

		// Check language
		if (!line.matchString(language + "*", true))
			continue;

		// Get object name
		Common::StringTokenizer tokenizer(line, "#");
		if (tokenizer.empty())
			error("[ObjectHandler::addFromFile] Invalid line format (missing # separators)");

		tokenizer.nextToken();
		if (tokenizer.empty())
			error("[ObjectHandler::addFromFile] Invalid line format (missing object name)");

		_objects.push_back(new ObjectInfo(id, tokenizer.nextToken(), tokenizer.nextToken()));
	}

	delete archive;
}

Common::String ObjectHandler::getDescription(ObjectId id) {
	if (!_objects.has(id))
		return "";

	return _objects.get(id)->getDescription();
}

Common::String ObjectHandler::getName(ObjectId id) {
	if (!_objects.has(id))
		return "";

	return _objects.get(id)->getName();
}

#pragma endregion

} // End of namespace Ring
