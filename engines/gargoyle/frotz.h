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

#ifndef FROTZ_H
#define FROTZ_H

#include "gargoyle/gargoyle.h"

namespace Frotz {

/*** Constants that may be set at compile time ***/

#ifndef MAX_UNDO_SLOTS
#define MAX_UNDO_SLOTS 500
#endif
#ifndef MAX_FILE_NAME
#define MAX_FILE_NAME 256
#endif
#ifndef TEXT_BUFFER_SIZE
#define TEXT_BUFFER_SIZE 200
#endif
#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE 200
#endif
#ifndef STACK_SIZE
#define STACK_SIZE 32768
#endif

#define FROTZ_DOMAIN "Frotz"

typedef uint8 zbyte;
typedef uint16 zword;
typedef uint32 zlong;
typedef unsigned char zchar;

enum story {
	AMFV = Gargoyle::GID_ZCODE_AMFV,
    ARTHUR = Gargoyle::GID_ZCODE_ARTHUR,
    BEYOND_ZORK = Gargoyle::GID_ZCODE_BEYOND_ZORK,
	HHGTTG = Gargoyle::GID_ZCODE_HHGTTG,
    JOURNEY = Gargoyle::GID_ZCODE_JOURNEY,
    LURKING_HORROR = Gargoyle::GID_ZCODE_LURKING_HORROR,
    SHERLOCK = Gargoyle::GID_ZCODE_SHERLOCK,
    SHOGUN = Gargoyle::GID_ZCODE_SHOGUN,
    ZORK_ZERO = Gargoyle::GID_ZCODE_ZORK_ZERO,
    UNKNOWN = Gargoyle::GID_ZCODE_UNKNOWN
};


#define FALSE false 
#define TRUE true

enum ZCodeVersion {
	V1 = 1,
	V2 = 2,
	V3 = 3,
	V4 = 4,
	V5 = 5,
	V6 = 6,
	V7 = 7,
	V8 = 8
};

typedef story FrotzGameId;

#ifndef DEFAULT_SAVE_NAME
//#define DEFAULT_SAVE_NAME "story.sav"
#define DEFAULT_SAVE_NAME ""
#endif
#ifndef DEFAULT_SCRIPT_NAME
#define DEFAULT_SCRIPT_NAME "story.scr"
#endif
#ifndef DEFAULT_COMMAND_NAME
#define DEFAULT_COMMAND_NAME "story.rec"
#endif
#ifndef DEFAULT_AUXILARY_NAME
#define DEFAULT_AUXILARY_NAME "story.aux"
#endif
#ifndef DEFAULT_SAVE_DIR	/* DG */
#define DEFAULT_SAVE_DIR ".frotz-saves"
#endif

/*** Story file header format ***/

#define H_VERSION 0
#define H_CONFIG 1
#define H_RELEASE 2
#define H_RESIDENT_SIZE 4
#define H_START_PC 6
#define H_DICTIONARY 8
#define H_OBJECTS 10
#define H_GLOBALS 12
#define H_DYNAMIC_SIZE 14
#define H_FLAGS 16
#define H_SERIAL 18
#define H_ABBREVIATIONS 24
#define H_FILE_SIZE 26
#define H_CHECKSUM 28
#define H_INTERPRETER_NUMBER 30
#define H_INTERPRETER_VERSION 31
#define H_SCREEN_ROWS 32
#define H_SCREEN_COLS 33
#define H_SCREEN_WIDTH 34
#define H_SCREEN_HEIGHT 36
#define H_FONT_HEIGHT 38 /* this is the font width in V5 */
#define H_FONT_WIDTH 39 /* this is the font height in V5 */
#define H_FUNCTIONS_OFFSET 40
#define H_STRINGS_OFFSET 42
#define H_DEFAULT_BACKGROUND 44
#define H_DEFAULT_FOREGROUND 45
#define H_TERMINATING_KEYS 46
#define H_LINE_WIDTH 48
#define H_STANDARD_HIGH 50
#define H_STANDARD_LOW 51
#define H_ALPHABET 52
#define H_EXTENSION_TABLE 54
#define H_USER_NAME 56

#define HX_TABLE_SIZE 0
#define HX_MOUSE_X 1
#define HX_MOUSE_Y 2
#define HX_UNICODE_TABLE 3
#define HX_FLAGS 4
#define HX_FORE_COLOUR 5
#define HX_BACK_COLOUR 6

/*** Various Z-machine constants ***/

#define CONFIG_BYTE_SWAPPED 0x01 /* Story file is byte swapped         - V3  */
#define CONFIG_TIME         0x02 /* Status line displays time          - V3  */
#define CONFIG_TWODISKS     0x04 /* Story file occupied two disks      - V3  */
#define CONFIG_TANDY        0x08 /* Tandy licensed game                - V3  */
#define CONFIG_NOSTATUSLINE 0x10 /* Interpr can't support status lines - V3  */
#define CONFIG_SPLITSCREEN  0x20 /* Interpr supports split screen mode - V3  */
#define CONFIG_PROPORTIONAL 0x40 /* Interpr uses proportional font     - V3  */

#define CONFIG_COLOUR       0x01 /* Interpr supports colour            - V5+ */
#define CONFIG_PICTURES	    0x02 /* Interpr supports pictures	       - V6  */
#define CONFIG_BOLDFACE     0x04 /* Interpr supports boldface style    - V4+ */
#define CONFIG_EMPHASIS     0x08 /* Interpr supports emphasis style    - V4+ */
#define CONFIG_FIXED        0x10 /* Interpr supports fixed width style - V4+ */
#define CONFIG_SOUND        0x20 /* Interpr supports sound             - V6  */
#define CONFIG_TIMEDINPUT   0x80 /* Interpr supports timed input       - V4+ */

#define SCRIPTING_FLAG	  0x0001 /* Outputting to transscription file  - V1+ */
#define FIXED_FONT_FLAG   0x0002 /* Use fixed width font               - V3+ */
#define REFRESH_FLAG 	  0x0004 /* Refresh the screen                 - V6  */
#define GRAPHICS_FLAG	  0x0008 /* Game wants to use graphics         - V5+ */
#define OLD_SOUND_FLAG	  0x0010 /* Game wants to use sound effects    - V3  */
#define UNDO_FLAG	  0x0010 /* Game wants to use UNDO feature     - V5+ */
#define MOUSE_FLAG	  0x0020 /* Game wants to use a mouse          - V5+ */
#define COLOUR_FLAG	  0x0040 /* Game wants to use colours          - V5+ */
#define SOUND_FLAG	  0x0080 /* Game wants to use sound effects    - V5+ */
#define MENU_FLAG	  0x0100 /* Game wants to use menus            - V6  */

#define TRANSPARENT_FLAG  0x0001 /* Game wants to use transparency     - V6  */

#define INTERP_DEC_20 1
#define INTERP_APPLE_IIE 2
#define INTERP_MACINTOSH 3
#define INTERP_AMIGA 4
#define INTERP_ATARI_ST 5
#define INTERP_MSDOS 6
#define INTERP_CBM_128 7
#define INTERP_CBM_64 8
#define INTERP_APPLE_IIC 9
#define INTERP_APPLE_IIGS 10
#define INTERP_TANDY 11

#define BLACK_COLOUR 2
#define RED_COLOUR 3
#define GREEN_COLOUR 4
#define YELLOW_COLOUR 5
#define BLUE_COLOUR 6
#define MAGENTA_COLOUR 7
#define CYAN_COLOUR 8
#define WHITE_COLOUR 9
#define GREY_COLOUR 10		/* INTERP_MSDOS only */
#define LIGHTGREY_COLOUR 10 	/* INTERP_AMIGA only */
#define MEDIUMGREY_COLOUR 11 	/* INTERP_AMIGA only */
#define DARKGREY_COLOUR 12 	/* INTERP_AMIGA only */

enum {
	REVERSE_STYLE = 1,
	BOLDFACE_STYLE = 2,
	EMPHASIS_STYLE = 4,
	FIXED_WIDTH_STYLE = 8
};

/*** Constants for os_restart_game */

#define RESTART_BEGIN 0
#define RESTART_WPROP_SET 1
#define RESTART_END 2

/*** Constants for os_menu */

#define MENU_NEW 0
#define	MENU_ADD 1
#define MENU_REMOVE 2

/*** Character codes ***/

#define ZC_TIME_OUT 0x00
#define ZC_NEW_STYLE 0x01
#define ZC_NEW_FONT 0x02
#define ZC_BACKSPACE 0x08
#define ZC_INDENT 0x09
#define ZC_GAP 0x0b
#define ZC_RETURN 0x0d
#define ZC_HKEY_MIN 0x0e
#define ZC_HKEY_RECORD 0x0e
#define ZC_HKEY_PLAYBACK 0x0f
#define ZC_HKEY_SEED 0x10
#define ZC_HKEY_UNDO 0x11
#define ZC_HKEY_RESTART 0x12
#define ZC_HKEY_QUIT 0x13
#define ZC_HKEY_DEBUG 0x14
#define ZC_HKEY_HELP 0x15
#define ZC_HKEY_MAX 0x15
#define ZC_ESCAPE 0x1b
#define ZC_ASCII_MIN 0x20
#define ZC_ASCII_MAX 0x7e
#define ZC_BAD 0x7f
#define ZC_ARROW_MIN 0x81
#define ZC_ARROW_UP 0x81
#define ZC_ARROW_DOWN 0x82
#define ZC_ARROW_LEFT 0x83
#define ZC_ARROW_RIGHT 0x84
#define ZC_ARROW_MAX 0x84
#define ZC_FKEY_MIN 0x85
#define ZC_FKEY_MAX 0x90
#define ZC_NUMPAD_MIN 0x91
#define ZC_NUMPAD_MAX 0x9a
#define ZC_SINGLE_CLICK 0x9b
#define ZC_DOUBLE_CLICK 0x9c
#define ZC_MENU_CLICK 0x9d
#define ZC_LATIN1_MIN 0xa0
#define ZC_LATIN1_MAX 0xff

/*** File types ***/

#define FILE_RESTORE 0
#define FILE_SAVE 1
#define FILE_SCRIPT 2
#define FILE_PLAYBACK 3
#define FILE_RECORD 4
#define FILE_LOAD_AUX 5
#define FILE_SAVE_AUX 6

/*** Data access macros ***/

#define SET_BYTE(addr,v)  { zmp[addr] = v; }
#define LOW_BYTE(addr,v)  { v = zmp[addr]; }
#define CODE_BYTE(v)	  { v = *pcp++;    }

extern zbyte *pcp;
extern zbyte *zmp;

#define lo(v)	(v & 0xff)
#define hi(v)	(v >> 8)

#define SET_WORD(addr,v) { WRITE_BE_UINT16(&zmp[addr],v); }
#define LOW_WORD(addr,v) { v = READ_BE_UINT16(&zmp[addr]); }
#define HIGH_WORD(addr,v) { v = READ_BE_UINT16(&zmp[addr]); }
#define CODE_WORD(v) { v= READ_BE_UINT16(pcp); pcp += 2; }
#define GET_PC(v)         { v = pcp - zmp; }
#define SET_PC(v)         { pcp = zmp + v; }

/* Error codes */
#define ERR_TEXT_BUF_OVF 1	/* Text buffer overflow */
#define ERR_STORE_RANGE 2	/* Store out of dynamic memory */
#define ERR_DIV_ZERO 3		/* Division by zero */
#define ERR_ILL_OBJ 4		/* Illegal object */
#define ERR_ILL_ATTR 5		/* Illegal attribute */
#define ERR_NO_PROP 6		/* No such property */
#define ERR_STK_OVF 7		/* Stack overflow */
#define ERR_ILL_CALL_ADDR 8	/* Call to illegal address */
#define ERR_CALL_NON_RTN 9	/* Call to non-routine */
#define ERR_STK_UNDF 10		/* Stack underflow */
#define ERR_ILL_OPCODE 11	/* Illegal opcode */
#define ERR_BAD_FRAME 12	/* Bad stack frame */
#define ERR_ILL_JUMP_ADDR 13	/* Jump to illegal address */
#define ERR_SAVE_IN_INTER 14	/* Can't save while in interrupt */
#define ERR_STR3_NESTING 15	/* Nesting stream #3 too deep */
#define ERR_ILL_WIN 16		/* Illegal window */
#define ERR_ILL_WIN_PROP 17	/* Illegal window property */
#define ERR_ILL_PRINT_ADDR 18	/* Print at illegal address */
#define ERR_DICT_LEN 19	/* Illegal dictionary word length */
#define ERR_MAX_FATAL 19

/* Less serious errors */
#define ERR_JIN_0 20		/* @jin called with object 0 */
#define ERR_GET_CHILD_0 21	/* @get_child called with object 0 */
#define ERR_GET_PARENT_0 22	/* @get_parent called with object 0 */
#define ERR_GET_SIBLING_0 23	/* @get_sibling called with object 0 */
#define ERR_GET_PROP_ADDR_0 24	/* @get_prop_addr called with object 0 */
#define ERR_GET_PROP_0 25	/* @get_prop called with object 0 */
#define ERR_PUT_PROP_0 26	/* @put_prop called with object 0 */
#define ERR_CLEAR_ATTR_0 27	/* @clear_attr called with object 0 */
#define ERR_SET_ATTR_0 28	/* @set_attr called with object 0 */
#define ERR_TEST_ATTR_0 29	/* @test_attr called with object 0 */
#define ERR_MOVE_OBJECT_0 30	/* @move_object called moving object 0 */
#define ERR_MOVE_OBJECT_TO_0 31	/* @move_object called moving into object 0 */
#define ERR_REMOVE_OBJECT_0 32	/* @remove_object called with object 0 */
#define ERR_GET_NEXT_PROP_0 33	/* @get_next_prop called with object 0 */
#define ERR_NUM_ERRORS (33)
 
/* There are four error reporting modes: never report errors;
  report only the first time a given error type occurs; report
  every time an error occurs; or treat all errors as fatal
  errors, killing the interpreter. I strongly recommend
  "report once" as the default. But you can compile in a
  different default by changing the definition of
  ERR_DEFAULT_REPORT_MODE. In any case, the player can
  specify a report mode on the command line by typing "-Z 0"
  through "-Z 3". */

#define ERR_REPORT_NEVER (0)
#define ERR_REPORT_ONCE (1)
#define ERR_REPORT_ALWAYS (2)
#define ERR_REPORT_FATAL (3)

#define ERR_DEFAULT_REPORT_MODE ERR_REPORT_ONCE

struct undo_t {
    undo_t *next;
    undo_t *prev;
    long pc;
    long diff_size;
    zword frame_count;
    zword stack_size;
    zword frame_offset;
    /* undo diff and stack data follow */
};

#define EFFECT_PREPARE 1
#define EFFECT_PLAY 2
#define EFFECT_STOP 3
#define EFFECT_FINISH_WITH 4

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

} // end of namespace Frotz

#endif
