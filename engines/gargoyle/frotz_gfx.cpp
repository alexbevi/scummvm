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

#include "gargoyle/frotz_gfx.h"
#include "common/file.h"
#include "gargoyle/frotz.h"

// Constants for header fields at the start of a picture entry
#define PIC_NUMBER 0
#define PIC_WIDTH 2
#define PIC_HEIGHT 4
#define PIC_FLAGS 6
#define PIC_DATA 8
#define PIC_COLOUR 11

#define ALL_WHITE	224
#define ALL_BLACK	225

namespace Frotz {

uint32 DEFAULT_COLOUR_LIST[16] = {
	0x000000,		// current setting
	0xffffff,		// default setting
	0x000000,		// black
	0x0000A0,		// red
	0x00A700,		// green
	0x50F8FF,		// yellow
	0xA00000,		// blue
	0xA000A0,		// magenta
	0x00A700,		// cyan
	0xFFFFFF,		// white
	0x404040,		// dark grey
	0x800000,		// dark blue
	0xFFFFFF,		// bright white
	0x000000,		// black
	0xc0c0c0,		// user foreground (white)
	0x800000		// user background (blue)
};

FrotzPictures::FrotzPictures(Gargoyle::GargoyleEngine *engine, const Common::Point &screenSize,
							 zbyte gameVersion) {
	_engine = engine;
	_screenSize = screenSize;
	_gameVersion = gameVersion;
	_fileNumber = 1;
	_gfxMode = MODE_NONE;
	_gfxInfo = NULL;

	openPictureFile();
}

FrotzPictures::~FrotzPictures() {
	if (_gfxFp.isOpen()) _gfxFp.close();
	if (_gfxInfo) free(_gfxInfo);
}

bool FrotzPictures::picturesExists(const Common::String &target) {
	Common::File gfxFp;
	Common::String filename;
	char extension[5];

	// Attempt to open a .mg* filename, with a fallback for .eg* and .cg*
	for (int gfxType = MODE_MCGA; gfxType != MODE_NONE; --gfxType) {
		// Setup the extension
		strcpy(extension, "mg1");
		extension[0] = "mec"[3 - gfxType];

		// Form a filename from the game filename with the given extension
		filename = target;
		while ((filename.size() > 0) && (filename[filename.size() - 1] != '.'))
			filename.deleteLastChar();
		filename += extension;

		// Check if the file exists
		if (gfxFp.exists(filename))
			return true;
	}

	return false;
}

bool FrotzPictures::openPictureFile() {
	Common::String filename;
	char extension[5];

	if (_gfxInfo) free(_gfxInfo);
	_gfxInfo = NULL;
	
	// Attempt to open a .mg* filename, with a fallback for .eg* and .cg*
	int gfxType = (_gfxMode == MODE_NONE) ? MODE_MCGA : _gfxMode;
	do {
		// Setup the extension
		strcpy(extension, "mg1");
		extension[0] = "mec"[3 - gfxType];
		extension[2] = '0' + _fileNumber;

		// Form a filename from the game filename with the given extension
		filename = _engine->getFilename();
		while ((filename.size() > 0) && (filename[filename.size() - 1] != '.'))
			filename.deleteLastChar();
		filename += extension;

		// Try to open file
		if (_gfxFp.open(filename))
			break;

	} while ((_gfxMode == MODE_NONE) && (--gfxType != MODE_NONE));

    // If no file found, return warning
	if (!_gfxFp.isOpen()) {
		if (_engine->getGameID() != Gargoyle::GID_ZCODE_BEYOND_ZORK)
			warning("Cannot find graphics file for this game");

		return false;
	}

	if (_gfxMode == MODE_NONE)
		_gfxMode = gfxType;

    // Load header
	_header.fileno = _gfxFp.readByte();
    _header.flags = _gfxFp.readByte();
    _header.unused1 = _gfxFp.readUint16LE();
    _header.images = _gfxFp.readUint16LE(); 
    _header.link = _gfxFp.readUint16LE();
    _header.entry_size = _gfxFp.readByte();
    _header.padding = _gfxFp.readByte();
    _header.checksum = _gfxFp.readUint16LE();
    _header.unused2 = _gfxFp.readUint16LE();
    _header.version = _gfxFp.readUint16LE();

    // Load picture directory
	_gfxInfo = (byte *)malloc(_header.images * _header.entry_size);
	_gfxFp.read(_gfxInfo, _header.images * _header.entry_size);

	return true;
}

void FrotzPictures::copyRect(Graphics::Surface &s, int16 x, int16 y, int16 width, int16 height, int16 pitch,
							 double scaleX, double scaleY, const byte *bmmask, const byte *bmcontents) {
	int iScaleX = (int)scaleX, iScaleY = (int)scaleY;
	assert(((double)iScaleX == scaleX) && ((double)iScaleY == scaleY));

	byte *destP = (byte *)s.getBasePtr(x, y);
	const byte *maskP = NULL, *contentsP = NULL;

	for (int yc = 0; yc < height; ++yc, destP += s.pitch * iScaleY) {
		if (bmmask) maskP = bmmask + (yc * pitch);
		if (bmcontents) contentsP = bmcontents + (yc * pitch);

		byte *destPtr = destP;
		for (int xc = 0; xc < width; ++xc) {
			// Handle possible scaling for each pixel
			for (int xs = 0; xs < iScaleX; ++xs, ++destPtr) {
				for (int ys = 0; ys < iScaleY; ++ys) {
					byte *pTemp = destPtr + ys * s.pitch;
					byte pixel = *contentsP;

					uint8 destPixel = !maskP ? 0 : *pTemp & *maskP;
					if (pixel != 0) {
						destPixel |= (pixel + 17);
						*pTemp = destPixel;
					}
				}
			}

			if (maskP) ++maskP;
			if (contentsP) ++contentsP;
		}
	}
}

/**
 * The extra boolean on the end here shows whether we want to lie about the picture geometry.
 * If it is TRUE, the method will tell the dimensions that the picture WOULD be given the current
 * display size (the scaled size, in other words). If FALSE WinFrotz will return the actual,
 *  original dimensions of the data
 */
bool FrotzPictures::getPictureData(int picNumber, int16 *width, int16 *height, int32 *data, uint32 *colour, uint32 *flags, bool retScale) {
	double scaleX, scaleY;
    byte *info;
    int i;

	assert(_gfxMode != MODE_NONE);
	if (_gfxMode == MODE_MCGA) {
		scaleX = MIN((double)_screenSize.x / 320, (double)_screenSize.y / 200);
		scaleY = (double)scaleX;
	} else if (_gfxMode == MODE_CGA) {
		scaleX = (double)_screenSize.x / 640;
		scaleY = (double)_screenSize.y / 200;
	} else {
		scaleX = (double)_screenSize.x / 640;
		scaleY = (double)_screenSize.y / 200;
	}
	
	// For this ScummVM version, ensure that the scale values are whole numbers, and non-zero
	assert((scaleX != 0) && ((double)((int)scaleX) == scaleX) &&
		   (scaleY != 0) && ((double)((int)scaleY) == scaleY));

   /* Scan the picture directory. The pictures may be scattered among several files (in EGA mode only),
    * so try all the graphics files in turn. At the moment, Frotz assumes that the graphics header
    * entry "link" is set to a non-zero value when there is another graphics file. This is actually 
	* just a guess that works for the existing graphics files...
	*/
    int old = _fileNumber;
	bool foundPicture = false;
	info = NULL;

    do {
		if (_fileNumber == 0)
			break;

		info = _gfxInfo;

		for (i = 0; i < _header.images; i++) {
			if (picNumber == READ_LE_UINT16(info + PIC_NUMBER)) {
				foundPicture = true;
				break;
			}

			info += _header.entry_size;
		}
		if (foundPicture)
			break;

		++_fileNumber;

		if (_header.link == 0)
			_fileNumber = 1;

		if (!openPictureFile())
			return false;

	} while (_fileNumber != old);

	if (!foundPicture)
		return 0;

    /* Read all information from directory entry */

	assert(info);
    *height = READ_LE_UINT16(info + PIC_HEIGHT);
    *width = READ_LE_UINT16(info + PIC_WIDTH);
	if (retScale) {
		*height = (int)((double)*height * scaleY);
		i = (int)((double)*width * scaleX);
		*width = i;
	}
    *flags = READ_LE_UINT16(info + PIC_FLAGS);

	*data = info[PIC_DATA + 2] | (info[PIC_DATA + 1] << 8) | (info[PIC_DATA] << 16);

    if (_header.entry_size >= 14)
		*colour = info[PIC_COLOUR + 2] | (info[PIC_COLOUR + 1] << 8) | (info[PIC_COLOUR] << 16);
    else 
		*colour = 0;

    return true;
}

/**
 * Return true if the given picture is available. If so, store the picture width and height in 
 * the appropriate variables. Picture number 0 is a special case: Write the number of pictures 
 * available and the picture file release number into the height and width variables respectively 
 * when this picture number is asked for.
 */
bool FrotzPictures::getPictureInfo(int picNumber, int16 *width, int16 *height) {
    int32 data;
    uint32 colour;
    uint32 flags;

    if (picNumber == 0) {
		*height = _header.images;
		*width = _header.version;

		return true;
    }

    return getPictureData(picNumber,  width, height, &data, &colour, &flags, true);
}

/**
 * Display a picture at the given coordinates. Top left is (1,1).
 */
void FrotzPictures::drawPicture(int picNumber, Graphics::Surface &s, int x, int y) {
    unsigned char buf[512];
	uint32 palette[256];
    byte *table_val;
    uint16 *table_seq;
    int32 data;
    uint32 colour;
    unsigned int flags;
    unsigned int code;
    unsigned int prev_code = 0;
    unsigned int val;
    unsigned int transparent;
    int16 height, width;
    int count;
    int bits;
    int bits_per_code;
    int bits_left;
    int pos, scanwidth, pixy, cmp;
    int i;
	int	scaleY;
	double scaleX;
	byte *bmcontents = NULL, *bmmask = NULL;
	byte *bmoffset = NULL;
	bool has_transparency = false;

    --x;
    --y;

	pixy = y;
    /* Get all information on the current picture. */

    if (!getPictureData(picNumber, &width, &height, &data, &colour, &flags, false))
		return;

	if (_gfxMode == MODE_CGA) {
		scaleX = (double)_screenSize.x / 640;
		scaleY = _screenSize.y / 200;
	} else if (_gfxMode == MODE_MCGA) {
		scaleX = MIN((double)_screenSize.x / 320, (double)_screenSize.y / 200);
		scaleY = (int)scaleX;
	} else {
		scaleX = (double)_screenSize.x / 640;
		scaleY = _screenSize.y / 200;
	}

	/* DWORD align DIB scan lines */
	if (width % sizeof(uint32))
		scanwidth = (width / sizeof(uint32) + 1) * sizeof(uint32);
	else
		scanwidth = width;

    /* Allocate memory for decompression. */

	table_seq = (uint16 *)malloc(3840 * sizeof(uint16));
	table_val = (byte *)malloc(3840 * sizeof(uint8));

	if (table_val == NULL && table_seq != NULL) {
		free(table_seq);
		table_seq = NULL;
	}
	if (table_seq == NULL && table_val != NULL) {
		free(table_val);
		table_val = NULL;
	}

    if (table_val == NULL || table_seq == NULL)
		return;

    /* When the given picture provides a colour map then activate it. This is used only in MCGA mode; 
	 * note that the same colour map is used for all pictures on the screen. The first colour to be
     * defined is colour 2; colours 0 and 1 are not used. Each colour map may define up to 14 colours
	 * (i.e. colour 2 to 15). The colour map that is used for pictures in MCGA mode does not affect
	 * the standard Z-machine colours which are used for text. 
	 * (This is based on Amiga interpreters which had to work with 16 colours. Colour 0 and 1 were 
	 * used for text, and changing the text colours actually changed the palette entries 0 and 1.)
	 */

	// CGA pics tend to run over the end of the array, so pad by scanline
	bmcontents = (byte *)malloc(scanwidth * height + scanwidth);
	bmmask = (byte *)malloc(scanwidth * height + scanwidth);
	
	if (!bmcontents || !bmmask)
		error("Could not allocate bitmap memory");

	Common::set_to(bmcontents, bmcontents + scanwidth * height + scanwidth, 0);
	Common::set_to(bmmask, bmmask + scanwidth * height + scanwidth, 0);
	Common::set_to(&palette[0], &palette[256], 0);

	bmoffset = bmcontents;

    if (colour != 0) {
		_gfxFp.seek(colour);

		count = _gfxFp.readByte();
		assert(count <= 239);

		for (i = 2; i <= count + 2; ++i) {
			uint8 r = _gfxFp.readByte();
			uint8 g = _gfxFp.readByte();
			uint8 b = _gfxFp.readByte();
			palette[i] = r | (g << 8) | (b << 16);
		}

		palette[ALL_WHITE] = 0xffffff;
		palette[ALL_BLACK] = 0;

		// Set the palette entries
		g_system->setPalette((const byte *)&palette[0], 17, count + 2);
    }

	// Hardcode the two palette entries at 224 and 225 for black and white respectively
	palette[ALL_WHITE] = 0xffffff;
	palette[ALL_BLACK] = 0;
	g_system->setPalette((const byte *)&palette[ALL_WHITE], ALL_WHITE, 2);

	// Place the file pointer at the start of the picture data.
	_gfxFp.seek(data);

    /* Bit 0 of the flags variable indicates that the picture wants to use a transparent colour; 
	 * the top four bits hold the index of the colour which is supposed to be transparent. In CGA
	 * and MCGA modes this will always be 0; in EGA mode this may be any colour between 0 and 15.
	 */
    transparent = 0xffff;

    if (flags & 1)
		transparent = flags >> 12;

    /* The compressed picture is a stream of bits. We read the file byte-wise, storing the current 
	 * byte in a variable called "bits". The number of bits which have not already been used is 
	 * stored in "bits_left". At the start of decompression, 9 bits make one code.
     * During the process this can rise up to 12 bits per code.
	 */
    bits_per_code = 9;
    bits_left = 0;

    /* The uncompressed picture is just a long sequence of bytes. Each byte holds the colour 
	 * of a pixel; therefore all resulting bytes will be in the range from 0 to 15. One line 
	 * of pixels follows another, starting at the top left, ending at the bottom right.
     * There is one exception to this: In CGA mode only, when bit 0 in the flags variable is clear,
	 * each byte in the uncompressed data holds the pattern for the next 8 pixels (high bits first).
	 * Our position in the current line is stored in "pos". It runs from 0 to "width".
	 */
	pos = 0;

    /* Prepare EGA board. */

    // Loop until a special code signals the end of the picture.
    for (;;) {
		code = 0;

		// Read the next code from the graphics file.
		i = bits_per_code;

		do {
			if (bits_left <= 0) {
				bits = _gfxFp.readByte();
				bits_left = 8;
			}

			code |= (bits >> (8 - bits_left)) << (bits_per_code - i);

			i -= bits_left;
			bits_left = -i;

		} while (i > 0);

		code &= 0xfff >> (12 - bits_per_code);

		/* Code 256 resets the number of bits per code to 9 and clears the table (see below).
		 * The first code will always be 256, allowing us to lazily omit the initialisation 
		 * of "count" at the start of decompression.
		 */
		if (code == 256) {
			bits_per_code = 9;
			count = 257;

			continue;
		}

		/* The second special code, 257, marks the end of the picture. */
		if (code == 257)
			break;

		/* Codes from 0 to 255 are literals, i.e. they represent the byte of the same value. 
		 * All other codes refer to sequences of bytes that are stored in a table. A special 
		 * case is the sequence that is next to be defined. When this sequence is chosen, 
		 * we must repeat the previous sequence and prefix it with its last byte.
		 */
		val = code;
		i = 0;

		if (code == (uint)count) {
			val = prev_code;
			i = 1;
		}

		while (val > 255) {
			buf[i++] = table_val[val - 256];
			val = table_seq[val - 256];
		}

		buf[i] = (byte)val;

		if (code == (uint)count)
			buf[0] = (byte)val;

		/* For each code, add a new sequence to the table: identical to the previous sequence, 
		 * but prefixed with the last byte of the current one. After that, increment the number of
		 * bits per code if the index of the next sequence to define takes too many bits. The 
		 * maximum number of bits is 12, so the size of the table has to be (2 ^ 12 - 256).
		 */
		table_val[count - 256] = (byte)val;
		table_seq[count - 256] = prev_code;

		if (++count == (1 << bits_per_code) && bits_per_code < 12)
			++bits_per_code;

		/* The current sequence has to be read backwards; so when we were talking about "prefixing"
		 * we actually meant appending, and when we were talking about the "last" byte we actually
		 * meant the first byte. Experiments show that the entire sequence is never longer than 
		 * 512 bytes. Therefore a buffer of 512 bytes suffices to reverse the order of the sequence.
		 */
		do {
			if (pos >= width) {
				pos = 0;
				bmoffset += scanwidth;
				pixy += scaleY;
			}

			/* Fetch the colour for the next pixel. */
			val = buf[i--];

			/* Display the next pixel */
			if (_gfxMode != MODE_CGA) {
				bmoffset[pos++] = (byte)val;
			} else {
				if (flags & 1) {
					if (val == 2)
						bmoffset[pos++] = ALL_WHITE;
					else
						bmoffset[pos++] = ALL_BLACK;
				} else {
					cmp = 0x80;
					while(cmp) {
						if (val & cmp)
							bmoffset[pos++] = ALL_WHITE;
						else
							bmoffset[pos++] = ALL_BLACK;
						cmp = cmp >> 1;
					}
				}
			} 

			/* If it's transparent, go back and deal with it. This is actually a valid
			 * optimization; transparency is an infrequent occurence and enclosing the above in 
			 * an if() else() would involve a jump for instruction for every pixel...
			 */
			if (val == transparent) {
				--pos;
				has_transparency = true;
				bmmask[bmoffset - bmcontents + pos] = 0xFF;
				bmoffset[pos++] = 0;
			}

		} while (i >= 0);

		/* Remember the current code; this will be needed to build the next sequence in the table. */
		prev_code = code;
    }

	pos = 0;


	if (has_transparency) {
		copyRect(s, x, y, width, height, scanwidth, scaleX, scaleY, &bmmask[0], &bmcontents[0]);
	} else {
		copyRect(s, x, y, width, height, scanwidth, scaleX, scaleY, NULL, &bmcontents[0]);
	}

    /* Free memory. */
	free(bmcontents);
	free(bmmask);
	free(table_seq);
	free(table_val);
}

} // end of namespace Frotz
