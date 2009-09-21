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

#include "gargoyle/frotz.h"
#include "gargoyle/frotz_interp.h"
#include "gargoyle/frotz_funcs.h"
#include "gargoyle/frotz_static.h"
#include "gargoyle/gargoyle.h"
#include "gargoyle/frotz_static.h"

#include "common/EventRecorder.h"
#include "common/events.h"
#include "common/file.h"
#include "graphics/cursorman.h"
#include "graphics/font.h"
#include "graphics/fontman.h"
#include "engines/engine.h"

namespace Frotz {

typedef void (FrotzInterpreter::*ZCodeFn)(void);

//void (*op0_opcodes[0x10])(void) = {
static ZCodeFn op0_opcodes[0x10] = {
	&FrotzInterpreter::z_rtrue,
	&FrotzInterpreter::z_rfalse,
	&FrotzInterpreter::z_print,
	&FrotzInterpreter::z_print_ret,
	&FrotzInterpreter::z_nop,
	&FrotzInterpreter::z_save,
	&FrotzInterpreter::z_restore,
	&FrotzInterpreter::z_restart,
	&FrotzInterpreter::z_ret_popped,
	&FrotzInterpreter::z_catch,
	&FrotzInterpreter::z_quit,
	&FrotzInterpreter::z_new_line,
	&FrotzInterpreter::z_show_status,
	&FrotzInterpreter::z_verify,
	&FrotzInterpreter::__extended__,
	&FrotzInterpreter::z_piracy
};

static ZCodeFn op1_opcodes[0x10] = {
	&FrotzInterpreter::z_jz,
	&FrotzInterpreter::z_get_sibling,
	&FrotzInterpreter::z_get_child,
	&FrotzInterpreter::z_get_parent,
	&FrotzInterpreter::z_get_prop_len,
	&FrotzInterpreter::z_inc,
	&FrotzInterpreter::z_dec,
	&FrotzInterpreter::z_print_addr,
	&FrotzInterpreter::z_call_s,
	&FrotzInterpreter::z_remove_obj,
	&FrotzInterpreter::z_print_obj,
	&FrotzInterpreter::z_ret,
	&FrotzInterpreter::z_jump,
	&FrotzInterpreter::z_print_paddr,
	&FrotzInterpreter::z_load,
	&FrotzInterpreter::z_call_n
};

static ZCodeFn var_opcodes[0x40] = {
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::z_je,
	&FrotzInterpreter::z_jl,
	&FrotzInterpreter::z_jg,
	&FrotzInterpreter::z_dec_chk,
	&FrotzInterpreter::z_inc_chk,
	&FrotzInterpreter::z_jin,
	&FrotzInterpreter::z_test,
	&FrotzInterpreter::z_or,
	&FrotzInterpreter::z_and,
	&FrotzInterpreter::z_test_attr,
	&FrotzInterpreter::z_set_attr,
	&FrotzInterpreter::z_clear_attr,
	&FrotzInterpreter::z_store,
	&FrotzInterpreter::z_insert_obj,
	&FrotzInterpreter::z_loadw,
	&FrotzInterpreter::z_loadb,
	&FrotzInterpreter::z_get_prop,
	&FrotzInterpreter::z_get_prop_addr,
	&FrotzInterpreter::z_get_next_prop,
	&FrotzInterpreter::z_add,
	&FrotzInterpreter::z_sub,
	&FrotzInterpreter::z_mul,
	&FrotzInterpreter::z_div,
	&FrotzInterpreter::z_mod,
	&FrotzInterpreter::z_call_s,
	&FrotzInterpreter::z_call_n,
	&FrotzInterpreter::z_set_colour,
	&FrotzInterpreter::z_throw,
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::z_call_s,
	&FrotzInterpreter::z_storew,
	&FrotzInterpreter::z_storeb,
	&FrotzInterpreter::z_put_prop,
	&FrotzInterpreter::z_read,
	&FrotzInterpreter::z_print_char,
	&FrotzInterpreter::z_print_num,
	&FrotzInterpreter::z_random,
	&FrotzInterpreter::z_push,
	&FrotzInterpreter::z_pull,
	&FrotzInterpreter::z_split_window,
	&FrotzInterpreter::z_set_window,
	&FrotzInterpreter::z_call_s,
	&FrotzInterpreter::z_erase_window,
	&FrotzInterpreter::z_erase_line,
	&FrotzInterpreter::z_set_cursor,
	&FrotzInterpreter::z_get_cursor,
	&FrotzInterpreter::z_set_text_style,
	&FrotzInterpreter::z_buffer_mode,
	&FrotzInterpreter::z_output_stream,
	&FrotzInterpreter::z_input_stream,
	&FrotzInterpreter::z_sound_effect,
	&FrotzInterpreter::z_read_char,
	&FrotzInterpreter::z_scan_table,
	&FrotzInterpreter::z_not,
	&FrotzInterpreter::z_call_n,
	&FrotzInterpreter::z_call_n,
	&FrotzInterpreter::z_tokenise,
	&FrotzInterpreter::z_encode_text,
	&FrotzInterpreter::z_copy_table,
	&FrotzInterpreter::z_print_table,
	&FrotzInterpreter::z_check_arg_count
};

static ZCodeFn ext_opcodes[0x1e] = {
	&FrotzInterpreter::z_save,
	&FrotzInterpreter::z_restore,
	&FrotzInterpreter::z_log_shift,
	&FrotzInterpreter::z_art_shift,
	&FrotzInterpreter::z_set_font,
	&FrotzInterpreter::z_draw_picture,
	&FrotzInterpreter::z_picture_data,
	&FrotzInterpreter::z_erase_picture,
	&FrotzInterpreter::z_set_margins,
	&FrotzInterpreter::z_save_undo,
	&FrotzInterpreter::z_restore_undo,
	&FrotzInterpreter::z_print_unicode,
	&FrotzInterpreter::z_check_unicode,
	&FrotzInterpreter::z_set_true_colour,	/* spec 1.1 */
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::__illegal__,
	&FrotzInterpreter::z_move_window,
	&FrotzInterpreter::z_window_size,
	&FrotzInterpreter::z_window_style,
	&FrotzInterpreter::z_get_wind_prop,
	&FrotzInterpreter::z_scroll_window,
	&FrotzInterpreter::z_pop_stack,
	&FrotzInterpreter::z_read_mouse,
	&FrotzInterpreter::z_mouse_window,
	&FrotzInterpreter::z_push_stack,
	&FrotzInterpreter::z_put_wind_prop,
	&FrotzInterpreter::z_print_form,
	&FrotzInterpreter::z_make_menu,
	&FrotzInterpreter::z_picture_table,
	&FrotzInterpreter::z_buffer_screen	/* spec 1.1 */
};

/**
 * Support function that encapsulates a check for EOF as well as a readByte()
 */
int inline fgetc(Common::InSaveFile *f) {
	if (f->eos())
		return EOF;
	return f->readByte();
}

FrotzInterpreter::FrotzInterpreter(Gargoyle::GargoyleEngine &engine): 
		Interpreter(engine), _os(_h, *this) {
	g_eventRec.registerRandomSource(_rnd, "frotzInterpreter");

	initInterpVars();

	initDebugger();

	// Handle loading up a stream with the game contents

	Common::File f;
	_gameFile = NULL;
	if (!hasBlorbFile() || !isBlorbGame()) {
		// Load the Z-code directly from the game's filename
		f.open(_engine.getFilename());
		_gameFile = f.readStream(f.size());

	} else {
		// Game file is in the Blorb file, so find it

		// Look for an executable chunk
		Gargoyle::bb_result_t result;
		if (getBlorbFile()->loadResource(bb_method_FilePos, &result, 
				bb_ID_Exec, 0) == Gargoyle::bb_err_None) {
			
			unsigned int id = getBlorbFile()->chunks[result.chunknum]->type;
			if (id == bb_make_id('Z','C','O','D')) {
				// Found game file, so get it separately
				getBlorbFile()->stream()->seek(result.data.startpos);
				_gameFile = getBlorbFile()->stream()->readStream(result.length);
			} 
		}
	}

	if (!_gameFile)
		error("Could not locate game file data");

	// Call initialisation methods
	init_buffer();
	init_err();
	init_files();
	init_fonts();
	init_memory();
	init_palette();
	init_process();
	init_screen();
	init_sound();
	init_text();
	_os.init_screen();
	init_undo();
	z_restart();
}

FrotzInterpreter::~FrotzInterpreter() {
	if (_gameFile)
		delete _gameFile;
	if (sfp)
		delete sfp;
	if (_gfxFont)
		delete _gfxFont;
}

void FrotzInterpreter::initInterpVars() {
	redirect_depth = -1;
	random_interval = 0;
	random_counter = 0;
	ostream_screen = true;
	ostream_script = false;
	ostream_memory = false;
	ostream_record = false;
	istream_replay = false;
	message = false;

	option_attribute_assignment = 0;
	option_attribute_testing = 0;
	option_context_lines = 0;
	option_object_locating = 0;
	option_object_movement = 0;
	option_left_margin = 4;
	option_right_margin = 4;
	option_ignore_errors = 0;
	option_piracy = 0;
	option_undo_slots = MAX_UNDO_SLOTS;
	option_expand_abbreviations = 0;
	option_script_cols = 80;
	option_save_quetzal = 1;
	option_sound = 1;

	sfp = NULL;

}

/**
 * Setup the custom font needed 
 */
void FrotzInterpreter::init_fonts() {
	_gfxFont = new Graphics::NewFont(infocomGfxFont);
	FontMan.assignFontToName(S_GFX_CHAR_FONT, _gfxFont);
}

void FrotzInterpreter::init_palette() {
	if (_h.h_version == V6) {
		uint32 fgColour = DEFAULT_COLOUR_LIST[1];
		uint32 bgColour = DEFAULT_COLOUR_LIST[11];

		DEFAULT_COLOUR_LIST[14] = fgColour;
		DEFAULT_COLOUR_LIST[15] = bgColour;
	}

	// Set the initial palette
	g_system->setPalette((const byte *)&DEFAULT_COLOUR_LIST[0], 0, 16);

	// Clear the remainder of the palette
	uint32 emptyPalette[240];
	Common::set_to(&emptyPalette[0], &emptyPalette[240], 0);
	g_system->setPalette((const byte *)&emptyPalette[0], 16, 240);
}

void FrotzInterpreter::go() {
	interpret();
	reset_memory();

	if (!_quitFlag)
		_os.reset_screen();
}

/**
 * Load and execute an extended opcode.
 */

/* --------------------------------------------------------------------------
 * Z code stub functions
 *
 * --------------------------------------------------------------------------
 */

void FrotzInterpreter::__extended__(void) {
	zbyte opcode;
	zbyte specifier;

	CODE_BYTE(opcode)
	CODE_BYTE(specifier)

	load_all_operands(specifier);

	/* extended opcodes from 0x1e on are reserved for future spec' */
	if (opcode < 0x1e)
		CALL_MEMBER_FN(*this, ext_opcodes[opcode])();

}

/**
 * Exit game because an unknown opcode has been hit.
 */
void FrotzInterpreter::__illegal__(void) {
	runtime_error(ERR_ILL_OPCODE);
}

/* --------------------------------------------------------------------------
 * Buffer Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Initialize buffer variables.
 */
void FrotzInterpreter::init_buffer(void) {
	memset(main_buffer, 0, TEXT_BUFFER_SIZE);
	bufpos = 0;
	bufprev_c = 0;
	buffer_locked = false;
}

/**
 * Copy the contents of the text buffer to the output streams.
 */
void FrotzInterpreter::flush_buffer(void) {
	/* Make sure we stop when flush_buffer is called from flush_buffer.
	   Note that this is difficult to avoid as we might print a newline
	   during flush_buffer, which might cause a newline interrupt, that
	   might execute any arbitrary opcode, which might flush the buffer. */

	if (buffer_locked || (bufpos == 0))
		return;

	/* Send the buffer to the output streams */

	main_buffer[bufpos] = 0;
	buffer_locked = true; 
	stream_word(main_buffer); 
	buffer_locked = false;

	/* Reset the buffer */

	bufpos = 0;
	bufprev_c = 0;
}/* flush_buffer */

/**
 * High level newline function.
 */
void FrotzInterpreter::new_line(void) {
	flush_buffer(); 
	stream_new_line();

}/* new_line */

/**
 * High level output function.
 */
void FrotzInterpreter::print_char(zword c) {
	static bool flag = false;

	if (message || ostream_memory || enable_buffering) {
		if (!flag) {
			/* Characters 0 and ZC_RETURN are special cases */

			if (c == ZC_RETURN) { 
				new_line(); 
				return; 
			}
		
			if (c == 0)
				return;

			/* Flush the buffer before a whitespace or after a hyphen */

			if (c == ' ' || c == ZC_INDENT || c == ZC_GAP || (bufprev_c == '-' && c != '-'))
				flush_buffer();

			/* Set the flag if this is part one of a style or font change */

			if (c == ZC_NEW_FONT || c == ZC_NEW_STYLE)
				flag = TRUE;

			/* Remember the current character code */

			bufprev_c = c;

		} else 
			flag = FALSE;

		/* Insert the character into the buffer */

		main_buffer[bufpos++] = c;

		if (bufpos == TEXT_BUFFER_SIZE)
			runtime_error(ERR_TEXT_BUF_OVF);

	} else 
		stream_char(c);

}

/* --------------------------------------------------------------------------
 * Err Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Initialise error reporting.
 */
void FrotzInterpreter::init_err(void) {
	int i;

	/* Initialize the counters. */
	
	for (i = 0; i < ERR_NUM_ERRORS; i++)
		error_count[i] = 0;
}

/**
 * An error has occurred. Ignore it, pass it to _os.fatal or report
 * it according to err_report_mode.
 *
 * errnum : Numeric code for error (1 to ERR_NUM_ERRORS)
 */
void FrotzInterpreter::runtime_error(int errnum) {
	int wasfirst;
	
	if (errnum <= 0 || errnum > ERR_NUM_ERRORS)
		return;

	if (err_report_mode == ERR_REPORT_FATAL || (!option_ignore_errors && errnum <= ERR_MAX_FATAL)) {
		flush_buffer();
		_os.fatal(err_messages[errnum - 1]);
		return;
	}

	wasfirst = (error_count[errnum - 1] == 0);
	error_count[errnum - 1]++;
	
	if ((err_report_mode == ERR_REPORT_ALWAYS) || (err_report_mode == ERR_REPORT_ONCE && wasfirst)) {
		long pc;

		GET_PC(pc);
		print_string("Warning: ");
		print_string(err_messages[errnum - 1]);
		print_string(" (PC = ");
		print_long(pc, 16);
		print_char(')');
		
		if (err_report_mode == ERR_REPORT_ONCE) {
			print_string(" (will ignore further occurrences)");
		} else {
			print_string(" (occurence ");
			print_long(error_count[errnum - 1], 10);
			print_char(')');
		}

		new_line();
	}
}

/**
 * Print an unsigned 32bit number in decimal or hex.
 */
void FrotzInterpreter::print_long(unsigned long value, int base) {
	unsigned long i;
	char c;

	for (i = (base == 10 ? 1000000000 : 0x10000000); i != 0; i /= base) {
		if (value >= i || i == 1) {
			c = (value / i) % base;
			print_char (c + (c <= 9 ? '0' : 'a' - 10));
		}
	}
}/* print_long */

/* --------------------------------------------------------------------------
 * Fastmem Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Read a value from the header extension (former mouse table).
 */
zword FrotzInterpreter::get_header_extension(int entry) {
	zword addr;
	zword val;

	if (_h.h_extension_table == 0 || entry > _h.hx_table_size)
		return 0;

	addr = _h.h_extension_table + 2 * entry;
	LOW_WORD(addr, val)

	return val;
}/* get_header_extension */

/**
 * Set an entry in the header extension (former mouse table).
 */
void FrotzInterpreter::set_header_extension(int entry, zword val) {
	zword addr;

	if (_h.h_extension_table == 0 || entry > _h.hx_table_size)
		return;

	addr = _h.h_extension_table + 2 * entry;
	SET_WORD(addr, val)

}/* set_header_extension */

/**
 * Set all header fields which hold information about the interpreter.
 */
void FrotzInterpreter::restart_header(void) {
	zword screen_x_size;
	zword screen_y_size;
	zbyte font_x_size;
	zbyte font_y_size;

	int i;

	SET_BYTE(H_CONFIG, _h.h_config)
	SET_WORD(H_FLAGS, _h.h_flags)

	if (_h.h_version >= V4) {
		SET_BYTE(H_INTERPRETER_NUMBER, _h.h_interpreter_number)
		SET_BYTE(H_INTERPRETER_VERSION, _h.h_interpreter_version)
		SET_BYTE(H_SCREEN_ROWS, _h.h_screen_rows)
		SET_BYTE(H_SCREEN_COLS, _h.h_screen_cols)
	}

	/* It's less trouble to use font size 1x1 for V5 games, especially
	   because of a bug in the unreleased German version of "Zork 1" */

	if (_h.h_version != V6) {
		screen_x_size = (zword)_h.h_screen_cols;
		screen_y_size = (zword)_h.h_screen_rows;
		font_x_size = 1;
		font_y_size = 1;
	} else {
		screen_x_size = _h.h_screen_width;
		screen_y_size = _h.h_screen_height;
		font_x_size = _h.h_font_width;
		font_y_size = _h.h_font_height;
	}

	if (_h.h_version >= V5) {
		SET_WORD(H_SCREEN_WIDTH, screen_x_size)
		SET_WORD(H_SCREEN_HEIGHT, screen_y_size)
		SET_BYTE(H_FONT_HEIGHT, font_y_size)
		SET_BYTE(H_FONT_WIDTH, font_x_size)
		SET_BYTE(H_DEFAULT_BACKGROUND, _h.h_default_background)
		SET_BYTE(H_DEFAULT_FOREGROUND, _h.h_default_foreground)
	}

	if ((_h.h_version >= V3) && (_h.h_user_name[0] != 0))
		for (i = 0; i < 8; i++)
			storeb((zword) (H_USER_NAME + i), _h.h_user_name[i]);

	SET_BYTE(H_STANDARD_HIGH, _h.h_standard_high)
	SET_BYTE(H_STANDARD_LOW, _h.h_standard_low)

	set_header_extension(HX_FLAGS, _h.hx_flags);
	set_header_extension(HX_FORE_COLOUR, _h.hx_fore_colour);
	set_header_extension(HX_BACK_COLOUR, _h.hx_back_colour);
}/* restart_header */

/**
 * Allocate memory and load the story file.
 */
void FrotzInterpreter::init_memory(void) {
	strcpy(_h.save_name, DEFAULT_SAVE_NAME);
	strcpy(_h.auxilary_name, DEFAULT_AUXILARY_NAME);
	zmp = NULL;
	pcp = NULL;
	first_restart = TRUE;
	init_fp_pos = 0;
	first_undo = last_undo = curr_undo = NULL;
	undo_mem = prev_zmp = undo_diff = NULL;
	undo_count = 0;

	long size;
	zword addr;
	unsigned n;
	int i, j;

	/* Open story file */

	_gameFile->seek(0);
	_h.story_size = _gameFile->size();
	init_fp_pos = _gameFile->pos();

	/* Allocate memory for story header */

	if ((zmp = (zbyte *) malloc (64)) == NULL)
		_os.fatal("Out of memory");

	/* Load header into memory */

	if (_gameFile->read(zmp, 64) != 64)
		_os.fatal("Story file read error");

	/* Copy header fields to global variables */

	LOW_BYTE(H_VERSION, _h.h_version)

	if (_h.h_version < V1 || _h.h_version > V8)
		_os.fatal("Unknown Z-code version");

	LOW_BYTE(H_CONFIG, _h.h_config)

	if (_h.h_version == V3 && (_h.h_config & CONFIG_BYTE_SWAPPED))
		_os.fatal("Byte swapped story file");

	LOW_WORD(H_RELEASE, _h.h_release)
	LOW_WORD(H_RESIDENT_SIZE, _h.h_resident_size)
	LOW_WORD(H_START_PC, _h.h_start_pc)
	LOW_WORD(H_DICTIONARY, _h.h_dictionary)
	LOW_WORD(H_OBJECTS, _h.h_objects)
	LOW_WORD(H_GLOBALS, _h.h_globals)
	LOW_WORD(H_DYNAMIC_SIZE, _h.h_dynamic_size)
	LOW_WORD(H_FLAGS, _h.h_flags)

	for (i = 0, addr = H_SERIAL; i < 6; i++, addr++)
		LOW_BYTE(addr, _h.h_serial[i])

	/* Auto-detect buggy story files that need special fixes */

	_h.story_id = UNKNOWN;

	for (i = 0; game_records[i].story_id != UNKNOWN; i++) {
		if (_h.h_release == game_records[i].release) {
			for (j = 0; j < 6; j++)
				if (_h.h_serial[j] != game_records[i].serial[j])
					goto no_match;

			_h.story_id = game_records[i].story_id;
		}

		no_match: ; /* null statement */
	}

	LOW_WORD(H_ABBREVIATIONS, _h.h_abbreviations)
	LOW_WORD(H_FILE_SIZE, _h.h_file_size)

	/* Calculate story file size in bytes */

	if (_h.h_file_size != 0) {
		_h.story_size = (long) 2 * _h.h_file_size;

		if (_h.h_version >= V4)
			_h.story_size *= 2;
		if (_h.h_version >= V6)
			_h.story_size *= 2;

		if (_h.story_id == AMFV && _h.h_release == 47)
			_h.story_size = (long) 2 * _h.h_file_size;

	} else if (_h.story_size > 0) {	/* _os.path_open() set the size */

	} else {		/* some old games lack the file size entry */
		_gameFile->seek(0, SEEK_END);
		_h.story_size = _gameFile->pos() - init_fp_pos;
		_gameFile->seek(init_fp_pos + 64, SEEK_SET);
	}

	LOW_WORD(H_CHECKSUM, _h.h_checksum)
	LOW_WORD(H_ALPHABET, _h.h_alphabet)
	LOW_WORD(H_FUNCTIONS_OFFSET, _h.h_functions_offset)
	LOW_WORD(H_STRINGS_OFFSET, _h.h_strings_offset)
	LOW_WORD(H_TERMINATING_KEYS, _h.h_terminating_keys)
	LOW_WORD(H_EXTENSION_TABLE, _h.h_extension_table)

	/* Zork Zero beta and Macintosh versions don't have the graphics flag set */

	if (_h.story_id == ZORK_ZERO) {
		if (_h.h_release ==  96 || _h.h_release == 153 || _h.h_release == 242 || _h.h_release == 296) {
			_h.h_flags |= GRAPHICS_FLAG;
		}
	}

	/* Adjust opcode tables */

	if (_h.h_version <= V4) {
		op0_opcodes[0x09] = &FrotzInterpreter::z_pop;
		op1_opcodes[0x0f] = &FrotzInterpreter::z_not;
	} else {
		op0_opcodes[0x09] = &FrotzInterpreter::z_catch;
		op1_opcodes[0x0f] = &FrotzInterpreter::z_call_n;
	}

	/* Allocate memory for story data */

	if ((zmp = (zbyte *) realloc (zmp, _h.story_size)) == NULL)
		_os.fatal("Out of memory");

	/* Load story file in chunks of 32KB */

	n = 0x8000;

	for (size = 64; size < _h.story_size; size += n) {
		if (_h.story_size - size < 0x8000)
			n = (unsigned) (_h.story_size - size);

		SET_PC(size)

		if (_gameFile->read(pcp, n) != n) 
			_os.fatal("Story file read error");
	}

	first_restart = TRUE;

	/* Read header extension table */

	_h.hx_table_size = get_header_extension(HX_TABLE_SIZE);
	_h.hx_unicode_table = get_header_extension(HX_UNICODE_TABLE);
	_h.hx_flags = get_header_extension(HX_FLAGS);
}

/**
 * Allocate memory for multiple undo. It is important not to occupy
 * all the memory available, since the IO interface may need memory
 * during the game, e.g. for loading sounds or pictures.
 */
void FrotzInterpreter::init_undo(void) {
	void *reserved = NULL;	/* makes compilers shut up */
	reserve_mem = 0;

	if (reserve_mem != 0) {
		if ((reserved = malloc (reserve_mem)) == NULL)
			return;
	}

	/* Allocate _h.h_dynamic_size bytes for previous dynamic zmp state
	   + 1.5 _h.h_dynamic_size for Quetzal diff + 2. */
	undo_mem = (zbyte *)malloc((_h.h_dynamic_size * 5) / 2 + 2);
	if (undo_mem != NULL) {
		prev_zmp = undo_mem;
		undo_diff = undo_mem + _h.h_dynamic_size;
		memcpy(prev_zmp, zmp, _h.h_dynamic_size);
	} else
		option_undo_slots = 0;

	if (reserve_mem != 0)
		free (reserved);
}

/**
 * Free count undo blocks from the beginning of the undo list.
 */
void FrotzInterpreter::free_undo(int count) {
	undo_t *p;

	if (count > undo_count)
		count = undo_count;
	while (count--) {
		p = first_undo;
		if (curr_undo == first_undo)
			curr_undo = curr_undo->next;
		first_undo = first_undo->next;
		free (p);
		undo_count--;
	}

	if (first_undo)
		first_undo->prev = NULL;
	else
		last_undo = NULL;
}

/**
 * Close the story file and deallocate memory.
 */
void FrotzInterpreter::reset_memory(void) {
	if (undo_mem) {
		free_undo(undo_count);
		free(undo_mem);
	}
	undo_mem = NULL;
	undo_count = 0;

	if (zmp)
		free(zmp);
	zmp = NULL;

	if (decoded)
		free(decoded);
	decoded = NULL;

	if (encoded)
		free(encoded);
	encoded = NULL;
}/* reset_memory */

/**
 * Write a byte value to the dynamic Z-machine memory.
 */
void FrotzInterpreter::storeb(zword addr, zbyte value) {
	if (addr >= _h.h_dynamic_size)
		runtime_error(ERR_STORE_RANGE);

	if (addr == H_FLAGS + 1) {	/* flags register is modified */
		_h.h_flags &= ~(SCRIPTING_FLAG | FIXED_FONT_FLAG);
		_h.h_flags |= value & (SCRIPTING_FLAG | FIXED_FONT_FLAG);

		if (value & SCRIPTING_FLAG) {
			if (!ostream_script)
				script_open();
		} else {
			if (ostream_script)
				script_close();
		}

		refresh_text_style();
	}

	SET_BYTE(addr, value)
}

/**
 * Write a word value to the dynamic Z-machine memory.
 */
void FrotzInterpreter::storew(zword addr, zword value) {
	storeb((zword) (addr + 0), hi (value));
	storeb((zword) (addr + 1), lo (value));
}

/**
 * Read a default file name from the memory of the Z-machine and
 * copy it to a string.
 */
void FrotzInterpreter::get_default_name(char *default_name, zword addr) {
	if (addr != 0) {
		zbyte len;
		int i;

		LOW_BYTE(addr, len)
		addr++;

		for (i = 0; i < len; i++) {
			zbyte c;

			LOW_BYTE(addr, c)
			addr++;

			if (c >= 'A' && c <= 'Z')
				c += 'a' - 'A';

			default_name[i] = c;
		}

		default_name[i] = 0;

		if (strchr(default_name, '.') == NULL)
			strcpy(default_name + i, ".AUX");

	} else 
		strcpy (default_name, _h.auxilary_name);
}


/**
 * This function does the dirty work for z_restore_undo.
 */
int FrotzInterpreter::restore_undo(void) {
	if (option_undo_slots == 0)	/* undo feature unavailable */
		return -1;

	if (curr_undo == NULL)	/* no saved game state */
		return 0;

	/* undo possible */
	memcpy(zmp, prev_zmp, _h.h_dynamic_size);
	SET_PC(curr_undo->pc)
	sp = stack + STACK_SIZE - curr_undo->stack_size;
	fp = stack + curr_undo->frame_offset;
	frame_count = curr_undo->frame_count;
	mem_undiff((zbyte *) (curr_undo + 1), curr_undo->diff_size, prev_zmp);
	memcpy(sp, (zbyte *)(curr_undo + 1) + curr_undo->diff_size,
		curr_undo->stack_size * sizeof (*sp));

	curr_undo = curr_undo->prev;
	restart_header();

	return 2;
}

/**
 * This function does the dirty work for z_save_undo.
 */
int FrotzInterpreter::save_undo(void) {
	long diff_size;
	zword stack_size;
	undo_t *p;

	if (option_undo_slots == 0)		/* undo feature unavailable */
		return -1;

	/* save undo possible */
	while (last_undo != curr_undo) {
		p = last_undo;
		last_undo = last_undo->prev;
		free(p);
		undo_count--;
	}
	if (last_undo)
		last_undo->next = NULL;
	else
		first_undo = NULL;

	if (undo_count == option_undo_slots)
		free_undo (1);

	diff_size = mem_diff(zmp, prev_zmp, _h.h_dynamic_size, undo_diff);
	stack_size = stack + STACK_SIZE - sp;
	do {
		p = (undo_t *)malloc (sizeof (undo_t) + diff_size + stack_size * sizeof (*sp));
		if (p == NULL)
			free_undo (1);
	} while (!p && undo_count);
	if (p == NULL)
		return -1;

	GET_PC(p->pc)
	p->frame_count = frame_count;
	p->diff_size = diff_size;
	p->stack_size = stack_size;
	p->frame_offset = fp - stack;
	memcpy(p + 1, undo_diff, diff_size);
	memcpy((zbyte *)(p + 1) + diff_size, sp, stack_size * sizeof (*sp));

	if (!first_undo) {
		p->prev = NULL;
		first_undo = p;
	} else {
		last_undo->next = p;
		p->prev = last_undo;
	}
	p->next = NULL;
	curr_undo = last_undo = p;
	undo_count++;
	return 1;
}

/* --------------------------------------------------------------------------
 * Input Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Check if the given key is an input terminator.
 */
bool FrotzInterpreter::is_terminator(zword key) {
	if (key == ZC_TIME_OUT)
		return TRUE;
	if (key == ZC_RETURN)
		return TRUE;
	if (key >= ZC_HKEY_MIN && key <= ZC_HKEY_MAX)
		return TRUE;

	if (_h.h_terminating_keys != 0) {
		if (key >= ZC_ARROW_MIN && key <= ZC_MENU_CLICK) {
			zword addr = _h.h_terminating_keys;
			zbyte c;

			do {
				LOW_BYTE(addr, c)
				if (c == 255 || key == translate_from_zscii (c))
					return TRUE;
				addr++;
			} while (c != 0);
		}
	}

	return FALSE;

}

/**
 * Ask the user a question; return true if the answer is yes.
 */
bool FrotzInterpreter::read_yes_or_no(const char *s) {
	zword key;

	print_string(s);
	print_string("? (y/n) >");

	key = stream_read_key(0, 0, FALSE);

	if (key == 'y' || key == 'Y') {
		print_string ("y\n");
		return TRUE;
	} else {
		print_string ("n\n");
		return FALSE;
	}
}/* read_yes_or_no */

/**
 * Read a string from the current input stream.
 */
void FrotzInterpreter::read_string(int max, zchar *buffer) {
	zword key;
	buffer[0] = 0;

	do {
		key = stream_read_input(max, buffer, 0, 0, FALSE, FALSE);
	} while (key != ZC_RETURN);
}

/**
 * Ask the user to type in a number and return it.
 */
int FrotzInterpreter::read_number(void) {
	zchar buffer[6];
	int value = 0;
	int i;

	read_string(5, buffer);

	for (i = 0; buffer[i] != 0; i++)
		if (buffer[i] >= '0' && buffer[i] <= '9')
			value = 10 * value + buffer[i] - '0';

	return value;
}

/* --------------------------------------------------------------------------
 * Files Methods
 *
 * --------------------------------------------------------------------------
 */

void FrotzInterpreter::init_files() {
	script_width = 0;
	strcpy(_h.script_name, DEFAULT_SCRIPT_NAME);
	strcpy(_h.command_name, DEFAULT_COMMAND_NAME);
}

/**
 * Open the transscript file. 'AMFV' makes this more complicated as it
 * turns transscription on/off several times to exclude some text from
 * the transscription file. This wasn't a problem for the original V4
 * interpreters which always sent transscription to the printer, but it
 * means a problem to modern interpreters that offer to open a new file
 * every time transscription is turned on. Our solution is to append to
 * the old transscription file in V1 to V4, and to ask for a new file
 * name in V5+.
 */
void FrotzInterpreter::script_open(void) {
	static bool script_valid = FALSE;
	char new_name[MAX_FILE_NAME + 1];

	_h.h_flags &= ~SCRIPTING_FLAG;

	if (_h.h_version >= V5 || !script_valid) {
		if (!_os.read_file_name(new_name, _h.script_name, FILE_SCRIPT))
			goto done;

		strcpy(_h.script_name, new_name);
	}

	// If an existing script is open, then close it
	if (sfp)
		delete sfp;

	// FIXME: I'm not aware of any way to append to a file in ScummVM, so currently I
	// read in the entire file contents and then write them back out again
	Common::InSaveFile *scriptFile;
	scriptFile = _saveFileMan->openForLoading(_h.script_name);
	if (scriptFile != NULL) {
		// Get in the existing content length, and write it out to the script file
		uint32 size = scriptFile->size();
		byte *buf = (byte *)malloc(size);
		scriptFile->read(buf, size);
		delete scriptFile;

		sfp = _saveFileMan->openForSaving(_h.script_name);
		sfp->write(buf, size);
		delete buf;
	} else {
		// Create a new script file for saving
		sfp = _saveFileMan->openForSaving(_h.script_name);
	}

	_h.h_flags |= SCRIPTING_FLAG;

	script_valid = TRUE;
	ostream_script = TRUE;

	script_width = 0;

done:
	SET_WORD(H_FLAGS, _h.h_flags)
}

/**
 * Stop transscription.
 */
void FrotzInterpreter::script_close(void) {
	_h.h_flags &= ~SCRIPTING_FLAG;
	SET_WORD(H_FLAGS, _h.h_flags)

	delete sfp;
	sfp = NULL;
	ostream_script = false;
}

/**
 * Write a newline to the transscript file.
 */
void FrotzInterpreter::script_new_line(void) {
	if (!sfp) return;

	sfp->writeByte('\n');
//	if (sfp->ioFailed()) script_close();

	script_width = 0;
}/* script_new_line */

/**
 * Write a single character to the transscript file.
 */
void FrotzInterpreter::script_char(zword c) {
	if (!sfp) return;

	if (c == ZC_INDENT && script_width != 0)
		c = ' ';

	if (c == ZC_INDENT)
	{ script_char(' '); script_char(' '); script_char(' '); return; }
	if (c == ZC_GAP)
	{ script_char(' '); script_char(' '); return; }
	if (c > 0xff)
	{ script_char('?'); return; }

	sfp->writeByte(c);
	script_width++;
}

/**
 * Write a string to the transscript file.
 */
void FrotzInterpreter::script_word(const zchar *s) {
	int width;
	int i;

	if (*s == ZC_INDENT && script_width != 0)
		script_char(*s++);

	for (i = 0, width = 0; s[i] != 0; i++)
		if (s[i] == ZC_NEW_STYLE || s[i] == ZC_NEW_FONT)
			i++;
		else if (s[i] == ZC_GAP)
			width += 3;
		else if (s[i] == ZC_INDENT)
			width += 2;
		else
			width += 1;

	if (option_script_cols != 0 && script_width + width > option_script_cols) {
		if (*s == ' ' || *s == ZC_INDENT || *s == ZC_GAP)
			s++;

		script_new_line();
	}

	for (i = 0; s[i] != 0; i++) {
		if (s[i] == ZC_NEW_FONT || s[i] == ZC_NEW_STYLE)
			i++;
		else
			script_char(s[i]);
	}
}

/**
 * Send an input line to the transscript file.
 */
void FrotzInterpreter::script_write_input(const zchar *buf, zword key) {
	int width;
	int i;

	for (i = 0, width = 0; buf[i] != 0; i++)
		width++;

	if (option_script_cols != 0 && script_width + width > option_script_cols)
		script_new_line();

	for (i = 0; buf[i] != 0; i++)
		script_char(buf[i]);

	if (key == ZC_RETURN)
		script_new_line();
}

/**
 * Remove an input line from the transscript file.
 */
void FrotzInterpreter::script_erase_input(const zchar *buf) {
	int width;
	int i;

	for (i = 0, width = 0; buf[i] != 0; i++)
		width++;

	warning("TODO: Can't currently seek backwards in stream file");
//	sfp->seek(-width, SEEK_CUR); script_width -= width;
}

/**
 * Start sending a "debugging" message to the transscript file.
 */
void FrotzInterpreter::script_mssg_on(void) {
	if (script_width != 0)
		script_new_line();

	script_char(ZC_INDENT);
}

/**
 * Stop writing a "debugging" message.
 */
void FrotzInterpreter::script_mssg_off(void) {
	script_new_line();
}

/**
 * Open a file to record the player's input.
 */
void FrotzInterpreter::record_open(void) {
	char new_name[MAX_FILE_NAME + 1];

	if (_os.read_file_name (new_name, _h.command_name, FILE_RECORD)) {
		strcpy(_h.command_name, new_name);

		rfp = _saveFileMan->openForSaving(new_name);
		if (rfp)
			ostream_record = TRUE;
		else
			print_string("Cannot open file\n");
	}
}

/**
 * Stop recording the player's input.
 */
void FrotzInterpreter::record_close(void) {
	rfp->finalize();
	delete rfp;
	rfp = NULL;
	ostream_record = FALSE;
}

/**
 * Helper function for record_char.
 */
void FrotzInterpreter::record_code(int c, bool force_encoding) {
	if (force_encoding || c == '[' || c < 0x20 || c > 0x7e) {
		int i;

		rfp->writeByte('[');

		for (i = 10000; i != 0; i /= 10)
			if (c >= i || i == 1)
		rfp->writeByte('0' + (c / i) % 10);

		rfp->writeByte(']');
	} else 
		rfp->writeByte(c);
}/* record_code */

/**
 * Write a character to the command file.
 */
void FrotzInterpreter::record_char(zword c) {
	if (c != ZC_RETURN) {
		if (c < ZC_HKEY_MIN || c > ZC_HKEY_MAX) {
			record_code(translate_to_zscii (c), FALSE);

			if (c == ZC_SINGLE_CLICK || c == ZC_DOUBLE_CLICK) {
				record_code(_h.mousePos.x, TRUE);
				record_code(_h.mousePos.y, TRUE);
			}
		} else 
			record_code(1000 + c - ZC_HKEY_MIN, TRUE);
	}
}

/**
 * Copy a keystroke to the command file.
 */
void FrotzInterpreter::record_write_key (zword key) {
	record_char(key);

	rfp->writeByte('\n');
//	if (rfp->ioFailed()) record_close();
}

/**
 * Copy a line of input to a command file.
 */
void FrotzInterpreter::record_write_input (const zchar *buf, zword key) {
	zword c;

	while ((c = *buf++) != 0)
		record_char(c);

	record_char(key);

	rfp->writeByte('\n');
//	if (rfp->ioFailed()) record_close();
}

/**
 * Open a file of commands for playback.
 */
void FrotzInterpreter::replay_open(void) {
	char new_name[MAX_FILE_NAME + 1];

	if (_os.read_file_name(new_name, _h.command_name, FILE_PLAYBACK)) {
		strcpy(_h.command_name, new_name);

		pfp = _saveFileMan->openForLoading(new_name);
		if (pfp) {
			set_more_prompts (read_yes_or_no ("Do you want MORE prompts"));

			istream_replay = TRUE;

		} else 
			print_string("Cannot open file\n");
	}
}

/**
 * Stop playback of commands.
 */
void FrotzInterpreter::replay_close(void) {
	set_more_prompts(TRUE);
	delete pfp;
	pfp = NULL;
	istream_replay = FALSE;
}

/**
 * Helper function for replay_key and replay_line.
 */
int FrotzInterpreter::replay_code(void) {
	int c;

	if ((c = fgetc(pfp)) == '[') {
		int c2;
		c = 0;

		while ((c2 = fgetc(pfp)) != EOF && c2 >= '0' && c2 <= '9')
			c = 10 * c + c2 - '0';

		return (c2 == ']') ? c : EOF;
	} else 
		return c;
}

/**
 * Read a character from the command file.
 */
zword FrotzInterpreter::replay_char(void) {
	int c;

	if ((c = replay_code()) != EOF) {
		if (c != '\n') {
			if (c < 1000) {
				c = translate_from_zscii(c);

				if (c == ZC_SINGLE_CLICK || c == ZC_DOUBLE_CLICK) {
					_h.mousePos.x = replay_code();
					_h.mousePos.y = replay_code();
				}

				return c;
			} else 
				return ZC_HKEY_MIN + c - 1000;
		}

		//ungetc('\n', pfp);
		pfp->seek(-1, SEEK_CUR);

		return ZC_RETURN;
	} else 
		return ZC_BAD;
}

/**
 * Read a keystroke from a command file.
 */
zword FrotzInterpreter::replay_read_key(void) {
	zword key;

	key = replay_char();

	if (fgetc(pfp) != '\n') {
		replay_close();
		return ZC_BAD;
	} else 
		return key;
}

/**
 * Read a line of input from a command file.
 */
zword FrotzInterpreter::replay_read_input(zchar *buf) {
	zword c;

	for (;;) {
		c = replay_char();

		if (c == ZC_BAD || is_terminator (c))
			break;

		*buf++ = c;
	}

	*buf = 0;

	if (fgetc (pfp) != '\n') {
		replay_close();
		return ZC_BAD;
	} else 
		return c;
}

/* --------------------------------------------------------------------------
 * Hotkey Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Allows user to toggle cheating options on/off.
 */
bool FrotzInterpreter::hot_key_debugging(void) {
	print_string ("Debugging options\n");

	option_attribute_assignment = read_yes_or_no("Watch attribute assignment");
	option_attribute_testing = read_yes_or_no("Watch attribute testing");
	option_object_movement = read_yes_or_no("Watch object movement");
	option_object_locating = read_yes_or_no("Watch object locating");

	return false;
}

/**
 * Displays a list of all hot keys.
 */
bool FrotzInterpreter::hot_key_help(void) {
	print_string ("Help\n");

	print_string (
		"\n"
		"Alt-D  debugging options\n"
		"Alt-H  help\n"
		"Alt-N  new game\n"
		"Alt-P  playback on\n"
		"Alt-R  recording on/off\n"
		"Alt-S  seed random numbers\n"
		"Alt-U  undo one turn\n"
		"Alt-X  exit game\n");

	return false;
}

/**
 * Allows user to turn playback on.
 */
bool FrotzInterpreter::hot_key_playback(void) {
	print_string("Playback on\n");

	if (!istream_replay)
		replay_open();

	return FALSE;
}/* hot_key_playback */

/**
 * Allows user to turn recording on/off.
 */
bool FrotzInterpreter::hot_key_recording(void) {
	if (istream_replay) {
		print_string("Playback off\n");
		replay_close();
	} else if (ostream_record) {
		print_string("Recording off\n");
		record_close();
	} else {
		print_string("Recording on\n");
		record_open();
	}

	return false;
}

/**
 * Allows user to seed the random number seed.
 */
bool FrotzInterpreter::hot_key_seed(void) {
	print_string("Seed random numbers\n");

	print_string("Enter seed value (or return to randomize): ");
	seed_random(read_number());

	return false;
}

/*
 * hot_key_undo
 *
 * ...allows user to undo the previous turn.
 *
 */
bool FrotzInterpreter::hot_key_undo(void) {
	print_string ("Undo one turn\n");

	if (restore_undo()) {
		if (_h.h_version >= V5) {		/* for V5+ games we must */
			store (2);			/* store 2 (for success) */
			return true;		/* and abort the input   */
		}

		if (_h.h_version <= V3) {		/* for V3- games we must */
			z_show_status();		/* draw the status line  */
			return false;			/* and continue input    */
		}
	} else 
		print_string("No more undo information available.\n");

	return false;
}

/**
 * Allows user to start a new game.
 */
bool FrotzInterpreter::hot_key_restart (void) {
	print_string("New game\n");

	if (read_yes_or_no("Do you wish to restart")) {
		z_restart();
		return true;
    } else 
		return false;
}

/**
 * Allows user to exit the game.
 */
bool FrotzInterpreter::hot_key_quit(void) {
	print_string("Exit game\n");

	if (read_yes_or_no("Do you wish to quit")) {
		z_quit();
		return true;
    } else 
		return false;
}

/**
 * Perform the action associated with a so-called hot key. Return
 * true to abort the current input action.
 */
bool FrotzInterpreter::handle_hot_key(zword key) {
	if (_quitFlag) {
		// Exit immediately
		z_quit();
		return true;
	}

	if (cwin == 0) {
		bool aborting;
		aborting = FALSE;

		print_string("\nHot key -- ");

		switch (key) {
		case ZC_HKEY_RECORD: aborting = hot_key_recording(); break;
		case ZC_HKEY_PLAYBACK: aborting = hot_key_playback(); break;
		case ZC_HKEY_SEED: aborting = hot_key_seed(); break;
		case ZC_HKEY_UNDO: aborting = hot_key_undo(); break;
		case ZC_HKEY_RESTART: aborting = hot_key_restart(); break;
		case ZC_HKEY_QUIT: aborting = hot_key_quit(); break;
		case ZC_HKEY_DEBUG: aborting = hot_key_debugging(); break;
		case ZC_HKEY_HELP: aborting = hot_key_help(); break;
		}

		if (aborting)
			return true;

		print_string("\nContinue input...\n");
	}

	return false;

}

/* --------------------------------------------------------------------------
 * Objects Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Calculate the address of an object.
 */
zword FrotzInterpreter::object_address(zword obj) {
	/* Check object number */

	if (obj > ((_h.h_version <= V3) ? 255 : MAX_OBJECT)) {
		print_string("@Attempt to address illegal object ");
		print_num(obj);
		print_string(".  This is normally fatal.");
		new_line();
		runtime_error(ERR_ILL_OBJ);
	}

    /* Return object address */
    if (_h.h_version <= V3)
		return _h.h_objects + ((obj - 1) * O1_SIZE + 62);
    else
		return _h.h_objects + ((obj - 1) * O4_SIZE + 126);
}

/**
 * Return the address of the given object's name.
 */
zword FrotzInterpreter::object_name(zword object) {
	zword obj_addr;
	zword name_addr;

	obj_addr = object_address(object);

	/* The object name address is found at the start of the properties */
	if (_h.h_version <= V3)
		obj_addr += O1_PROPERTY_OFFSET;
	else
		obj_addr += O4_PROPERTY_OFFSET;

	LOW_WORD(obj_addr, name_addr)

	return name_addr;
}

/**
 * Calculate the start address of the property list associated with
 * an object.
 */
zword FrotzInterpreter::first_property (zword obj) {
	zword prop_addr;
	zbyte size;

	/* Fetch address of object name */
	prop_addr = object_name(obj);

	/* Get length of object name */
	LOW_BYTE(prop_addr, size)

	/* Add name length to pointer */
	return prop_addr + 1 + 2 * size;
}

/**
 * Calculate the address of the next property in a property list.
 */
zword FrotzInterpreter::next_property(zword prop_addr) {
	zbyte value;

	/* Load the current property id */

	LOW_BYTE(prop_addr, value)
	prop_addr++;

	/* Calculate the length of this property */
	if (_h.h_version <= V3)
		value >>= 5;
	else if (!(value & 0x80))
		value >>= 6;
	else {
		LOW_BYTE(prop_addr, value)
		value &= 0x3f;

		if (value == 0) value = 64;	/* demanded by Spec 1.0 */
	}

	/* Add property length to current property pointer */
	return prop_addr + value + 1;
}

/**
 * Unlink an object from its parent and siblings.
 */
void FrotzInterpreter::unlink_object(zword object) {
	zword obj_addr;
	zword parent_addr;
	zword sibling_addr;

	if (object == 0) {
		runtime_error(ERR_REMOVE_OBJECT_0);
		return;
	}

	obj_addr = object_address(object);

	if (_h.h_version <= V3) {
		zbyte parent;
		zbyte younger_sibling;
		zbyte older_sibling;
		zbyte zero = 0;

		/* Get parent of object, and return if no parent */
		obj_addr += O1_PARENT;
		LOW_BYTE(obj_addr, parent)
		if (!parent)
			return;

		/* Get (older) sibling of object and set both parent and sibling pointers to 0 */
		SET_BYTE(obj_addr, zero)
		obj_addr += O1_SIBLING - O1_PARENT;
		LOW_BYTE(obj_addr, older_sibling)
		SET_BYTE(obj_addr, zero)

		/* Get first child of parent (the youngest sibling of the object) */
		parent_addr = object_address(parent) + O1_CHILD;
		LOW_BYTE(parent_addr, younger_sibling)

		/* Remove object from the list of siblings */

		if (younger_sibling == object)
			SET_BYTE(parent_addr, older_sibling)
		else {
			do {
				sibling_addr = object_address(younger_sibling) + O1_SIBLING;
				LOW_BYTE(sibling_addr, younger_sibling)
			} while (younger_sibling != object);

			SET_BYTE(sibling_addr, older_sibling)
		}
	} else {
		zword parent;
		zword younger_sibling;
		zword older_sibling;
		zword zero = 0;

		/* Get parent of object, and return if no parent */
		obj_addr += O4_PARENT;
		LOW_WORD(obj_addr, parent)
		if (!parent)
			return;

		/* Get (older) sibling of object and set both parent and sibling pointers to 0 */
		SET_WORD(obj_addr, zero)
		obj_addr += O4_SIBLING - O4_PARENT;
		LOW_WORD(obj_addr, older_sibling)
		SET_WORD(obj_addr, zero)

		/* Get first child of parent (the youngest sibling of the object) */
		parent_addr = object_address(parent) + O4_CHILD;
		LOW_WORD(parent_addr, younger_sibling)

		/* Remove object from the list of siblings */
		if (younger_sibling == object)
			SET_WORD(parent_addr, older_sibling)
		else {
			do {
				sibling_addr = object_address(younger_sibling) + O4_SIBLING;
				LOW_WORD(sibling_addr, younger_sibling)
			} while (younger_sibling != object);

			SET_WORD (sibling_addr, older_sibling)
		}
	}
}

/* --------------------------------------------------------------------------
 * Process Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Initialize process variables.
 */
void FrotzInterpreter::init_process(void) {
	process_finished = 0;
}

/**
 * Load an operand, either a variable or a constant.
 */
void FrotzInterpreter::load_operand(zbyte type) {
	zword value;

	if (type & 2) { 			/* variable */
		zbyte variable;

		CODE_BYTE(variable)

		if (variable == 0)
			value = *sp++;
		else if (variable < 16)
			value = *(fp - variable);
		else {
			zword addr = _h.h_globals + 2 * (variable - 16);
			LOW_WORD(addr, value)
		}
	} else if (type & 1) { 		/* small constant */
		zbyte bvalue;

		CODE_BYTE(bvalue)
		value = bvalue;

	} else 
		CODE_WORD(value) 		/* large constant */

	zargs[zargc++] = value;
}

/**
 * Given the operand specifier byte, load all (up to four) operands
 * for a VAR or EXT opcode.
 */
void FrotzInterpreter::load_all_operands(zbyte specifier) {
	int i;

	for (i = 6; i >= 0; i -= 2) {
		zbyte type = (specifier >> i) & 0x03;

		if (type == 3)
			break;

		load_operand(type);
	}
}

/**
 * Call a subroutine. Save PC and FP then load new PC and initialise
 * new stack frame. Note that the caller may legally provide less or
 * more arguments than the function actually has. The call type "ct"
 * can be 0 (z_call_s), 1 (z_call_n) or 2 (direct call).
 */
void FrotzInterpreter::call(zword routine, int argc, zword *args, int ct) {
	long pc;
	zword value;
	zbyte count;
	int i;

	if (sp - stack < 4)
		runtime_error(ERR_STK_OVF);

	GET_PC(pc)

	*--sp = (zword)(pc >> 9);
	*--sp = (zword)(pc & 0x1ff);
	*--sp = (zword)(fp - stack - 1);
	*--sp = (zword)(argc | (ct << (option_save_quetzal ? 12 : 8)));

	fp = sp;
	frame_count++;

	/* Calculate byte address of routine */

	if (_h.h_version <= V3)
		pc = (long)routine << 1;
	else if (_h.h_version <= V5)
		pc = (long)routine << 2;
	else if (_h.h_version <= V7)
		pc = ((long)routine << 2) + ((long)_h.h_functions_offset << 3);
	else /* (_h.h_version <= V8) */
		pc = (long)routine << 3;

	if (pc >= _h.story_size)
		runtime_error(ERR_ILL_CALL_ADDR);

	SET_PC(pc)

	/* Initialise local variables */

	CODE_BYTE(count)

	if (count > 15)
		runtime_error(ERR_CALL_NON_RTN);
	if (sp - stack < count)
		runtime_error(ERR_STK_OVF);

	if (option_save_quetzal)
		fp[0] |= (zword)count << 8;	/* Save local var count for Quetzal. */

	value = 0;

	for (i = 0; i < count; i++) {
		if (_h.h_version <= V4)		/* V1 to V4 games provide default */
			CODE_WORD(value)		/* values for all local variables */

		*--sp = (zword) ((argc-- > 0) ? args[i] : value);
	}

	/* Start main loop for direct calls */

	if (ct == 2)
		interpret();
}

/**
 * Return from the current subroutine and restore the previous stack
 * frame. The result may be stored (0), thrown away (1) or pushed on
 * the stack (2). In the latter case a direct call has been finished
 * and we must exit the interpreter loop.
 *
 */
void FrotzInterpreter::ret(zword value) {
	long pc;
	int ct;

	if (sp > fp)
		runtime_error(ERR_STK_UNDF);

	sp = fp;

	ct = *sp++ >> (option_save_quetzal ? 12 : 8);
	frame_count--;
	fp = stack + 1 + *sp++;
	pc = *sp++;
	pc = ((long)*sp++ << 9) | pc;

	SET_PC(pc)

	/* Handle resulting value */

	if (ct == 0)
		store(value);
	if (ct == 2)
		*--sp = value;

	/* Stop main loop for direct calls */
	if (ct == 2)
		process_finished++;
}

/**
 * Take a jump after an instruction based on the flag, either true or
 * false. The branch can be short or long; it is encoded in one or two
 * bytes respectively. When bit 7 of the first byte is set, the jump
 * takes place if the flag is true; otherwise it is taken if the flag
 * is false. When bit 6 of the first byte is set, the branch is short;
 * otherwise it is long. The offset occupies the bottom 6 bits of the
 * first byte plus all the bits in the second byte for long branches.
 * Uniquely, an offset of 0 means return false, and an offset of 1 is
 * return true.
 */
void FrotzInterpreter::branch(bool flag) {
	long pc;
	zword offset;
	zbyte specifier;
	zbyte off1;
	zbyte off2;

	CODE_BYTE(specifier)

	off1 = specifier & 0x3f;

	if (!flag)
		specifier ^= 0x80;

	if (!(specifier & 0x40)) {		/* it's a long branch */
		if (off1 & 0x20)		/* propagate sign bit */
			off1 |= 0xc0;

		CODE_BYTE(off2)

		offset = (off1 << 8) | off2;

	} else 
		offset = off1;		/* it's a short branch */
		
	if (specifier & 0x80) {
		if (offset > 1) {		/* normal branch */
			GET_PC(pc)
			pc += (short)offset - 2;
			SET_PC(pc)
		} else 
		ret (offset);		/* special case, return 0 or 1 */
	}
}

/**
 * Store an operand, either as a variable or pushed on the stack.
 */
void FrotzInterpreter::store(zword value) {
	zbyte variable;

	CODE_BYTE(variable)

	if (variable == 0)
		*--sp = value;
	else if (variable < 16)
		*(fp - variable) = value;
	else {
		zword addr = _h.h_globals + 2 * (variable - 16);
		SET_WORD(addr, value)
	}
}

/**
 * Call the interpreter loop directly. This is necessary when
 *
 * - a sound effect has been finished
 * - a read instruction has timed out
 * - a newline countdown has hit zero
 *
 * The interpreter returns the result value on the stack.
 */
int FrotzInterpreter::direct_call(zword addr) {
	zword saved_zargs[8];
	int saved_zargc;
	int i;

	/* Calls to address 0 return false */
	if (addr == 0)
		return 0;

	/* Save operands and operand count */
	for (i = 0; i < 8; i++)
		saved_zargs[i] = zargs[i];

	saved_zargc = zargc;

	/* Call routine directly */
	call(addr, 0, 0, 2);

	/* Restore operands and operand count */

	for (i = 0; i < 8; i++)
		zargs[i] = saved_zargs[i];

	zargc = saved_zargc;

	/* Resulting value lies on top of the stack */
	return (short)*sp++;
}

/* --------------------------------------------------------------------------
 * Random Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Set the seed value for the random number generator.
 */
void FrotzInterpreter::seed_random (int value) {
	// Unused
}

/* --------------------------------------------------------------------------
 * Screen Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Begin output redirection to the memory of the Z-machine.
 */
void FrotzInterpreter::memory_open(zword table, zword xsize, bool buffering) {
	if (++redirect_depth < MAX_NESTING) {
		if (!buffering)
			xsize = 0xffff;
		if (buffering && (short) xsize <= 0)
			xsize = get_max_width((zword) (- (short)xsize));

		storew (table, 0);

		redirect[redirect_depth].table = table;
		redirect[redirect_depth].width = 0;
		redirect[redirect_depth].total = 0;
		redirect[redirect_depth].xsize = xsize;

		ostream_memory = TRUE;
   } else 
	   runtime_error(ERR_STR3_NESTING);

}

/**
 * Redirect a newline to the memory of the Z-machine.
 */
void FrotzInterpreter::memory_new_line(void) {
	zword size;
	zword addr;

	redirect[redirect_depth].total += redirect[redirect_depth].width;
	redirect[redirect_depth].width = 0;

	addr = redirect[redirect_depth].table;

	LOW_WORD(addr, size)
	addr += 2;

	if (redirect[redirect_depth].xsize != 0xffff) {
		redirect[redirect_depth].table = addr + size;
		size = 0;
	} else 
		storeb((zword)(addr + (size++)), 13);

	storew(redirect[redirect_depth].table, size);
}

/**
 * Redirect a string of characters to the memory of the Z-machine.
 */
void FrotzInterpreter::memory_word(const zchar *s) {
	zword size;
	zword addr;
	zword c;

	if (_h.h_version == V6) {
		int width = _os.string_width(s);

		if (redirect[redirect_depth].xsize != 0xffff)
			if (redirect[redirect_depth].width + width > redirect[redirect_depth].xsize) {
				if (*s == ' ' || *s == ZC_INDENT || *s == ZC_GAP)
					width = _os.string_width(++s);

				memory_new_line();
			}

			redirect[redirect_depth].width += width;
	}

	addr = redirect[redirect_depth].table;

	LOW_WORD(addr, size)
	addr += 2;

	while ((c = *s++) != 0)
	storeb((zword)(addr + (size++)), translate_to_zscii(c));

	storew(redirect[redirect_depth].table, size);
}

/**
 * End of output redirection.
 */
void FrotzInterpreter::memory_close(void) {
	if (redirect_depth >= 0) {
		if (redirect[redirect_depth].xsize != 0xffff)
			memory_new_line();

		if (_h.h_version == V6) {
			_h.h_line_width = (redirect[redirect_depth].xsize != 0xffff) ?
			redirect[redirect_depth].total : redirect[redirect_depth].width;

			SET_WORD(H_LINE_WIDTH, _h.h_line_width)
		}

		if (redirect_depth == 0)
			ostream_memory = FALSE;

		redirect_depth--;
	}
}

/* --------------------------------------------------------------------------
 * Screen Methods
 *
 * --------------------------------------------------------------------------
 */

void FrotzInterpreter::init_screen() {
	// Initialize backend - for V6 games, use 640x400, since it's exactly 2x scaled from 320x200.
	// For all other versions, use the taller screen size of 640x480
	int y = (_h.h_version == V6) ? 400 : 480;
	_screen.init(Common::Rect(0, 0, 640, y), 640);
	initGraphics(640, y, true);

	font_height = 1;
	font_width = 1;
	input_redraw = false;
	more_prompts = true;
	discarding = false;
	cursor = true;
	input_window = 0;
	cwp = &wp[0];
}

/**
 * Return the window number in zargs[0]. In V6 only, -3 refers to the
 * current window.
 */
zword FrotzInterpreter::winarg0(void) {
	if (_h.h_version == V6 && (int16)zargs[0] == -3)
		return cwin;

	if (zargs[0] >= ((_h.h_version == V6) ? 8 : 2))
		runtime_error(ERR_ILL_WIN);

	return zargs[0];
}

/**
 * Return the (optional) window number in zargs[2]. -3 refers to the
 * current window. This optional window number was only used by some
 * V6 opcodes: set_cursor, set_margins, set_colour.
 */
zword FrotzInterpreter::winarg2(void) {
	if (zargc < 3 || (short)zargs[2] == -3)
		return cwin;

	if (zargs[2] >= 8)
		runtime_error(ERR_ILL_WIN);

	return zargs[2];
}

/**
 * Move the hardware cursor to make it match the window properties.
 */
void FrotzInterpreter::update_cursor(void) {
	_os.set_cursor (
		cwp->sd.y_pos + cwp->sd.y_cursor - 1,
		cwp->sd.x_pos + cwp->sd.x_cursor - 1);
}

/**
 * Reset the cursor of a given window to its initial position.
 */
void FrotzInterpreter::reset_cursor(zword win) {
	int lines = 0;

	if (_h.h_version <= V4 && win == 0)
		lines = wp[0].sd.y_size / hi(wp[0].sd.font_size) - 1;

	wp[win].sd.y_cursor = hi(wp[0].sd.font_size) * lines + 1;
	wp[win].sd.x_cursor = wp[win].sd.left + 1;

	if (win == cwin)
		update_cursor();
}

/**
 * Check if the Amiga screen model should be used, required for
 * some Infocom games.
 */
bool FrotzInterpreter::amiga_screen_model(void) {
	if (_h.h_interpreter_number == INTERP_AMIGA) {
		switch (_h.story_id) {
		case BEYOND_ZORK:
		case ZORK_ZERO:
		case SHOGUN:
		case ARTHUR:
		case JOURNEY:
			return true;
		default:
			break;
		}
	}

	return false;
}

/**
 * Turn more prompts on/off.
 */
void FrotzInterpreter::set_more_prompts(bool flag) {
	if (flag && !more_prompts)
		cwp->sd.line_count = 0;

	more_prompts = flag;
}

/**
 * Return the #screen units from the cursor to the end of the line.
 */
int FrotzInterpreter::units_left(void) {
	if (_os.wrap_window (cwp - wp) == 0) return 999;

	return cwp->sd.x_size - cwp->sd.right - cwp->sd.x_cursor + 1;
}

/**
 * Return maximum width of a line in the given window. This is used in
 * connection with the extended output stream #3 call in V6.
 */
zword FrotzInterpreter::get_max_width(zword win) {
	if (_h.h_version == V6) {
		if (win >= 8)
			runtime_error(ERR_ILL_WIN);

		return wp[win].sd.x_size - wp[win].sd.left - wp[win].sd.right;
	} else 
		return 0xffff;
}

/**
 * Decrement the newline counter. Call the newline interrupt when the
 * counter hits zero. This is a helper function for screen_new_line.
 */
void FrotzInterpreter::countdown(void) {
	if (cwp->sd.nl_countdown != 0)
		if (--cwp->sd.nl_countdown == 0)
			direct_call(cwp->sd.nl_routine);
}

/**
 * Print a newline to the screen.
 */
void FrotzInterpreter::screen_new_line(void) {
	if (discarding) return;

	if (_os.wrap_window(cwp - wp) == 0)
		_os.display_char('\n');

	/* Handle newline interrupts at the start (for most cases) */
	if (_h.h_interpreter_number != INTERP_MSDOS || _h.story_id != ZORK_ZERO || _h.h_release != 393)
		countdown();

	/* Check whether the last input line gets destroyed */
	if (input_window == cwin)
		input_redraw = TRUE;

	/* If the cursor has not reached the bottom line, then move it to
	   the next line; otherwise scroll the window or reset the cursor
	   to the top left. */
	cwp->sd.x_cursor = cwp->sd.left + 1;

	int rowHeight = font_height;

	if (cwp->sd.y_cursor + 2 * rowHeight - 1 > cwp->sd.y_size)
		if (enable_scrolling) {
			zword y = cwp->sd.y_pos;
			zword x = cwp->sd.x_pos;

			_os.scroll_area(y,
				x,
				y + cwp->sd.y_size - 1,
				x + cwp->sd.x_size - 1,
				rowHeight);

		} else 
			cwp->sd.y_cursor = 1;
	else 
		cwp->sd.y_cursor += rowHeight;

	update_cursor();

	/* See if we need to print a more prompt (unless the game has set
	   the line counter to -999 in order to suppress more prompts). */
	if (enable_scrolling && (short)cwp->sd.line_count != -999) {

		zword above = (cwp->sd.y_cursor - 1) / rowHeight;
		zword below = (cwp->sd.y_size - cwp->sd.y_cursor + 1) / rowHeight;

		cwp->sd.line_count++;

		if ((short)cwp->sd.line_count >= (short)above + below - 1) {
			if (more_prompts)
				_os.more_prompt();

			cwp->sd.line_count = option_context_lines;
		}
	}

	/* Handle newline interrupts at the end for Zork Zero under DOS */
	if (_h.h_interpreter_number == INTERP_MSDOS && _h.story_id == ZORK_ZERO && _h.h_release == 393)
		countdown();
}

/**
 * Display a single character on the screen.
 */
void FrotzInterpreter::screen_char(zword c) {
	int width;

	if (discarding) return;

	if (c == ZC_INDENT && cwp->sd.x_cursor != cwp->sd.left + 1)
		c = ' ';

	if (units_left() < (width = _os.char_width(c))) {
		if (!enable_wrapping)
			{ cwp->sd.x_cursor = cwp->sd.x_size - cwp->sd.right; return; }

		screen_new_line();
	}

	_os.display_char(c); cwp->sd.x_cursor += width;
}

/**
 * Display a string of characters on the screen. If the word doesn't fit
 * then use wrapping or clipping depending on the current setting of the
 * enable_wrapping flag.
 */
void FrotzInterpreter::screen_word(const zchar *s) {
	int width;

	if (discarding) return;

	if (*s == ZC_INDENT && cwp->sd.x_cursor != cwp->sd.left + 1)
		screen_char(*s++);

	
	if (units_left() < (width = _os.string_width(s))) {
		if (!enable_wrapping) {
			zword c;

			while ((c = *s++) != 0)
				if (c == ZC_NEW_FONT || c == ZC_NEW_STYLE) {
					int arg = (int) *s++;

					if (c == ZC_NEW_FONT)
						_os.set_font (arg);
					if (c == ZC_NEW_STYLE)
						_os.set_text_style (arg);
				} else 
					screen_char(c);

			return;
		}

		if (*s == ' ' || *s == ZC_INDENT || *s == ZC_GAP)
			width = _os.string_width(++s);

		screen_new_line();
	}

	_os.display_string(s); cwp->sd.x_cursor += width;
}

/**
 * Display an input line on the screen. This is required during playback.
 */
void FrotzInterpreter::screen_write_input(const zchar *buf, zword key) {
	int width;

	if (units_left() < (width = _os.string_width(buf)))
		screen_new_line();

	_os.display_string(buf); cwp->sd.x_cursor += width;

    if (key == ZC_RETURN)
		screen_new_line();
}

/**
 * Remove an input line that has already been printed from the screen
 * as if it was deleted by the player. This could be necessary during
 * playback.
 */
void FrotzInterpreter::screen_erase_input(const zchar *buf) {
	if (buf[0] != 0) {
		int width = _os.string_width (buf);

		zword y;
		zword x;

		cwp->sd.x_cursor -= width;

		y = cwp->sd.y_pos + cwp->sd.y_cursor - 1;
		x = cwp->sd.x_pos + cwp->sd.x_cursor - 1;

		_os.erase_area(y, x, y + font_height - 1, x + width - 1, -1);
		_os.set_cursor(y, x);
	}
}

/**
 * Read an input line from the keyboard and return the terminating key.
 */
zword FrotzInterpreter::console_read_input(int max, zchar *buf, zword timeout, bool continued) {
	zword key;
	int i;

	/* Make sure there is some space for input */
	if (cwin == 0 && units_left() + _os.string_width(buf) < 10 * font_width)
		screen_new_line();

	/* Make sure the input line is visible */

	if (continued && input_redraw)
		screen_write_input(buf, (zword)-1);

	input_window = cwin;
	input_redraw = FALSE;

	/* Get input line from IO interface */
	cwp->sd.x_cursor -= _os.string_width(buf);
	key = _os.read_line(max, buf, timeout, units_left(), continued);
	cwp->sd.x_cursor += _os.string_width(buf);

	if (key != ZC_TIME_OUT)
		for (i = 0; i < 8; i++)
			wp[i].sd.line_count = 0;

	/* Add a newline if the input was terminated normally */

	if (key == ZC_RETURN)
		screen_new_line();

	return key;
}

/**
 * Read a single keystroke and return it.
 */
zword FrotzInterpreter::console_read_key (zword timeout) {
	zword key;
	int i;

	key = _os.read_key(timeout, cursor);

	if (key != ZC_TIME_OUT)
	for (i = 0; i < 8; i++)
		wp[i].sd.line_count = 0;

	return key;
}

/**
 * Set the three enable_*** variables to make them match the attributes
 * of the current window.
 */
void FrotzInterpreter::update_attributes(void) {
	zword attr = cwp->sd.attribute;

	enable_wrapping = (attr & 1) != 0;
	enable_scrolling = (attr & 2) != 0;
	enable_scripting = (attr & 4) != 0;
	enable_buffering = (attr & 8) != 0;

	/* Some story files forget to select wrapping for printing hints */
	if (_h.story_id == ZORK_ZERO && _h.h_release == 366)
		if (cwin == 0)
			enable_wrapping = TRUE;
	if (_h.story_id == SHOGUN && _h.h_release <= 295)
		if (cwin == 0)
			enable_wrapping = TRUE;
}

/**
 * Set the right text style. This can be necessary when the fixed font
 * flag is changed, or when a new window is selected, or when the game
 * uses the set_text_style opcode.
 */
void FrotzInterpreter::refresh_text_style(void) {
	zword style;

	if (_h.h_version != V6) {
		style = wp[0].sd.style;

		if (cwin != 0 || _h.h_flags & FIXED_FONT_FLAG)
			style |= FIXED_WIDTH_STYLE;

	} else 
		style = cwp->sd.style;

	if (!ostream_memory && ostream_screen && enable_buffering) {
		print_char(ZC_NEW_STYLE);
		print_char(style);
	} else 
		_os.set_text_style(style);
}

/**
 * Set the current window. In V6 every window has its own set of window
 * properties such as colours, text style, cursor position and size.
 */
void FrotzInterpreter::set_window(zword win) {
	flush_buffer();

	cwin = win; cwp = wp + win;
	update_attributes();

	if (_h.h_version == V6) {
		_os.set_colour(lo(cwp->sd.colour), hi(cwp->sd.colour));

		if (_os.font_data(cwp->font, &font_height, &font_width))
			_os.set_font(cwp->font);

		_os.set_text_style (cwp->sd.style);
	} else 
		refresh_text_style();

	if (_h.h_version != V6 && win != 0) {
		wp[win].sd.y_cursor = 1;
		wp[win].sd.x_cursor = 1;
	}

	update_cursor();
}

/**
 * Erase a window to background colour.
 */
void FrotzInterpreter::erase_window(zword win) {
	zword y = wp[win].sd.y_pos;
	zword x = wp[win].sd.x_pos;

	if (_h.h_version == V6 && win != cwin && !amiga_screen_model())
		_os.set_colour(lo(wp[win].sd.colour), hi(wp[win].sd.colour));

//	if (hi(wp[win].sd.colour) != TRANSPARENT_COLOUR) {
		_os.erase_area(y - 1, x - 1,
				y + wp[win].sd.y_size - 1,
				x + wp[win].sd.x_size - 1,
				win);
//	}

	if (_h.h_version == V6 && win != cwin && !amiga_screen_model())
		_os.set_colour(lo(cwp->sd.colour), hi(cwp->sd.colour));

	reset_cursor(win);

	wp[win].sd.line_count = 0;
}

/**
 * Divide the screen into upper (1) and lower (0) windows. In V3 the upper
 * window appears below the status line.
 */
void FrotzInterpreter::split_window(zword height) {
	zword stat_height = 0;

	flush_buffer();

	/* Calculate height of status line and upper window */
	if (_h.h_version != V6)
		height *= hi(wp[1].sd.font_size);

	if (_h.h_version <= V3)
		stat_height = hi(wp[7].sd.font_size);

	/* Cursor of upper window mustn't be swallowed by the lower window */
	wp[1].sd.y_cursor += wp[1].sd.y_pos - 1 - stat_height;

	wp[1].sd.y_pos = 1 + stat_height;
	wp[1].sd.y_size = height;

	if ((short)wp[1].sd.y_cursor > (short)wp[1].sd.y_size)
		reset_cursor(1);

	/* Cursor of lower window mustn't be swallowed by the upper window */
	wp[0].sd.y_cursor += wp[0].sd.y_pos - 1 - stat_height - height;

	wp[0].sd.y_pos = 1 + stat_height + height;
	wp[0].sd.y_size = _h.h_screen_height - stat_height - height;

	if ((short)wp[0].sd.y_cursor < 1)
		reset_cursor(0);

	/* Erase the upper window in V3 only */
	if (_h.h_version == V3 && height != 0)
		erase_window(1);

	_os.window_height(0, wp[0].sd.y_size);
	_os.window_height(1, wp[1].sd.y_size);
}

/**
 * Erase the entire screen to background colour.
 */
void FrotzInterpreter::erase_screen(zword win) {
	int i;

	// Make sure cwp is valid
	if ((cwp >= &wp[0]) && (cwp <= &wp[7])) {
//		if (hi(cwp->sd.colour) != TRANSPARENT_COLOUR)
			_os.erase_area(1, 1, _h.h_screen_height, _h.h_screen_width, -2);
	}

	if ((int16)win == -1) {
		split_window(0);
		set_window(0);
		reset_cursor(0);
	}

	for (i = 0; i < 8; i++)
		wp[i].sd.line_count = 0;
}

/**
 * Try to adapt the window properties to a new screen size.
 */
void FrotzInterpreter::resize_screen(void) {
	if (_h.h_version != V6) {
		int h = wp[0].sd.y_pos + wp[0].sd.y_size;

		wp[0].sd.x_size = _h.h_screen_width;
		wp[1].sd.x_size = _h.h_screen_width;
		wp[7].sd.x_size = _h.h_screen_width;

		wp[0].sd.y_size = _h.h_screen_height - wp[1].sd.y_size;
		if (_h.h_version <= V3)
			wp[0].sd.y_size -= hi (wp[7].sd.font_size);

		if (_os.font_data(TEXT_FONT, &font_height, &font_width)) {
			int i;
			for (i = 0; i < 8; i++)
				wp[i].sd.font_size = (font_height << 8) | font_width;
		}

		if (cwin == 0) {
			int lines = wp[0].sd.y_cursor + font_height - wp[0].sd.y_size - 1;

			if (lines > 0) {
				if (lines % font_height != 0)
					lines += font_height;
				lines /= font_height;

				if (wp[0].sd.y_cursor > (font_height * lines)) {
					_os.scroll_area (wp[0].sd.y_pos,
							wp[0].sd.x_pos,
							h - 1,
							wp[0].sd.x_pos + wp[0].sd.x_size - 1,
							font_height * lines);

					wp[0].sd.y_cursor -= (font_height * lines);
					update_cursor();
				}
			}
		}

		_os.window_height(0, wp[0].sd.y_size);
	}
}

/**
 * Prepare the screen for a new game.
 */
void FrotzInterpreter::restart_screen(void) {
	/* Use default settings */
	_os.set_colour(_h.h_default_foreground, _h.h_default_background);

	if (_os.font_data(TEXT_FONT, &font_height, &font_width))
		_os.set_font(TEXT_FONT);

	_os.set_text_style(0);
	cursor = TRUE;

	/* Initialise window properties */
	mwin = 1;

	for (cwp = wp; cwp < wp + 8; cwp++) {
		cwp->sd.y_pos = 1;
		cwp->sd.x_pos = 1;
		cwp->sd.y_size = 0;
		cwp->sd.x_size = 0;
		cwp->sd.y_cursor = 1;
		cwp->sd.x_cursor = 1;
		cwp->sd.left = 0;
		cwp->sd.right = 0;
		cwp->sd.nl_routine = 0;
		cwp->sd.nl_countdown = 0;
		cwp->sd.style = 0;
		cwp->sd.colour = (_h.h_default_background << 8) | _h.h_default_foreground;
		cwp->font = TEXT_FONT;
		cwp->sd.font_size = (font_height << 8) | font_width;
		cwp->sd.attribute = 8;
		cwp->true_fore = _h.hx_fore_colour;
		cwp->true_back = _h.hx_back_colour;
	}

	/* Prepare lower/upper windows and status line */
	wp[0].sd.attribute = 15;

	wp[0].sd.left = option_left_margin;
	wp[0].sd.right = option_right_margin;

	wp[0].sd.x_size = _h.h_screen_width;
	wp[1].sd.x_size = _h.h_screen_width;

	if (_h.h_version <= V3)
		wp[7].sd.x_size = _h.h_screen_width;

	_os.restart_game(RESTART_WPROP_SET);

	/* Clear the screen, unsplit it and select window 0 */
	erase_screen((zword)(-1));
}

/**
 * Return false if the last mouse click occured outside the current
 * mouse window; otherwise write the mouse arrow coordinates to the
 * memory of the header extension table and return true.
 */
bool FrotzInterpreter::validate_click (void) {
	if (mwin >= 0) {
		if (_h.mousePos.y < wp[mwin].sd.y_pos || _h.mousePos.y >= wp[mwin].sd.y_pos + wp[mwin].sd.y_size)
			return FALSE;
		if (_h.mousePos.x < wp[mwin].sd.x_pos || _h.mousePos.x >= wp[mwin].sd.x_pos + wp[mwin].sd.x_size)
			return FALSE;

		_h.hx_mouse_y = _h.mousePos.y - wp[mwin].sd.y_pos + 1;
		_h.hx_mouse_x = _h.mousePos.x - wp[mwin].sd.x_pos + 1;

	} else {
		if (_h.mousePos.y < 1 || _h.mousePos.y > _h.h_screen_height)
			return FALSE;
		if (_h.mousePos.x < 1 || _h.mousePos.x > _h.h_screen_width)
			return FALSE;

		_h.hx_mouse_y = _h.mousePos.y;
		_h.hx_mouse_x = _h.mousePos.x;
	}

	if (_h.h_version != V6) {
		_h.hx_mouse_y = (_h.hx_mouse_y - 1) / _h.h_font_height + 1;
		_h.hx_mouse_x = (_h.hx_mouse_x - 1) / _h.h_font_width + 1;
	}

	set_header_extension(HX_MOUSE_X, _h.hx_mouse_x);
	set_header_extension(HX_MOUSE_Y, _h.hx_mouse_y);

	return TRUE;
}

/**
 * Start printing a so-called debugging message. The contents of the
 * message are passed to the message stream, a Frotz specific output
 * stream with maximum priority.
 */
void FrotzInterpreter::screen_mssg_on(void) {
	if (cwin == 0) {		/* messages in window 0 only */
		_os.set_text_style(0);

		if (cwp->sd.x_cursor != cwp->sd.left + 1)
			screen_new_line();

		screen_char(ZC_INDENT);
	} else 
		discarding = TRUE; 	/* discard messages in other windows */
}

/**
 * Stop printing a "debugging" message.
 */
void FrotzInterpreter::screen_mssg_off(void) {
	if (cwin == 0) {		/* messages in window 0 only */
		screen_new_line();

		refresh_text_style();

	} else 
		discarding = FALSE; 	/* message has been discarded */
}

/**
 * Pad the status line with spaces up to the given position.
 */
void FrotzInterpreter::pad_status_line(int column) {
	int spaces;

	flush_buffer();

	spaces = units_left() / _os.char_width(' ') - column;

    /* while (spaces--) */
    /* Justin Wesley's fix for narrow displays (Agenda PDA) */
	while (spaces-- > 0)
	screen_char(' ');
}

/**
 * Get the colours for a given window.
 */
void FrotzInterpreter::get_window_colours(zword win, zbyte* fore, zbyte* back) {
	*fore = lo(wp[win].sd.colour);
	*back = hi(wp[win].sd.colour);
}

/**
 * Get the font for a given window.
 */
zword FrotzInterpreter::get_window_font(zword win) {
	zword font = wp[win].font;

	if (font == TEXT_FONT)
		if (_h.h_version != V6) {
			if (win != 0 || _h.h_flags & FIXED_FONT_FLAG)
				font = FIXED_WIDTH_FONT;
		} else {
			if (wp[win].sd.style & FIXED_WIDTH_STYLE)
				font = FIXED_WIDTH_FONT;
		}

	return font;
}

/**
 * Check if a colour is set in any window.
 */
int FrotzInterpreter::colour_in_use(zword colour) {
	int max = (_h.h_version == V6) ? 8 : 2;
	int i;

	for (i = 0; i < max; i++) {
		zword bg = hi(wp[i].sd.colour);
		zword fg = lo(wp[i].sd.colour);

		if (colour == fg || colour == bg)
			return 1;
	}

	return 0;
}

/**
 * Get the currently active window.
 */
zword FrotzInterpreter::get_current_window(void) {
	return cwp - wp;
}

/* --------------------------------------------------------------------------
 * Sound Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Initialises the sound system
 */
void FrotzInterpreter::init_sound(void) {
	sound_locked = false;
	playing = FALSE;
	next_sample = 0;
	next_volume = 0;
}

/**
 * Call the IO interface to play a sample.
 */
void FrotzInterpreter::start_sample(int number, int volume, int repeats, zword eos) {
	if (_h.story_id == LURKING_HORROR)
	repeats = lh_repeats[number];

	_os.start_sample(number, volume, repeats, eos);

	playing = true;
}

/**
 * Play a sample that has been delayed until the previous sound effect has
 * finished.  This is necessary for two samples in The Lurking Horror that
 * immediately follow other samples.
 */
void FrotzInterpreter::start_next_sample(void) {
	if (next_sample != 0)
		start_sample(next_sample, next_volume, 0, 0);

	next_sample = 0;
	next_volume = 0;
}

/**
 * Call the Z-code routine which was given as the last parameter of
 * a sound_effect call. This function may be called from a hardware
 * interrupt (which requires extremely careful programming).
 */
void FrotzInterpreter::end_of_sound(zword routine) {
/*#if defined(DJGPP) && defined(SOUND_SUPPORT)
	end_of_sound_flag = 0;
#endif*/
	playing = false;

	if (!sound_locked) {
		if (_h.story_id == LURKING_HORROR)
			start_next_sample();

		direct_call(routine);
	}
}

/* --------------------------------------------------------------------------
 * Stream Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Write a single character to the scrollback buffer.
 */
void FrotzInterpreter::scrollback_char(zchar c) {
	if (c == ZC_INDENT) { 
		scrollback_char(' '); scrollback_char(' '); scrollback_char(' '); return; 
	}
	if (c == ZC_GAP) { 
		scrollback_char(' '); scrollback_char(' '); return; 
	}

	_os.scrollback_char(c);
}

/**
 * Write a string to the scrollback buffer.
 */
void FrotzInterpreter::scrollback_word(const zchar *s) {
	int i;

	for (i = 0; s[i] != 0; i++)

	if (s[i] == ZC_NEW_FONT || s[i] == ZC_NEW_STYLE)
		i++;
	else
		scrollback_char(s[i]);

}

/**
 * Send an input line to the scrollback buffer.
 */
void FrotzInterpreter::scrollback_write_input(const zchar *buf, zword key) {
	for (int i = 0; buf[i] != 0; i++)
		scrollback_char(buf[i]);

	if (key == ZC_RETURN)
		scrollback_char('\n');
}

/**
 * Remove an input line from the scrollback buffer.
 */
void FrotzInterpreter::scrollback_erase_input(const zchar *buf) {
	int width;
	int i;

	for (i = 0, width = 0; buf[i] != 0; i++)
		width++;

	_os.scrollback_erase(width);
}

/**
 * Start printing a "debugging" message.
 */
void FrotzInterpreter::stream_mssg_on(void) {
	flush_buffer();

	if (ostream_screen)
		screen_mssg_on();
	if (ostream_script && enable_scripting)
		script_mssg_on();

	message = true;
}

/**
 * Stop printing a "debugging" message.
 */
void FrotzInterpreter::stream_mssg_off(void) {
	flush_buffer();

	if (ostream_screen)
		screen_mssg_off();
	if (ostream_script && enable_scripting)
		script_mssg_off();

	message = false;
}

/**
 * Send a single character to the output stream.
 */
void FrotzInterpreter::stream_char(zchar c) {
	if (ostream_screen)
		screen_char(c);
	if (ostream_script && enable_scripting)
		script_char(c);
	if (enable_scripting)
		scrollback_char(c);
}

/**
 * Send a string of characters to the output streams.
 */
void FrotzInterpreter::stream_word(const zchar *s) {
	if (ostream_memory && !message)
		memory_word(s);
	else {
		if (ostream_screen)
			screen_word(s);
		if (ostream_script && enable_scripting)
			script_word(s);
		if (enable_scripting)
			scrollback_word(s);
	}
}

/**
 * Send a newline to the output streams.
 */
void FrotzInterpreter::stream_new_line(void) {
	if (ostream_memory && !message)
		memory_new_line();
	else {
		if (ostream_screen)
			screen_new_line();
		if (ostream_script && enable_scripting)
			script_new_line();
		if (enable_scripting)
			_os.scrollback_char('\n');
	}
}

/**
 * Read a single keystroke from the current input stream.
 */
zword FrotzInterpreter::stream_read_key(zword timeout, zword routine, bool hot_keys) {
	zword key = ZC_BAD;

	flush_buffer();

	/* Read key from current input stream */
continue_input:

	do {
		if (istream_replay)
			key = replay_read_key();
		else
			key = console_read_key (timeout);
	} while (key == ZC_BAD);

	/* Verify mouse clicks */
	if (key == ZC_SINGLE_CLICK || key == ZC_DOUBLE_CLICK)
		if (!validate_click())
			goto continue_input;

	/* Copy key to the command file */
	if (ostream_record && !istream_replay)
		record_write_key(key);

	/* Handle timeouts */
	if (key == ZC_TIME_OUT)
		if (direct_call(routine) == 0)
			goto continue_input;

	/* Handle hot keys */
	if (hot_keys && key >= ZC_HKEY_MIN && key <= ZC_HKEY_MAX) {
		if (_h.h_version == V4 && key == ZC_HKEY_UNDO)
			goto continue_input;
		if (!handle_hot_key (key))
			goto continue_input;

		return ZC_BAD;
	}

	/* Return key */
	return key;
}

/**
 * Read a line of input from the current input stream.
 */
zword FrotzInterpreter::stream_read_input(int max, zchar *buf, zword timeout, 
			zword routine, bool hot_keys, bool no_scripting ) {
	zword key = ZC_BAD;
	bool no_scrollback = no_scripting;

	if (_h.h_version == V6 && _h.story_id == UNKNOWN && !ostream_script)
		no_scrollback = FALSE;

	flush_buffer();

	/* Remove initial input from the transscript file or from the screen */
	if (ostream_script && enable_scripting && !no_scripting)
		script_erase_input(buf);
	if (enable_scripting && !no_scrollback)
		scrollback_erase_input(buf);
	if (istream_replay)
		screen_erase_input(buf);

	/* Read input line from current input stream */
continue_input:

	do {
		if (istream_replay)
			key = replay_read_input(buf);
		else
			key = console_read_input(max, buf, timeout, key != ZC_BAD);

	} while (key == ZC_BAD);

	/* Verify mouse clicks */
	if (key == ZC_SINGLE_CLICK || key == ZC_DOUBLE_CLICK)
		if (!validate_click())
			goto continue_input;

	/* Copy input line to the command file */
	if (ostream_record && !istream_replay)
		record_write_input(buf, key);

	/* Handle timeouts */
	if (key == ZC_TIME_OUT)
		if (direct_call (routine) == 0)
			goto continue_input;

	/* Handle hot keys */
	if (hot_keys && key >= ZC_HKEY_MIN && key <= ZC_HKEY_MAX) {
		if (!handle_hot_key (key))
			goto continue_input;

		return ZC_BAD;
	}

	/* Copy input line to transscript file or to the screen */
	if (ostream_script && enable_scripting && !no_scripting)
		script_write_input(buf, key);
	if (enable_scripting && !no_scrollback)
		scrollback_write_input(buf, key);
	if (istream_replay)
		screen_write_input(buf, key);

	/* Return terminating key */
	return key;
}

/* --------------------------------------------------------------------------
 * Text Methods
 *
 * --------------------------------------------------------------------------
 */

/**
 * Initialize text variables.
 */
void FrotzInterpreter::init_text(void) {
	decoded = NULL;
	encoded = NULL;
	resolution = 0;
}

/**
 * Map a ZSCII character into Unicode.
 */
zword FrotzInterpreter::translate_from_zscii(zbyte c) {
	if (c == 0xfc)
		return ZC_MENU_CLICK;
	if (c == 0xfd)
		return ZC_DOUBLE_CLICK;
	if (c == 0xfe)
		return ZC_SINGLE_CLICK;

	if (c >= 0x9b && _h.story_id != BEYOND_ZORK) {
		if (_h.hx_unicode_table != 0) {	/* game has its own Unicode table */
			zbyte N;

			LOW_BYTE(_h.hx_unicode_table, N)

			if (c - 0x9b < N) {
				zword addr = _h.hx_unicode_table + 1 + 2 * (c - 0x9b);
				zword unicode;

				LOW_WORD(addr, unicode)

				if (unicode < 0x20)
					return '?';

				return unicode;
			} else 
				return '?';

		} else {			/* game uses standard set */
			if (c <= 0xdf) 
				return zscii_to_latin1[c - 0x9b];
			else 
				return '?';
		}
	}

	return (zword)c;
}

/**
 * Convert a Unicode character to ZSCII, returning 0 on failure.
 */
zbyte FrotzInterpreter::unicode_to_zscii(zword c) {
	if (c >= ZC_LATIN1_MIN) {
		if (_h.hx_unicode_table != 0) {	/* game has its own Unicode table */
			zbyte N;

			LOW_BYTE(_h.hx_unicode_table, N)

			for (int i = 0x9b; i < 0x9b + N; i++) {
				zword addr = _h.hx_unicode_table + 1 + 2 * (i - 0x9b);
				zword unicode;

				LOW_WORD(addr, unicode)

				if (c == unicode)
					return (zbyte) i;
			}

			return 0;

		} else {			/* game uses standard set */
			for (int i = 0x9b; i <= 0xdf; i++)
				if (c == zscii_to_latin1[i - 0x9b])
					return (zbyte)i;

			return 0;
		}
	}

	return (zbyte)c;
}

/**
 * Map a Unicode character onto the ZSCII alphabet.
 */
zbyte FrotzInterpreter::translate_to_zscii (zword c) {
	if (c == ZC_SINGLE_CLICK)
		return 0xfe;
	if (c == ZC_DOUBLE_CLICK)
		return 0xfd;
	if (c == ZC_MENU_CLICK)
		return 0xfc;
	if (c == 0)
		return 0;

	c = unicode_to_zscii(c);
	if (c == 0)
		c = '?';

	return (zbyte) c;
}

/**
 * Return a character from one of the three character sets.
 */
zword FrotzInterpreter::alphabet(int set, int index) {
	if (_h.h_version > V1 && set == 2 && index == 1)
		return 0x0D;		/* always newline */

	if (_h.h_alphabet != 0) {	/* game uses its own alphabet */
		zbyte c;

		zword addr = _h.h_alphabet + 26 * set + index;
		LOW_BYTE(addr, c)

		return translate_from_zscii(c);

	} else if (set == 0)		/* game uses default alphabet */
		return 'a' + index;
	else if (set == 1)
		return 'A' + index;
	else if (_h.h_version == V1)
		return " 0123456789.,!?_#'\"/\\<-:()"[index];
	else
		return " ^0123456789.,!?_#'\"/\\-:()"[index];

}

/**
 * Find the number of bytes used for dictionary resolution.
 */
void FrotzInterpreter::find_resolution(void) {
	zword dct = _h.h_dictionary;
	zbyte sep_count;
	zbyte entry_len;

	LOW_BYTE(dct, sep_count)
	dct += 1 + sep_count;  /* skip word separators */
	LOW_BYTE(dct, entry_len)

	resolution = (_h.h_version <= V3) ? 2 : 3;

	if (2 * resolution > entry_len) {
		runtime_error(ERR_DICT_LEN);
	}

	decoded = (zword *)malloc(sizeof (zword) * (3 * resolution) + 1);
	encoded = (zword *)malloc(sizeof (zword) * resolution);
}

/**
 * Copy a ZSCII string from the memory to the global "decoded" string.
 */
void FrotzInterpreter::load_string (zword addr, zword length) {
	int i = 0;

	if (resolution == 0) find_resolution();

	while (i < 3 * resolution) {
		if (i < length) {
			zbyte c;

			LOW_BYTE(addr, c)
			addr++;

			decoded[i++] = translate_from_zscii (c);
		} else 
			decoded[i++] = 0;
	}
}

/**
 * Encode the Unicode text in the global "decoded" string then write
 * the result to the global "encoded" array. (This is used to look up
 * words in the dictionary.) Up to V3 the vocabulary resolution is
 * two, and from V4 it is three Z-characters.
 * Because each word contains three Z-characters, that makes six or
 * nine Z-characters respectively. Longer words are chopped to the
 * proper size, shorter words are are padded out with 5's. For word
 * completion we pad with 0s and 31s, the minimum and maximum
 * Z-characters.
 */
void FrotzInterpreter::encode_text(int padding) {
	static zword again[] = { 'a', 'g', 'a', 'i', 'n', 0, 0, 0, 0 };
	static zword examine[] = { 'e', 'x', 'a', 'm', 'i', 'n', 'e', 0, 0 };
	static zword wait[] = { 'w', 'a', 'i', 't', 0, 0, 0, 0, 0 };

	zbyte *zchars;
	const zword *ptr;
	zword c;
	int i = 0;

	if (resolution == 0) find_resolution();

	zchars = (zbyte *)malloc (sizeof (zbyte) * 3 * (resolution + 1));
	ptr = decoded;

	/* Expand abbreviations that some old Infocom games lack */

	if (option_expand_abbreviations && (_h.h_version <= V8)) {
		if (padding == 0x05 && decoded[1] == 0) {
			switch (decoded[0]) {
			case 'g': ptr = again; break;
			case 'x': ptr = examine; break;
			case 'z': ptr = wait; break;
			}
		}
	}

	/* Translate string to a sequence of Z-characters */

	while (i < 3 * resolution) {
		if ((c = *ptr++) != 0) {
			int index, set;
			zbyte c2;

			if (c == 32) {
				zchars[i++] = 0;
				continue;
			}

			/* Search character in the alphabet */

			for (set = 0; set < 3; set++)
			for (index = 0; index < 26; index++)
				if (c == alphabet (set, index))
				goto letter_found;

			/* Character not found, store its ZSCII value */

			c2 = translate_to_zscii (c);

			zchars[i++] = 5;
			zchars[i++] = 6;
			zchars[i++] = c2 >> 5;
			zchars[i++] = c2 & 0x1f;

			continue;

		letter_found:

			/* Character found, store its index */

			if (set != 0)
			zchars[i++] = ((_h.h_version <= V2) ? 1 : 3) + set;

			zchars[i++] = index + 6;

		} else 
			zchars[i++] = padding;
	}

	/* Three Z-characters make a 16bit word */

	for (i = 0; i < resolution; i++)
		encoded[i] =
			(zchars[3 * i + 0] << 10) |
			(zchars[3 * i + 1] << 5) |
			(zchars[3 * i + 2]);

	encoded[resolution - 1] |= 0x8000;

	free(zchars);
}

#define outchar(c)	if (st==VOCABULARY) *ptr++=c; else print_char(c)

/**
 * Convert encoded text to Unicode. The encoded text consists of 16bit
 * words. Every word holds 3 Z-characters (5 bits each) plus a spare
 * bit to mark the last word. The Z-characters translate to ZSCII by
 * looking at the current current character set. Some select another
 * character set, others refer to abbreviations.
 *
 * There are several different string types:
 *
 *	LOW_STRING - from the lower 64KB (byte address)
 *	ABBREVIATION - from the abbreviations table (word address)
 *	HIGH_STRING - from the end of the memory map (packed address)
 *	EMBEDDED_STRING - from the instruction stream (at PC)
 *	VOCABULARY - from the dictionary (byte address)
 *
 * The last type is only used for word completion.
 */
void FrotzInterpreter::decode_text(enum string_type st, zword addr) {
	zword *ptr;
	long byte_addr;
	zword c2;
	zword code;
	zbyte c, prev_c = 0;
	int shift_state = 0;
	int shift_lock = 0;
	int status = 0;

	ptr = NULL;		/* makes compilers shut up */
	byte_addr = 0;

	if (resolution == 0) 
		find_resolution();

	/* Calculate the byte address if necessary */
	if (st == ABBREVIATION)
		byte_addr = (long) addr << 1;
	else if (st == HIGH_STRING) {
		if (_h.h_version <= V3)
			byte_addr = (long) addr << 1;
		else if (_h.h_version <= V5)
			byte_addr = (long) addr << 2;
		else if (_h.h_version <= V7)
			byte_addr = ((long) addr << 2) + ((long) _h.h_strings_offset << 3);
		else /* (_h.h_version <= V8) */
			byte_addr = (long) addr << 3;

		if (byte_addr >= _h.story_size)
			runtime_error(ERR_ILL_PRINT_ADDR);
	}

	/* Loop until a 16bit word has the highest bit set */
	if (st == VOCABULARY)
		ptr = decoded;

	do {
		int i;

		/* Fetch the next 16bit word */
		if (st == LOW_STRING || st == VOCABULARY) {
			LOW_WORD(addr, code)
			addr += 2;
		} else if (st == HIGH_STRING || st == ABBREVIATION) {
			HIGH_WORD(byte_addr, code)
			byte_addr += 2;
		} else
			CODE_WORD(code)

		/* Read its three Z-characters */
		for (i = 10; i >= 0; i -= 5) {
			zword abbr_addr;
			zword ptr_addr;
			zword zc;

			c = (code >> i) & 0x1f;

			switch (status) {
			case 0:	/* normal operation */
				if (shift_state == 2 && c == 6)
					status = 2;
				else if (_h.h_version == V1 && c == 1)
					new_line();
				else if (_h.h_version >= V2 && shift_state == 2 && c == 7)
					new_line();
				else if (c >= 6)
					outchar (alphabet (shift_state, c - 6));
				else if (c == 0)
					outchar (' ');
				else if (_h.h_version >= V2 && c == 1)
					status = 1;
				else if (_h.h_version >= V3 && c <= 3)
					status = 1;
				else {
					shift_state = (shift_lock + (c & 1) + 1) % 3;

					if (_h.h_version <= V2 && c >= 4)
						shift_lock = shift_state;
					break;
				}

				shift_state = shift_lock;
				break;

			case 1:	/* abbreviation */
				ptr_addr = _h.h_abbreviations + 64 * (prev_c - 1) + 2 * c;

				LOW_WORD(ptr_addr, abbr_addr)
				decode_text (ABBREVIATION, abbr_addr);

				status = 0;
				break;

			case 2:	/* ZSCII character - first part */
				status = 3;
				break;

			case 3:	/* ZSCII character - second part */
				zc = (prev_c << 5) | c;

				c2 = translate_from_zscii (zc);
				outchar (c2);

				status = 0;
				break;
			}

			prev_c = c;
		}

	} while (!(code & 0x8000));

	if (st == VOCABULARY)
	*ptr = 0;
}

#undef outchar

/**
 * print_num
 *
 * Print a signed 16bit number.
 *
 */
void FrotzInterpreter::print_num(zword value) {
	int i;

	/* Print sign */
	if ((short) value < 0) {
		print_char('-');
		value = - (short) value;
	}

	/* Print absolute value */
	for (i = 10000; i != 0; i /= 10)
		if (value >= i || i == 1)
			print_char('0' + (value / i) % 10);
}

/*
 * print_object
 *
 * Print an object description.
 *
 */
void FrotzInterpreter::print_object(zword object) {
	zword addr = object_name (object);
	zword code = 0x94a5;
	zbyte length;

	LOW_BYTE(addr, length)
	addr++;

	if (length != 0)
		LOW_WORD(addr, code)

	if (code == 0x94a5) { 	/* encoded text 0x94a5 == empty string */
		print_string ("object#");	/* supply a generic name */
		print_num (object);		/* for anonymous objects */
	} else 
		decode_text (LOW_STRING, addr);
}

/*
 * print_string
 *
 * Print a string of ASCII characters.
 *
 */
void FrotzInterpreter::print_string(const char *s) {
	char c;

	while ((c = *s++) != 0) {
		if (c == '\n')
			new_line();
		else
			print_char (c);
	}
}

/**
 * Scan a dictionary searching for the given word. The first argument
 * can be
 *
 * 0x00 - find the first word which is >= the given one
 * 0x05 - find the word which exactly matches the given one
 * 0x1f - find the last word which is <= the given one
 *
 * The return value is 0 if the search fails.
 */
zword FrotzInterpreter::lookup_text(int padding, zword dct) {
	zword entry_addr;
	zword entry_count;
	zword entry;
	zword addr;
	zbyte entry_len;
	zbyte sep_count;
	int entry_number;
	int lower, upper;
	int i;
	bool sorted;

	if (resolution == 0) 
		find_resolution();

	encode_text(padding);

	LOW_BYTE(dct, sep_count)		/* skip word separators */
	dct += 1 + sep_count;
	LOW_BYTE(dct, entry_len)		/* get length of entries */
	dct += 1;
	LOW_WORD(dct, entry_count)		/* get number of entries */
	dct += 2;

	if ((short)entry_count < 0) {	/* bad luck, entries aren't sorted */
		entry_count = - (short) entry_count;
		sorted = false;
	} else 
		sorted = true;		/* entries are sorted */

	lower = 0;
	upper = entry_count - 1;

	while (lower <= upper) {
		if (sorted)						 /* binary search */
			entry_number = (lower + upper) / 2;
		else									/* linear search */
			entry_number = lower;

		entry_addr = dct + entry_number * entry_len;

		/* Compare word to dictionary entry */
		addr = entry_addr;

		for (i = 0; i < resolution; i++) {
			LOW_WORD(addr, entry)
			if (encoded[i] != entry)
				goto continuing;
			
			addr += 2;
		}

		return entry_addr;		/* exact match found, return now */

	continuing:
		if (sorted)				/* binary search */
			if (encoded[i] > entry)
				lower = entry_number + 1;
			else
				upper = entry_number - 1;

		else lower++;						   /* linear search */
	}

	/* No exact match has been found */
	if (padding == 0x05)
		return 0;

	entry_number = (padding == 0x00) ? lower : upper;

	if (entry_number == -1 || entry_number == entry_count)
		return 0;

	return dct + entry_number * entry_len;
}/* lookup_text */

/**
 * Translate a single word to a token and append it to the token
 * buffer. Every token consists of the address of the dictionary
 * entry, the length of the word and the offset of the word from
 * the start of the text buffer. Unknown words cause empty slots
 * if the flag is set (such that the text can be scanned several
 * times with different dictionaries); otherwise they are zero.
 */
void FrotzInterpreter::tokenise_text(zword text, zword length, 
		zword from, zword parse, zword dct, bool flag) {
	zword addr;
	zbyte token_max, token_count;

	LOW_BYTE(parse, token_max)
	parse++;
	LOW_BYTE(parse, token_count)

	if (token_count < token_max) {	/* sufficient space left for token? */
		storeb (parse++, token_count + 1);

		load_string((zword) (text + from), length);
		addr = lookup_text (0x05, dct);

		if (addr != 0 || !flag) {
			parse += 4 * token_count;

			storew((zword)(parse + 0), addr);
			storeb((zword)(parse + 2), length);
			storeb((zword)(parse + 3), from);
		}
	}
}

/**
 * Split an input line into words and translate the words to tokens.
 */
void FrotzInterpreter::tokenise_line (zword text, zword token, zword dct, bool flag) {
	zword addr1;
	zword addr2;
	zbyte length;
	zbyte c;

	length = 0;		/* makes compilers shut up */

	/* Use standard dictionary if the given dictionary is zero */
	if (dct == 0)
		dct = _h.h_dictionary;

	/* Remove all tokens before inserting new ones */
	storeb ((zword) (token + 1), 0);

	/* Move the first pointer across the text buffer searching for the
	   beginning of a word. If this succeeds, store the position in a
	   second pointer. Move the first pointer searching for the end of
	   the word. When it is found, "tokenise" the word. Continue until
	   the end of the buffer is reached. */
	addr1 = text;
	addr2 = 0;

	if (_h.h_version >= V5) {
		addr1++;
		LOW_BYTE(addr1, length)
	}

	do {
		zword sep_addr;
		zbyte sep_count;
		zbyte separator;

		/* Fetch next ZSCII character */
		addr1++;

		if (_h.h_version >= V5 && addr1 == text + 2 + length)
			c = 0;
		else
			LOW_BYTE(addr1, c)

		/* Check for separator */
		sep_addr = dct;
		LOW_BYTE(sep_addr, sep_count)
		++sep_addr;

		do {
			LOW_BYTE(sep_addr, separator)
			sep_addr++;
		} while (c != separator && --sep_count != 0);

		/* This could be the start or the end of a word */
		if (sep_count == 0 && c != ' ' && c != 0) {
			if (addr2 == 0)
			addr2 = addr1;
		} else if (addr2 != 0) {
			tokenise_text(text, (zword)(addr1 - addr2), (zword)(addr2 - text), token, dct, flag);
			addr2 = 0;
		}

		/* Translate separator (which is a word in its own right) */

		if (sep_count != 0)
			tokenise_text(text, (zword)(1), (zword)(addr1 - text), token, dct, flag );

	} while (c != 0);
}

/*
 * completion
 *
 * Scan the vocabulary to complete the last word on the input line
 * (similar to "tcsh" under Unix). The return value is
 *
 *	2 ==> completion is impossible
 *	1 ==> completion is ambiguous
 *	0 ==> completion is successful
 *
 * The function also returns a string in its second argument. In case
 * of 2, the string is empty; in case of 1, the string is the longest
 * extension of the last word on the input line that is common to all
 * possible completions (for instance, if the last word on the input
 * is "fo" and its only possible completions are "follow" and "folly"
 * then the string is "ll"); in case of 0, the string is an extension
 * to the last word that results in the only possible completion.
 *
 */
int FrotzInterpreter::completion(const zword *zbuffer, zword *result) {
	zword minaddr;
	zword maxaddr;
	zword *ptr;
	zword c;
	int len;
	int i;

	*result = 0;
	if (resolution == 0) 
		find_resolution();

	/* Copy last word to "decoded" string */
	len = 0;
	while ((c = *zbuffer++) != 0)

	if (c != ' ') {
		if (len < 3 * resolution)
		decoded[len++] = c;
	} else 
		len = 0;

	decoded[len] = 0;

	/* Search the dictionary for first and last possible extensions */
	minaddr = lookup_text (0x00, _h.h_dictionary);
	maxaddr = lookup_text (0x1f, _h.h_dictionary);

	if (minaddr == 0 || maxaddr == 0 || minaddr > maxaddr)
		return 2;

	/* Copy first extension to "result" string */
	decode_text(VOCABULARY, minaddr);

	ptr = result;
	for (i = len; (c = decoded[i]) != 0; i++)
		*ptr++ = c;
	*ptr = 0;

	/* Merge second extension with "result" string */
	decode_text(VOCABULARY, maxaddr);

	for (i = len, ptr = result; (c = decoded[i]) != 0; i++, ptr++)
		if (*ptr != c) break;
	*ptr = 0;

	/* Search was ambiguous or successful */
	return (minaddr == maxaddr) ? 0 : 1;
}

/**
 * Z-code interpreter main loop
 */
void FrotzInterpreter::interpret(void) {
	while (!_quitFlag && (process_finished == 0)) {
		manageEvents();

		zbyte opcode;
		uint32 offset;
		GET_PC(offset);
		CODE_BYTE(opcode)

		debugC(MESSAGE_DETAILED, Gargoyle::kGargoyleDebugScripts, "Offset %xh, opcode %xh\n", offset, opcode);

		zargc = 0;

		if (opcode < 0x80) {			/* 2OP opcodes */
			load_operand((zbyte)(opcode & 0x40) ? 2 : 1);
			load_operand((zbyte)(opcode & 0x20) ? 2 : 1);

			CALL_MEMBER_FN(*this, var_opcodes[opcode & 0x1f])();

		} else if (opcode < 0xb0) {		/* 1OP opcodes */
			load_operand((zbyte)(opcode >> 4));
			CALL_MEMBER_FN(*this, op1_opcodes[opcode & 0x0f])();

		} else if (opcode < 0xc0) {		/* 0OP opcodes */
			CALL_MEMBER_FN(*this, op0_opcodes[opcode - 0xb0])();

		} else {				/* VAR opcodes */
			zbyte specifier1;
			zbyte specifier2;

			if (opcode == 0xec || opcode == 0xfa) {		/* opcodes 0xec */
				CODE_BYTE(specifier1)					/* and 0xfa are */
				CODE_BYTE(specifier2)					/* call opcodes */
				load_all_operands(specifier1);			/* with up to 8 */
				load_all_operands(specifier2);			/* arguments	*/
			} else {
				CODE_BYTE(specifier1)
				load_all_operands(specifier1);
			}

			CALL_MEMBER_FN(*this, var_opcodes[opcode - 0xc0])();
		}

/*#if defined(DJGPP) && defined(SOUND_SUPPORT)
		if (end_of_sound_flag)
			end_of_sound();
#endif*/

		_os.tick();
	}

	--process_finished;
}

} // end of namespace Frotz
