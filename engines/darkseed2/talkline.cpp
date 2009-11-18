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

#include "common/stream.h"

#include "engines/darkseed2/talkline.h"
#include "engines/darkseed2/resources.h"

namespace DarkSeed2 {

TalkLine::TalkLine(const Resources &resources, const Common::String &talkName) {
	_resources = &resources;

	_talkName = talkName;

	_wav = 0;

	// Reading the sound
	if (_resources->hasResource(talkName + ".WAV"))
		_wav = _resources->getResource(talkName + ".WAV");

	// Reading the text
	Resource *txt;
	if (_resources->hasResource(talkName + ".TXT")) {
		txt = _resources->getResource(talkName + ".TXT");

		Common::SeekableReadStream &txtStream = txt->getStream();

		while (!txtStream.err() && !txtStream.eos()) {
			Common::String line = txtStream.readLine();
			if (line.empty() && txtStream.eos())
				continue;

			if (!_txt.empty())
				_txt += '\n';
			_txt += line;
		}

	}
}

TalkLine::~TalkLine() {
	delete _wav;
}

bool TalkLine::hasWAV() const {
	return _wav != 0;
}

bool TalkLine::hasTXT() const {
	return !_txt.empty();
}

const Resource &TalkLine::getWAV() const {
	if (!_wav)
		error("Resource %s.WAV does not exist", _talkName.c_str());

	return *_wav;
}

const Common::String &TalkLine::getTXT() const {
	return _txt;
}

} // End of namespace DarkSeed2
