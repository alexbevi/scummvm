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

#ifndef RING_PUZZLE_H
#define RING_PUZZLE_H

#include "ring/shared.h"

namespace Ring {

class Accessibility;
class Application;
class ImageHandle;
class Movability;
class ObjectPresentation;
class PresentationAnimation;
class PresentationImage;
class SoundEntry;
class SoundItem;
class Text;
class Visual;

class Puzzle : public BaseObject {
public:
	Puzzle(Application *application, PuzzleId id);
	~Puzzle();

	void setBackgroundImage(Common::String filename, uint32 a3, uint32 a4, bool isActive, LoadFrom loadFrom);

	void addPresentationText(Text *text);
	void addPresentationImage(ImageHandle *image);
	void addPresentationAnimation(ObjectPresentation *objectPresentation);
	void addAccessibility(Accessibility *accessibility);

	// Movability
	void addMovability(Movability *movability);
	void setMovabilityOnOrOff(bool enableHotspot);
	void setMovabilityOnOrOff(bool enableHotspot, uint32 fromMovability, uint32 toMovability);

	// Sound
	void addAmbientSound(SoundEntry *entry, uint32 volume, uint32 a3, bool isOn, uint32 fadeFrames, uint32 a6, uint32 a7);
	void add3DSound(SoundEntry *entry, uint32 volume, bool isOn, uint32 a4, uint32 a5, uint32 fadeFrames, float a7, uint32 a9);
	void setAmbientSoundOn(Id soundId);
	void setAmbientSoundOff(Id soundId);
	void setAmbientSoundVolume(Id soundId, uint32 volume);

	// Accessors
	Movability *getMovability(uint32 index);

private:
	Application *_application;

	ImageHandle *_background;
	Common::Array<Movability *>            _movabilities;
	Common::Array<Accessibility *>         _accessibilities;
	Common::Array<ImageHandle *>           _presentationImages;
	Common::Array<ObjectPresentation *>    _presentationAnimations;
	Common::Array<Text *>                  _texts;
	Common::Array<SoundItem *>             _soundItems;
	Common::Array<Visual *>                _visuals;
	uint32 _field_24;
	byte   _field_28;
	uint32 _field_29;

	SoundItem *getSoundItem(Id soundId);

	static bool imagePriorityCompare(ImageHandle *image1, ImageHandle *image2);
};

} // End of namespace Ring

#endif // RING_PUZZLE_H
