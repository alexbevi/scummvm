/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/debug.h"

#include "engines/stark/resources/anim.h"
#include "engines/stark/xrcreader.h"

namespace Stark {

Resource *Anim::construct(Resource *parent, byte subType, uint16 index, const Common::String &name) {
	switch (subType) {
	case kAnimSub1:
		return new AnimSub1(parent, subType, index, name);
	case kAnimSub2:
		return new UnimplementedResource(parent, TYPE, subType, index, name);
	case kAnimSub3:
		return new UnimplementedResource(parent, TYPE, subType, index, name);
	case kAnimSub4:
		return new UnimplementedResource(parent, TYPE, subType, index, name);
	default:
		error("Unknown anim subtype %d", subType);
	}
}

Anim::~Anim() {
}

Anim::Anim(Resource *parent, byte subType, uint16 index, const Common::String &name) :
				Resource(parent, subType, index, name),
				_field_30(0),
				_numFrames(0),
				_refCount(0) {
	_type = TYPE;
}

void Anim::readData(XRCReadStream *stream) {
	_field_30 = stream->readUint32LE();
	_numFrames = stream->readUint32LE();
}

void Anim::reference(Item *item) {
	_refCount++;
}
void Anim::dereference(Item *item) {
	_refCount--;
}
bool Anim::isReferenced() {
	return _refCount > 0;
}

void Anim::printData() {
	debug("field_30: %d", _field_30);
	debug("numFrames: %d", _numFrames);
}

AnimSub1::~AnimSub1() {
}

AnimSub1::AnimSub1(Resource *parent, byte subType, uint16 index, const Common::String &name) :
				Anim(parent, subType, index, name),
				_field_3C(0) {
}

void AnimSub1::readData(XRCReadStream *stream) {
	Anim::readData(stream);

	_field_3C = stream->readFloat();
}

void AnimSub1::printData() {
	Anim::printData();

	debug("field_3C: %f", _field_3C);
}

} // End of namespace Stark
