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

#ifndef FROTZ_INTERP_H
#define FROTZ_INTERP_H

#include "common/scummsys.h"
#include "common/file.h"
#include "common/random.h"
#include "common/savefile.h"
#include "common/stack.h"
#include "common/stream.h"
#include "common/util.h"
#include "engines/engine.h"

#include "gargoyle/frotz.h"
#include "gargoyle/frotz_gfx.h"
#include "gargoyle/frotz_interp.h"
#include "gargoyle/frotz_sound.h"
#include "gargoyle/gargoyle.h"

namespace Frotz {

#define MAX_OBJECT 2000

#define O1_PARENT 4
#define O1_SIBLING 5
#define O1_CHILD 6
#define O1_PROPERTY_OFFSET 7
#define O1_SIZE 9

#define O4_PARENT 6
#define O4_SIBLING 8
#define O4_CHILD 10
#define O4_PROPERTY_OFFSET 12
#define O4_SIZE 14

enum string_type {
	LOW_STRING, ABBREVIATION, HIGH_STRING, EMBEDDED_STRING, VOCABULARY
};

enum FrotzGameType {GT_INFOCOM, GT_ZCODE};

#define MAX_NESTING 16
struct RedirectRecord {
	zword xsize;
	zword table;
	zword width;
	zword total;
};

enum FontType {TEXT_FONT = 1, PICTURE_FONT = 2, GRAPHICS_FONT = 3, FIXED_WIDTH_FONT = 4};

#define S_GFX_CHAR_FONT "charGfx"

struct screen_data2 {
	// Keep these in order, as they get accessed using a zword * array
	zword y_pos;
	zword x_pos;
	zword y_size;
	zword x_size;
	zword y_cursor;
	zword x_cursor;
	zword left;
	zword right;
	zword nl_routine;
	zword nl_countdown;
	zword style;
	zword colour;
	zword fontId;
	zword font_size;
	zword attribute;
	zword line_count;
};


struct screen_data {
	union {
		screen_data2 sd;
		zword arr[16];
	};

	// Extra fields
	zword true_fore;
	zword true_back;
	FontType font;
};

class FrotzInterpreter;

class ZHeaderData {
public:
	ZHeaderData();

//	char *story_name;
	long story_size;
	enum story story_id;

	char save_name[MAX_FILE_NAME + 1];
	char script_name[MAX_FILE_NAME + 1];
	char command_name[MAX_FILE_NAME + 1];
	char auxilary_name[MAX_FILE_NAME + 1];

	/*** Story file header data ***/
	zbyte h_version;
	zbyte h_config;
	zword h_release;
	zword h_resident_size;
	zword h_start_pc;
	zword h_dictionary;
	zword h_objects;
	zword h_globals;
	zword h_dynamic_size;
	zword h_flags;
	zbyte h_serial[6];
	zword h_abbreviations;
	zword h_file_size;
	zword h_checksum;
	zbyte h_interpreter_number;
	zbyte h_interpreter_version;
	zbyte h_screen_rows;
	zbyte h_screen_cols;
	zword h_screen_width;
	zword h_screen_height;
	zbyte h_font_height;
	zbyte h_font_width;
	zword h_functions_offset;
	zword h_strings_offset;
	zbyte h_default_background;
	zbyte h_default_foreground;
	zword h_terminating_keys;
	zword h_line_width;
	zbyte h_standard_high;
	zbyte h_standard_low;
	zword h_alphabet;
	zword h_extension_table;
	zbyte h_user_name[8];

	zword hx_table_size;
	zword hx_mouse_x;
	zword hx_mouse_y;
	zword hx_unicode_table;
	zword hx_flags;
	zword hx_fore_colour;
	zword hx_back_colour;
	Common::Point mousePos;
};

class ZSystem {
private:
	Gargoyle::GargoyleEngine *_engine;
	FrotzInterpreter &_interp;
	Gargoyle::ScreenSurface &_screen;
	ZHeaderData &_h;
	FrotzPictures *_frotzGfx;
	FrotzSound *_frotzSound;
	bool _exitPause;
	bool _tandyFlag;
	bool _morePrompts;
	bool _fastScrolling;
	int _gfxScaling;
	Common::String _username;

	Common::Stack<char> _scrollback;

	int translateKeypress(int keypress);
public:
	ZSystem(ZHeaderData &headerData, FrotzInterpreter &interp);
	virtual ~ZSystem();

	FrotzSound *getSound() { return _frotzSound; }

	void beep(int number);
	void display_char(zchar c);
	void display_string(const zchar *s);
	void erase_area(int top, int left, int bottom, int right, int win);
	void fatal(const char *s);
	int font_data(FontType font, int *height, int *width);
	int read_file_name(char *file_name, const char *default_name, int flag);
	void init_screen(void);
	void more_prompt(void);
	void process_arguments(int argc, char *argv[]);
	zword read_line(int max, zchar *buf, int timeout, int width, int continued);
	zchar read_key(int timeout, int cursor);
	zword read_mouse(void);
	void reset_screen(void);
	void scroll_area(int top, int left, int bottom, int right, int units);
	uint32 get_colour(int index);
	void set_colour(int new_foreground, int new_background);
	int from_true_colour(zword colour);
	zword to_true_colour(int index);
	void set_cursor(int row, int col);
	void set_font(int new_font);
	void set_text_style(int new_style);
	int string_width(const zchar *s);
	int char_width(zword c);
	int check_unicode(int font, zword c);
	int peek_colour(void);
	int picture_data(int picture, int *height, int *width);
	void draw_picture(int picture, int y, int x);
	int random_seed(void);
	void restart_game(int stage);
	bool path_open(Common::File &f, const char *name);
	void finish_with_sample(int number);
	void prepare_sample(int number);
	void start_sample(int number, int volume, int repeats, zword eos);
	void stop_sample(int number);
	void  scrollback_char(zchar c);
	void scrollback_erase(int erase);
	void tick(void);
	int buffer_screen(int mode);
	int wrap_window(int win);
	void window_height(int win, int height);

	static uint32 RGB5_to_RGB(zword c);
};

class FrotzInterpreter: public Gargoyle::Interpreter {
private:
	ZHeaderData _h;
	ZSystem _os;
	Common::RandomSource _rnd;
	Common::SeekableReadStream *_gameFile;
	Graphics::Font *_gfxFont;

	/*** Various data ***/

	zword stack[STACK_SIZE];
	zword *sp;
	zword *fp;
	zword frame_count;

	zword zargs[8];
	int zargc;

	bool ostream_screen;
	bool ostream_script;
	bool ostream_memory;
	bool ostream_record;
	bool istream_replay;
	bool message;

	int cwin;
	int mwin;

	int mouse_x;
	int mouse_y;
	int menu_selected;

	bool enable_wrapping;
	bool enable_scripting;
	bool enable_scrolling;
	bool enable_buffering;

	int option_attribute_assignment;
	int option_attribute_testing;
	int option_object_locating;
	int option_object_movement;
	int option_context_lines;
	int option_left_margin;
	int option_right_margin;
	int option_ignore_errors;
	int option_piracy;
	int option_undo_slots;
	int option_expand_abbreviations;
	int option_script_cols;
	int option_save_quetzal;
	int option_sound;	/* dg */
	char *option_zcode_path;	/* dg */

	long reserve_mem;

	/* Definitions for error handling functions and error codes. */
	int err_report_mode;
	int error_count[ERR_NUM_ERRORS];

	// Buffer fields
	zchar main_buffer[TEXT_BUFFER_SIZE];
	int bufpos;
	zword bufprev_c;
	bool buffer_locked;
	// Files fields
	int script_width;
	Common::OutSaveFile *rfp, *sfp;
	Common::InSaveFile *pfp;
	// Fastmem fields
	zbyte *zmp;
	zbyte *pcp;
	bool first_restart;
	long init_fp_pos;
	undo_t *first_undo, *last_undo, *curr_undo;
	zbyte *undo_mem, *prev_zmp, *undo_diff;
	int undo_count;
	// Process fields
	int process_finished;
	// Random fields
	int random_interval;
	int random_counter;
	// Redirect fields
	int redirect_depth;
	RedirectRecord redirect[MAX_NESTING];
	// Screen fields
	int font_height;
	int font_width;
	bool input_redraw;
	bool more_prompts;
	bool discarding;
	bool cursor;
	int input_window;
	screen_data wp[8];
	screen_data *cwp;
	// Sound fields
	int next_sample;
	int next_volume;
	bool sound_locked;
	bool playing;
	// Text fields
	zword *decoded;
	zword *encoded;
	int resolution;

private:
	void initInterpVars();
	void init_fonts();
	void init_palette();
	// Buffer methods
	void init_buffer(void);
	void flush_buffer(void);
	void new_line(void);
	void print_char(zword c);
	// Error methods
	void init_err(void);
	void runtime_error(int errnum);
	void print_long(unsigned long value, int base);
	// Fastmem methods
	zword get_header_extension(int entry);
	void set_header_extension(int entry, zword val);
	void restart_header(void);
	void init_memory(void);
	void init_undo(void);
	void free_undo(int count);
	void reset_memory(void);
	void storeb(zword addr, zbyte value);
	void storew (zword addr, zword value);
	void get_default_name(char *default_name, zword addr);
	int restore_undo(void);
	int save_undo(void);
	// Files methods
	void init_files();
	void script_open(void);
	void script_close(void);
	void script_new_line(void);
	void script_char(zword c);
	void script_word(const zchar *s);
	void script_write_input(const zchar *buf, zword key);
	void script_erase_input(const zchar *buf);
	void script_mssg_on(void);
	void script_mssg_off(void);
	void record_open(void);
	void record_close(void);
	void record_code(int c, bool force_encoding);
	void record_char(zword c);
	void record_write_key(zword key);
	void record_write_input (const zchar *buf, zword key);
	void replay_open(void);
	void replay_close(void);
	int replay_code(void);
	zword replay_char(void);
	zword replay_read_key(void);
	zword replay_read_input(zchar *buf);
	// Hotkey methods
	bool hot_key_debugging(void);
	bool hot_key_help(void);
	bool hot_key_playback(void);
	bool hot_key_recording(void);
	bool hot_key_seed(void);
	bool hot_key_undo(void);
	bool hot_key_restart(void);
	bool hot_key_quit(void);
	bool handle_hot_key(zword key);
	// Input methods
	bool is_terminator(zword key);
	bool read_yes_or_no(const char *s);
	void read_string(int max, zchar *buffer);
	int read_number(void);
	// Object methods
	zword object_address(zword obj);
	zword object_name(zword object);
	zword first_property (zword obj);
	zword next_property(zword prop_addr);
	void unlink_object(zword object);
	// Process methods
	void init_process(void);
	void load_operand(zbyte type);
	void load_all_operands(zbyte specifier);
	void interpret(void);
	void call(zword routine, int argc, zword *args, int ct);
	void ret(zword value);
	void branch(bool flag); 
	void store(zword value);
	int direct_call(zword addr);
	// Random methods
	void seed_random (int value);
	// Redirect methods
	void memory_open(zword table, zword xsize, bool buffering);
	void memory_new_line(void);
	void memory_word(const zchar *s);
	void memory_close(void);
	// Screen methods
	void init_screen();
	zword winarg0(void);
	zword winarg2(void);
	void update_cursor(void);
	void reset_cursor(zword win);
	bool amiga_screen_model(void);
	void set_more_prompts(bool flag);
	int units_left(void);
	zword get_max_width(zword win);
	void countdown(void);
	void screen_new_line(void);
	void screen_char(zword c);
	void screen_word(const zchar *s);
	void screen_write_input(const zchar *buf, zword key);
	void screen_erase_input(const zchar *buf);
	zword console_read_input(int max, zchar *buf, zword timeout, bool continued);
	zword console_read_key(zword timeout);
	void update_attributes(void);
	void refresh_text_style(void);
	void set_window(zword win);
	void erase_window(zword win);
	void split_window(zword height);
	void erase_screen(zword win);
	void resize_screen(void);
	void restart_screen(void);
	bool validate_click(void);
	void screen_mssg_on(void);
	void screen_mssg_off(void);
	void pad_status_line (int column);
	void get_window_colours(zword win, zbyte* fore, zbyte* back);
	zword get_window_font(zword win);
	int colour_in_use(zword colour);
	zword get_current_window(void);
	// Sound methods
	void init_sound(void);
	void start_sample(int number, int volume, int repeats, zword eos);
	void start_next_sample(void);
	void end_of_sound(zword routine);
	// Stream methods
	void scrollback_char(zchar c);
	void scrollback_word(const zchar *s);
	void scrollback_write_input(const zchar *buf, zword key);
	void scrollback_erase_input(const zchar *buf);
	void stream_mssg_on(void);
	void stream_mssg_off(void);
	void stream_char(zchar c);
	void stream_word(const zchar *s);
	void stream_new_line(void);
	zword stream_read_key(zword timeout, zword routine, bool hot_keys);
	zword stream_read_input(int max, zchar *buf, zword timeout,  zword routine, 
		bool hot_keys, bool no_scripting);
	// Text functions
	void init_text(void);
	zword translate_from_zscii(zbyte c);
	zbyte unicode_to_zscii(zword c);
	zbyte translate_to_zscii (zword c);
	zword alphabet(int set, int index);
	void find_resolution(void);
	void load_string (zword addr, zword length);
	void encode_text(int padding);
	void decode_text(enum string_type st, zword addr);
	void print_num(zword value);
	void print_object(zword object);
	void print_string(const char *s);
	zword lookup_text(int padding, zword dct);
	void tokenise_text(zword text, zword length, zword from, zword parse, zword dct, bool flag); 
	void tokenise_line(zword text, zword token, zword dct, bool flag);
	int completion(const zword *buffer, zword *result);
	bool load_game(Common::ReadStream *saveFile);
	bool save_game(Common::WriteStream *saveFile);
public:
	// Z functions
	void z_restart(void);
	void z_restore(void);
	void z_save_undo(void);
	void z_verify(void);
	void z_restore_undo(void);
	void z_save(void);
	void z_sound_effect(void);
	void z_check_unicode(void);
	void z_encode_text(void);
	void z_new_line(void);
	void z_print(void);
	void z_print_addr(void);
	void z_print_char(void);
	void z_print_form(void);
	void z_print_num(void);
	void z_print_obj(void);
	void z_print_paddr(void);
	void z_print_ret(void);
	void z_print_unicode(void);
	void z_tokenise(void);
	void z_output_stream(void);
	void z_input_stream(void);
	void z_make_menu(void);
	void z_read(void);
	void z_read_char(void);
	void z_read_mouse(void);
	void z_clear_attr(void);
	void z_jin(void);
	void z_get_child(void);
	void z_get_next_prop(void);
	void z_get_parent(void);
	void z_get_prop(void);
	void z_get_prop_addr(void);
	void z_get_prop_len(void);
	void z_get_sibling(void);
	void z_insert_obj(void);
	void z_put_prop(void);
	void z_remove_obj(void);
	void z_set_attr(void);
	void z_test_attr(void);
	void z_buffer_mode(void);
	void z_draw_picture(void);
	void z_erase_line(void);
	void z_erase_picture(void);
	void z_erase_window(void);
	void z_get_cursor(void);
	void z_get_wind_prop(void);
	void z_mouse_window(void);
	void z_move_window(void);
	void z_picture_table(void);
	void z_picture_data(void);
	void z_buffer_screen(void);
	void z_print_table(void);
	void z_put_wind_prop(void);
	void z_scroll_window(void);
	void z_set_colour(void);
	void z_set_true_colour(void);
	void z_set_font(void);
	void z_set_cursor(void);
	void z_set_margins(void);
	void z_set_text_style(void);
	void z_set_window(void);
	void z_show_status(void);
	void z_split_window(void);
	void z_window_size(void);
	void z_window_style(void);
	void z_copy_table(void);
	void z_loadb(void);
	void z_loadw(void);
	void z_scan_table(void);
	void z_storeb(void);
	void z_storew(void);
	void z_random(void);

	// Z Opcode functions
	void __extended__(void);
	void __illegal__(void);
	void z_catch(void);
	void z_throw(void);
	void z_call_n(void);
	void z_call_s(void);
	void z_check_arg_count(void);
	void z_jump(void);
	void z_nop(void);
	void z_quit(void);
	void z_ret(void);
	void z_ret_popped(void);
	void z_rfalse(void);
	void z_rtrue(void);
	void z_add(void);
	void z_and(void);
	void z_art_shift(void);
	void z_div(void);
	void z_je(void);
	void z_jg(void);
	void z_jl(void);
	void z_jz(void);
	void z_log_shift(void);
	void z_mod(void);
	void z_mul(void);
	void z_not(void);
	void z_or(void);
	void z_sub(void);
	void z_test(void);
	void z_dec(void);
	void z_dec_chk(void);
	void z_inc(void);
	void z_inc_chk(void);
	void z_load(void);
	void z_pop(void);
	void z_pop_stack(void);
	void z_pull(void);
	void z_push(void);
	void z_push_stack(void);
	void z_store(void);
	void z_piracy(void);

public:
	FrotzInterpreter(Gargoyle::GargoyleEngine &engine);
	virtual ~FrotzInterpreter();
	void go();

	static bool validate_savegame(Common::ReadStream *saveFile, char *gameDescription);
	static bool validateFile(const Common::FSNode &file, char *target, char *version, Common::Language &lang);
	static bool identifyGame(const Common::String &target, Gargoyle::GargoyleGameId &gameId);

protected:
	// Debugger methods
	void initDebugger();
	bool cmd_sound(int argc, const char **argv);
};

} // End of namespace Frotz

#endif

