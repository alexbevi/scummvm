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
 * $URL$
 * $Id$
 */

#include "gargoyle/blorb.h"
#include "common/endian.h"
#include "common/util.h"

namespace Gargoyle {

/* blorblib.c: Blorb file reader library, version 1.0.2.
	Designed by Andrew Plotkin <erkyrath@eblong.com>
	http://www.eblong.com/zarf/blorb/index.html
	
	This is portable code to read a Blorb file. Add it to your
	interpreter, #include "blorb.h", and you're ready to go.
*/

/* More four-byte constants. */

#define bb_ID_FORM (bb_make_id('F', 'O', 'R', 'M'))
#define bb_ID_IFRS (bb_make_id('I', 'F', 'R', 'S'))
#define bb_ID_RIdx (bb_make_id('R', 'I', 'd', 'x'))
#define bb_ID_IFhd (bb_make_id('I', 'F', 'h', 'd'))
#define bb_ID_Reso (bb_make_id('R', 'e', 's', 'o'))
#define bb_ID_Loop (bb_make_id('L', 'o', 'o', 'p'))
#define bb_ID_RelN (bb_make_id('R', 'e', 'l', 'N'))
#define bb_ID_Plte (bb_make_id('P', 'l', 't', 'e'))

/* Turn a four-byte constant into a string. This returns a static buffer,
	so if you call it twice, the old value gets overwritten. */

char *bb_id_to_string(uint32 id) {
	static char buf[5];
	buf[0] = (id >> 24) & 0xff;
	buf[1] = (id >> 16) & 0xff;
	buf[2] = (id >> 8) & 0xff;
	buf[3] = (id) & 0xff;
	buf[4] = '\0';
	return buf;
}


/* This is used for binary searching and quicksorting the resource pointer list. */
bool sortsplot(const bb_resdesc_t *a1, const bb_resdesc_t *a2) {
	return (a1->usage < a2->usage);
}

/*-----------------------------------------------------------------------------------
 * Class based Blorb implementation
 *
 *-----------------------------------------------------------------------------------
 */

BlorbFile::BlorbFile() {
	_stream = NULL;
	_ownsStream = false;
	_zheader = NULL;
	_resolution = NULL;
	palette = NULL;
	auxpict = NULL;
}

BlorbFile::~BlorbFile() {
	if ((_stream != NULL) && _ownsStream)
		delete _stream;
	
	destroyMap();
}

/**
 * Opens up the contents of the passed stream for accessing as a Blorb file.
 * Matches the original method bb_create_map
 * @param stream		the stream containing the Blorb file to read
 * @param ownStream	 If true, makes the BlorbFile responsible for destroying the stream
 */

bb_err_t BlorbFile::open(Common::SeekableReadStream *srcStream, bool ownsStream) {
	_stream = srcStream;
	_ownsStream = ownsStream;

	bb_err_t err;
	size_t readlen;
	uint32 nextpos, totallength;
	int chunks_size;
	uint32 buffer[4];
	
	/* First, chew through the file and index the chunks. */
	
	_stream->seek(0);
	
	readlen = _stream->read(buffer, sizeof(uint32) * 3);
	if (readlen != (sizeof(uint32) * 3))
		return bb_err_Read;
	
	if (FROM_BE_32(buffer[0]) != bb_ID_FORM)
		return bb_err_Format;
	if (FROM_BE_32(buffer[2]) != bb_ID_IFRS)
		return bb_err_Format;
	
	totallength = FROM_BE_32(buffer[1]) + 8;
	nextpos = 12;

	chunks_size = 8;

	while (nextpos < totallength) {
		uint32 type, len;
		bb_chunkdesc_t *chu;
		
		_stream->seek(nextpos);
//		if (_stream->ioFailed())
//			return bb_err_Read;
		
		readlen = _stream->read(buffer, sizeof(uint32) * 2);
		if (readlen != (sizeof(uint32) * 2))
			return bb_err_Read;
		
		type = FROM_BE_32(buffer[0]);
		len = FROM_BE_32(buffer[1]);

		chu = (bb_chunkdesc_t *)malloc(sizeof(bb_chunkdesc_t));
		chunks.push_back(chu);
		
		chu->type = type;
		chu->startpos = nextpos;
		if (type == bb_ID_FORM) {
			chu->datpos = nextpos;
			chu->len = len+8;
		} else {
			chu->datpos = nextpos+8;
			chu->len = len;
		}
		chu->ptr = NULL;
		chu->auxdatnum = -1;
		
		nextpos = nextpos + len + 8;
		if (nextpos & 1)
			++nextpos;
			
		if (nextpos > totallength)
			return bb_err_Format;
	}
	
	/* The basic IFF structure seems to be ok, and we have a list of
		chunks. Now we allocate the map structure itself. */
	
	_releasenum = 0;
	_zheader = NULL;
	_resolution = NULL;
	palettechunk = -1;
	palette = NULL;
	auxpict = NULL;
	
	/* Now we do everything else involved in loading the Blorb file,
		such as building resource lists. */
	
	err = initializeMap();
	if (err) {
		destroyMap();
		return err;
	}
	
	return bb_err_None;
}

/**
 * Turn an error code into a string describing the error.
 * @param err		the error code value
 */

const char *BlorbFile::errToString(bb_err_t err) {
	switch (err) {
	case bb_err_None:
		return "ok";
	case bb_err_CompileTime:
		return "library compiled wrong";
	case bb_err_Alloc:
		return "cannot allocate memory";
	case bb_err_Read:
		return "cannot read from file";
	case bb_err_NotAMap:
		return "map structure is bad";
	case bb_err_Format:
		return "bad format in Blorb file";
	case bb_err_NotFound:
		return "data not found";
	default:
		return "unknown error";
	}
}

bb_err_t BlorbFile::loadChunkByType(int method, bb_result_t *res, uint32 type, int count) {
	uint ix;
	
	for (ix = 0; ix < chunks.size(); ++ix) {
		if (chunks[ix]->type == type) {
			if (count == 0)
				break;
			--count;
		}
	}
	
	if (ix >= chunks.size()) 
		return bb_err_NotFound;

	return loadChunkByNumber(method, res, ix);
}

bb_err_t BlorbFile::loadChunkByNumber(int method, bb_result_t *res, int chunknum) {
	bb_chunkdesc_t *chu;
	
	if (chunknum < 0 || chunknum >= (int)chunks.size())
		return bb_err_NotFound;

	chu = chunks[chunknum];
	
	switch (method) {
	case bb_method_DontLoad:
		/* do nothing */
		break;
		
	case bb_method_FilePos:
		res->data.startpos = chu->datpos;
		break;
		
	case bb_method_Memory:
		if (!chu->ptr) {
			size_t readlen;
			void *dat = malloc(chu->len);
			
			if (!dat)
				return bb_err_Alloc;
			
			_stream->seek(chu->datpos);
//			if (_stream->ioFailed())
//				return bb_err_Read;
			
			readlen = _stream->read(dat, chu->len);
			if (readlen != chu->len)
				return bb_err_Read;
			
			chu->ptr = dat;
		}
		res->data.ptr = chu->ptr;
		break;
	}
	
	res->length = chu->len;
	res->chunknum = chunknum;
	
	return bb_err_None;
}

bb_err_t BlorbFile::loadResource(int method, bb_result_t *res, uint32 usage, int resnum) {
	// Scan the resource list 
	for (uint ix = 0; ix < ressorted.size(); ++ix) {
		if (ressorted[ix]->usage == usage) 
			return loadChunkByNumber(method, res, ressorted[ix]->chunknum);
		else if (ressorted[ix]->usage > usage)
			break;
	}

	return bb_err_NotFound;
}

bb_err_t BlorbFile::unloadChunk(int chunknum) {
	bb_chunkdesc_t *chu;
	
	if (chunknum < 0 || chunknum >= (int)chunks.size())
		return bb_err_NotFound;

	chu = chunks[chunknum];
	
	if (chu->ptr) {
		free(chu->ptr);
		chu->ptr = NULL;
	}
	
	return bb_err_None;
}

bb_err_t BlorbFile::countResources(uint32 usage, int *num, int *min, int *max) {
	int count, minval, maxval, val;
	
	count = 0;
	minval = 0;
	maxval = 0;
	
	for (uint ix = 0; ix < resources.size(); ++ix) {
		if (resources[ix].usage == usage) {
			val = resources[ix].resnum;
			if (count == 0) {
				++count;
				minval = val;
				maxval = val;
			} else {
				++count;
				if (val < minval)
					minval = val;
				if (val > maxval)
					maxval = val;
			}
		}
	}
	
	if (num)
		*num = count;
	if (min)
		*min = minval;
	if (max)
		*max = maxval;
	
	return bb_err_None;
}

uint16 BlorbFile::getReleaseNum() const {
	return _releasenum;
}

bb_zheader_t *BlorbFile::getZheader() const {
	return _zheader;
}

bb_resolution_t *BlorbFile::getResolution() const {
	return _resolution;
}

bb_err_t BlorbFile::getPalette(bb_palette_t **res) {
	int ix;
	bb_err_t err;

	if (res)
		*res = NULL;

	if (palettechunk < 0) {
		return bb_err_None;
	}
	
	if (!palette) {
		bb_result_t chunkres;
		bb_palette_t *pal;
		unsigned char *ptr;
		
		pal = (bb_palette_t *)malloc(sizeof(bb_palette_t));
		if (!pal)
			return bb_err_Alloc;
			
		err = loadChunkByNumber(bb_method_Memory, &chunkres, palettechunk);
		if (err)
			return err;
		
		ptr = (unsigned char *)chunkres.data.ptr;
		
		if (chunkres.length == 1) {
			int val = ptr[0];
			if (val != 16 && val != 32)
				return bb_err_Format;
			pal->isdirect = true;
			pal->data.depth = val;
		} else {
			int size = chunkres.length / 3;
			bb_colour_t *colours = (bb_colour_t *)malloc(size * sizeof(bb_colour_t));
			if (!colours)
				return bb_err_Alloc;
			if (size < 1 || size > 256)
				return bb_err_Format;
			for (ix = 0; ix < size; ++ix) {
				colours[ix].red   = ptr[ix * 3 + 0];
				colours[ix].green = ptr[ix * 3 + 1];
				colours[ix].blue  = ptr[ix * 3 + 2];
			}
			pal->isdirect = false;
			pal->data.table.numcolours = size;
			pal->data.table.colours = colours;
		}
		
		unloadChunk(palettechunk);
		palette = pal;
	}
	
	if (res)
		*res = palette;
	return bb_err_None;
}

bb_err_t BlorbFile::loadResource_pict(int method, bb_result_t *res,  int resnum, bb_aux_pict_t **auxdata) {
	bb_err_t err;
	
	if (auxdata)
		*auxdata = NULL;
	
	err = loadResource(method, res, bb_ID_Pict, resnum);
	if (err)
		return err;
	
	if (auxdata) {
		bb_chunkdesc_t *chu = chunks[resnum];
		if (chu->auxdatnum >= 0 && auxpict) {
			*auxdata = &(auxpict[chu->auxdatnum]);
		}
	}

	return bb_err_None;
}

bb_err_t BlorbFile::loadResource_snd(int method, bb_result_t *res, int resnum, bb_aux_sound_t **auxdata) {
	bb_err_t err;
	
	if (auxdata)
		*auxdata = NULL;
	
	err = loadResource(method, res, bb_ID_Snd, resnum);
	if (err)
		return err;
	
	if (auxdata) {
		bb_chunkdesc_t *chu = chunks[resnum];
		if (chu->auxdatnum >= 0 && chu->auxdatnum < (int)auxsound.size())
			*auxdata = auxsound[chu->auxdatnum];
	}

	return bb_err_None;
}

/*------- Support methods --------------------------------------------------*/

bb_err_t BlorbFile::initializeMap() {
	int jx;
	bb_result_t chunkres;
	bb_err_t err;
	uint32 *ptr;
	uint32 len;
	uint32 val;
	int numres;
	int gotindex = false; 

	for (uint ix = 0; ix < chunks.size(); ++ix) {
		bb_chunkdesc_t *chu = chunks[ix];

		switch (chu->type) {
		case bb_ID_RIdx:
			/* Resource index chunk: build the resource list and sort it. */
			if (gotindex) 
				return bb_err_Format; /* duplicate index chunk */

			err = loadChunkByNumber(bb_method_Memory, &chunkres, ix);
			if (err) 
				return err;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;
			val = ptr[0];
			numres = FROM_BE_32(val);

			if (numres) {
				uint ix2;

				if ((int)len != numres * 12 + 4)
					return bb_err_Format; /* bad length field */

				resources.resize(numres);
				ressorted.resize(numres);

				ix2 = 0;
				for (jx = 0; jx < numres; ++jx) {
					bb_resdesc_t *res = &resources[jx];
					uint32 respos;

					val = ptr[1+jx*3];
					res->usage = FROM_BE_32(val);
					val = ptr[2+jx*3];
					res->resnum = FROM_BE_32(val);
					val = ptr[3+jx*3];
					respos = FROM_BE_32(val);

					while (ix2 < chunks.size() && chunks[ix2]->startpos < respos)
						++ix2;

					if (ix2 >= chunks.size() || chunks[ix2]->startpos != respos)
						return bb_err_Format; /* start pos does not match a real chunk */

					res->chunknum = ix2;

					ressorted[jx] = res;
				}

				// Sort the resource list 
				Common::sort(ressorted.begin(), ressorted.end(), sortsplot);
			}

			unloadChunk(ix);
			gotindex = true;
			break;

		case bb_ID_RelN:
			/* Release number chunk: Get the release number. */
			err = loadChunkByNumber(bb_method_Memory, &chunkres, ix);
			if (err) 
				return err;

			if (chunkres.length != 2)
				return bb_err_Format;

			{
				_releasenum = READ_BE_UINT16(chunkres.data.ptr);
			}

			unloadChunk(ix);
			break;

		case bb_ID_IFhd:
			/* Z-header chunk: Get the header info. */
			err = loadChunkByNumber(bb_method_Memory, &chunkres, ix);
			if (err) 
				return err;

			if (chunkres.length < 13)
				return bb_err_Format;

			{
				bb_zheader_t *head = (bb_zheader_t *)malloc(sizeof(bb_zheader_t));
				if (!head)
					return bb_err_Alloc;

				head->releasenum = READ_BE_UINT16(chunkres.data.ptr);
				head->checksum = READ_BE_UINT16((uint8 *)chunkres.data.ptr + 4);

				for (jx = 0; jx < 6; ++jx) {
					head->serialnum[jx] = ((char *)(chunkres.data.ptr))[2 + jx];
				}

				_zheader = head;
			}

			unloadChunk(ix);
			break;

		case bb_ID_Reso:
			/* Resolution chunk: Get the window size data, and resolution
				ratios for images. */
			err = loadChunkByNumber(bb_method_Memory, &chunkres, ix);
			if (err) 
				return err;

			if (chunkres.length < 24)
				return bb_err_Format;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;

			{
				bb_resolution_t *reso = (bb_resolution_t *)malloc(sizeof(bb_resolution_t));
				if (!reso)
					return bb_err_Alloc;

				reso->px = FROM_BE_32(ptr[0]);
				reso->py = FROM_BE_32(ptr[1]);
				reso->minx = FROM_BE_32(ptr[2]);
				reso->miny = FROM_BE_32(ptr[3]);
				reso->maxx = FROM_BE_32(ptr[4]);
				reso->maxy = FROM_BE_32(ptr[5]);

				_resolution = reso;
			}

			ptr += 6;
			len -= 6 * 4;

			len = len / 28;

			if (len) {
				bb_aux_pict_t *aux = (bb_aux_pict_t *)malloc(len * sizeof(bb_aux_pict_t)); 

				for (jx = 0; jx < (int)len; ++jx, ptr += 7) {
					bb_result_t res;

					int resnum = FROM_BE_32(ptr[0]);
					err = loadResource(bb_method_DontLoad, &res, bb_ID_Pict, resnum);
					if (!err) {
						bb_chunkdesc_t *chunk = chunks[resnum];
						if (chunk->auxdatnum != -1)
							return bb_err_Format; /* two image entries for this resource */
						chunk->auxdatnum = jx;
						aux[jx].ratnum = FROM_BE_32(ptr[1]);
						aux[jx].ratden = FROM_BE_32(ptr[2]);
						aux[jx].minnum = FROM_BE_32(ptr[3]);
						aux[jx].minden = FROM_BE_32(ptr[4]);
						aux[jx].maxnum = FROM_BE_32(ptr[5]);
						aux[jx].maxden = FROM_BE_32(ptr[6]);
					}
				}

				auxpict = aux;
			}

			unloadChunk(ix);
			break;

		case bb_ID_Loop:
			/* Looping chunk: Get looping data for sounds. */
			err = loadChunkByNumber(bb_method_Memory, &chunkres, ix);
			if (err) 
				return err;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;

			len = len / 8;

			if (len) {
				for (jx = 0; jx < (int)len; ++jx, ptr += 2) {
					bb_result_t res;
					bb_aux_sound_t *rec = (bb_aux_sound_t *)malloc(sizeof(bb_aux_sound_t));
					auxsound.push_back(rec);

					int resnum = FROM_BE_32(ptr[0]);
					err = loadResource(bb_method_DontLoad, &res, bb_ID_Snd, resnum);
					if (!err) {
						bb_chunkdesc_t *chunk = chunks[resnum];
						if (chunk->auxdatnum != -1)
							return bb_err_Format; /* two looping entries for this resource */
						chunk->auxdatnum = jx;
						rec->repeats = FROM_BE_32(ptr[1]);
					}
				}
			}
			
			unloadChunk(ix);
			break;
		
		case bb_ID_Plte:
			/* Palette chunk: Don't get the palette info now, since it may
				be large and the interpreter may not care. But remember
				the chunk number in case the interpreter asks later. */
				
			palettechunk = ix;
			break;
		}
	}
	
	return bb_err_None;
}

bb_err_t BlorbFile::destroyMap() {
	uint ix;

	for (ix = 0; ix < chunks.size(); ++ix)
		free(chunks[ix]);
	chunks.clear();

	resources.clear();
	ressorted.clear();

	if (_zheader) {
		free(_zheader);
		_zheader = NULL;
	}

	if (_resolution) {
		free(_resolution);
		_resolution = NULL;
	}

	if (palette) {
		if (!palette->isdirect && palette->data.table.colours) {
			free(palette->data.table.colours);
			palette->data.table.colours = NULL;
		}
		free(palette);
		palette = NULL;
	}

	for (ix = 0; ix < auxsound.size(); ++ix)
		free(auxsound[ix]);
	auxsound.clear();

	if (auxpict) {
		free(auxpict);
		auxpict = NULL;
	}

	return bb_err_None;
}

/**
 * Returns true if the specified stream is a valid Blorb file
 */

bool BlorbFile::isBlorb(Common::SeekableReadStream *stream) {
	stream->seek(0);
	uint32 v1 = stream->readUint32BE();
	stream->skip(sizeof(uint32));
	uint32 v2 = stream->readUint32BE();

	return (v1 == bb_ID_FORM) && (v2 == bb_ID_IFRS);
}

} // end of namespace Gargoyle
