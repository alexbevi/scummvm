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
	_field_15 = 0;;
	_field_19 = 0;
	_field_1D = 0;
	_field_21 = 3;
	_field_25 = 0;
	_field_29 = 0;
	_field_2D = 3;
	_field_31 = 101;
	_field_32 = 0;
	_field_36 = 0;
	_field_3A = 0;
	_field_3E = 3;
	_field_42 = 0;
	_field_46 = 0;
	_field_4A = 3;
	_field_4E = 101;
	_field_4F = 0;
	_field_53 = 0;
	_field_57 = 0;
	_field_5B = 3;
	_field_5F = 0;
	_field_63 = 0;
	_field_67 = 3;
	_field_6B = 101;
	_field_6C = 0;
	_field_70 = 0;
	_field_74 = 0;
	_field_78 = 3;
	_field_7C = 0;
	_field_80 = 0;
	_field_84 = 3;
	_field_88 = 101;
	_animationImage = NULL;
}

Object::~Object() {
	CLEAR_ARRAY(Accessibility, _accessibilities);
	CLEAR_ARRAY(ObjectPresentation, _presentations);
	SAFE_DELETE(_animationImage);

	// Zero-out passed pointers
	_application = NULL;
}

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
