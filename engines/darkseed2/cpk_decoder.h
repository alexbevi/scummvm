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

#ifndef CPK_DECODER_H
#define CPK_DECODER_H

#include "common/system.h"
#include "common/events.h"
#include "common/file.h"
#include "common/endian.h"

#include "sound/audiostream.h"
#include "sound/mixer.h"

#include "video/video_decoder.h"
#include "video/codecs/cinepak.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

struct SampleTableEntry {
	uint32 offset;
	uint32 length;
	uint32 sampleInfo1;
	uint32 sampleInfo2;
};

class SegaFILMDecoder : public Video::VideoDecoder {
public:
	SegaFILMDecoder(Audio::Mixer *mixer,
			Audio::Mixer::SoundType soundType = Audio::Mixer::kPlainSoundType);
	~SegaFILMDecoder();

	bool load(Common::SeekableReadStream *stream);
	void close();

	uint16 getWidth() const { return _width; }
	uint16 getHeight() const { return _height; }
	uint32 getFrameCount() const { return _frameCount; }
	uint32 getElapsedTime() const;
	bool isVideoLoaded() const { return _stream != 0; }
	const ::Graphics::Surface *decodeNextFrame();
	::Graphics::PixelFormat getPixelFormat() const;
	uint32 getTimeToNextFrame() const;

protected:
	Audio::Mixer *_mixer;
	Audio::Mixer::SoundType _soundType;

	Common::SeekableReadStream *_stream;

	Audio::QueuingAudioStream *_audioStream;
	Audio::SoundHandle _audioStreamHandle;
	uint16 _audioFlags;

	uint32 _sampleCount;
	uint32 _sampleTablePosition;
	SampleTableEntry *_sampleTable;

	uint32 _baseFreq;
	uint32 _nextFrameStartTime;

	uint32 _frameCount;
	Video::Codec *_codec;
	uint16 _width, _height;
};

} // End of namespace DarkSeed2

#endif
