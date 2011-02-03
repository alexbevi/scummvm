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

#ifndef RING_OBJECT_H
#define RING_OBJECT_H

#include "ring/shared.h"

#include "common/events.h"

namespace Ring {

class Application;
class Accessibility;
class AnimationImage;
class ObjectPresentation;
class Puzzle;

class Object : public BaseObject {
public:
	Object(Application *application, ObjectId id, Common::String language, Common::String name, byte a5);
	~Object();

	// Presentation
	void addPresentation();
	void addTextToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String text, uint32 a5, uint32 a6, FontId fontId, byte a8, byte a9, byte a10, uint32 a11, uint32 a12, uint32 a13);
	void addImageToPuzzle(uint32 presentationIndex, Puzzle *puzzle, Common::String filename, uint32 a5, uint32 a6, bool isActive, byte a8, uint32 priority, byte a10, LoadFrom loadFrom);
	void showPresentation(uint32 presentationIndex);
	void showPresentations();
	void hidePresentation(uint32 presentationIndex);
	void hidePresentations();

	void addPuzzleAccessibility(Puzzle *puzzle, Common::Rect rect, bool enabled, uint32 a9, uint32 a10);
	void setAccessibilityKey(uint32 accessibilityIndex, Common::KeyCode key);

private:
	Application *_application;

	Common::String _language;
	Common::String _name;
	byte    _field_C;
	Common::Array<Accessibility *> _accessibilities;
	Common::Array<ObjectPresentation *> _presentations;
	uint32  _field_15;
	uint32  _field_19;
	uint32  _field_1D;
	uint32  _field_21;
	uint32  _field_25;
	uint32  _field_29;
	uint32  _field_2D;
	byte    _field_31;
	uint32  _field_32;
	uint32  _field_36;
	uint32  _field_3A;
	uint32  _field_3E;
	uint32  _field_42;
	uint32  _field_46;
	uint32  _field_4A;
	byte    _field_4E;
	uint32  _field_4F;
	uint32  _field_53;
	uint32  _field_57;
	uint32  _field_5B;
	uint32  _field_5F;
	uint32  _field_63;
	uint32  _field_67;
	byte    _field_6B;
	uint32  _field_6C;
	uint32  _field_70;
	uint32  _field_74;
	uint32  _field_78;
	uint32  _field_7C;
	uint32  _field_80;
	uint32  _field_84;
	byte    _field_88;
	AnimationImage *_animationImage;
};

class ObjectInfo : public BaseObject {
public:
	ObjectInfo(ObjectId id, Common::String language, Common::String name);
	~ObjectInfo() {}

	// Accessors
	Common::String getLanguage() { return _language; }
	Common::String getName() { return _name; }

private:
	Common::String _language;
	Common::String _name;
};

class ObjectHandler {
public:
	ObjectHandler();
	~ObjectHandler();

	void addFromFile(Common::String filename, Common::String language);

	// Accessors
	Common::String getLanguage(ObjectId id);
	Common::String getName(ObjectId id);

private:
	AssociativeArray<ObjectInfo *> _objects;
};

} // End of namespace Ring

#endif // RING_OBJECT_H
