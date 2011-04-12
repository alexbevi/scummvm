/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "fvf_decoder.h"
#include "common/stream.h"

#include "audio/decoders/raw.h"

namespace Unity {

FVFDecoder::FVFDecoder(Audio::Mixer *mixer, Audio::Mixer::SoundType soundType) {
	_mixer = mixer;
	_soundType = soundType;

	_fileStream = NULL;
	_audioHandle = new Audio::SoundHandle();
}

FVFDecoder::~FVFDecoder() {
	close();
	delete _audioHandle;
}

bool FVFDecoder::loadStream(Common::SeekableReadStream *stream) {
	close();

	_fileStream = stream;

	// *** signature
	uint32 signature;
	signature = _fileStream->readUint32BE();
	if (signature != MKTAG('F','V','F',' '))
		error("missing FVF header");

	_fileStream->skip(12);

	// *** offsets to data/headers
	uint32 data_offset = _fileStream->readUint32LE();
	uint32 last_data_offset = _fileStream->readUint32LE();
	(void)last_data_offset; // offset of the last block; we don't care
	uint32 header_offset = _fileStream->readUint32LE();
	uint32 audio_header_offset = _fileStream->readUint32LE();
	// (64 obviously junk bytes are here)

	// *** main header
	_fileStream->seek(header_offset);

	uint16 unknown = _fileStream->readUint16LE();
	assert(unknown == 40); // likely some header length (but this header is 44 bytes?)
	uint16 unknown2 = _fileStream->readUint16LE();
	assert(unknown2 == 1);

	_bpp = _fileStream->readUint16LE();
	_width = _fileStream->readUint16LE();
	_height = _fileStream->readUint16LE();
	uint32 microsecondsPerFrame = _fileStream->readUint32LE();

	// only tested with these parameters
	assert(_bpp == 16);
	assert(_width == 320);
	assert(_height == 200);
	assert(microsecondsPerFrame == 66666); // 15fps

	_frameRate = Common::Rational(1000000, microsecondsPerFrame);

	uint32 padding = _fileStream->readUint32LE();
	assert(padding == 0);

	_frameCount = _fileStream->readUint32LE();

	padding = _fileStream->readUint32LE();
	assert(padding == 0);

	uint16 unknown3 = _fileStream->readUint16LE();
	assert(unknown3 == 0x18);

	palette_entry_count = _fileStream->readByte();
	assert(palette_entry_count == 112);
	header_pal_count = _fileStream->readByte();
	assert(header_pal_count == 15);
	uint32 header_pal_offset = _fileStream->readUint32LE();

	// *** audio header
	_fileStream->seek(audio_header_offset);
	uint16 unknown4 = _fileStream->readUint16LE();
	assert(unknown4 == 0);
	uint16 unknown5 = _fileStream->readUint16LE();
	assert(unknown5 == 1);
	uint16 unknown6 = _fileStream->readUint16LE();
	assert(unknown6 == 1);
	uint16 samplesize = _fileStream->readUint16LE();
	assert(samplesize == 8);
	uint16 samplerate = _fileStream->readUint16LE();
	assert(samplerate == 22050);
	uint16 unknown7 = _fileStream->readUint16LE();
	assert(unknown7 == 0);
	uint16 unknown8 = _fileStream->readUint16LE();
	assert(unknown8 == 0);
	uint16 unknown9 = _fileStream->readUint16LE();
	assert(unknown9 == 0);
	uint16 unknown10 = _fileStream->readUint16LE();
	assert(unknown10 == 2);

	// *** header palette entries
	_fileStream->seek(header_pal_offset);
	// we don't support rendering in paletted mode
	_fileStream->skip(3 * header_pal_count);

	debug(1, "read FVF video header: video %dx%d (%d bpp)", _width, _height, _bpp);

	setupTables();

	_fileStream->seek(data_offset);
	readNextBlock();

	_surface.create(_width, _height, getPixelFormat().bytesPerPixel);

	_audioStream = createAudioStream();
	if (_audioStream)
		_mixer->playStream(_soundType, _audioHandle, _audioStream);

	return true;
}

void FVFDecoder::close() {
	if (!_fileStream) return;

	delete _fileStream;

	_mixer->stopHandle(*_audioHandle);
	_audioStream = 0;
}

void FVFDecoder::readNextBlock() {
	uint16 curr_block_header_size = _fileStream->readUint16LE();
	assert(curr_block_header_size == 0x10);

	uint16 curr_block_frame_count = _fileStream->readUint16LE();
	uint32 prev_block_size = _fileStream->readUint32LE();
	uint32 curr_block_size = _fileStream->readUint32LE();
	uint32 next_block_size = _fileStream->readUint32LE();

	curr_block_remaining_frames = curr_block_frame_count;
	curr_block_remaining_size = curr_block_size - curr_block_header_size;

	// we don't care
	(void)prev_block_size;
	(void)next_block_size;
}

Graphics::Surface *FVFDecoder::decodeNextFrame() {
	assert(_fileStream);

	_curFrame++;
	assert((unsigned int)_curFrame < _frameCount);

	// this code assumes (as is the case for all FVFs encountered so far)
	// that everything is sequential, so doesn't bother seeking around..

	if (!curr_block_remaining_frames) {
		_fileStream->skip(curr_block_remaining_size);

		readNextBlock();
	}

	uint16 header_length = _fileStream->readUint16LE();
	assert(header_length == 0x18);

	uint16 frame_length = _fileStream->readUint16LE();
	assert(frame_length <= curr_block_remaining_size);

	uint16 unknown = _fileStream->readUint16LE();
	(void)unknown; // TODO

	uint32 video_offset = _fileStream->readUint32LE();
	uint32 palette_offset = _fileStream->readUint32LE();
	uint32 audio_offset = _fileStream->readUint32LE();

	uint32 unknown2 = _fileStream->readUint32LE();
	(void)unknown2; // TODO
	uint16 unknown3 = _fileStream->readUint16LE();
	(void)unknown3; // TODO

	uint32 read_so_far = header_length;

	::Graphics::Surface *surf = NULL;

	if (video_offset != 0) {
		video_offset -= read_so_far;
		assert(video_offset == 0);

		uint16 video_length = _fileStream->readUint16LE();
		read_so_far += 2;

		uint16 *video_frame = (uint16 *)malloc(video_length);
		_fileStream->read(video_frame, video_length);

		decodeVideoFrame(video_frame, video_length);
		surf = &_surface;

		read_so_far += video_length;
		free(video_frame);
	}

	if (palette_offset != 0) {
		palette_offset -= read_so_far;
		assert(palette_offset == 0);

		// we don't support rendering in paletted mode
		_fileStream->skip(palette_entry_count * 3);

		read_so_far += palette_entry_count * 3;
	}

	if (audio_offset != 0) {
		audio_offset -= read_so_far;
		assert(audio_offset == 0);

		uint32 audio_length = _fileStream->readUint32LE();
		read_so_far += 4;

		byte *audio_data = (byte *)malloc(audio_length);
		_fileStream->read(audio_data, audio_length);
		read_so_far += audio_length;

		_audioStream->queueBuffer(audio_data, audio_length, DisposeAfterUse::YES, ::Audio::FLAG_UNSIGNED);
	}

	if (_curFrame == 1)
		_startTime = g_system->getMillis();

	assert(read_so_far == frame_length);

	curr_block_remaining_frames--;
	curr_block_remaining_size -= frame_length;

	return surf;
}

Graphics::PixelFormat FVFDecoder::getPixelFormat() const {
	// TODO: de-hardcode this

	//return Graphics::PixelFormat(2, 5, 5, 5, 0, 0, 5, 10, 0);
	return Graphics::PixelFormat(3, 8, 8, 8, 0, 16, 8, 0, 0);
}

Audio::QueuingAudioStream *FVFDecoder::createAudioStream() {
	// TODO: de-hardcode this?

	return Audio::makeQueuingAudioStream(22050, false);
}

byte table1_x[16] = { 0, 4, 0, 4, 8, 0xc, 8, 0xc, 0, 4, 0, 4, 8, 0xc, 8, 0xc };
byte table1_y[16] = { 0, 0, 4, 4, 0, 0, 4, 4, 8, 8, 0xc, 0xc, 8, 8, 0xc, 0xc };

void SetupSubsetBlockIdToOffset(uint32 *ptr, unsigned int &ptroffset, unsigned int counter,
	unsigned int width, unsigned int size, unsigned int count, unsigned int base, bool halved) {
	unsigned int ourcount = width / size;
	if (halved) { ourcount /= 2; }

	assert(count <= 16);

	for (unsigned int i = 0; i < ourcount; i++) {
		for (unsigned int j = 0; j < count; j++) {
			ptr[ptroffset++] = base + (i * size) +
				table1_x[j] + ((counter + table1_y[j]) * width);
			assert(ptr[ptroffset - 1] < 64000 - 960); // these are lookups into back_buffer
		}
	}
}

byte table2_x[16] = { 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3 };
byte table2_y[16] = { 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3 };

void SetupSubsetBlockStatusLookup(uint32 *ptr, unsigned int &ptroffset, unsigned int counter,
	unsigned int width, unsigned int size, unsigned int count, unsigned int base, bool halved) {
	unsigned int ourcount = width / (size * 4);
	if (halved) ourcount /= 2;

	assert(count <= 16);

	for (unsigned int i = 0; i < ourcount; i++) {
		for (unsigned int j = 0; j < count; j++) {
			ptr[ptroffset++] = base + (i * size) +
				table2_x[j] + ((table2_y[j] + counter/4) * width)/4;
			assert(ptr[ptroffset - 1] < 6000); // these are lookups into block_status
		}
	}
}

void SetupBlockIdToOffset(uint32 *ptr) {
	unsigned int ptroffset = 0;
	unsigned int counter;

	for (counter = 0; counter < 192; counter += 16) {
		SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 16, 16, 0, false);
	}
	SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 16, 8, 0, false);

	for (counter = 0; counter < 96; counter += 8) {
		SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 16, 8, 0, true);
	}
	SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 8, 2, 0, true);

	for (counter = 0; counter < 96; counter += 8) {
		SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 16, 8, 160, true);
	}
	SetupSubsetBlockIdToOffset(ptr, ptroffset, counter, 320, 8, 2, 160, true);
}

void SetupBlockStatusLookup(uint32 *ptr) {
	unsigned int ptroffset = 0;
	unsigned int counter;

	for (counter = 0; counter < 192; counter += 16) {
		SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 4, 16, 0, false);
	}
	SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 4, 8, 0, false);

	for (counter = 200; counter < 296; counter += 8) {
		SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 4, 8, 0, true);
	}
	SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 2, 2, 0, true);

	for (counter = 200; counter < 296; counter += 8) {
		SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 4, 8, 40, true);
	}
	SetupSubsetBlockStatusLookup(ptr, ptroffset, counter, 320, 2, 2, 40, true);
}

void FVFDecoder::setupTables() {
	memset(storage, 0, 256*2); // most likely unnecessary

	curr_status = 2;
	memset(block_status, 0, 6000);

	memset(real_front_buffer, 0xAB, 64000);
	memset(real_back_buffer, 0xAB, 64000);
	memset(real_colour_front_buffer, 0xAB, 32000);
	memset(real_colour_back_buffer, 0xAB, 32000);

	front_buffer = real_front_buffer;
	back_buffer = real_back_buffer;
	colour_front_buffer = real_colour_front_buffer;
	colour_back_buffer = real_colour_back_buffer;

	unsigned int a = 0;
	for (int i = -64; i < 64; i++) {
		unsigned int b = 0;
		for (unsigned int j = 0; j < 256; j++) {
			int x = (4 * i) + (int)(((3 * b) + 2) >> 2);
			b++;
			if (x < 0)
				x = 0;
			else if (x > 0xff)
				x = 0xff;
			modify_lookup[a++] = x;
		}
	}
	assert(a == 32768);

	SetupBlockIdToOffset(block_id_to_offset);
	SetupBlockStatusLookup(block_status_lookup);

	for (unsigned int x = 0; x < 320; x += 2) {
		for (unsigned int y = 0; y < 200; y += 2) {
			block_lookup[(x + 160*y)/2] = x + 320*y;
		}
	}
}

inline unsigned int FVFDecoder::GetOddPixelOffset(unsigned int in) {
	//assert(in % 2 == 0);
	// TODO: necessary?
	//return *(uint16*)(((byte *)block_lookup) + in);
	return block_lookup[in/2];
}

void FVFDecoder::decodeVideoFrame(uint16 *frame, unsigned int len) {
	debug(1, "FVF: decoding video frame");

	uint16 flags = READ_LE_UINT16(frame + 1); // +4

	if (flags & 0x8) {
		// unused in the game?
		error("FVF: flag 8 set");
		// curr_status = 2;
		return;
	}

	// preserve the previous data
	byte *was_front = front_buffer, *was_colour_front = colour_front_buffer;

	decodeVideoFrameData(frame, len);

	if (flags & 0x1) {
		was_front = front_buffer;
		was_colour_front = colour_front_buffer;
		// allow the first frame to iteratively render
		for (unsigned int i = 0; i < 15; i++) {
			decodeVideoFrameData(frame, len);
		}
	}

	// 24bpp only for now, because i am lazy
	byte *source = was_front;
	byte *source_colour = was_colour_front;

	byte *target = (byte *)_surface.pixels;
	unsigned int i = 0;
	// colour is processed in 4x4 blocks
	for (unsigned int y = 0; y < _height/4; y++) {
		for (unsigned int x = 0; x < _width/4; x++) {
			// 0xf = both video/colour status is 3: already rendered
			if (block_status[i] != 0xf) {
				byte *targ = target;

				// 4 lines
				//uint16 colour1, colour2;
				for (unsigned int j = 0; j < 4; j++) {
					// 4 input pixels = 12 output bytes
					byte *srcdata = source + (j * 320);

					byte *src_col_ptr = source_colour;
					if (j > 1) src_col_ptr += 320;

					byte colour1, colour2;
					colour1 = *(src_col_ptr + 0);
					colour2 = *(src_col_ptr + 160);
#if defined(SCUMM_BIG_ENDIAN)
					SWAP(colour1, colour2);
#endif
					*(targ + 0) = colour2;
					*(targ + 1) = *srcdata++;
					*(targ + 2) = colour1;
					*(targ + 3) = colour2;
					*(targ + 4) = *srcdata++;
					*(targ + 5) = colour1;

					src_col_ptr++;
					colour1 = *(src_col_ptr + 0);
					colour2 = *(src_col_ptr + 160);
#if defined(SCUMM_BIG_ENDIAN)
					SWAP(colour1, colour2);
#endif
					*(targ + 6) = colour2;
					*(targ + 7) = *srcdata++;
					*(targ + 8) = colour1;
					*(targ + 9) = colour2;
					*(targ + 10) = *srcdata++;
					*(targ + 11) = colour1;

					targ += 320 * 3; // 24bpp
				}
			}

			i++;
			source += 4;
			//target += 4 * 2; // 16bpp
			target += 4 * 3; // 24bpp
			source_colour += 2;
		}
		source += 320 * 3;
		//target += 320 * 3 * 2; // 16bpp
		target += 320 * 3 * 3; // 24bpp
		source_colour += 160 * 3;
	}

	curr_status = 2;
	if (flags & 0x4) {
		curr_status = 3;
	}
}

void FVFDecoder::decodeVideoFrameData(uint16 *frame, unsigned int len) {
	// TODO: alignment fail everywhere, as far as the eye can see

	byte *our_front_buffer = front_buffer;
	byte *our_back_buffer = back_buffer;
	byte *our_colour_front_buffer = colour_front_buffer;
	byte *our_colour_back_buffer = colour_back_buffer;

	uint16 flags = READ_LE_UINT16(frame + 1);

	if ((flags & 0x1) || (flags & 0x4)) {
		// swap buffers
		debug(1, "FVF: swapping buffers");

		front_buffer = our_back_buffer;
		back_buffer = our_front_buffer;
		colour_front_buffer = our_colour_back_buffer;
		colour_back_buffer = our_colour_front_buffer;
	}

	byte *data = (byte *)(frame + 3);
	byte *end_of_data = (byte *)frame + len;

	// move some data (always 0 or 16 words?) into the storage
	uint16 storage_count = READ_LE_UINT16(frame + 2);
	if (storage_count) debug(2, "FVF: placing %d words into storage", storage_count);
	uint16 *storage_ptr = (uint16 *)storage;
	while (storage_count--) {
		*storage_ptr = (int16)READ_LE_UINT16(data);

		storage_ptr++;
		data += 2;
	}

	uint32 curr_block_id = 0;
	uint32 block_limit = 4000;

	bool pass1 = true;
	while (pass1 || curr_block_id < block_limit) {
		assert(data < end_of_data);

		if (curr_block_id >= block_limit) {
			assert(pass1);
			assert(curr_block_id == block_limit);

			// switch to rendering into the colour buffer
			// (this isn't done in paletted mode, but we don't support that)
			our_front_buffer = our_colour_front_buffer;
			our_back_buffer = our_colour_back_buffer;

			block_limit = 6000;
			pass1 = false;
		}

		// the first three bits (0, 1 and 2) hold the operation type
		unsigned int case_type = (*data) & 0x7;
		switch(case_type) {
		case 0:
		case 1:
		case 2:
		case 3:
			{
			uint32 info = READ_LE_UINT32(data);
			uint32 modifier = (info << 5) & 0x7F00;
			uint32 src_block = (info >> 9) & 0x7FFE;
			data += 3;

			unsigned int offset_src = GetOddPixelOffset(src_block);
			unsigned int offset_dest = block_id_to_offset[curr_block_id];

			// four lines
			for (unsigned int i = 0; i < 4; i++) {
				byte *dest = our_front_buffer + offset_dest + (320 * i);

				unsigned int src = offset_src;
				switch (case_type) {
				case 0: src += (640 * i); break; // normal copy
				case 1: src += (640 * i) + 6; break; // horz mirrored
				case 2: src += (640 * (3 - i)); break; // vert mirrored
				case 3: src += (640 * (3 - i)) + 6; break; // horz+vert mirrored
				}

				for (unsigned int j = 0; j < 4; j++) {
					*dest++ = modify_lookup[modifier + our_back_buffer[src]];
					switch (case_type) {
					case 0: case 2: src += 2; break;
					case 1: case 3: src -= 2; break;
					}
				}
			}

			block_status[block_status_lookup[curr_block_id++]] = 0;

			} break;

		case 4: {
			unsigned int count = (*data >> 3) & 0x1F;

			data++;

			for (unsigned int j = 0; j < count + 1; j++) {
				unsigned int offset = block_status_lookup[curr_block_id];

				if (block_status[offset] & 2) {
					// already been rendered
					block_status[offset] = 3;
				} else {
					block_status[offset] = curr_status;

					unsigned int offset_both = block_id_to_offset[curr_block_id];

					for (unsigned int i = 0; i < 320*4; i += 320) {
						*(uint32 *)(our_front_buffer + offset_both + i) = *(uint32 *)(our_back_buffer + offset_both + i);
					}
				}
				curr_block_id++;
			}

			} break;

		case 5: {
			// bits 4, 5, 6 and 7
			byte info = *data >> 4;

			int16 input = storage[info];

			unsigned int count = 0;

			if ((*data >> 3) & 1) {
				data++;
				count = (*data) + 1;
				data++;
			} else {
				data++;
			}

			for (unsigned int j = 0; j < count + 1; j++) {
				unsigned int offset_dest = block_id_to_offset[curr_block_id];
				unsigned int offset_src = (int)offset_dest + (int)input;

				for (unsigned int i = 0; i < 320*4; i += 320) {
					*(uint32 *)(our_front_buffer + offset_dest + i) = *(uint32 *)(our_back_buffer + offset_src + i);
				}

				block_status[block_status_lookup[curr_block_id++]] = 0;
			}

			} break;

		case 6: {
			data++;

			unsigned int offset_dest = block_id_to_offset[curr_block_id];
			for (unsigned int i = 0; i < 320*4; i += 320) {
				*(uint32*)(our_front_buffer + offset_dest + i) = *((uint32 *)data);
				data += 4;
			}

			block_status[block_status_lookup[curr_block_id++]] = 0;

			} break;

		case 7: {
			// 0x78 = more bits: 3, 4, 5 and 6
			byte subtype = (*data & 0x78) >> 3;

			switch (subtype) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7: {
				data++;
				uint32 modifier = *data; // remember, this is a byte
				data++;
				uint16 src_block = READ_LE_UINT16(data);
				data += 2;

				bool recursive = (modifier & 0x80) == 0x80;
				modifier = (modifier << 8) & 0x7F00;

				byte recursive_offset;
				if (recursive) {
					// the last two bits
					recursive_offset = (src_block >> 8) >> 6;
				}

				src_block &= 0x3FFF; // discard any bits used by 'recursive' flag above
				unsigned int offset_src = GetOddPixelOffset(src_block*2);
				unsigned int offset_dest = block_id_to_offset[curr_block_id];
				for (unsigned int i = 0; i < 8; i++) {
					unsigned int src = offset_src;
					switch (subtype) {
						case 0: src += (640 * i); break; // normal copy
						case 1: src += (640 * i) + 2*7; break; // horz mirror
						case 2: src += (640 * (7 - i)); break; // vert mirror
						case 3: src += (640 * (7 - i)) + 2*7; break; // horz+vert mirror
						case 4: src += (640 * 0) + 2*i; break;
						case 5: src += (640 * 0) + 2*(7 - i); break;
						case 6: src += (640 * 7) + 2*i; break;
						case 7: src += (640 * 7) + 2*(7 - i); break;
					}

					byte *dest = our_front_buffer + offset_dest + (320 * i);

					for (unsigned int j = 0; j < 8; j++) {
						*dest++ = modify_lookup[modifier + our_back_buffer[src]];
						switch (subtype) {
							case 0: src += 2; break;
							case 1: src -= 2; break;
							case 2: src += 2; break;
							case 3: src -= 2; break;
							case 4: src += 640; break;
							case 5: src += 640; break;
							case 6: src -= 640; break;
							case 7: src -= 640; break;
						}
					}
				}

				// mark a 2x2 block as modified
				unsigned int status_offset = block_status_lookup[curr_block_id];
				block_status[status_offset] = 0;
				block_status[status_offset + 1] = 0;
				block_status[status_offset + 80] = 0;
				block_status[status_offset + 81] = 0;

				if (!recursive) {
					curr_block_id += 4;
					break;
				}

				// recursive call: block id offset by some value
				unsigned int temp_block_id = curr_block_id + recursive_offset;

				// TODO: de-duplicate
				byte subsubtype = (*data & 0x7);
				switch (subsubtype) {
				case 0:
				case 1:
				case 2:
				case 3: {
					uint32 info = READ_LE_UINT32(data);
					data += 3;

					uint32 recursive_modifier = (info << 5) & 0x7F00; // 32767 - 255
					uint32 recursive_src_block = (info >> 9) & 0x7FFE; // 32767 - 1

					unsigned int recursive_offset_src = GetOddPixelOffset(recursive_src_block);
					unsigned int recursive_offset_dest = block_id_to_offset[temp_block_id];

					for (unsigned int i = 0; i < 4; i++) {
						byte *dest = our_front_buffer + recursive_offset_dest + (320 * i);

						unsigned int src = recursive_offset_src;
						switch (subsubtype) {
						case 0: src += (640 * i); break; // normal copy
						case 1: src += (640 * i) + 6; break; // horz mirrored
						case 2: src += (640 * (3 - i)); break; // vert mirrored
						case 3: src += (640 * (3 - i)) + 6; break; // horz+vert mirrored
						}

						for (unsigned int j = 0; j < 4; j++) {
							*dest++ = modify_lookup[recursive_modifier + our_back_buffer[src]];
							switch (subsubtype) {
							case 0: case 2: src += 2; break;
							case 1: case 3: src -= 2; break;
							}
						}
					}
					} break;

				case 4: {
					data++;
					unsigned int recursive_offset2 = block_status_lookup[temp_block_id];
					block_status[recursive_offset2] |= curr_status;

					unsigned int recursive_offset_both = block_id_to_offset[temp_block_id];
					for (unsigned int i = 0; i < 320*4; i += 320) {
						*(uint32 *)(our_front_buffer + recursive_offset_both + i) = *(uint32 *)(our_back_buffer + recursive_offset_both + i);
					}
					} break;

				case 5: {
					byte info = (*data >> 3) & 0x1f;
					data++;
					// TODO: is this hackery strictly necessary? fix endianism/alignment
					int16 recursive_offset2 = *(int16 *)((byte *)storage + info);

					unsigned int recursive_offset_dest = block_id_to_offset[temp_block_id];
					unsigned int recursive_offset_src = (int)recursive_offset_dest + (int)recursive_offset2;
					for (unsigned int i = 0; i < 320*4; i += 320) {
						*(uint32 *)(our_front_buffer + recursive_offset_dest + i) = *(uint32 *)(our_back_buffer + recursive_offset_src + i);
					}
					} break;

				case 6: {
					data++;
					unsigned int recursive_offset_dest = block_id_to_offset[temp_block_id];
					for (unsigned int i = 0; i < 320*4; i += 320) {
						*(uint32 *)(our_front_buffer + recursive_offset_dest + i) = *((uint32*)data);
						data += 4;
					}
					} break;

				case 7: error("unhandled case");
				default: error("internal error");
				}

				if (subsubtype != 4 && subsubtype != 5) {
					block_status[block_status_lookup[temp_block_id++]] = 0;
				}

				curr_block_id += 4;

				} break;

			case 12: {
				uint32 info = READ_LE_UINT32(data);
				info &= 0xFFFFFF;
				info >>= 8;
				if (*data & 0x80) { // bit 7
					data += 3;
					info += 0x120;
				} else {
					info &= 0xff;
					data += 2;
					info += 0x20;
				}
				for (unsigned int j = 0; j < info + 1; j++) {
					unsigned int offset = block_status_lookup[curr_block_id];
					if (block_status[offset] & 2) {
						// already been rendered
						block_status[offset] = 3;
					} else {
						block_status[offset] = curr_status;

						unsigned int offset_both = block_id_to_offset[curr_block_id];

						for (unsigned int i = 0; i < 320*4; i += 320) {
							*(uint32 *)(our_front_buffer + offset_both + i) = *(uint32 *)(our_back_buffer + offset_both + i);
						}
					}
					curr_block_id++;
				}
				} break;

			case 15: {
				// haven't encountered this in any videos, so untested
				error("FVF decoder hit untested case 7/15");

				data++;

				uint16 offset = READ_LE_UINT16(data);
				data += 2;

				uint16 info = READ_LE_UINT16(data);
				data += 2;
				*(uint16*)(our_front_buffer + offset) = info;

				info = READ_LE_UINT16(data);
				*(uint16*)(our_front_buffer + offset + 320) = info;
				data += 2;

				} break;

			case 8:
			case 9:
			case 10:
			case 11:
			case 13:
			case 14:
				error("FVF: unknown block type %d in video frame", subtype);

			default: error("internal error");
			}
			} break;

		default: error("internal error");
		}
	}

	if (!pass1) {
		// propogate the status of the colour blocks to the video blocks
		unsigned int in_offset = 4000;
		unsigned int out_offset = 0;
		// colour is processed in blocks of 4x4 with 2x2 colour blocks, so this makes 320x200 into 80x50
		for (unsigned int i = 0; i < 25; i++) {
			for (unsigned int j = 0; j < 40; j++) {
				// copy status from bits 0/1 to bits 2/3
				byte x = (block_status[in_offset] & block_status[in_offset + 40]) << 2;
				block_status[out_offset] |= x;
				block_status[out_offset + 1] |= x;
				block_status[out_offset + 80] |= x;
				block_status[out_offset + 81] |= x;
				out_offset += 2;
				in_offset++;
			}
			out_offset += 80;
			in_offset += 40;
		}
	}
}

} // Unity

