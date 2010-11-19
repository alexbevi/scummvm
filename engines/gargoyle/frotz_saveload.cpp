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

#include "common/memstream.h"
#include "gargoyle/frotz_interp.h"
#include "graphics/scaler.h"
#include "graphics/thumbnail.h"

namespace Frotz {

#define makeid(a,b,c,d) ((uint32) (((a)<<24) | ((b)<<16) | ((c)<<8) | (d)))

#define ID_FORM makeid ('F','O','R','M')
#define ID_IFZS makeid ('I','F','Z','S')
#define ID_IFhd makeid ('I','F','h','d')
#define ID_UMem makeid ('U','M','e','m')
#define ID_CMem makeid ('C','M','e','m')
#define ID_Stks makeid ('S','t','k','s')
#define ID_ANNO makeid ('A','N','N','O')
#define ID_AUTH makeid ('A','U','T','H')
#define ID_THMB makeid ('T','H','M','B')

#define WRITE_BYTE(id) sf.writeByte(id)
#define WRITE_WORD(id) sf.writeUint16BE(id)
#define WRITE_LONG(id) sf.writeUint32BE(id)
#define WRITE_CHUNK(id,len) sf.writeUint32BE(id); sf.writeUint32BE(len)

#define MAX_SAVEGAME_SIZE (128 * 1024)

#define GOT_HEADER	0x01
#define GOT_STACK	0x02
#define GOT_MEMORY	0x04
#define GOT_NONE	0x00
#define GOT_ALL		0x07
#define GOT_ERROR	0x80

/**
 * Saves the current game in the Quetzal format
 */
bool FrotzInterpreter::save_game(Common::WriteStream *saveFile) {
	zword frames[STACK_SIZE / 4 + 1];
    uint32 ifzslen = 0, cmemlen = 0, stkslen = 0;
    uint32 pc;
    uint16 i, j, n;
    uint16 nvars, nargs, nstk, *p;
    uint8 var;
    long cmempos = 0, stkspos;
    byte c;
	uint32 descSizes;

	// For Quetzal saving we'll need the facility to retroactively set chunk sizes, so we'll use
	// a pre-allocated buffer and a memory write stream
	byte *buffer = (byte *)malloc(MAX_SAVEGAME_SIZE);
	assert(buffer);
	Common::MemoryWriteStream sf(buffer, MAX_SAVEGAME_SIZE);

    // Write `IFZS' header.
	WRITE_CHUNK(ID_FORM, 0);
	WRITE_LONG(ID_IFZS);

    /* Write `IFhd' chunk. */
    GET_PC(pc);
    WRITE_CHUNK(ID_IFhd, 13);
	WRITE_WORD(_h.h_release);
	sf.write(&zmp[H_SERIAL], 6);
	WRITE_WORD(_h.h_checksum);
	WRITE_LONG(pc << 8);		// Includes pad	

	// Write out AUTH chunk
	const char *AUTH_STR = "Savegame created by ScummVM Frotz engine";
	WRITE_CHUNK(ID_AUTH, ((strlen(AUTH_STR) + 1) / 2) * 2);
	sf.write(AUTH_STR, strlen(AUTH_STR));
	if ((strlen(AUTH_STR) % 2) != 0)
		sf.writeByte(0);

	// Write out ANNO chunk, which will contain the savegame description
	WRITE_CHUNK(ID_ANNO, ((strlen(_saveDescription) + 1) / 2) * 2);
	sf.write(_saveDescription, strlen(_saveDescription));
	if ((strlen(_saveDescription) % 2) != 0)
		sf.writeByte(0); 

	descSizes = ((strlen(AUTH_STR) + 1) / 2) * 2 + ((strlen(_saveDescription) + 1) / 2) * 2;

    // Write `CMem' chunk.
	cmempos = sf.pos();
	_gameFile->seek(0);
    WRITE_CHUNK(ID_CMem, 0);

	// j holds current run length.
    for (i = 0, j = 0, cmemlen = 0; i < _h.h_dynamic_size; ++i) {
		c = _gameFile->readByte() ^ zmp[i];

		if (c == 0)
			++j;	// It's a run of equal bytes.
		else {
			// Write out any run there may be.
			if (j > 0) {
				for (; j > 0x100; j -= 0x100) {
					WRITE_BYTE(0); WRITE_BYTE(0xff);
					cmemlen += 2;
				}
				WRITE_BYTE(0); WRITE_BYTE(j - 1);
				cmemlen += 2;
				j = 0;
			}
			// Any runs are now written. Write this (nonzero) byte.
			WRITE_BYTE(c);
			++cmemlen;
		}
    }

    /*
     * Reached end of dynamic memory. We ignore any unwritten run there may be
     * at this point.
     */
    if (cmemlen & 1)	// Chunk length must be even.
		WRITE_BYTE(0);

    // Write `Stks' chunk. This contains the stack dump of the current game
	stkspos = sf.pos();
    WRITE_CHUNK(ID_Stks, 0);

    /*
     * We construct a list of frame indices, most recent first, in `frames'.
     * These indices are the offsets into the `stack' array of the word before
     * the first word pushed in each frame.
     */
    frames[0] = sp - stack;	/* The frame we'd get by doing a call now. */
    for (i = fp - stack + 4, n = 0; i < STACK_SIZE + 4; i = stack[i - 3] + 5)
		frames[++n] = i;

    /*
     * All versions other than V6 can use evaluation stack outside a function
     * context. We write a faked stack frame (most fields zero) to cater for
     * this.
     */
    if (_h.h_version != V6) {
		for (i=0; i < 6; ++i)
			sf.writeByte(0);

		nstk = STACK_SIZE - frames[n];
		WRITE_WORD(nstk);

		for (j = STACK_SIZE - 1; j >= frames[n]; --j)
			WRITE_WORD(stack[j]);

		stkslen = 8 + 2 * nstk;
    }

    // Write out the rest of the stack frames.
    for (i = n; i > 0; --i) {
		p = stack + frames[i] - 4;	// Points to call frame.
		nvars = (p[0] & 0x0F00) >> 8;
		nargs =  p[0] & 0x00FF;
		nstk  =  frames[i] - frames[i-1] - nvars - 4;
		pc    =  ((uint32) p[3] << 9) | p[2];

		switch (p[0] & 0xF000) {	// Check type of call.
	    case 0x0000:	// Function.
			var = zmp[pc];
			pc = ((pc + 1) << 8) | nvars;
			break;
	    case 0x1000:	// Procedure.
			var = 0;
			pc = (pc << 8) | 0x10 | nvars;	// Set procedure flag.
			break;
	    // case 0x2000:
	    default:
			runtime_error(ERR_SAVE_IN_INTER);
			return 0;
		}

		if (nargs != 0)
			nargs = (1 << nargs) - 1;	// Make args into bitmap.

		/* Write the main part of the frame... */
		WRITE_LONG(pc);
		WRITE_BYTE(var);
		WRITE_BYTE(nargs);
		WRITE_WORD(nstk);

		// Write the variables and eval stack.
		for (j = 0, --p; j < nvars + nstk; ++j, --p)
			WRITE_WORD(*p);

		/* Calculate length written thus far. */
		stkslen += 8 + 2 * (nvars + nstk);
    }

    /* Fill in variable chunk lengths. */
    ifzslen = 5*8 + descSizes + 4 + 14 + cmemlen + stkslen;
    if (cmemlen & 1)
		++ifzslen;

	WRITE_BE_UINT32(&buffer[4], ifzslen);
	WRITE_BE_UINT32(&buffer[cmempos + 4], cmemlen);
	WRITE_BE_UINT32(&buffer[stkspos + 4], stkslen);

	// Write out everything to the output save file
	saveFile->write(&buffer[0], sf.pos());
	delete buffer;

    // After all that, still nothing went wrong!
    return !saveFile->err();
}

static inline void msg(bool fatalFlag, const char *s) {
	if (fatalFlag) error(s); else warning(s);
}
#define MSG(s) msg(fatal, s)

/**
 * Loads the specified Quetzal format savegame
 */
bool FrotzInterpreter::load_game(Common::ReadStream *saveFile) {
    uint32 ifzslen, currlen, tmpl;
    uint32 pc;
    uint16 i, tmpw;
    bool fatal = false;
    uint8 skip, progress = GOT_NONE;
    uint8 x, y;
	int runLength;

    // Check it's really an `IFZS' file.
	tmpl = saveFile->readUint32BE();
	ifzslen = saveFile->readUint32BE();
	currlen = saveFile->readUint32BE();

    if (tmpl != ID_FORM || currlen != ID_IFZS) {
		warning("Unrecognised savegame format");
		return false;
	}

    if ((ifzslen & 1) || (ifzslen < 4))
		// Sanity checks
		return false;

    ifzslen -= 4;

    // Read each chunk and process it.
    while (ifzslen > 0) {
		// Read chunk header.
		if (ifzslen < 8)
			// Couldn't contain a chunk.
			return false;

		tmpl = saveFile->readUint32BE();
		currlen = saveFile->readUint32BE();;
		ifzslen -= 8;	// Reduce remaining by size of header.

		// Handle chunk body.
		if (ifzslen < currlen)
			// Chunk goes past EOF?!
			return false;

		skip = currlen & 1;
		ifzslen -= currlen + (uint32)skip;

		switch (tmpl) {
		// `IFhd' header chunk; must be first in file.
		case ID_IFhd:
			if (progress & GOT_HEADER) {
				MSG("Save file has multiple two IFZS chunks!");
				return false;
			}
			progress |= GOT_HEADER;

			if (currlen < 13) return false;

			tmpw = saveFile->readUint16BE();
			if (tmpw != _h.h_release)
				progress = GOT_ERROR;


			for (i = H_SERIAL; i < H_SERIAL + 6; ++i) {
				x = saveFile->readByte();
				if (x != zmp[i])
					progress = GOT_ERROR;
			}

			tmpw = saveFile->readUint16BE();
			if (tmpw != _h.h_checksum)
				progress = GOT_ERROR;

			if (progress & GOT_ERROR) {
				MSG("File was not saved from this story!");
				return false;
			}
		
			// Get the PC
			x = saveFile->readByte();
			pc = (uint32) x << 16;
			x = saveFile->readByte();
			pc |= (uint32) x << 8;
			x = saveFile->readByte();
			pc |= (uint32) x;

			/* Since the PC has been changed, any errors from here on have to be fatal */
			SET_PC (pc);
			fatal = true;

			// Skip the rest of the chunk
			for (i = 13; i < currlen; ++i)
				saveFile->readByte();
			break;

		/* `Stks' stacks chunk; restoring this is quite complex. ;) */
		case ID_Stks:
			if (progress & GOT_STACK) {
				MSG("File contains two stack chunks!");
				break;
			}
			progress |= GOT_STACK;

			fatal = true;	// Setting SP means errors must be fatal.
			sp = stack + STACK_SIZE;

			/*
			 * All versions other than V6 may use evaluation stack outside
			 * any function context. As a result a faked function context
			 * will be present in the file here. We skip this context, but
			 * load the associated stack onto the stack proper...
			 */
			if (_h.h_version != V6) {
				if (currlen < 8) return false;
				for (i = 0; i < 6; ++i)
					saveFile->readByte();

				tmpw = saveFile->readUint16BE();
				if (tmpw > STACK_SIZE) {
					MSG("Save-file has too much stack (and I can't cope).");
					return false;
				}
				currlen -= 8;

				if (currlen < (uint)tmpw * 2)			return false;
				for (i=0; i<tmpw; ++i)
					*--sp = saveFile->readUint16BE();
				currlen -= tmpw*2;
			}

			// We now proceed to load the main block of stack frames.
			for (fp = stack + STACK_SIZE, frame_count = 0; currlen > 0;
					currlen -= 8, ++frame_count) {
				if (currlen < 8)				return false;
				if (sp - stack < 4) {
					/* No space for frame. */
					MSG("Save-file has too much stack (and I can't cope).");
					return false;
				}

				// Read PC, procedure flag and formal param count.
				tmpl = saveFile->readUint32BE();
				y = (int) (tmpl & 0x0F);	/* Number of formals. */
				tmpw = y << 8;

				// Read result variable.
				x = saveFile->readByte();

				// Check the procedure flag...
				if (tmpl & 0x10) {
					tmpw |= 0x1000;	// It's a procedure.
					tmpl >>= 8;		// Shift to get PC value.
				} else {
					// Functions have type 0, so no need to or anything.
					tmpl >>= 8;		// Shift to get PC value.
					--tmpl;			// Point at result byte.

					// Sanity check on result variable...
					if (zmp[tmpl] != x) {
						MSG("Save-file has wrong variable number on stack (possibly wrong game version?)");
						return false;
					}
				}
				*--sp = (uint16) (tmpl >> 9);		// High part of PC
				*--sp = (uint16) (tmpl & 0x1FF);	// Low part of PC
				*--sp = (uint16) (fp - stack - 1);	// FP

				// Read and process argument mask.
				x = saveFile->readByte();
				++x;	// Should now be a power of 2
				for (i = 0; i < 8; ++i)
					if (x & (1 << i))
						break;

				if (x ^ (1<<i)) {
					// Not a power of 2
					MSG("Save-file uses incomplete argument lists (which I can't handle)");
					return false;
				}
				*--sp = tmpw | i;
				fp = sp;	// FP for next frame.

				// Read amount of eval stack used.
				tmpw = saveFile->readUint16BE();

				tmpw += y;	/* Amount of stack + number of locals. */
				if (sp - stack <= tmpw) {
					MSG("Save-file has too much stack (and I can't cope).");
					return false;
				}
				if (currlen < (uint)tmpw * 2)			return false;
				for (i=0; i<tmpw; ++i)
					*--sp = saveFile->readUint16BE();
				currlen -= tmpw * 2;
			}
			/* End of `Stks' processing... */
			break;

		/* Any more special chunk types must go in HERE or ABOVE. */
		/* `CMem' compressed memory chunk; uncompress it. */
		case ID_CMem:
			if (!(progress & GOT_MEMORY)) {
				/* Don't complain if two. */
				_gameFile->seek(0);

				i=0;	// Bytes written to data area.
				for (; currlen > 0; --currlen) {
					x = saveFile->readByte();
					if (x == 0) {
						// Start run.
						// Check for bogus run.
						if (currlen < 2) {
							MSG("File contains bogus `CMem' chunk.");
							for (; currlen > 0; --currlen)
								saveFile->readByte();	// Skip rest.
							currlen = 1;
							i = 0xFFFF;
							break; // Keep going; may be a `UMem' too.
						}

						// Copy story file to memory during the run.
						--currlen;
						runLength = saveFile->readByte() + 1;
						for (; runLength > 0 && i < _h.h_dynamic_size; --runLength, ++i)
							zmp[i] = _gameFile->readByte();

					} else {
						// Not a run. 
						y = _gameFile->readByte();
						zmp[i] = (uint8)(x ^ y);
						++i;
					}

					// Make sure we don't load too much.
					if (i > _h.h_dynamic_size) {
						MSG("warning: `CMem' chunk too long!");
						for (; currlen > 1; --currlen)
							saveFile->readByte();	// Skip rest
						break;	// Keep going; there may be a `UMem' too.
					}
				}

				// If chunk is short, assume a run.
				for (; i < _h.h_dynamic_size; ++i) {
					y = _gameFile->readByte();
					zmp[i] = y;
				}

				if (currlen == 0)
					progress |= GOT_MEMORY;	/* Only if succeeded. */
				break;
			}

		/* Fall right thru (to default) if already GOT_MEMORY */
		/* `UMem' uncompressed memory chunk; load it. */
		case ID_UMem:
			if (!(progress & GOT_MEMORY)) {
				// Don't complain if two.
				// Must be exactly the right size.
				if (currlen == _h.h_dynamic_size) {
					if (saveFile->read(zmp, currlen) == currlen) {
						progress |= GOT_MEMORY;		// Only on success.
						break;
					}
				} else
					MSG("`UMem' chunk wrong size!");

				/* Fall into default action (skip chunk) on errors. */
			}
			// Fall thru (to default) if already GOT_MEMORY

		/* Unrecognised chunk type; skip it. */
		default:
			// Skip chunk
			for (i = 0; i < currlen; ++i)
				saveFile->readByte();
			break;
		}

		if (skip)
			saveFile->readByte();	// Skip pad byte.
	}

    /*
     * We've reached the end of the file. For the restoration to have been a
     * success, we must have had one of each of the required chunks.
     */
    if (!(progress & GOT_HEADER))
		MSG("error: no valid header (`IFhd') chunk in file.");
    if (!(progress & GOT_STACK))
		MSG("error: no valid stack (`Stks') chunk in file.");
    if (!(progress & GOT_MEMORY))
		MSG("error: no valid memory (`CMem' or `UMem') chunk in file.");

    return (progress == GOT_ALL) ? true : !fatal;
}

/**
 * Validates that the specified file is a valid Quetzal savegame, and returns the
 * game description if it has one
 */
bool FrotzInterpreter::validate_savegame(Common::ReadStream *saveFile, char *gameDescription) {
    uint32 ifzslen, currlen, tmpl;
    uint8 skip;

    // Check it's really an `IFZS' file.
	tmpl = saveFile->readUint32BE();
	ifzslen = saveFile->readUint32BE();
	currlen = saveFile->readUint32BE();

    if (tmpl != ID_FORM || currlen != ID_IFZS)
		return false;

    if ((ifzslen & 1) || (ifzslen < 4))
		// Sanity checks
		return false;

    ifzslen -= 4;

    // Read each chunk and process it.
    while (ifzslen > 0) {
		// Read chunk header.
		if (ifzslen < 8)
			// Couldn't contain a chunk.
			return false;

		tmpl = saveFile->readUint32BE();
		currlen = saveFile->readUint32BE();;
		ifzslen -= 8;	// Reduce remaining by size of header.

		// Handle chunk body.
		if (ifzslen < currlen)
			// Chunk goes past EOF?!
			return false;

		skip = currlen & 1;
		ifzslen -= currlen + (uint32)skip;

		switch (tmpl) {
		// `IFhd' header chunk; must be first in file.
		case ID_ANNO:
			// Get the game description
			if (currlen > 99) return false;
			saveFile->read(gameDescription, currlen);
			gameDescription[currlen] = '\0';
			return true;

	    // Skip all the other chunk types 
	    default:
			for (uint i = 0; i < currlen; ++i)
				saveFile->readByte();	
			break;
		}

		if (skip)
			saveFile->readByte();		// Skip pad byte
	}

	// Savegame doesn't have an ANNO chunk, so return a default description
	strcpy(gameDescription, "Unnamed Savegame");
	return true;
}

} // end of namespace Frotz
