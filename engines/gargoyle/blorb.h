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

#ifndef GARGOYLE_BLORB_H
#define GARGOYLE_BLORB_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/endian.h"
#include "common/file.h"
#include "graphics/surface.h"

namespace Gargoyle {

/* blorb.h: Header file for Blorb library, version 1.0.2.
    Designed by Andrew Plotkin <erkyrath@eblong.com>
    http://www.eblong.com/zarf/blorb/index.html
    
    This is the header that a Z-machine interpreter should include.
    It defines everything that the interpreter has to know.
*/

// Match the Blorb endian handling to that of ScummVM's system wide endian setting
#ifdef SCUMM_LITTLE_ENDIAN
#define BLORB_LITTLE_ENDIAN
#else
#define BLORB_BIG_ENDIAN 
#endif

/* Error type and error codes */
enum bb_err_t {
	bb_err_None = 0,
	bb_err_CompileTime = 1,
	bb_err_Alloc = 2,
	bb_err_Read = 3,
	bb_err_NotAMap = 4,
	bb_err_Format = 5,
	bb_err_NotFound = 6
};

/* Methods for loading a chunk */
#define bb_method_DontLoad (0)
#define bb_method_Memory (1)
#define bb_method_FilePos (2)

/* Four-byte constants */

#define bb_make_id(c1, c2, c3, c4)  \
    (uint32)(((c1) << 24) | ((c2) << 16) | ((c3) << 8) | (c4))

#define bb_ID_Snd       (bb_make_id('S', 'n', 'd', ' '))
#define bb_ID_Exec      (bb_make_id('E', 'x', 'e', 'c'))
#define bb_ID_Pict      (bb_make_id('P', 'i', 'c', 't'))
#define bb_ID_Copyright (bb_make_id('(', 'c', ')', ' '))
#define bb_ID_AUTH      (bb_make_id('A', 'U', 'T', 'H'))
#define bb_ID_ANNO      (bb_make_id('A', 'N', 'N', 'O'))

/* bb_result_t: Result when you try to load a chunk. */
typedef struct bb_result_struct {
	uint32 chunknum;
    union {
        void *ptr; /* A pointer to the data (if you used bb_method_Memory) */
        uint32 startpos; /* The position in the file (if you used bb_method_FilePos) */
    } data;
    uint32 length; /* The length of the data */
} bb_result_t;

/* bb_aux_sound_t: Extra data which may be associated with a sound. */
typedef struct bb_aux_sound_struct {
    char repeats;
} bb_aux_sound_t;

/* bb_aux_pict_t: Extra data which may be associated with an image. */
typedef struct bb_aux_pict_struct {
    uint32 ratnum, ratden;
    uint32 minnum, minden;
    uint32 maxnum, maxden;
} bb_aux_pict_t;

/* bb_resolution_t: The global resolution data. */
typedef struct bb_resolution_struct {
    uint32 px, py;
    uint32 minx, miny;
    uint32 maxx, maxy;
} bb_resolution_t;

/* bb_colour_t: Guess what. */
typedef struct bb_colour_struct {
    unsigned char red, green, blue;
} bb_colour_t;

/* bb_palette_t: The palette data. */
typedef struct bb_palette_struct {
    int isdirect; 
    union {
        int depth; /* The depth (if isdirect is TRUE). Either 16 or 32. */
        struct {
            int numcolours;
            bb_colour_t *colours;
        } table; /* The list of colours (if isdirect is FALSE). */
    } data;
} bb_palette_t;

/* bb_zheader_t: Information to identify a Z-code file. */
typedef struct bb_zheader_struct {
    uint16 releasenum; /* Bytes $2-3 of header. */
    char serialnum[6]; /* Bytes $12-17 of header. */
    uint16 checksum; /* Bytes $1C-1D of header. */
    /* The initpc field is not used by Blorb. */
} bb_zheader_t;

/* bb_chunkdesc_t: Describes one chunk of the Blorb file. */
typedef struct bb_chunkdesc_struct {
	uint32 type;
	uint32 len;
	uint32 startpos; /* start of chunk header */
	uint32 datpos; /* start of data (either startpos or startpos+8) */
	
	void *ptr; /* pointer to malloc'd data, if loaded */
	int auxdatnum; /* entry in the auxsound/auxpict array; -1 if none.
		This only applies to chunks that represent resources;  */
	
} bb_chunkdesc_t;

/* bb_resdesc_t: Describes one resource in the Blorb file. */
typedef struct bb_resdesc_struct {
	uint32 usage;
	int resnum;
	int chunknum;
} bb_resdesc_t;


class BlorbFile {
private:
	Common::SeekableReadStream *_stream;
	bool _ownsStream;
	bb_zheader_t *_zheader;
	int _releasenum; 
	bb_resolution_t *_resolution;

	bb_err_t initializeMap();
	bb_err_t destroyMap();
public:
	Common::Array<bb_chunkdesc_t *> chunks;	/* Chunk list */
	Common::Array<bb_resdesc_t> resources;	/* list of resource descriptors */
	Common::Array<bb_resdesc_t *> ressorted; /* list of pointers to descriptors 
			in resources -- sorted by usage and resource number. */
	Common::Array<bb_aux_sound_t *> auxsound; /* extra information about sounds */

	int palettechunk; /* chunk number of palette, or -1 if there is none. */
	bb_palette_t *palette;
	bb_aux_pict_t *auxpict; /* extra information about pictures */


	BlorbFile();
	~BlorbFile();
	
	bb_err_t open(Common::SeekableReadStream *stream, bool ownsStream = false);
	bb_err_t loadChunkByType(int method, bb_result_t *res, uint32 type, int count);
	bb_err_t loadChunkByNumber(int method, bb_result_t *res, int chunknum);
	bb_err_t loadResource(int method, bb_result_t *res, uint32 usage, int resnum);

	bb_err_t unloadChunk(int chunknum);
	bb_err_t countResources(uint32 usage, int *num, int *min, int *max);
	bb_err_t getPalette(bb_palette_t **res);
	bb_err_t loadResource_pict(int method, bb_result_t *res,  int resnum, bb_aux_pict_t **auxdata);
	bb_err_t loadResource_snd(int method, bb_result_t *res, int resnum, bb_aux_sound_t **auxdata);
			
	Graphics::Surface *getPicture(int picNumber) { error("BlorbFile::loadPicture unimplemented"); }

	Common::SeekableReadStream *stream() { return _stream; }
	uint16 getReleaseNum() const;
	bb_zheader_t *getZheader() const;
	bb_resolution_t *getResolution() const;
	const char *errToString(bb_err_t err);

	static bool isBlorb(Common::SeekableReadStream *stream);
};

} // end of namespace Gargoyle

#endif
