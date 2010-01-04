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

#ifndef DARKSEED2_CONVERSATIONBOX_H
#define DARKSEED2_CONVERSATIONBOX_H

#include "common/rect.h"
#include "common/str.h"
#include "common/array.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sprite.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class Graphics;
class TalkManager;

class Conversation;

class TextObject;
class Sprite;
class TalkLine;

class ConversationBox {
public:
	static const int32 kWidth  = 640; ///< The box's width.
	static const int32 kHeight =  70; ///< The box's heigth.

	ConversationBox(Resources &resources, Variables &variables,
			Graphics &graphics, TalkManager &talkManager);
	~ConversationBox();

	/** Notify the box that a new palette is active. */
	void newPalette();

	/** Start the specified conversation. */
	bool start(const Common::String &conversation);
	/** Restart the conversation. */
	bool restart();

	/** Is the conversation still running? */
	bool isActive() const;

	/** Move the box to these coordinates. */
	void move(int32 x, int32 y);

	/** Redraw the conversation box. */
	void redraw(Sprite &sprite, Common::Rect area);

	/** Notify that the mouse was moved. */
	void notifyMouseMove(int32 x, int32 y);
	/** Notify that the mouse clicked. */
	void notifyClicked(int32 x, int32 y);

	/** Check for changes in the box's status. */
	void updateStatus();

private:
	/** A scrolling action. */
	enum ScrollAction {
		kScrollActionUp,   ///< Scroll up.
		kScrollActionDown, ///< Scroll down.
		kScrollActionNone  ///< No scroll.
	};

	/** A box's state. */
	enum State {
		kStateWaitUserAction, ///< Waiting for the user to do something.
		kStatePlayingLine,    ///< Playing an entry's line.
		kStatePlayingReply    ///< Playign an entry's reply.
	};

	/** A conversation line. */
	struct Line {
		/** The talk line with the sound and text. */
		TalkLine *talk;
		/** The line's text wrapped to the text area. */
		Common::StringList texts;
		/** The graphical text lines of the wrapped text lines. */
		Common::Array<TextObject *> textObjects;

		Line(TalkLine *line = 0, byte color = 0);
		~Line();

		/** Return the line's name. */
		const Common::String &getName() const;
	};

	/** A reference to a physical line. */
	struct PhysLineRef {
		/** Which real line does it belong to. */
		uint32 n;
		/** Iterator to the real line. */
		Common::Array<Line *>::const_iterator it1;
		/** Iterator to the line's text part. */
		Common::StringList::const_iterator it2;
		/** Iterator to the line's graphic part. */
		Common::Array<TextObject *>::iterator it3;

		/** Return the line's name. */
		const Common::String &getName() const;
		/** Return the line's text. */
		const Common::String &getString() const;
		/** Return the line's graphic. */
		TextObject &getTextObject();
		/** Return the line. */
		Line *getLine();

		/** Return the line's number. */
		uint32 getLineNum() const;
		/** Is the physical line the first line of a real line? */
		bool isTop() const;
	};

	Resources   *_resources;
	Variables   *_variables;
	Graphics    *_graphics;
	TalkManager *_talkMan;

	Common::Rect _area; ///< The area where the box is visible.

	/** The currently running conversation. */
	Conversation *_conversation;

	Sprite *_origSprites; ///< The original box part sprites.
	Sprite *_sprites;     ///< The box part sprites adapted to the current palette.

	TextObject *_markerSelect;   ///< Marker text of a selected line.
	TextObject *_markerUnselect; ///< Marker text of an unselected line.

	Common::Array<Line *> _lines; ///< All current conversation lines.

	Common::Rect _textAreas[3];   ///< Areas of the 3 visible lines.
	Common::Rect _scrollAreas[2]; ///< Areas of the scroll up/down buttons.

	uint32 _physLineCount; ///< Number of physical lines.
	uint32 _physLineTop;   ///< The visible physical line at the top.

	uint32 _selected; ///< The selected physical line.

	Sprite _box; ///< The box's sprite.

	byte _colorSelected;   ///< Color index of a selected line.
	byte _colorUnselected; ///< Color index of an unselected line.
	byte _colorShading;    ///< Color index of the background shading.

	State _state; ///< The current state.

	uint8  _curSpeaker;                     ///< The current playing line's speaker.
	uint16 _curReply;                       ///< The current playing reply.
	Common::Array<TalkLine *> _nextReplies; ///< The replies playing next.

	/** Load all needed sprites. */
	void loadSprites();
	/** Reset all needed sprites and adjust to the current palette. */
	void resetSprites();

	/** Rebuild the box's sprite. */
	void rebuild();

	/** Update the color indices from the current palette. */
	void updateColors();

	// Update helpers
	void clearLines();
	void clearReplies();
	void updateLines();
	void updateScroll();
	void drawLines();
	void redrawLines();

	/** Scroll the lines. */
	void doScroll(ScrollAction scroll);
	/** Pick a line. */
	void pickLine(Line *line);

	/** Translate the physical line number to a real line number. */
	uint32 physLineNumToRealLineNum(uint32 physLineNum) const;

	/** Find the nth physical line. */
	bool findPhysLine(uint32 n, PhysLineRef &ref) const;
	/** Find the next physical line. */
	bool nextPhysLine(PhysLineRef &ref) const;
	/** Helper method for nextPhysLine. */
	bool nextPhysRealLine(PhysLineRef &ref) const;

	/** Get the text area the coordinates are in. */
	int getTextArea(int32 x, int32 y);
	/** Get the scroll action area the coordinates are in. */
	ScrollAction getScrollAction(int32 x, int32 y);

	/** Get the currently selected line. */
	Line *getSelectedLine();

	bool canScroll()     const; ///< Is scrolling possible?
	bool canScrollUp()   const; ///< Is scrolling up possible?
	bool canScrollDown() const; ///< Is scrolling down possible?

	/** Speak that line. */
	void speakLine(TalkLine &line);

	void speakerVariable(uint8 speaker, bool on);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATIONBOX_H
