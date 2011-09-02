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

#include "ring/sound/soundloader.h"

#include "ring/helpers.h"

namespace Ring {

SoundLoader::SoundLoader(SoundFormat format) {
	_field_4 = 0;
	_field_8 = 0;
	_field_C = 0;
	_field_10 = 0;
	_field_14 = 0;
	_compressedStream = NULL;
	_field_1A = 0;
	_field_1E = 0;
	_format = format;
}

SoundLoader::~SoundLoader() {
	SAFE_DELETE(_compressedStream);
}

bool SoundLoader::load(const Common::String &path, SoundEntryStream *soundEntry) {
	error("[SoundLoader::load] Not implemented");
}

void SoundLoader::close() {
	error("[SoundLoader::close] Not implemented");
}

int SoundLoader::getChunk() {
	error("[SoundLoader::getChunk] Not implemented");
}

int SoundLoader::getDataSize() {
	error("[SoundLoader::getDataSize] Not implemented");
}

int SoundLoader::read() {
	error("[SoundLoader::read] Not implemented");
}

} // End of namespace Ring
