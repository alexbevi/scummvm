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

#include "common/config-manager.h"
#include "gargoyle/frotz_interp.h"
#include "gargoyle/frotz_static.h"

namespace Frotz {

/*
 * z_restart, re-load dynamic area, clear the stack and set the PC.
 *
 * 	no zargs used
 *
 */
void FrotzInterpreter::z_restart(void) {
	flush_buffer();
	_os.restart_game(RESTART_BEGIN);
	seed_random(0);

	if (!first_restart) {
		_gameFile->seek(init_fp_pos, SEEK_SET);

		if (_gameFile->read(zmp, _h.h_dynamic_size) != _h.h_dynamic_size)
			_os.fatal("Story file read error");

	} else 
		first_restart = FALSE;

	restart_header();
	restart_screen();

	sp = fp = stack + STACK_SIZE;
	frame_count = 0;

	if (_h.h_version != V6) {
		long pc = (long) _h.h_start_pc;
		SET_PC(pc)

	} else 
	call(_h.h_start_pc, 0, NULL, 0);

	_os.restart_game(RESTART_END);
}

/*
 * z_restore, restore [a part of] a Z-machine state from disk
 *
 *	zargs[0] = address of area to restore (optional)
 *	zargs[1] = number of bytes to restore
 *	zargs[2] = address of suggested file name
 *	zargs[3] = whether to ask for confirmation of the file name
 *
 */
void FrotzInterpreter::z_restore(void) {
	char new_name[MAX_FILE_NAME + 1];
	char default_name[MAX_FILE_NAME + 1];
	Common::InSaveFile *gfp;

	zword success = 0;

	if (zargc != 0) {
		/* Get the file name */
		get_default_name(default_name, (zargc >= 3) ? zargs[2] : 0);

		if ((zargc >= 4) ? zargs[3] : 1) {
			if (_os.read_file_name(new_name, default_name, FILE_LOAD_AUX) == 0)
				goto finished;

			strcpy(_h.auxilary_name, new_name);

		} else 
			strcpy(new_name, default_name);

		/* Open auxilary file */

		gfp = _saveFileMan->openForLoading(new_name);
		if (!gfp)
			goto finished;

		/* Load auxilary file */

		success = gfp->read(zmp + zargs[0], zargs[1]);

		/* Close auxilary file */
		delete gfp;
		
	} else {
		// If a load hasn't already been requested, show the load game dialog
		if (_saveSlot == -1) {
			showSaveLoadDialog(false);
			if (_saveSlot == -1)
				goto finished;
		}
		getSaveFilename(new_name, _saveSlot);
		strcpy(_h.save_name, new_name);

		Common::FSNode savePath(ConfMan.get("savepath"));
		Common::FSNode file = savePath.getChild(new_name);

		// Open the file for saving
		gfp = file.createReadStream();
		if (!gfp)
			goto finished;

		// Load the savegame
		success = load_game(gfp);

		_saveSlot = -1;
		delete gfp;

		if (success) {
			// Close game file
			zbyte old_screen_rows;
			zbyte old_screen_cols;

			/* In V3, reset the upper window. */
			if (_h.h_version == V3)
				split_window(0);

			LOW_BYTE(H_SCREEN_ROWS, old_screen_rows);
			LOW_BYTE(H_SCREEN_COLS, old_screen_cols);

			/* Reload cached header fields. */
			restart_header();

			/*
			 * Since QUETZAL files may be saved on many different machines,
			 * the screen sizes may vary a lot. Erasing the status window
			 * seems to cover up most of the resulting badness.
			 */
			if (_h.h_version > V3 && _h.h_version != V6
				&& (_h.h_screen_rows != old_screen_rows
				|| _h.h_screen_cols != old_screen_cols))
				erase_window(1);
		}
	}

finished:

	if (_h.h_version <= V3)
		branch(success);
	else
		store(success ? 2 : 0);
}

/*
 * z_save_undo, save the current Z-machine state for a future undo.
 *
 *	no zargs used
 *
 */
void FrotzInterpreter::z_save_undo(void) {
	store((zword) save_undo());
}

/*
 * z_verify, check the story file integrity.
 *
 *	no zargs used
 *
 */
void FrotzInterpreter::z_verify(void) {
	zword checksum = 0;
	long i;

	/* Sum all bytes in story file except header bytes */
	_gameFile->seek(init_fp_pos + 64, SEEK_SET);

	for (i = 64; i < _h.story_size; i++)
		checksum += _gameFile->readByte();

	/* Branch if the checksums are equal */
	branch(checksum == _h.h_checksum);
}

/*
 * z_restore_undo, restore a Z-machine state from memory.
 *
 *	no zargs used
 *
 */
void FrotzInterpreter::z_restore_undo(void) {
	store((zword) restore_undo());
}

/*
 * z_save, save [a part of] the Z-machine state to disk.
 *
 *	zargs[0] = address of memory area to save (optional)
 *	zargs[1] = number of bytes to save
 *	zargs[2] = address of suggested file name
 *	zargs[3] = whether to ask for confirmation of the file name
 *
 */
void FrotzInterpreter::z_save(void) {
	char new_name[MAX_FILE_NAME + 1];
	char default_name[MAX_FILE_NAME + 1];
	Common::WriteStream *gfp;
	zword success = 0;

	if (zargc != 0) {
		/* Get the file name */
		get_default_name(default_name, (zargc >= 3) ? zargs[2] : 0);

		if ((zargc >= 4) ? zargs[3] : 1) {
			if (_os.read_file_name(new_name, default_name, FILE_SAVE_AUX) == 0)
				goto finished;

			strcpy(_h.auxilary_name, new_name);

		} else 
			strcpy(new_name, default_name);

		/* Open auxilary file */
		gfp = _saveFileMan->openForSaving(new_name);
		if (!gfp)
			goto finished;

		/* Write auxilary file */
		success = gfp->write(zmp + zargs[0], zargs[1]);

		/* Close auxilary file */
		gfp->finalize();
		delete gfp;
		gfp = NULL;

	} else {
		// If a save hasn't already been requested, show the savegame dialog
		if (_saveSlot == -1) {
			showSaveLoadDialog(true);
			if (_saveSlot == -1)
				goto finished;
		}

		getSaveFilename(new_name, _saveSlot);
		strcpy(_h.save_name, new_name);

		// Open a file for saving. Note that the normal SaveFileManager method isn't used since, for
		// inter-interpreter compatibility we don't want the gamefiles being compressed

		Common::FSNode savePath(ConfMan.get("savepath"));
		Common::FSNode file = savePath.getChild(new_name);

		// Open the file for saving
		gfp = file.createWriteStream();
		if (!gfp)
			goto finished;

		success = save_game(gfp);

		// Close game file and check for errors

		gfp->finalize();
		_saveSlot = -1;

		success = !gfp->err();
		delete gfp;
	}

finished:

	if (_h.h_version <= V3)
		branch(success != 0);
	else
		store(success);
}

/**
 * load / play / stop / discard a sound effect.
 *
 *	zargs[0] = number of bleep (1 or 2) or sample
 *	zargs[1] = operation to perform (samples only)
 *	zargs[2] = repeats and volume (play sample only)
 *	zargs[3] = end-of-sound routine (play sample only, optional)
 *
 * Note: Volumes range from 1 to 8, volume 255 is the default volume.
 *	 Repeats are stored in the high byte, 255 is infinite loop.
 */
void FrotzInterpreter::z_sound_effect(void) {
	zword number = zargs[0];
	zword effect = zargs[1];
	zword volume = zargs[2];

	if (zargc < 1)
		number = 0;
	if (zargc < 2)
		effect = EFFECT_PLAY;
	if (zargc < 3)
		volume = 8;

	if (number >= 3 || number == 0) {
		sound_locked = true;

		if (_h.story_id == LURKING_HORROR && (number == 9 || number == 16)) {
			if (effect == EFFECT_PLAY) {
				next_sample = number;
				next_volume = volume;

				sound_locked = false;

				if (!playing)
					start_next_sample();

			} else 
				sound_locked = false;

			return;
		}

		playing = false;

		switch (effect) {
		case EFFECT_PREPARE:
			_os.prepare_sample(number);
			break;
		case EFFECT_PLAY:
			start_sample(number, lo(volume), hi(volume), (zargc == 4) ? zargs[3] : 0);
			break;
		case EFFECT_STOP:
			_os.stop_sample(number);
			break;
		case EFFECT_FINISH_WITH:
			_os.finish_with_sample(number);
			break;
		}

		sound_locked = false;

	} else 
		_os.beep(number);
}

/**
 * Test if a unicode character can be printed (bit 0) and read (bit 1).
 *
 * 	zargs[0] = Unicode
 */
void FrotzInterpreter::z_check_unicode(void) {
	zword c = zargs[0];
	zword result = 0;

	if (c <= 0x1f) {
		if ((c == 0x08) || (c == 0x0d) || (c == 0x1b))
			result = 2;
	}
	else if (c <= 0x7e)
		result = 3;
	else
		result = _os.check_unicode(get_window_font(cwin), c);

	store(result);
}

/**
 * Encode a ZSCII string for use in a dictionary.
 *
 *	zargs[0] = address of text buffer
 *	zargs[1] = length of ASCII string
 *	zargs[2] = offset of ASCII string within the text buffer
 *	zargs[3] = address to store encoded text in
 *
 * This is a V5+ opcode and therefore the dictionary resolution must be
 * three 16bit words.
 */
void FrotzInterpreter::z_encode_text(void) {
	int i;

	load_string((zword)(zargs[0] + zargs[2]), zargs[1]);
	encode_text(0x05);

	for (i = 0; i < resolution; i++)
		storew((zword)(zargs[3] + 2 * i), encoded[i]);
}

/**
 * Print a new line.
 *
 * 	no zargs used
 */
void FrotzInterpreter::z_new_line(void) {
	new_line();
}

/**
 * Print a string embedded in the instruction stream.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_print(void) {
	decode_text(EMBEDDED_STRING, 0);
}

/**
 * Print a string from the lower 64KB.
 *
 *	zargs[0] = address of string to print
 */
void FrotzInterpreter::z_print_addr(void) {
	decode_text(LOW_STRING, zargs[0]);
}

/**
 * Print a single ZSCII character.
 *
 *	zargs[0] = ZSCII character to be printed
 */
void FrotzInterpreter::z_print_char(void) {
	print_char(translate_from_zscii(zargs[0]));
}

/**
 * Print a formatted table.
 *
 *	zargs[0] = address of formatted table to be printed
 */
void FrotzInterpreter::z_print_form(void) {
	zword count;
	zword addr = zargs[0];

	bool first = true;

	for (;;) {
		LOW_WORD(addr, count)
		addr += 2;

		if (count == 0)
			break;

		if (!first)
			new_line();

		while (count--) {
			zbyte c;
			LOW_BYTE(addr, c)
			addr++;

			print_char(translate_from_zscii(c));
		}

		first = false;
	}
}

/*
 * Print a signed number.
 *
 * 	zargs[0] = number to print
 */
void FrotzInterpreter::z_print_num(void) {
	print_num(zargs[0]);
}

/**
 * Print an object description.
 *
 * 	zargs[0] = number of object to be printed
 */
void FrotzInterpreter::z_print_obj(void) {
	print_object(zargs[0]);
}

/**
 * Print the string at the given packed address.
 *
 * 	zargs[0] = packed address of string to be printed
 */
void FrotzInterpreter::z_print_paddr(void) {
	decode_text(HIGH_STRING, zargs[0]);
}

/**
 * Print the string at PC, print newline then return true.
 *
 * 	no zargs used
 */
void FrotzInterpreter::z_print_ret(void) {
	decode_text(EMBEDDED_STRING, 0);
	new_line();
	ret(1);
}

/**
 * z_print_unicode
 *
 * 	zargs[0] = Unicode
 */
void FrotzInterpreter::z_print_unicode(void) {
	if (zargs[0] < 0x20)
		print_char('?');
	else
		print_char(zargs[0]);
}

/**
 * Make a lexical analysis of a ZSCII string.
 *
 *	zargs[0] = address of string to analyze
 *	zargs[1] = address of token buffer
 *	zargs[2] = address of dictionary (optional)
 *	zargs[3] = set when unknown words cause empty slots (optional)
 */
void FrotzInterpreter::z_tokenise(void) {
	/* Supply default arguments */
	if (zargc < 3)
		zargs[2] = 0;
	if (zargc < 4)
		zargs[3] = 0;

	/* Call tokenise_line to do the real work */
	tokenise_line(zargs[0], zargs[1], zargs[2], zargs[3] != 0);
}

/**
 * Open or close an output stream.
 *
 *	zargs[0] = stream to open (positive) or close (negative)
 *	zargs[1] = address to redirect output to (stream 3 only)
 *	zargs[2] = width of redirected output (stream 3 only, optional)
 */
void FrotzInterpreter::z_output_stream(void) {
	flush_buffer();

	switch ((int16)zargs[0]) {
	case 1:
		ostream_screen = TRUE;
		break;
	case -1:
		ostream_screen = FALSE;
		break;
	case  2: 
		if (!ostream_script) script_open();
		break;
	case -2: 
		if (ostream_script) script_close();
		break;
	case  3: 
		memory_open(zargs[1], zargs[2], zargc >= 3);
		break;
	case -3: 
		memory_close();
		break;
	case  4: 
		if (!ostream_record) record_open();
		break;
	case -4: if (ostream_record) record_close();
		break;
	}
}

/**
 * Select an input stream.
 *
 *	zargs[0] = input stream to be selected
 */
void FrotzInterpreter::z_input_stream(void) {
	flush_buffer();

	if (zargs[0] == 0 && istream_replay)
		replay_close();
	if (zargs[0] == 1 && !istream_replay)
		replay_open();
}

/**
 * Add or remove a menu and branch if successful.
 *
 *	zargs[0] = number of menu
 *	zargs[1] = table of menu entries or 0 to remove menu
 */
void FrotzInterpreter::z_make_menu(void) {
	/* This opcode was only used for the Macintosh version of Journey, with isn't
	   currently supported. It controls menus with numbers greater than 2 
	   (menus 0, 1 and 2 are system menus). */

	branch(false);
}

/**
 * Read a line of input and (in V5+) store the terminating key.
 *
 *	zargs[0] = address of text buffer
 *	zargs[1] = address of token buffer
 *	zargs[2] = timeout in tenths of a second (optional)
 *	zargs[3] = packed address of routine to be called on timeout
 */
void FrotzInterpreter::z_read(void) {
	zchar buffer[INPUT_BUFFER_SIZE];
	zword addr;
	zword key;
	zbyte max, size;
	zbyte c;
	int i;

	/* Supply default arguments */
	if (zargc < 3)
		zargs[2] = 0;

	/* Get maximum input size */
	addr = zargs[0];

	LOW_BYTE(addr, max)

	if (_h.h_version <= V4)
		max--;

	if (max >= INPUT_BUFFER_SIZE)
		max = INPUT_BUFFER_SIZE - 1;

	/* Get initial input size */
	if (_h.h_version >= V5) {
		addr++;
		LOW_BYTE(addr, size)
	} else 
		size = 0;

	/* Copy initial input to local buffer */
	for (i = 0; i < size; i++) {
		addr++;
		LOW_BYTE(addr, c)
		buffer[i] = translate_from_zscii(c);
	}

	buffer[i] = 0;

	/* Draw status line for V1 to V3 games */

	if (_h.h_version <= V3)
		z_show_status();

	/* Read input from current input stream */
	key = stream_read_input(
		max, buffer,		/* buffer and size */
		zargs[2],		/* timeout value   */
		zargs[3],		/* timeout routine */
		true,		 	   	/* enable hot keys */
		_h.h_version == V6);	/* no script in V6 */

	if (key == ZC_BAD)
		return;

	/* Perform save_undo for V1 to V4 games */
	if (_h.h_version <= V4)
		save_undo();

	/* Copy local buffer back to dynamic memory */
	for (i = 0; buffer[i] != 0; i++) {
		if (key == ZC_RETURN) {
			buffer[i] = buffer[i];
		}

		storeb ((zword)(zargs[0] + ((_h.h_version <= V4) ? 1 : 2) + i), translate_to_zscii (buffer[i]));
	}

	/* Add null character (V1-V4) or write input length into 2nd byte */
	if (_h.h_version <= V4)
		storeb((zword)(zargs[0] + 1 + i), 0);
	else
		storeb((zword)(zargs[0] + 1), i);

	/* Tokenise line if a token buffer is present */
	if (key == ZC_RETURN && zargs[1] != 0)
		tokenise_line(zargs[0], zargs[1], 0, FALSE);

	/* Store key */
	if (_h.h_version >= V5)
		store(translate_to_zscii(key));
}

/**
 * Read and store a key.
 *
 *	zargs[0] = input device (must be 1)
 *	zargs[1] = timeout in tenths of a second (optional)
 *	zargs[2] = packed address of routine to be called on timeout
 */
void FrotzInterpreter::z_read_char(void) {
	zword key;

	/* Supply default arguments */
	if (zargc < 2)
		zargs[1] = 0;

	/* Read input from the current input stream */
	key = stream_read_key (
		zargs[1],	/* timeout value   */
		zargs[2],	/* timeout routine */
		true);  	/* enable hot keys */

	if (key == ZC_BAD)
		return;

	/* Store key */
	store(translate_to_zscii(key));
}

/**
 * Write the current mouse status into a table.
 *
 *	zargs[0] = address of table
 */
void FrotzInterpreter::z_read_mouse(void) {
	zword btn;

	/* Read the mouse position, the last menu click and which buttons are down */
	btn = _os.read_mouse();
	_h.mousePos.y = mouse_y;
	_h.mousePos.x = mouse_x;

	storew((zword)(zargs[0] + 0), _h.mousePos.y);
	storew((zword)(zargs[0] + 2), _h.mousePos.x);
	storew((zword)(zargs[0] + 4), btn);				/* mouse button bits */
	storew((zword)(zargs[0] + 6), menu_selected);	/* menu selection */
}

/**
 * Clear an object attribute.
 *
 *	zargs[0] = object
 *	zargs[1] = number of attribute to be cleared
 */
void FrotzInterpreter::z_clear_attr(void) {
	zword obj_addr;
	zbyte value;

	if (_h.story_id == SHERLOCK)
	if (zargs[1] == 48)
		return;

	if (zargs[1] > ((_h.h_version <= V3) ? 31 : 47))
		runtime_error(ERR_ILL_ATTR);

	/* If we are monitoring attribute assignment display a short note */

	if (option_attribute_assignment) {
		stream_mssg_on();
		print_string("@clear_attr ");
		print_object(zargs[0]);
		print_string(" ");
		print_num(zargs[1]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_CLEAR_ATTR_0);
		return;
	}

	/* Get attribute address */
	obj_addr = object_address (zargs[0]) + zargs[1] / 8;

	/* Clear attribute bit */
	LOW_BYTE(obj_addr, value)
	value &= ~(0x80 >> (zargs[1] & 7));
	SET_BYTE(obj_addr, value)
}

/**
 * Branch if the first object is inside the second.
 *
 *	zargs[0] = first object
 *	zargs[1] = second object
 */
void FrotzInterpreter::z_jin(void) {
	zword obj_addr;

	/* If we are monitoring object locating display a short note */
	if (option_object_locating) {
		stream_mssg_on();
		print_string("@jin ");
		print_object(zargs[0]);
		print_string(" ");
		print_object(zargs[1]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_JIN_0);
		branch(0 == zargs[1]);
		return;
	}

	obj_addr = object_address(zargs[0]);

	if (_h.h_version <= V3) {
		zbyte parent;

		/* Get parent id from object */

		obj_addr += O1_PARENT;
		LOW_BYTE(obj_addr, parent)

		/* Branch if the parent is obj2 */
		branch(parent == zargs[1]);
	} else {
		zword parent;

		/* Get parent id from object */
		obj_addr += O4_PARENT;
		LOW_WORD(obj_addr, parent)

		/* Branch if the parent is obj2 */

		branch(parent == zargs[1]);
	}
}

/**
 * Store the child of an object.
 *
 *	zargs[0] = object
 */
void FrotzInterpreter::z_get_child(void) {
	zword obj_addr;

	/* If we are monitoring object locating display a short note */
	if (option_object_locating) {
		stream_mssg_on();
		print_string("@get_child ");
		print_object(zargs[0]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_CHILD_0);
		store(0);
		branch(FALSE);
		return;
	}

	obj_addr = object_address(zargs[0]);

	if (_h.h_version <= V3) {
		zbyte child;

		/* Get child id from object */
		obj_addr += O1_CHILD;
		LOW_BYTE(obj_addr, child)

		/* Store child id and branch */
		store(child);
		branch(child != 0);
	} else {
		zword child;

		/* Get child id from object */
		obj_addr += O4_CHILD;
		LOW_WORD(obj_addr, child)

		/* Store child id and branch */
		store(child);
		branch(child != 0);
	}
}

/**
 * Store the number of the first or next property.
 *
 *	zargs[0] = object
 *	zargs[1] = address of current property (0 gets the first property)
 */
void FrotzInterpreter::z_get_next_prop (void) {
	zword prop_addr;
	zbyte value;
	zbyte mask;

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_NEXT_PROP_0);
		store(0);
		return;
	}

	/* Property id is in bottom five (six) bits */
	mask = (_h.h_version <= V3) ? 0x1f : 0x3f;

	/* Load address of first property */
	prop_addr = first_property (zargs[0]);

	if (zargs[1] != 0) {
		/* Scan down the property list */

		do {
			LOW_BYTE(prop_addr, value)
			prop_addr = next_property(prop_addr);
		} while ((value & mask) > zargs[1]);

		/* Exit if the property does not exist */

		if ((value & mask) != zargs[1])
			runtime_error(ERR_NO_PROP);
	}

	/* Return the property id */
	LOW_BYTE(prop_addr, value)
	store((zword) (value & mask));
}

/**
 * Store the parent of an object.
 *
 *	zargs[0] = object
 */
void FrotzInterpreter::z_get_parent (void) {
	zword obj_addr;

	/* If we are monitoring object locating display a short note */
	if (option_object_locating) {
		stream_mssg_on();
		print_string("@get_parent ");
		print_object(zargs[0]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_PARENT_0);
		store (0);
		return;
	}

	obj_addr = object_address (zargs[0]);

	if (_h.h_version <= V3) {
		zbyte parent;

		/* Get parent id from object */

		obj_addr += O1_PARENT;
		LOW_BYTE(obj_addr, parent)

		/* Store parent */
		store(parent);
	} else {
		zword parent;

		/* Get parent id from object */
		obj_addr += O4_PARENT;
		LOW_WORD(obj_addr, parent)

		/* Store parent */
		store(parent);
	}
}

/**
 * Store the value of an object property.
 *
 *	zargs[0] = object
 *	zargs[1] = number of property to be examined
 */
void FrotzInterpreter::z_get_prop(void) {
	zword prop_addr;
	zword wprop_val;
	zbyte bprop_val;
	zbyte value;
	zbyte mask;

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_PROP_0);
		store(0);
		return;
	}

	/* Property id is in bottom five (six) bits */
	mask = (_h.h_version <= V3) ? 0x1f : 0x3f;

	/* Load address of first property */
	prop_addr = first_property(zargs[0]);

	/* Scan down the property list */
	for (;;) {
		LOW_BYTE(prop_addr, value)
		if ((value & mask) <= zargs[1])
			break;
	
		prop_addr = next_property(prop_addr);
	}

	if ((value & mask) == zargs[1]) {	/* property found */
		/* Load property (byte or word sized) */
		prop_addr++;

		if ((_h.h_version <= V3 && !(value & 0xe0)) || (_h.h_version >= V4 && !(value & 0xc0))) {
			LOW_BYTE(prop_addr, bprop_val)
			wprop_val = bprop_val;

		} else 
			LOW_WORD(prop_addr, wprop_val)

	} else {	/* property not found */
		/* Load default value */
		prop_addr = _h.h_objects + 2 * (zargs[1] - 1);
		LOW_WORD(prop_addr, wprop_val)
	}

	/* Store the property value */
	store(wprop_val);
}

/**
 * Store the address of an object property.
 *
 *	zargs[0] = object
 *	zargs[1] = number of property to be examined
 */
void FrotzInterpreter::z_get_prop_addr(void) {
	zword prop_addr;
	zbyte value;
	zbyte mask;

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_PROP_ADDR_0);
		store(0);
		return;
	}

	if (_h.story_id == BEYOND_ZORK)
		if (zargs[0] > MAX_OBJECT) { store (0); return; }

	/* Property id is in bottom five (six) bits */
	mask = (_h.h_version <= V3) ? 0x1f : 0x3f;

	/* Load address of first property */
	prop_addr = first_property (zargs[0]);

	/* Scan down the property list */
	for (;;) {
		LOW_BYTE(prop_addr, value)
		if ((value & mask) <= zargs[1])
			break;
		prop_addr = next_property(prop_addr);
	}

	/* Calculate the property address or return zero */
	if ((value & mask) == zargs[1]) {
		if (_h.h_version >= V4 && (value & 0x80))
			prop_addr++;
		store((zword)(prop_addr + 1));

	} else 
		store(0);
}

/**
 * Store the length of an object property.
 *
 * 	zargs[0] = address of property to be examined
 */
void FrotzInterpreter::z_get_prop_len(void) {
	zword addr;
	zbyte value;

	/* Back up the property pointer to the property id */
	addr = zargs[0] - 1;
	LOW_BYTE(addr, value)

	/* Calculate length of property */
	if (_h.h_version <= V3)
		value = (value >> 5) + 1;
	else if (!(value & 0x80))
		value = (value >> 6) + 1;
	else {
		value &= 0x3f;

		if (value == 0) value = 64;	/* demanded by Spec 1.0 */
	}

	/* Store length of property */
	store (value);
}

/**
 * Store the sibling of an object.
 *
 *	zargs[0] = object
 */
void FrotzInterpreter::z_get_sibling(void) {
	zword obj_addr;

	if (zargs[0] == 0) {
		runtime_error(ERR_GET_SIBLING_0);
		store(0);
		branch(FALSE);
		return;
	}

	obj_addr = object_address (zargs[0]);

	if (_h.h_version <= V3) {
		zbyte sibling;

		/* Get sibling id from object */
		obj_addr += O1_SIBLING;
		LOW_BYTE(obj_addr, sibling)

		/* Store sibling and branch */
		store(sibling);
		branch(sibling != 0);
	} else {
		zword sibling;

		/* Get sibling id from object */
		obj_addr += O4_SIBLING;
		LOW_WORD(obj_addr, sibling)

		/* Store sibling and branch */
		store(sibling);
		branch(sibling != 0);
	}
}

/**
 * Make an object the first child of another object.
 *
 *	zargs[0] = object to be moved
 *	zargs[1] = destination object
 */
void FrotzInterpreter::z_insert_obj(void) {
	zword obj1 = zargs[0];
	zword obj2 = zargs[1];
	zword obj1_addr;
	zword obj2_addr;

	/* If we are monitoring object movements display a short note */
	if (option_object_movement) {
		stream_mssg_on();
		print_string("@move_obj ");
		print_object(obj1);
		print_string(" ");
		print_object(obj2);
		stream_mssg_off();
	}

	if (obj1 == 0) {
		runtime_error(ERR_MOVE_OBJECT_0);
		return;
	}

	if (obj2 == 0) {
		runtime_error(ERR_MOVE_OBJECT_TO_0);
		return;
	}

	/* Get addresses of both objects */
	obj1_addr = object_address(obj1);
	obj2_addr = object_address(obj2);

	/* Remove object 1 from current parent */
	unlink_object (obj1);

	/* Make object 1 first child of object 2 */
	if (_h.h_version <= V3) {
		zbyte child;

		obj1_addr += O1_PARENT;
		SET_BYTE(obj1_addr, obj2)
		obj2_addr += O1_CHILD;
		LOW_BYTE(obj2_addr, child)
		SET_BYTE(obj2_addr, obj1)
		obj1_addr += O1_SIBLING - O1_PARENT;
		SET_BYTE(obj1_addr, child)
	} else {
		zword child;

		obj1_addr += O4_PARENT;
		SET_WORD(obj1_addr, obj2)
		obj2_addr += O4_CHILD;
		LOW_WORD(obj2_addr, child)
		SET_WORD(obj2_addr, obj1)
		obj1_addr += O4_SIBLING - O4_PARENT;
		SET_WORD(obj1_addr, child)
	}
}

/**
 * Set the value of an object property.
 *
 *	zargs[0] = object
 *	zargs[1] = number of property to set
 *	zargs[2] = value to set property to
 */
void FrotzInterpreter::z_put_prop(void) {
	zword prop_addr;
	zword value;
	zbyte mask;

	if (zargs[0] == 0) {
		runtime_error(ERR_PUT_PROP_0);
		return;
	}

	/* Property id is in bottom five or six bits */
	mask = (_h.h_version <= V3) ? 0x1f : 0x3f;

	/* Load address of first property */
	prop_addr = first_property(zargs[0]);

	/* Scan down the property list */
	for (;;) {
		LOW_BYTE(prop_addr, value)
		if ((value & mask) <= zargs[1])
			break;
		prop_addr = next_property(prop_addr);
	}

	/* Exit if the property does not exist */
	if ((value & mask) != zargs[1])
		runtime_error(ERR_NO_PROP);

	/* Store the new property value (byte or word sized) */
	prop_addr++;

	if ((_h.h_version <= V3 && !(value & 0xe0)) || (_h.h_version >= V4 && !(value & 0xc0))) {
		zbyte v = zargs[2];
		SET_BYTE(prop_addr, v)
	} else {
		zword v = zargs[2];
		SET_WORD(prop_addr, v)
	}
}

/**
 * Unlink an object from its parent and siblings.
 *
 *	zargs[0] = object
 */
void FrotzInterpreter::z_remove_obj(void) {
	/* If we are monitoring object movements display a short note */
	if (option_object_movement) {
		stream_mssg_on();
		print_string("@remove_obj ");
		print_object(zargs[0]);
		stream_mssg_off();
	}

	/* Call unlink_object to do the job */
	unlink_object(zargs[0]);
}

/*
 * Set an object attribute.
 *
 *	zargs[0] = object
 *	zargs[1] = number of attribute to set
 *
 */
void FrotzInterpreter::z_set_attr(void) {
	zword obj_addr;
	zbyte value;

	if (_h.story_id == SHERLOCK)
		if (zargs[1] == 48)
			return;

	if (zargs[1] > ((_h.h_version <= V3) ? 31 : 47))
		runtime_error(ERR_ILL_ATTR);

	/* If we are monitoring attribute assignment display a short note */
	if (option_attribute_assignment) {
		stream_mssg_on();
		print_string("@set_attr ");
		print_object(zargs[0]);
		print_string(" ");
		print_num(zargs[1]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_SET_ATTR_0);
		return;
	}

	/* Get attribute address */
	obj_addr = object_address(zargs[0]) + zargs[1] / 8;

	/* Load attribute byte */
	LOW_BYTE(obj_addr, value)

	/* Set attribute bit */
	value |= 0x80 >> (zargs[1] & 7);

	/* Store attribute byte */
	SET_BYTE(obj_addr, value)
}

/**
 * Branch if an object attribute is set.
 *
 *	zargs[0] = object
 *	zargs[1] = number of attribute to test
 */
void FrotzInterpreter::z_test_attr(void) {
	zword obj_addr;
	zbyte value;

	if (zargs[1] > ((_h.h_version <= V3) ? 31 : 47))
		runtime_error(ERR_ILL_ATTR);

	/* If we are monitoring attribute testing display a short note */
	if (option_attribute_testing) {
		stream_mssg_on();
		print_string("@test_attr ");
		print_object(zargs[0]);
		print_string(" ");
		print_num(zargs[1]);
		stream_mssg_off();
	}

	if (zargs[0] == 0) {
		runtime_error(ERR_TEST_ATTR_0);
		branch(FALSE);
		return;
	}

	/* Get attribute address */
	obj_addr = object_address(zargs[0]) + zargs[1] / 8;

	/* Load attribute byte */
	LOW_BYTE(obj_addr, value)

	/* Test attribute */
	branch((value & (0x80 >> (zargs[1] & 7))) != 0);
}

/**
 * Turn text buffering on/off.
 *
 *	zargs[0] = new text buffering flag (0 or 1)
 */
void FrotzInterpreter::z_buffer_mode (void) {
	/* Infocom's V6 games rarely use the buffer_mode opcode. If they do
	   then only to print text immediately, without any delay. This was
	   used to give the player some sign of life while the game was
	   spending much time on parsing a complicated input line. (To turn
	   off word wrapping, V6 games use the window_style opcode instead.)
	   Today we can afford to ignore buffer_mode in V6. */

	if (_h.h_version != V6) {
		flush_buffer();
		wp[0].sd.attribute &= ~8;

		if (zargs[0] != 0)
			wp[0].sd.attribute |= 8;

		update_attributes ();
	}
}

/**
 * Draw a picture.
 *
 *	zargs[0] = number of picture to draw
 *	zargs[1] = y-coordinate of top left corner
 *	zargs[2] = x-coordinate of top left corner
 */
void FrotzInterpreter::z_draw_picture(void) {
	zword pic = zargs[0];

	zword y = zargs[1];
	zword x = zargs[2];

	int i;
	flush_buffer();

	if (y == 0)			/* use cursor line if y-coordinate is 0 */
		y = cwp->sd.y_cursor;
	if (x == 0)			/* use cursor column if x-coordinate is 0 */
		x = cwp->sd.x_cursor;

	y += cwp->sd.y_pos - 1;
	x += cwp->sd.x_pos - 1;

	/* The following is necessary to make Amiga and Macintosh story
	   files work with MCGA graphics files.  Some screen-filling
	   pictures of the original Amiga release like the borders of
	   Zork Zero were split into several MCGA pictures (left, right
	   and top borders).  We pretend this has not happened. */
	for (i = 0; game_mapper[i].story_id != UNKNOWN; i++) {
		if (_h.story_id == game_mapper[i].story_id && pic == game_mapper[i].pic) {
			int height1, width1;
			int height2, width2;

			int delta = 0;

			_os.picture_data (pic, &height1, &width1);
			_os.picture_data (game_mapper[i].pic2, &height2, &width2);

			if (_h.story_id == ARTHUR && pic == 54)
				delta = _h.h_screen_width / 160;

			_os.draw_picture(game_mapper[i].pic1, y + height1, x + delta);
			_os.draw_picture(game_mapper[i].pic2, y + height1, x + width1 - width2 - delta);
		}
	}

	_os.draw_picture(pic, y, x);

	if (_h.story_id == SHOGUN) {
		if (pic == 3) {
			int height, width;

			_os.picture_data(59, &height, &width);
			_os.draw_picture(59, y, _h.h_screen_width - width + 1);
		}
	}
}

/**
 * Erase the line starting at the cursor position.
 *
 *	zargs[0] = 1 + #units to erase (1 clears to the end of the line)
 */
void FrotzInterpreter::z_erase_line(void) {
	zword pixels = zargs[0];
	zword y, x;

	flush_buffer();

	/* Do nothing if the background is transparent */
//	if (hi (cwp->colour) == TRANSPARENT_COLOUR) return;

	/* Clipping at the right margin of the current window */
	if (--pixels == 0 || pixels > units_left ())
		pixels = units_left ();

	/* Erase from cursor position */
	y = cwp->sd.y_pos + cwp->sd.y_cursor - 1;
	x = cwp->sd.x_pos + cwp->sd.x_cursor - 1;

	_os.erase_area (y, x, y + font_height - 1, x + pixels - 1, -1);
}

/**
 * Erase a picture with background colour.
 *
 *	zargs[0] = number of picture to erase
 *	zargs[1] = y-coordinate of top left corner (optional)
 *	zargs[2] = x-coordinate of top left corner (optional)
 */
void FrotzInterpreter::z_erase_picture(void) {
	int height, width;

	zword y = zargs[1];
	zword x = zargs[2];

	flush_buffer();

	/* Do nothing if the background is transparent */
//	if (hi (cwp->colour) == TRANSPARENT_COLOUR) return;

	if (y == 0)		/* use cursor line if y-coordinate is 0 */
		y = cwp->sd.y_cursor;
	if (x == 0)    	/* use cursor column if x-coordinate is 0 */
		x = cwp->sd.x_cursor;

	_os.picture_data(zargs[0], &height, &width);

	y += cwp->sd.y_pos - 1;
	x += cwp->sd.x_pos - 1;

	_os.erase_area(y, x, y + height - 1, x + width - 1, -1);
}

/**
 * Erase a window or the screen to background colour.
 *
 *	zargs[0] = window (-3 current, -2 screen, -1 screen & unsplit)
 */
void FrotzInterpreter::z_erase_window(void) {
	flush_buffer();

	if ((short) zargs[0] == -1 || (short) zargs[0] == -2)
		erase_screen (zargs[0]);
	else
		erase_window (winarg0());
}

/**
 * Write the cursor coordinates into a table.
 *
 *	zargs[0] = address to write information to
 */
void FrotzInterpreter::z_get_cursor(void) {
	zword y, x;

	flush_buffer();

	y = cwp->sd.y_cursor;
	x = cwp->sd.x_cursor;

	if (_h.h_version != V6) {	/* convert to grid positions */
		y = (y - 1) / _h.h_font_height + 1;
		x = (x - 1) / _h.h_font_width + 1;
	}

	storew((zword)(zargs[0] + 0), y);
	storew((zword)(zargs[0] + 2), x);
}

/**
 * Store the value of a window property.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = number of window property to be stored
 */
void FrotzInterpreter::z_get_wind_prop(void) {
	flush_buffer();

	if (zargs[1] < 16)
		store((wp + winarg0())->arr[zargs[1]]);
    else if (zargs[1] == 16)
		store(_os.to_true_colour(lo(wp[winarg0()].sd.colour)));
    else if (zargs[1] == 17) {
		zword bg = hi(wp[winarg0()].sd.colour);

/*		if (bg == TRANSPARENT_COLOUR)
			store((zword) -4);
		else*/
			store(_os.to_true_colour(bg));
	} else
		runtime_error(ERR_ILL_WIN_PROP);
}

/**
 * Select a window as mouse window.
 *
 *	zargs[0] = window number (-3 is the current) or -1 for the screen
 */
void FrotzInterpreter::z_mouse_window(void) {
	mwin = ((short) zargs[0] == -1) ? -1 : winarg0();
}

/**
 * Place a window on the screen.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = y-coordinate
 *	zargs[2] = x-coordinate
 */
void FrotzInterpreter::z_move_window(void) {
	zword win = winarg0();

	flush_buffer();

	wp[win].sd.y_pos = zargs[1];
	wp[win].sd.x_pos = zargs[2];

	if (win == cwin)
		update_cursor();
}

/**
 * Get information on a picture or the graphics file.
 *
 *	zargs[0] = number of picture or 0 for the graphics file
 *	zargs[1] = address to write information to
 */
void FrotzInterpreter::z_picture_data(void) {
	zword pic = zargs[0];
	zword table = zargs[1];

	int height, width;
	int i;

	bool avail = _os.picture_data(pic, &height, &width) != 0;

	for (i = 0; game_mapper[i].story_id != UNKNOWN; i++)
		if (_h.story_id == game_mapper[i].story_id) {
			if (pic == game_mapper[i].pic) {
				int height2, width2;

				avail &= _os.picture_data(game_mapper[i].pic1, &height2, &width2) != 0;
				avail &= _os.picture_data(game_mapper[i].pic2, &height2, &width2) != 0;

				height += height2;

			} else if (pic == game_mapper[i].pic1 || pic == game_mapper[i].pic2)
				avail = FALSE;
		}

	storew((zword)(table + 0), (zword)(height));
	storew((zword)(table + 2), (zword)(width));

	branch(avail);
}

/**
 * Prepare a group of pictures for faster display.
 *
 *	zargs[0] = address of table holding the picture numbers
 */
void FrotzInterpreter::z_picture_table(void) {
	/* This opcode is used by Shogun and Zork Zero when the player
	   encounters built-in games such as Peggleboz. Nowadays it is
	   not very helpful to hold the picture data in memory because
	   even a small disk cache avoids re-loading of data. */
}

/**
 * Set the screen buffering mode.
 *
 *	zargs[0] = mode
 */
void FrotzInterpreter::z_buffer_screen(void) {
	store((zword)_os.buffer_screen((zargs[0] == (zword) -1) ? -1 : zargs[0]));
}

/*
 * z_print_table, print ASCII text in a rectangular area.
 *
 *	zargs[0] = address of text to be printed
 *	zargs[1] = width of rectangular area
 *	zargs[2] = height of rectangular area (optional)
 *	zargs[3] = number of char's to skip between lines (optional)
 *
 */
void FrotzInterpreter::z_print_table(void) {
	zword addr = zargs[0];
	zword x;
	int i, j;

	flush_buffer();

	/* Supply default arguments */
	if (zargc < 3)
		zargs[2] = 1;
	if (zargc < 4)
		zargs[3] = 0;

	/* Write text in width x height rectangle */
	x = cwp->sd.x_cursor;

	for (i = 0; i < zargs[2]; i++) {
		if (i != 0) {
			flush_buffer();

			cwp->sd.y_cursor += font_height;
			cwp->sd.x_cursor = x;

			update_cursor ();
		}

		for (j = 0; j < zargs[1]; j++) {
			zbyte c;

			LOW_BYTE(addr, c)
			addr++;

			print_char(c);
		}

		addr += zargs[3];
	}
}

/**
 * Set the value of a window property.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = number of window property to set
 *	zargs[2] = value to set window property to
 */
void FrotzInterpreter::z_put_wind_prop(void) {
	flush_buffer();

	if (zargs[1] >= 16)
		runtime_error (ERR_ILL_WIN_PROP);

	(wp + winarg0())->arr[zargs[1]] = zargs[2];
}

/**
 * Scroll a window up or down.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = #screen units to scroll up (positive) or down (negative)
 */
void FrotzInterpreter::z_scroll_window(void) {
	zword win = winarg0();
	zword y, x;

	flush_buffer();

	/* Use the correct set of colours when scrolling the window */
	if (win != cwin && !amiga_screen_model ())
		_os.set_colour(lo(wp[win].sd.colour), hi(wp[win].sd.colour));

	y = wp[win].sd.y_pos;
	x = wp[win].sd.x_pos;

	_os.scroll_area(y,
			x,
			y + wp[win].sd.y_size - 1,
			x + wp[win].sd.x_size - 1,
			(short)zargs[1]);

	if (win != cwin && !amiga_screen_model())
		_os.set_colour(lo(cwp->sd.colour), hi(cwp->sd.colour));
}

/**
 * Set the foreground and background colours.
 *
 *	zargs[0] = foreground colour
 *	zargs[1] = background colour
 *	zargs[2] = window (-3 is the current one, optional)
 */
void FrotzInterpreter::z_set_colour(void) {
	zword win = (_h.h_version == V6) ? winarg2 () : 0;

	zword fg = zargs[0];
	zword bg = zargs[1];

	flush_buffer();

	if ((short) fg == -1)	/* colour -1 is the colour at the cursor */
		fg = _os.peek_colour();
	if ((short) bg == -1)
		bg = _os.peek_colour();

	if (fg == 0)		/* colour 0 means keep current colour */
		fg = lo(wp[win].sd.colour);
	if (bg == 0)
		bg = hi(wp[win].sd.colour);

	if (fg == 1)		/* colour 1 is the system default colour */
		fg = _h.h_default_foreground;
	if (bg == 1)
		bg = _h.h_default_background;

/*	if (fg == TRANSPARENT_COLOUR)
		fg = lo(wp[win].colour);
	if (bg == TRANSPARENT_COLOUR && !(_h.hx_flags & TRANSPARENT_FLAG))
		bg = hi(wp[win].colour);*/

	if (_h.h_version == V6 && amiga_screen_model())

	/* Changing colours of window 0 affects the entire screen */
	if (win == 0) {
		int i;

		for (i = 1; i < 8; i++) {
			zword bg2 = hi(wp[i].sd.colour);
			zword fg2 = lo(wp[i].sd.colour);

			if (bg2 < 16)
				bg2 = (bg2 == lo(wp[0].sd.colour)) ? fg : bg;
			if (fg2 < 16)
				fg2 = (fg2 == lo(wp[0].sd.colour)) ? fg : bg;

			wp[i].sd.colour = (bg2 << 8) | fg2;
		}
	}

	wp[win].sd.colour = (bg << 8) | fg;

	if (win == cwin || _h.h_version != V6)
		_os.set_colour(fg, bg);
}

/**
 * Set the foreground and background colours to specific RGB colour values.
 *
 *	zargs[0] = foreground colour
 *	zargs[1] = background colour
 *	zargs[2] = window (-3 is the current one, optional)
 */
void FrotzInterpreter::z_set_true_colour(void) {
	zword win = (_h.h_version == V6) ? winarg2() : 0;

	zword true_fg = zargs[0];
	zword true_bg = zargs[1];

	zword fg = 0;
	zword bg = 0;

	flush_buffer();

	switch ((short)true_fg) {
	case -1:	/* colour -1 is the system default colour */
		fg = _h.h_default_foreground;
		break;

	case -2:	/* colour -2 means keep current colour */
		fg = lo(wp[win].sd.colour);
		break;

	case -3:	/* colour -3 is the colour at the cursor */
		fg = _os.peek_colour();
		break;

	case -4:
		fg = lo(wp[win].sd.colour);
		break;

	default:
		fg = _os.from_true_colour(true_fg);
		break;
	}

	switch ((short)true_bg) {
	case -1:	/* colour -1 is the system default colour */
		bg = _h.h_default_background;
		break;

	case -2:	/* colour -2 means keep current colour */
		bg = hi (wp[win].sd.colour);
		break;

	case -3:	/* colour -3 is the colour at the cursor */
		bg = _os.peek_colour ();
		break;

	case -4:	/* colour -4 means transparent */
/*		if (_h.hx_flags & TRANSPARENT_FLAG)
			bg = TRANSPARENT_COLOUR;
		else*/
			bg = hi (wp[win].sd.colour);
		break;

	default:
		bg = _os.from_true_colour (true_bg);
		break;
	}

	wp[win].sd.colour = (bg << 8) | fg;

	if (win == cwin || _h.h_version != V6)
		_os.set_colour(fg, bg);
}

/**
 * Set the font for text output and store the previous font.
 *
 * 	zargs[0] = number of font or 0 to keep current font
 *	zargs[1] = window (-3 is the current one, optional)
 */
void FrotzInterpreter::z_set_font(void) {
	zword font = zargs[0];
	zword win = 0;

	if (_h.h_version == V6) {
		if (zargc < 2 || (short) zargs[1] == -3)
			win = cwin;
		else if (zargs[1] >= 8)
			runtime_error (ERR_ILL_WIN);
		else
			win = zargs[1];
	}

	if (font != 0) {
		if (_os.font_data((FontType)font, &font_height, &font_width)) {
			store(wp[win].font);

			wp[win].sd.fontId = font;
			wp[win].font = (FontType)font;
			wp[win].sd.font_size = (font_height << 8) | font_width;

			if ((_h.h_version != V6) || (win == cwin)) {
				if (!ostream_memory && ostream_screen && enable_buffering) {
					print_char(ZC_NEW_FONT);
					print_char(font);
				} else 
					_os.set_font(font);
			}
		} else 
			store(0);
	} else 
		store(wp[win].font);
}

/**
 * Set the cursor position or turn the cursor on/off.
 *
 *	zargs[0] = y-coordinate or -2/-1 for cursor on/off
 *	zargs[1] = x-coordinate
 *	zargs[2] = window (-3 is the current one, optional)
 */
void FrotzInterpreter::z_set_cursor(void) {
	zword win = (_h.h_version == V6) ? winarg2 () : 1;

	zword y = zargs[0];
	zword x = zargs[1];

	flush_buffer();

	/* Supply default arguments */
	if (zargc < 3)
		zargs[2] = (zword)-3;

	/* Handle cursor on/off */
	if ((short)y < 0) {
		if ((short)y == -2)
			cursor = false;
		if ((short)y == -1)
			cursor = false;

		return;
	}

	/* Convert grid positions to screen units if this is not V6 */
	if (_h.h_version != V6) {
		if (cwin == 0)
			return;

		y = (y - 1) * _h.h_font_height + 1;
		x = (x - 1) * _h.h_font_width + 1;
	}

	/* Protect the margins */
	if (y == 0)			/* use cursor line if y-coordinate is 0 */
		y = wp[win].sd.y_cursor;
	if (x == 0)			/* use cursor column if x-coordinate is 0 */
		x = wp[win].sd.x_cursor;
	if (x <= wp[win].sd.left || x > wp[win].sd.x_size - wp[win].sd.right)
		x = wp[win].sd.left + 1;

	/* Move the cursor */
	wp[win].sd.y_cursor = y;
	wp[win].sd.x_cursor = x;

	if (win == cwin)
		update_cursor();
}

/**
 * Set the left and right margins of a window.
 *
 *	zargs[0] = left margin in pixels
 *	zargs[1] = right margin in pixels
 *	zargs[2] = window (-3 is the current one, optional)
 */
void FrotzInterpreter::z_set_margins(void) {
	zword win = winarg2 ();

	flush_buffer();

	wp[win].sd.left = zargs[0];
	wp[win].sd.right = zargs[1];

	/* Protect the margins */
	if (wp[win].sd.x_cursor <= zargs[0] || wp[win].sd.x_cursor > wp[win].sd.x_size - zargs[1]) {
		wp[win].sd.x_cursor = zargs[0] + 1;

		if (win == cwin)
			update_cursor ();
	}
}

/**
 * Set the style for text output.
 *
 * 	zargs[0] = style flags to set or 0 to reset text style
 */
void FrotzInterpreter::z_set_text_style(void) {
	zword win = (_h.h_version == V6) ? cwin : 0;
	zword style = zargs[0];

	wp[win].sd.style |= style;

	if (style == 0)
		wp[win].sd.style = 0;

	refresh_text_style();
}

/**
 * Select the current window.
 *
 *	zargs[0] = window to be selected (-3 is the current one)
 */
void FrotzInterpreter::z_set_window(void) {
	set_window(winarg0());
}/* z_set_window */

/**
 * Display the status line for V1 to V3 games.
 *
 *	no zargs used
 *
 */
void FrotzInterpreter::z_show_status(void) {
	zword global0;
	zword global1;
	zword global2;
	zword addr;

	bool brief = FALSE;

	/* One V5 game (Wishbringer Solid Gold) contains this opcode by
	   accident, so just return if the version number does not fit */

	if (_h.h_version >= V4)
		return;

	/* Read all relevant global variables from the memory of the
	   Z-machine into local variables */

	addr = _h.h_globals;
	LOW_WORD(addr, global0)
	addr += 2;
	LOW_WORD(addr, global1)
	addr += 2;
	LOW_WORD(addr, global2)

	/* Frotz uses window 7 for the status line. Don't forget to select
	   reverse and fixed width text style */
	set_window(7);

	print_char(ZC_NEW_STYLE);
	print_char(REVERSE_STYLE | FIXED_WIDTH_STYLE);

	/* If the screen width is below 55 characters then we have to use
	   the brief status line format */
	if (_h.h_screen_cols < 55)
		brief = TRUE;

	/* Print the object description for the global variable 0 */
	print_char(' ');
	print_object(global0);

	/* A header flag tells us whether we have to display the current
	   time or the score/moves information */
	if (_h.h_config & CONFIG_TIME) {	/* print hours and minutes */
		zword hours = (global1 + 11) % 12 + 1;
		pad_status_line(brief ? 15 : 20);

		print_string("Time: ");

		if (hours < 10)
			print_char(' ');
		print_num(hours);

		print_char(':');

		if (global2 < 10)
			print_char('0');
		print_num(global2);

		print_char(' ');

		print_char((global1 >= 12) ? 'p' : 'a');
		print_char('m');
	} else {				/* print score and moves */
		pad_status_line(brief ? 15 : 30);

		print_string(brief ? "S: " : "Score: ");
		print_num(global1);

		pad_status_line(brief ? 8 : 14);

		print_string(brief ? "M: " : "Moves: ");
		print_num(global2);
	}

	/* Pad the end of the status line with spaces */
	pad_status_line(0);

	/* Return to the lower window */
	set_window(0);
}

/**
 * Split the screen into an upper (1) and lower (0) window.
 *
 *	zargs[0] = height of upper window in screen units (V6) or #lines
 */
void FrotzInterpreter::z_split_window(void) {
	split_window(zargs[0]);
}

/**
 * Change the width and height of a window.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = new height in screen units
 *	zargs[2] = new width in screen units
 */
void FrotzInterpreter::z_window_size(void) {
	zword win = winarg0();

	flush_buffer();

	wp[win].sd.y_size = zargs[1];
	wp[win].sd.x_size = zargs[2];

	/* Keep the cursor within the window */
	if (wp[win].sd.y_cursor > zargs[1] || wp[win].sd.x_cursor > zargs[2])
		reset_cursor(win);

	_os.window_height(win, wp[win].sd.y_size);
}

/**
 * Set / clear / toggle window attributes.
 *
 *	zargs[0] = window (-3 is the current one)
 *	zargs[1] = window attribute flags
 *	zargs[2] = operation to perform (optional, defaults to 0)
 */
void FrotzInterpreter::z_window_style(void) {
	zword win = winarg0();
	zword flags = zargs[1];

	flush_buffer();

	/* Supply default arguments */
	if (zargc < 3)
		zargs[2] = 0;

	/* Set window style */
	switch (zargs[2]) {
	case 0: wp[win].sd.attribute = flags; break;
	case 1: wp[win].sd.attribute |= flags; break;
	case 2: wp[win].sd.attribute &= ~flags; break;
	case 3: wp[win].sd.attribute ^= flags; break;
	}

	if (cwin == win)
		update_attributes();
}

/**
 * Copy a table or fill it with zeroes.
 *
 *	zargs[0] = address of table
 * 	zargs[1] = destination address or 0 for fill
 *	zargs[2] = size of table
 *
 * Note: Copying is safe even when source and destination overlap; but
 *       if zargs[1] is negative the table _must_ be copied forwards.
 */
void FrotzInterpreter::z_copy_table(void) {
	zword addr;
	zword size = zargs[2];
	zbyte value;
	int i;

	if (zargs[1] == 0) {						/* zero table */
		for (i = 0; i < size; i++)
			storeb((zword)(zargs[0] + i), 0);
	} else if ((short) size < 0 || zargs[0] > zargs[1]) {	/* copy forwards */
		for (i = 0; i < (((short)size < 0) ? - (short)size : size); i++) {
			addr = zargs[0] + i;
			LOW_BYTE(addr, value)
			storeb((zword) (zargs[1] + i), value);
		}
	} else {						/* copy backwards */
		for (i = size - 1; i >= 0; i--) {
			addr = zargs[0] + i;
			LOW_BYTE(addr, value)
			storeb((zword)(zargs[1] + i), value);
		}
	}
}

/**
 * Store a value from a table of bytes.
 *
 *	zargs[0] = address of table
 *	zargs[1] = index of table entry to store
 */
void FrotzInterpreter::z_loadb(void) {
	zword addr = zargs[0] + zargs[1];
	zbyte value;

	LOW_BYTE(addr, value)

	store(value);
}

/**
 * Store a value from a table of words.
 *
 *	zargs[0] = address of table
 *	zargs[1] = index of table entry to store
 */
void FrotzInterpreter::z_loadw(void) {
	zword addr = zargs[0] + 2 * zargs[1];
	zword value;

	LOW_WORD(addr, value)

	store(value);
}

/**
 * Find and store the address of a target within a table.
 *
 *	zargs[0] = target value to be searched for
 *	zargs[1] = address of table
 *	zargs[2] = number of table entries to check value against
 *	zargs[3] = type of table (optional, defaults to 0x82)
 *
 * Note: The table is a word array if bit 7 of zargs[3] is set; otherwise
 *       it's a byte array. The lower bits hold the address step.
 */
void FrotzInterpreter::z_scan_table(void) {
	zword addr = zargs[1];
	int i;

	/* Supply default arguments */

	if (zargc < 4)
		zargs[3] = 0x82;

	/* Scan byte or word array */
	for (i = 0; i < zargs[2]; i++) {
		if (zargs[3] & 0x80) {	/* scan word array */
			zword wvalue;

			LOW_WORD(addr, wvalue)

			if (wvalue == zargs[0])
				goto finished;

		} else {		/* scan byte array */
			zbyte bvalue;

			LOW_BYTE(addr, bvalue)

			if (bvalue == zargs[0])
				goto finished;
		}

		addr += zargs[3] & 0x7f;
	}

	addr = 0;

finished:
	store(addr);
	branch(addr != 0);
}

/**
 * Write a byte into a table of bytes.
 *
 *	zargs[0] = address of table
 *	zargs[1] = index of table entry
 *	zargs[2] = value to be written
 */
void FrotzInterpreter::z_storeb(void) {
	storeb ((zword)(zargs[0] + zargs[1]), zargs[2]);
}

/**
 * Write a word into a table of words.
 *
 *	zargs[0] = address of table
 *	zargs[1] = index of table entry
 *	zargs[2] = value to be written
 */
void FrotzInterpreter::z_storew(void) {
	storew((zword)(zargs[0] + 2 * zargs[1]), zargs[2]);
}

/**
 * Store the current stack frame for later use with z_throw.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_catch(void) {
	store(option_save_quetzal ? frame_count : (zword)(fp - stack));
}

/*
 * z_random, store a random number or set the random number seed.
 *
 *	zargs[0] = range (positive) or seed value (negative)
 *
 */
void FrotzInterpreter::z_random() {
	if ((short) zargs[0] <= 0) {	/* set random seed */
		seed_random(-(short) zargs[0]);
			store(0);

	} else {				/* generate random number */
		zword result;

		if (random_interval != 0) {		/* ...in special mode */
			result = random_counter++;
			if (random_counter == random_interval) 
				random_counter = 0;
		} else {			/* ...in standard mode */
			result = _rnd->getRandomNumber(0xfffe);
		}

		store((zword)(result % zargs[0] + 1));
	}
}

/**
 * Go back to the given stack frame and return the given value.
 *
 *	zargs[0] = value to return
 *	zargs[1] = stack frame
 */
void FrotzInterpreter::z_throw(void) {
	if (option_save_quetzal) {
		if (zargs[1] > frame_count)
			runtime_error(ERR_BAD_FRAME);

		/* Unwind the stack a frame at a time. */
		for (; frame_count > zargs[1]; --frame_count)
			fp = stack + 1 + fp[1];
	} else {
		if (zargs[1] > STACK_SIZE)
			runtime_error(ERR_BAD_FRAME);

		fp = stack + zargs[1];
	}

	ret(zargs[0]);
}

/**
 * Call a subroutine and discard its result.
 *
 * 	zargs[0] = packed address of subroutine
 *	zargs[1] = first argument (optional)
 *	...
 *	zargs[7] = seventh argument (optional)
 */
void FrotzInterpreter::z_call_n(void) {
	if (zargs[0] != 0)
		call(zargs[0], zargc - 1, zargs + 1, 1);

}

/**
 * Call a subroutine and store its result.
 *
 * 	zargs[0] = packed address of subroutine
 *	zargs[1] = first argument (optional)
 *	...
 *	zargs[7] = seventh argument (optional)
 */
void FrotzInterpreter::z_call_s(void) {
	if (zargs[0] != 0)
		call(zargs[0], zargc - 1, zargs + 1, 0);
	else
		store(0);
}

/**
 * Branch if subroutine was called with >= n arg's.
 *
 * 	zargs[0] = number of arguments
 */
void FrotzInterpreter::z_check_arg_count(void) {
	if (fp == stack + STACK_SIZE)
		branch(zargs[0] == 0);
	else
		branch(zargs[0] <= (*fp & 0xff));
}

/**
 * Jump unconditionally to the given address.
 *
 *	zargs[0] = PC relative address
 */
void FrotzInterpreter::z_jump(void) {
	long pc;

	GET_PC(pc)

	pc += (short)zargs[0] - 2;

	if (pc >= _h.story_size)
		runtime_error(ERR_ILL_JUMP_ADDR);

	SET_PC(pc)
}

/**
 * No operation.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_nop(void) {
	/* Do nothing */
}

/**
 * Stop game and exit interpreter.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_quit(void) {
	_quitFlag = true;
}

/**
 * Return from a subroutine with the given value.
 *
 *	zargs[0] = value to return
 */
void FrotzInterpreter::z_ret(void) {
	ret(zargs[0]);
}

/**
 * Return from a subroutine with a value popped off the stack.
 *
 *	no zargs used
 */

void FrotzInterpreter::z_ret_popped(void) {
	ret(*sp++);
}

/**
 * Return from a subroutine with false (0).
 *
 * 	no zargs used
 */
void FrotzInterpreter::z_rfalse(void) {
	ret(0);
}

/**
 * Return from a subroutine with true (1).
 *
 * 	no zargs used
 */
void FrotzInterpreter::z_rtrue(void) {
	ret(1);
}

/**
 * 16bit addition.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_add(void) {
	store((zword)((short) zargs[0] + (short)zargs[1]));
}

/**
 * Bitwise AND operation.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 *
 */
void FrotzInterpreter::z_and(void) {
	store((zword)(zargs[0] & zargs[1]));
}

/**
 * arithmetic SHIFT operation.
 *
 *	zargs[0] = value
 *	zargs[1] = #positions to shift left (positive) or right
 */
void FrotzInterpreter::z_art_shift(void) {
	if ((short) zargs[1] > 0)
		store((zword)((short)zargs[0] << (short)zargs[1]));
	else
		store((zword)((short)zargs[0] >> - (short)zargs[1]));
}

/**
 * Signed 16bit division.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_div(void) {
	if (zargs[1] == 0)
		runtime_error(ERR_DIV_ZERO);

	store((zword)((short)zargs[0] / (short)zargs[1]));
}

/**
 * Branch if the first value equals any of the following.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value (optional)
 *	...
 *	zargs[3] = fourth value (optional)
 */
void FrotzInterpreter::z_je(void) {
	branch(
		zargc > 1 && (zargs[0] == zargs[1] || (
		zargc > 2 && (zargs[0] == zargs[2] || (
		zargc > 3 && (zargs[0] == zargs[3]))))));
}

/**
 * Branch if the first value is greater than the second.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_jg(void) {
	branch((short)zargs[0] > (short)zargs[1]);
}

/**
 * Branch if the first value is less than the second.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_jl(void) {
	branch((short)zargs[0] < (short)zargs[1]);
}

/**
 * Branch if value is zero.
 *
 * 	zargs[0] = value
 */
void FrotzInterpreter::z_jz(void) {
	branch((short)zargs[0] == 0);
}

/*
 * z_log_shift, logical SHIFT operation.
 *
 * 	zargs[0] = value
 *	zargs[1] = #positions to shift left (positive) or right (negative)
 *
 */
void FrotzInterpreter::z_log_shift(void) {
	if ((short) zargs[1] > 0)
		store((zword)(zargs[0] << (short)zargs[1]));
	else
		store((zword)(zargs[0] >> - (short)zargs[1]));
}

/**
 * Remainder after signed 16bit division.
 *
 * 	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_mod(void) {
	if (zargs[1] == 0)
		runtime_error(ERR_DIV_ZERO);

	store((zword)((short)zargs[0] % (short)zargs[1]));
}

/**
 * 16bit multiplication.
 *
 * 	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_mul(void) {
	store((zword)((short)zargs[0] * (short)zargs[1]));
}

/**
 * Bitwise NOT operation.
 *
 * 	zargs[0] = value
 */
void FrotzInterpreter::z_not(void) {
	store((zword)~zargs[0]);
}

/**
 * Bitwise OR operation.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_or(void) {
	store((zword)(zargs[0] | zargs[1]));
}

/**
 * 16bit substraction.
 *
 *	zargs[0] = first value
 *	zargs[1] = second value
 */
void FrotzInterpreter::z_sub(void) {
	store((zword)((short)zargs[0] - (short)zargs[1]));
}

/**
 * Branch if all the flags of a bit mask are set in a value.
 *
 *	zargs[0] = value to be examined
 *	zargs[1] = bit mask
 */
void FrotzInterpreter::z_test(void) {
	branch((zargs[0] & zargs[1]) == zargs[1]);
}

/**
 * Decrement a variable.
 *
 * 	zargs[0] = variable to decrement
 */
void FrotzInterpreter::z_dec (void) {
	zword value;

	if (zargs[0] == 0)
		(*sp)--;
	else if (zargs[0] < 16)
		(*(fp - zargs[0]))--;
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		LOW_WORD(addr, value)
		value--;
		SET_WORD(addr, value)
	}
}

/**
 * Decrement a variable and branch if now less than value.
 *
 * 	zargs[0] = variable to decrement
 * 	zargs[1] = value to check variable against
 */
void FrotzInterpreter::z_dec_chk(void) {
	zword value;

	if (zargs[0] == 0)
		value = --(*sp);
	else if (zargs[0] < 16)
		value = --(*(fp - zargs[0]));
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		LOW_WORD(addr, value)
		value--;
		SET_WORD(addr, value)
	}

	branch((short)value < (short)zargs[1]);
}

/**
 * Increment a variable.
 *
 * 	zargs[0] = variable to increment
 */
void FrotzInterpreter::z_inc(void) {
	zword value;

	if (zargs[0] == 0)
		(*sp)++;
	else if (zargs[0] < 16)
		(*(fp - zargs[0]))++;
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		LOW_WORD(addr, value)
		value++;
		SET_WORD(addr, value)
	}

}

/**
 * Increment a variable and branch if now greater than value.
 *
 * 	zargs[0] = variable to increment
 * 	zargs[1] = value to check variable against
 */
void FrotzInterpreter::z_inc_chk(void) {
	zword value;

	if (zargs[0] == 0)
		value = ++(*sp);
	else if (zargs[0] < 16)
		value = ++(*(fp - zargs[0]));
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		LOW_WORD(addr, value)
		value++;
		SET_WORD(addr, value)
	}

	branch((short)value > (short)zargs[1]);
}

/**
 * Store the value of a variable.
 *
 *	zargs[0] = variable to store
 */
void FrotzInterpreter::z_load(void) {
	zword value;

	if (zargs[0] == 0)
		value = *sp;
	else if (zargs[0] < 16)
		value = *(fp - zargs[0]);
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		LOW_WORD(addr, value)
	}

	store (value);
}

/**
 * Pop a value off the game stack and discard it.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_pop(void) {
	sp++;
}

/**
 * Pop n values off the game or user stack and discard them.
 *
 *	zargs[0] = number of values to discard
 *	zargs[1] = address of user stack (optional)
 */
void FrotzInterpreter::z_pop_stack(void) {
	if (zargc == 2) {		/* it's a user stack */
		zword size;
		zword addr = zargs[1];

		LOW_WORD(addr, size)

		size += zargs[0];
		storew(addr, size);

	} else 
		sp += zargs[0];	/* it's the game stack */
}

/**
 * Pop a value off...
 *
 * a) ...the game or a user stack and store it (V6)
 *
 *	zargs[0] = address of user stack (optional)
 *
 * b) ...the game stack and write it to a variable (other than V6)
 *
 *	zargs[0] = variable to write value to
 */
void FrotzInterpreter::z_pull(void) {
	zword value;

	if (_h.h_version != V6) {	/* not a V6 game, pop stack and write */
		value = *sp++;

		if (zargs[0] == 0)
			*sp = value;
		else if (zargs[0] < 16)
			*(fp - zargs[0]) = value;
		else {
			zword addr = _h.h_globals + 2 * (zargs[0] - 16);
			SET_WORD(addr, value)
		}
	} else {			/* it's V6, but is there a user stack? */
		if (zargc == 1) {	/* it's a user stack */
			zword size;
			zword addr = zargs[0];

			LOW_WORD(addr, size)

			size++;
			storew(addr, size);

			addr += 2 * size;
			LOW_WORD(addr, value)

		} else 
			value = *sp++;	/* it's the game stack */

		store (value);
	}
}

/**
 * Push a value onto the game stack.
 *
 *	zargs[0] = value to push onto the stack
 */
void FrotzInterpreter::z_push(void) {
	*--sp = zargs[0];
}

/**
 * Push a value onto a user stack then branch if successful.
 *
 *	zargs[0] = value to push onto the stack
 *	zargs[1] = address of user stack
 */
void FrotzInterpreter::z_push_stack(void) {
	zword size;
	zword addr = zargs[1];

	LOW_WORD(addr, size)

	if (size != 0) {
		storew((zword)(addr + 2 * size), zargs[0]);

		size--;
		storew(addr, size);
	}

	branch(size != 0);
}

/**
 * Write a value to a variable.
 *
 * 	zargs[0] = variable to be written to
 *	  zargs[1] = value to write
 */
void FrotzInterpreter::z_store(void) {
	zword value = zargs[1];

	if (zargs[0] == 0)
		*sp = value;
	else if (zargs[0] < 16)
		*(fp - zargs[0]) = value;
	else {
		zword addr = _h.h_globals + 2 * (zargs[0] - 16);
		SET_WORD(addr, value)
	}
}

/**
 * Branch if the story file is a legal copy.
 *
 *	no zargs used
 */
void FrotzInterpreter::z_piracy(void) {
	branch(!option_piracy);
}

} // end of namespace Frotz
