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

#include "ring/game/object.h"

#include "ring/game/puzzle.h"

#include "ring/graphics/accessibility.h"
#include "ring/graphics/animation.h"
#include "ring/graphics/hotspot.h"
#include "ring/graphics/presentation.h"

#include "ring/helpers.h"

#include "common/archive.h"
#include "common/tokenizer.h"

namespace Ring {

//////////////////////////////////////////////////////////////////////////
// Object
Object::Object(Application *application, ObjectId id, Common::String language, Common::String name, byte a5) : BaseObject(id), _application(application) {
	_language = language;
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
	_presentations.push_back(new ObjectPresentation(_application, this));
}

void Object::addTextToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String text, uint32 a5, uint32 a6, FontId fontId, byte a8, byte a9, byte a10, uint32 a11, uint32 a12, uint32 a13) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addTextToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addTextToPuzzle(puzzle, text, a5, a6, fontId, a8, a9, a10, a11, a12, a13);
}

void Object::addImageToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String filename, uint32 a5, uint32 a6, bool isActive, byte a8, uint32 priority, byte a10, LoadFrom loadFrom) {
	if (presentationIndex >= _presentations.size())
		error("[Object::addTextToPuzzle] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->addImageToPuzzle(puzzle, filename, a5, a6, isActive, a8, priority, a10, loadFrom);
}

void Object::showPresentation(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::showPresentation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->show();
}
void Object::showPresentations() {
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++)
		(*it)->show();
}

void Object::hidePresentation(uint32 presentationIndex) {
	if (presentationIndex >= _presentations.size())
		error("[Object::hidePresentation] Invalid presentation index (was: %d, max: %d)", presentationIndex, _presentations.size() - 1);

	_presentations[presentationIndex]->hide();
}

void Object::hidePresentations() {
	for (Common::Array<ObjectPresentation *>::iterator it = _presentations.begin(); it != _presentations.end(); it++)
		(*it)->hide();
}

#pragma endregion

#pragma region Accessibility

void Object::addPuzzleAccessibility(Puzzle *puzzle, Common::Rect rect, bool enabled, uint32 a9, uint32 a10) {
	Accessibility *accessibility = new Accessibility(this);
	accessibility->setHotspot(rect, enabled, a9, a10);

	_accessibilities.push_back(accessibility);

	puzzle->addAccessibility(accessibility);
}

void Object::setAccessibilityKey(uint32 accessibilityIndex, Common::KeyCode key) {
	if (accessibilityIndex >= _accessibilities.size())
		error("[Object::setAccessibilityKey] Invalid accessibility index (was: %d, max: %d)", accessibilityIndex, _accessibilities.size() - 1);

	_accessibilities[accessibilityIndex]->getHotspot()->setKey(key);
}

#pragma endregion

#pragma region Cursor

void Object::setCursor(ObjectCursor *cursor, uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType) {
	cursor->_field_0     = a2;
	cursor->_field_4     = a3;
	cursor->_field_8     = a4;
	cursor->_field_C     = a5;
	cursor->_field_10    = a6;
	cursor->_field_14    = a7;
	cursor->_field_18    = a8;
	cursor->_archiveType = archiveType;
}

void Object::setActiveCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType) {
	setCursor(&_activeCursor, a2, a3, a4, a5, a6, a7, a8, archiveType);
}

void Object::setPassiveCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType) {
	setCursor(&_passiveCursor, a2, a3, a4, a5, a6, a7, a8, archiveType);
}

void Object::setActiveDrawCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType) {
	setCursor(&_activeDrawCursor, a2, a3, a4, a5, a6, a7, a8, archiveType);
}

void Object::setPassiveDrawCursor(uint32 a2, uint32 a3, uint32 a4, uint32 a5, float a6, uint32 a7, uint32 a8, ArchiveType archiveType) {
	setCursor(&_passiveDrawCursor, a2, a3, a4, a5, a6, a7, a8, archiveType);
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// ObjectInfo
//////////////////////////////////////////////////////////////////////////
ObjectInfo::ObjectInfo(ObjectId id, Common::String language, Common::String name) :
	BaseObject(id), _language(language), _name(name) {
}

//////////////////////////////////////////////////////////////////////////
// ObjectHandler
//////////////////////////////////////////////////////////////////////////
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
		if (sscanf(line.c_str(), "%d", &id) == 1)
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

		_objects.push_back(new ObjectInfo(id, language, tokenizer.nextToken()));
	}
}

Common::String ObjectHandler::getLanguage(ObjectId id) {
	if (!_objects.has(id))
		return "";

	return _objects.get(id)->getLanguage();
}

Common::String ObjectHandler::getName(ObjectId id) {
	if (!_objects.has(id))
		return "";

	return _objects.get(id)->getName();
}

} // End of namespace Ring
