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

#include "gargoyle/frotz_interp.h"
#include "gargoyle/frotz_sound.h"
#include "gargoyle/frotz_static.h"
#include "gargoyle/blorb.h"
#include "gargoyle/graphics.h"

#include "common/config-manager.h"
#include "engines/engine.h"
#include "graphics/fontman.h"
#include "graphics/font.h"
#include "gui/message.h"

namespace Frotz {

ZHeaderData::ZHeaderData() {
	h_version = 0;
	h_config = 0;
	h_release = 0;
	h_resident_size = 0;
	h_start_pc = 0;
	h_dictionary = 0;
	h_objects = 0;
	h_globals = 0;
	h_dynamic_size = 0;
	h_flags = 0;
	Common::set_to(&h_serial[0], &h_serial[6], 0);
	h_abbreviations = 0;
	h_file_size = 0;
	h_checksum = 0;
	h_interpreter_number = 0;
	h_interpreter_version = 0;
	h_screen_rows = 0;
	h_screen_cols = 0;
	h_screen_width = 0;
	h_screen_height = 0;
	h_font_height = 1;
	h_font_width = 1;
	h_functions_offset = 0;
	h_strings_offset = 0;
	h_default_background = 0;
	h_default_foreground = 0;
	h_terminating_keys = 0;
	h_line_width = 0;
	h_standard_high = 1;
	h_standard_low = 1;
	h_alphabet = 0;
	h_extension_table = 0;
	Common::set_to(&h_user_name[0], &h_user_name[8], 0);

	hx_table_size = 0;
	hx_mouse_x = 0;
	hx_mouse_y = 0;
	hx_unicode_table = 0;
	hx_flags = 0;
	hx_fore_colour = 0;
	hx_back_colour = 0;
}

ZSystem::ZSystem(ZHeaderData &headerData, FrotzInterpreter &interp): _engine((Gargoyle::GargoyleEngine *) g_engine),
			_interp(interp), _screen(interp.screen()), _h(headerData), _exitPause(false) {

	_frotzGfx = NULL;
	_frotzSound = NULL;

	if (ConfMan.getDomain(FROTZ_DOMAIN) != NULL) {
		_tandyFlag = ConfMan.getBool("tandy_bit", FROTZ_DOMAIN);
		_username = ConfMan.get("username", FROTZ_DOMAIN);
		_gfxScaling = ConfMan.getInt("gfx_scaling", FROTZ_DOMAIN);
	} else {
		_tandyFlag = false;
		_username = "";
		_gfxScaling = 2;
	}

	if (ConfMan.getDomain(GARGOYLE_DOMAIN) != NULL) {
		_morePrompts = ConfMan.getBool("more_prompts", GARGOYLE_DOMAIN);
		_fastScrolling = ConfMan.getBool("fast_scrolling", GARGOYLE_DOMAIN);
	} else {
		_morePrompts = true;
		_fastScrolling = true;
	}
}

ZSystem::~ZSystem() {
	if (_frotzGfx) delete _frotzGfx;
	if (_frotzSound) delete _frotzSound;
}

/**
 * Play a beep sound. Ideally, the sound should be high- (number == 1)
 * or low-pitched (number == 2).
 */
void ZSystem::beep(int number) {
/*  _screen.flushDisplay();
  ::MessageBeep((number == 1) ? MB_ICONASTERISK : MB_OK);
  */
}

/**
 * Display a character of the current font using the current colours and
 * text style. The cursor moves to the next position. Printable codes are
 * all ASCII values from 32 to 126, ISO Latin-1 characters from 160 to
 * 255, ZC_GAP (gap between two sentences) and ZC_INDENT (paragraph
 * indentation), and Unicode characters above 255. The screen should not
 * be scrolled after printing to the bottom right corner.
 */
void ZSystem::display_char(zchar c) {
	if (c == ZC_INDENT) {
		display_char(' ');
		display_char(' ');
		display_char(' ');
	} else if (c == ZC_GAP) {
		display_char(' ');
		display_char(' ');
	} else if (_interp.isValidChar(c)) {
		_screen.addChar(c);
		_exitPause = true;
	}
}

/**
 * Pass a string of characters to display_char.
 */
void ZSystem::display_string(const zchar *s) {
	zword c;
	while ((c = *s++) != 0) {
		if ((c == ZC_NEW_FONT) || (c == ZC_NEW_STYLE)) {
			int arg = *s++;

			if (c == ZC_NEW_FONT)
				set_font(arg);
			if (c == ZC_NEW_STYLE)
				set_text_style(arg);
		} else
			display_char(c);
	}
}

/**
 * Fill a rectangular area of the screen with the current background
 * colour. Top left coordinates are (1,1). The cursor does not move.
 *
 * The final argument gives the window being changed, -1 if only a
 * portion of a window is being erased, or -2 if the whole screen is
 * being erased.
 */
void ZSystem::erase_area(int top, int left, int bottom, int right, int win) {
	_screen.flushText();
	_screen.fillRect(Common::Rect(left, top, right, bottom), 
		_screen.getTextSettings().background());
}

/**
 * Display error message and stop interpreter.
 */
void ZSystem::fatal(const char *s) {
	GUI::MessageDialog dialog(s);
	dialog.runModal();

	_interp.quit();
}

/**
 * Return true if the given font is available. The font can be
 *
 *    TEXT_FONT
 *    PICTURE_FONT
 *    GRAPHICS_FONT
 *    FIXED_WIDTH_FONT
 *
 * The font size should be stored in "height" and "width". If
 * the given font is unavailable then these values must _not_
 * be changed.
 */
int ZSystem::font_data(FontType font, int *height, int *width) {
	const Graphics::Font *f;
	Gargoyle::TextSettings &ts = _screen.getTextSettings();

	switch (font) {
	case TEXT_FONT:
		// Use the standard font
		f = ts.getFont(Gargoyle::FONT_STD);
		break;
	case GRAPHICS_FONT:
		// Infocom graphics font
		f = ts.getFont(S_GFX_CHAR_FONT);
		break;
	case FIXED_WIDTH_FONT:
		f = ts.getFont(Gargoyle::FONT_FIXED);
		break;
	default:
		return 0;
	}

	*height = f->getFontHeight();
	*width = f->getMaxCharWidth();
	return 1;
}

/**
 * Set the font for text output. The interpreter takes care not to
 * choose fonts which aren't supported by the interface.
 */
void ZSystem::set_font(int new_font) {
	_screen.flushText();
	Gargoyle::TextSettings &ts = _screen.getTextSettings();
	
	switch (new_font) {
	case TEXT_FONT:
		ts.setFont(Gargoyle::FONT_STD);
		break;

	case FIXED_WIDTH_FONT:
		ts.setFont(Gargoyle::FONT_FIXED);
		break;

	case GRAPHICS_FONT:
		// Infocom character font
		ts.setFont(S_GFX_CHAR_FONT);
		break;

	default:
		break;
	}
		
	_screen.applyTextSettings();
}

/**
 * Return the name of a file. Flag can be one of:
 *
 *    FILE_SAVE     - Save game file
 *    FILE_RESTORE  - Restore game file
 *    FILE_SCRIPT   - Transcript file
 *    FILE_RECORD   - Command file for recording
 *    FILE_PLAYBACK - Command file for playback
 *    FILE_SAVE_AUX - Save auxiliary ("preferred settings") file
 *    FILE_LOAD_AUX - Load auxiliary ("preferred settings") file
 *
 * The length of the file name is limited by MAX_FILE_NAME. Ideally
 * an interpreter should open a file requester to ask for the file
 * name. If it is unable to do that then this function should call
 * print_string and read_string to ask for a file name.
 */
int ZSystem::read_file_name(char *file_name, const char *default_name, int flag) {
	error("ZSystem::read_file_name is deprecated");
}

/**
 * Initialise the IO interface. Prepare screen and other devices
 * (mouse, sound card). Set various OS depending story file header
 * entries:
 *
 *     _h.h_config (aka flags 1)
 *     _h.h_flags (aka flags 2)
 *     h_screen._cols (aka screen width in characters)
 *     h_screen._rows (aka screen height in lines)
 *     h_screen._width
 *     h_screen._height
 *     _h.h_font_height (defaults to 1)
 *     _h.h_font_width (defaults to 1)
 *     _h.h_default_foreground
 *     _h.h_default_background
 *     _h.h_interpreter_number
 *     _h.h_interpreter_version
 *     _h.h_user_name (optional; not used by any game)
 *
 * Finally, set reserve_mem to the amount of memory (in bytes) that
 * should not be used for multiple undo and reserved for later use.
 */
void ZSystem::init_screen(void) {
	// Set the configuration
	if (_h.h_version == V3) {
		_h.h_config |= CONFIG_SPLITSCREEN;
		_h.h_config |= CONFIG_PROPORTIONAL;

		if (_tandyFlag)
			_h.h_config |= CONFIG_TANDY;
		else
			_h.h_config &= ~CONFIG_TANDY;
	}
	if (_h.h_version >= V4) {
		_h.h_config |= CONFIG_BOLDFACE;
		_h.h_config |= CONFIG_EMPHASIS;
		_h.h_config |= CONFIG_FIXED;
		_h.h_config |= CONFIG_TIMEDINPUT;
	}
	if (_h.h_version >= V5)
		_h.h_config |= CONFIG_COLOUR;
	if (_h.h_version == V6) {
		if (_interp.hasBlorbFile()) {
			_h.h_config |= CONFIG_PICTURES;
			_h.h_config |= CONFIG_SOUND;
		}
	}

	// Get the defined user name
	Common::set_to(&_h.h_user_name[0], &_h.h_user_name[8], 0);
	strncpy((char *)&_h.h_user_name[0], _username.c_str(), 8);

	_h.h_interpreter_version = 'F';
	if (_h.h_version == V6) {
		_h.h_default_foreground = 12;
		_h.h_default_background = 10;
	} else {
		_h.h_default_foreground = 14;
		_h.h_default_background = 15;
	}

	// Set the low two palette entries to copy the selected foreground/background colours
	DEFAULT_COLOUR_LIST[0] = DEFAULT_COLOUR_LIST[_h.h_default_background];
	DEFAULT_COLOUR_LIST[1] = DEFAULT_COLOUR_LIST[_h.h_default_foreground];
	g_system->setPalette((const byte *)&DEFAULT_COLOUR_LIST[0], 0, 2);

	Common::Point wndSize(_screen.width(), _screen.height());
	_screen.fillRect(Common::Rect(0, 0, wndSize.x, wndSize.y), _h.h_default_background);

//	_screen.getTextSettings().setStyle(FIXED_WIDTH_STYLE);
	set_font(FIXED_WIDTH_FONT);

	if (_h.story_id == BEYOND_ZORK) {
		// For Beyond Zork, we'll be forcing the ScummVM 6x12 font to be 8x12, so as to match the character
		// graphics font. This is needed for the room description and maps to display correctly
		_screen.getTextSettings().setMinCharBounds(8, 0);
	}

	// Get the font dimensions
	_h.h_font_width = (zbyte)_screen.getTextSettings().getCharWidth();
	_h.h_font_height = (zbyte)_screen.getTextSettings().getFontHeight();

	_h.h_screen_width = (zword)wndSize.x;
	_h.h_screen_height = (zword)wndSize.y;
	_h.h_screen_cols = (zbyte)(_h.h_screen_width / _h.h_font_width);
	_h.h_screen_rows = (zbyte)(_h.h_screen_height / _h.h_font_height);

	// Set the interpreter
	_h.h_interpreter_number = INTERP_MSDOS;
	_h.h_interpreter_version = 'F';

	// Check for sound
	if ((_h.h_version == V3) && (_h.h_flags & OLD_SOUND_FLAG)) {
		_frotzSound = FrotzSound::init(_interp.getBlorbFile(), _engine->getGameDataDir().getChild("sound"));
		if (_frotzSound != NULL)
			_h.h_flags &= ~OLD_SOUND_FLAG;
	} else if ((_h.h_version >= V4) && (_h.h_flags & SOUND_FLAG)) {
		_frotzSound = FrotzSound::init(_interp.getBlorbFile(), _engine->getGameDataDir().getChild("sound"));
		if (_frotzSound != NULL)
			_h.h_flags &= ~SOUND_FLAG;
	}

	if (_h.h_version >= V5) {
		zword mask = 0;

		if (_h.h_version == V6)
			mask |= TRANSPARENT_FLAG;

		// Mask out any unsupported bits in the extended flags
		_h.hx_flags &= mask;

		_h.hx_fore_colour = _interp.getDefaultColour(true);
		_h.hx_back_colour = _interp.getDefaultColour(false);
	}

	// Setup the graphics
	if (FrotzPictures::picturesExists(_engine->getFilename()))
		_frotzGfx = new FrotzPictures(_engine, Common::Point(_h.h_screen_width, _h.h_screen_height), 
			_h.h_version);
}

/**
 * Display a MORE prompt, wait for a keypress and remove the MORE
 * prompt from the screen.
 */
void ZSystem::more_prompt(void) {
	if (_morePrompts) {
		_screen.flushText();
		//_screen.ResetOverhang();

		// Save the current text position
		Common::Point point = _screen.getTextPosition();

		// Show a [More] prompt
		_screen.writeString(sysString(_engine->getLanguage(), IDS_MORE));
		_screen.drawCursor(true);
		_screen.flushDisplay();

		// Wait for a key press
		_interp.waitForPress();

		// Remove the [More] prompt
		_screen.fillRect(Common::Rect(point.x, point.y, 
			_screen.getTextPosition().x, point.y + _screen.getTextSettings().getFontHeight()), 
			_screen.getTextSettings().background());
		_screen.drawCursor(false);

		// Restore the current text position
		_screen.setTextPosition(point);
	}
}

/**
 * Handle command line switches. Some variables may be set to activate
 * special features of Frotz:
 *
 *     option_attribute_assignment
 *     option_attribute_testing
 *     option_context_lines
 *     option_object_locating
 *     option_object_movement
 *     option_left_margin
 *     option_right_margin
 *     option_ignore_errors
 *     option_piracy
 *     option_undo_slots
 *     option_expand_abbreviations
 *     option_script_cols
 *
 * The global pointer "story_name" is set to the story file name.
 */
void ZSystem::process_arguments(int argc, char *argv[]) {
	// Set default filenames
	Common::String filename = _engine->getFilename();
	while ((filename.size() > 0) && (filename[filename.size() - 1] != '.'))
		filename.deleteLastChar();

	if (filename.size() > 0) {
		strcpy(_h.save_name, filename.c_str());
		strcpy(_h.script_name,filename.c_str());
		strcpy(_h.command_name,filename.c_str());
		strcpy(_h.auxilary_name,filename.c_str());

		strcat(_h.save_name, ".sav");
		strcat(_h.script_name, ".log");
		strcat(_h.command_name, ".rec");
		strcat(_h.auxilary_name, ".aux");
	}
}

/**
 * Read a line of input from the keyboard into a buffer. The buffer
 * may already be primed with some text. In this case, the "initial"
 * text is already displayed on the screen. After the input action
 * is complete, the function returns with the terminating key value.
 * The length of the input should not exceed "max" characters plus
 * an extra 0 terminator.
 *
 * Terminating keys are the return key (13) and all function keys
 * (see the Specification of the Z-machine) which are accepted by
 * the is_terminator function. Mouse clicks behave like function
 * keys except that the mouse position is stored in global variables
 * "mouse_x" and "mouse_y" (top left coordinates are (1,1)).
 *
 * Furthermore, Frotz introduces some special terminating keys:
 *
 *     ZC_HKEY_PLAYBACK (Alt-P)
 *     ZC_HKEY_RECORD (Alt-R)
 *     ZC_HKEY_SEED (Alt-S)
 *     ZC_HKEY_UNDO (Alt-U)
 *     ZC_HKEY_RESTART (Alt-N, "new game")
 *     ZC_HKEY_QUIT (Alt-X, "exit game")
 *     ZC_HKEY_DEBUG (Alt-D)
 *     ZC_HKEY_HELP (Alt-H)
 *
 * If the timeout argument is not zero, the input gets interrupted
 * after timeout/10 seconds (and the return value is 0).
 *
 * The complete input line including the cursor must fit in "width"
 * screen units.
 *
 * The function may be called once again to continue after timeouts,
 * misplaced mouse clicks or hot keys. In this case the "continued"
 * flag will be set. This information can be useful if the interface
 * implements input line history.
 *
 * The screen is not scrolled after the return key was pressed. The
 * cursor is at the end of the input line when the function returns.
 *
 * Since Frotz 2.2 the helper function "completion" can be called
 * to implement word completion (similar to tcsh under Unix).
 */
zword ZSystem::read_line(int max, zchar *buf, int timeout, int width, int continued) {
	// For Beyond Zork, we display text using the ScummVM GUI 6x12 font as 8x12. Ensure this is turned off
	// for line reads, since we want the text input to still be at 6x12
	_screen.getTextSettings().setMinCharBounds(0, 0);

	int result = _interp.readLine((char *)buf, max, width, timeout * 100, continued);

	// Handle translating the terminating character/status to Frotz form
	result = translateKeypress(result);

	// Update the mouse position
	_h.mousePos.x = _interp.mousePos().x + 1;
	_h.mousePos.y = _interp.mousePos().y + 1;

	if (_h.story_id == BEYOND_ZORK)
		_screen.getTextSettings().setMinCharBounds(8, 0);

	return result;
}

/**
 *
 */
int ZSystem::translateKeypress(int keypress) {
	// Special cases
	switch (keypress) {
	case Gargoyle::GARG_KBD_QUIT:
		// Signal the quit
		keypress = ZC_HKEY_QUIT;
		break;
	case Gargoyle::GARG_KBD_TIMEOUT:
		// Timeout occurred
		keypress = ZC_TIME_OUT;
		break;
	case Gargoyle::GARG_MOUSE_CLICK:
		// Single click
		keypress = ZC_SINGLE_CLICK;
		break;
	case Gargoyle::GARG_MOUSE_DBLCLICK:
		// Double click
		keypress = ZC_DOUBLE_CLICK;
		break;
	case Common::KEYCODE_UP:
	case Common::KEYCODE_KP8:
		keypress = ZC_ARROW_UP;
		break;
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_KP2:
		keypress = ZC_ARROW_DOWN;
		break;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		keypress = ZC_RETURN;
		break;
	default:
		break;
	}

	return keypress;
}

/**
 * Read a single character from the keyboard (or a mouse click) and
 * return it. Input aborts after timeout/10 seconds.
 */
zchar ZSystem::read_key(int timeout, int cursor) {
	_screen.flushText();

	if (cursor)
		_screen.drawCursor(true);

	_screen.flushDisplay();
	
	int result;
	do {
		result = translateKeypress(_interp.readKey(timeout * 100));
	} while (result >= 256);

	_h.mousePos.x = _interp.mousePos().x + 1;
	_h.mousePos.y = _interp.mousePos().y + 1;
	return result;
}

/**
 * Store the mouse position in the global variables "mouse_x" and
 * "mouse_y", the code of the last clicked menu in "menu_selected"
 * and return the mouse buttons currently pressed.
 */
zword ZSystem::read_mouse(void) {
	// Get the mouse position
	_h.mousePos.x = _interp.mousePos().x + 1;
	_h.mousePos.y = _interp.mousePos().y + 1;

	// Get the mouse buttons
	zword btn = _interp.mouseButtons();

	return btn;
}

/**
 * Reset the screen before the program ends.
 */
void ZSystem::reset_screen(void) {
	_screen.flushDisplay();
	//_screen.ResetOverhang();

	if (_exitPause) {
		set_font(TEXT_FONT);
		set_text_style(0);
		_screen.nextLine();

		Common::String hit(sysString(_engine->getLanguage(), IDS_HIT_KEY_EXIT));
		for (uint i = 0; i < hit.size(); ++i)
			display_char((unsigned char)hit[i]);

		read_key(0,1);
	}
}

/**
 * Scroll a rectangular area of the screen up (units > 0) or down
 * (units < 0) and fill the empty space with the current background
 * colour. Top left coordinates are (1,1). The cursor stays put.
 */
void ZSystem::scroll_area(int top, int left, int bottom, int right, int units) {
	_screen.flushText();
	//_screen.ResetOverhang();

	_screen.scrollArea(Common::Rect(left - 1, top - 1, right, bottom), units);

	if (_fastScrolling)
		_screen.flushDisplay();
}

/**
 * Gets a specified standard Z-code colour
 */
uint32 ZSystem::get_colour(int index) {
	uint32 value;
	g_system->grabPalette((byte *)&value, index, 1);
	return value;
}

/**
 * set_colour
 *
 * Set the foreground and background colours which can be:
 *
 *     1
 *     BLACK_COLOUR
 *     RED_COLOUR
 *     GREEN_COLOUR
 *     YELLOW_COLOUR
 *     BLUE_COLOUR
 *     MAGENTA_COLOUR
 *     CYAN_COLOUR
 *     WHITE_COLOUR
 *     TRANSPARENT_COLOUR
 *
 *     Amiga only:
 *
 *     LIGHTGREY_COLOUR
 *     MEDIUMGREY_COLOUR
 *     DARKGREY_COLOUR
 *
 * There may be more colours in the range from 16 to 255; see the
 * remarks about peek_colour.
 */
void ZSystem::set_colour(int new_foreground, int new_background) {
	_screen.flushText();
	//_screen.ResetOverhang();

	if (new_foreground < 256)
		_screen.getTextSettings().fore = new_foreground;
	_screen.getTextSettings().foreDefault = (new_foreground == 1);

	if (new_background < 256)
		_screen.getTextSettings().back = new_background;

	_screen.getTextSettings().backDefault = (new_background == 1);
	_screen.getTextSettings().backTransparent = (new_background == 15);

	_screen.applyTextSettings();
}

/**
 * Given a true colour, return an appropriate colour index.
 */
int ZSystem::from_true_colour(zword colour) {
	return _interp.getColourIndex(RGB5_to_RGB(colour));
}

/**
 * Given a colour index, return the appropriate true colour.
 */
zword ZSystem::to_true_colour(int index) {
	error("ZSystem::to_true_colour");
	//return TrueToRGB5(_interp.getColour(index));
}

/**
 * Place the text cursor at the given coordinates. Top left is (1,1).
 */
void ZSystem::set_cursor(int row, int col) {
	_screen.flushText();
	//_screen.ResetOverhang();
	_screen.setTextPosition(Common::Point(col-1,row-1));
}

/**
 * Set the current text style. Following flags can be set:
 *
 *     REVERSE_STYLE
 *     BOLDFACE_STYLE
 *     EMPHASIS_STYLE (aka underline aka italics)
 *     FIXED_WIDTH_STYLE
 */
void ZSystem::set_text_style(int new_style) {
	_screen.flushText();
	_screen.getTextSettings().setStyle(new_style);
	_screen.applyTextSettings();
}

/**
 * Calculate the length of a word in screen units. Apart from letters,
 * the word may contain special codes:
 *
 *    ZC_NEW_STYLE - next character is a new text style
 *    ZC_NEW_FONT  - next character is a new font
 */
int ZSystem::string_width(const zchar *s) {
	// Look for style or font changes, or indents
	bool changes = false;
	const zchar *s1 = NULL;
	for (s1 = s; *s1 != 0; s1++)   {
		if ((*s1 == ZC_NEW_STYLE) || (*s1 == ZC_NEW_FONT)) {
			changes = true;
			break;
		}

		if ((*s1 == ZC_INDENT) || (*s1 == ZC_GAP)) {
			changes = true;
			break;
		}
	}

	int width = 0;

	// If there are no changes, just get the width of the string
	if (changes == false)
		width = _screen.getTextSettings().getStringWidth((const char *)s);
	else {
		// Get the width of a space
		int spWidth = char_width(' ');

		// Save the current text settings
		Gargoyle::TextSettings savedText = _screen.getTextSettings();

		// Work out the length of each section of the text
		s1 = s;
		const zchar *s2 = s;
		bool done = false;
		while (done == false) {
			// Is this the end of a section of text?
			switch (*s2) {
			case ZC_NEW_STYLE:
				if (s2 > s1)
					width += _screen.getTextSettings().getStringWidth(Common::String((const char *)s1, s2 - s1));
				s2++;
				set_text_style(*s2);
				s1 = s2+1;
				break;
			case ZC_NEW_FONT:
				if (s2 > s1)
					width += _screen.getTextSettings().getStringWidth(Common::String((const char *)s1, s2 - s1));
				s2++;
				set_font(*s2);
				s1 = s2+1;
				break;
			case ZC_INDENT:
				if (s2 > s1)
					width += _screen.getTextSettings().getStringWidth(Common::String((const char *)s1, s2 - s1));
				width += spWidth*3;
				s1++;
				break;
			case ZC_GAP:
				if (s2 > s1)
					width += _screen.getTextSettings().getStringWidth(Common::String((const char *)s1, s2 - s1));
				width += spWidth*2;
				s1++;
				break;
			case 0:
				if (s2 > s1)
					width += _screen.getTextSettings().getStringWidth(Common::String((const char *)s1, s2 - s1));
				done = true;
				break;
			}

			s2++;
		}

		// Restore the current text settings
		_screen.setTextSettings(savedText);
	}

	return width;
}

/**
 * Return the length of the character in screen units.
 *
 */
int ZSystem::char_width(zword c) {
	return _screen.getTextSettings().getCharWidth(c);
}

/**
 * Return with bit 0 set if the Unicode character can be
 * displayed, and bit 1 if it can be input.
 */
int ZSystem::check_unicode(int font, zword c) {
	bool canDisplay = (c >= 0x100) ? false : _interp.isValidChar((zchar) c);
	return canDisplay ? 3 : 2;
}

/**
 * Return the colour of the screen unit below the cursor. (If the
 * interface uses a text mode, it may return the background colour
 * of the character at the cursor position instead.) This is used
 * when text is printed on top of pictures. Note that this coulor
 * need not be in the standard set of Z-machine colours. To handle
 * this situation, Frotz entends the colour scheme: Colours above
 * 15 (and below 256) may be used by the interface to refer to non
 * standard colours. Of course, set_colour must be able to deal
 * with these colours.
 */
int ZSystem::peek_colour(void) {
	_screen.flushText();

	const byte *data = _screen.getBasePtr(_screen.getTextPosition().x,
		_screen.getTextPosition().y);
	byte result = *data;

	return result;
}

/**
 * Return true if the given picture is available. If so, store the
 * picture width and height in the appropriate variables. Picture
 * number 0 is a special case: Write the highest legal picture number
 * and the picture file release number into the height and width
 * variables respectively when this picture number is asked for.
 */
int ZSystem::picture_data(int picture, int *height, int *width) {
	if (_frotzGfx) {
		Common::Point pt;
		int result = _frotzGfx->getPictureInfo(picture, &pt.x, &pt.y);
		*width = pt.x;
		*height = pt.y;
		return result;
	} 

	Graphics::Surface *gfx = _interp.getPicture(picture);
	if (gfx != NULL) {
		*width = gfx->w;
		*height = gfx->h;
		delete gfx;

		return 1;
	} else {
		*width = 0;
		*height = 0;

		return 0;
	}
}

/**
 * Display a picture at the given coordinates.
 */
void ZSystem::draw_picture(int picture, int y, int x) {
	_screen.flushText();

	if (_frotzGfx) {
		_frotzGfx->drawPicture(picture, _screen, x, y);
	} else {
		Graphics::Surface *gfx = _interp.getPicture(picture);

		if (gfx != NULL) {
			signed short x1 = (signed short)x;
			signed short y1 = (signed short)y;
			_screen.copyFrom(gfx, x1 - 1, y1 - 1);
			delete gfx;
		}
	}
}

/**
 * Return an appropriate random seed value in the range from 0 to
 * 32767, possibly by using the current system time.
 */
int ZSystem::random_seed(void) {
	error("unused");
}

/**
 * This routine allows the interface to interfere with the process of
 * restarting a game at various stages:
 *
 *     RESTART_BEGIN - restart has just begun
 *     RESTART_WPROP_SET - window properties have been initialised
 *     RESTART_END - restart is complete
 */

void ZSystem::restart_game(int stage) {
	// Show Beyond Zork's title screen
	if ((stage == RESTART_BEGIN) && (_h.story_id == BEYOND_ZORK)) {
		int w, h;
		if (picture_data(1, &h, &w)) {
			_screen.empty();
			draw_picture(1, 1, 1);
			read_key(0, 0);
		}
	}
}

/**
 * Open a file in the current directory.
 */
bool ZSystem::path_open(Common::File &f, const char *name) {
	return f.open(name);
}

/**
 * Remove the current sample from memory (if any).
 */
void ZSystem::finish_with_sample(int number) {
	if (_frotzSound)
		_frotzSound->stop(number);
}

/**
 * Load the given sample from the disk.
 */
void ZSystem::prepare_sample(int number) {
}

/**
 * Play the given sample at the given volume (ranging from 1 to 8 and
 * 255 meaning a default volume). The sound is played once or several
 * times in the background (255 meaning forever). The end_of_sound
 * function is called as soon as the sound finishes, passing in the
 * eos argument.
 *
 */
void ZSystem::start_sample(int number, int volume, int repeats, zword eos) {
	const zbyte lurkingHorror_repeats[] = {
		0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x01, 0x01, 0x01, 0x01,
		0xff, 0x01, 0x01, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff
    };

	if (_h.story_id == LURKING_HORROR)
		repeats = lurkingHorror_repeats[number];
	else if (_interp.hasBlorbFile()) {
		if (volume == 255)
			volume = 8;

		if (repeats == 0)
			repeats = 1;
		else if (repeats == 255)
			repeats = -1;
	}

	if (_frotzSound)
		_frotzSound->play(number, repeats, volume, eos);
}

/**
 * Turn off the current sample.
 */
void ZSystem::stop_sample(int number) {
	if (_frotzSound)
		_frotzSound->stop(number);
}

/**
 * Write a character to the scrollback buffer.
 */
void ZSystem:: scrollback_char(zchar c) {
	_scrollback.push(c);
}

/**
 * Remove characters from the scrollback buffer.
 */
void ZSystem::scrollback_erase(int erase) {
	for (int i = 0; i < erase; ++i)
		_scrollback.pop();
}

/**
 * Called after each opcode.
 */
void ZSystem::tick(void) {
/*	static int count = 0;

	// Check for completed sounds
	if (++count > 1000) {
		count = 0;
		FrotzSound::Timer();
	}*/
}

/**
 * Set the screen buffering mode, and return the previous mode.
 * Possible values for mode are:
 *
 *     0 - update the display to reflect changes when possible
 *     1 - do not update the display
 *    -1 - redraw the screen, do not change the mode
 */

int ZSystem::buffer_screen(int mode) {
	if (mode == -1)
		_screen.flushDisplay();
	return 0;
}

/**
 * Return non-zero if the window should have text wrapped.
 */
int ZSystem::wrap_window(int win) {
	return 1;
}
/**
 * Called when the height of a window is changed.
 */
void ZSystem::window_height(int win, int height) {
}

/**
 * Converts a passed 5-bit RGB word to standard RGB
 */
uint32 ZSystem::RGB5_to_RGB(zword c) {
	int r = c & 0x001F;
	int g = (c & 0x03E0) >> 5;
	int b = (c & 0x7C00) >> 10;
	return (( r << 3) | (r >> 2)) | (((g << 3) | (g >> 2)) << 8) |
		(((b << 3) | (b >> 2)) << 16);
}

} // end of namespace Frotz
