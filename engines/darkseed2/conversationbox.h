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
#include "engines/darkseed2/saveable.h"
#include "engines/darkseed2/versionformats.h"
#include "engines/darkseed2/sprite.h"
#include "engines/darkseed2/font.h"

namespace DarkSeed2 {

class Resources;
class Variables;
class Graphics;
class TalkManager;

class Conversation;

class TextObject;
class Sprite;
class TalkLine;

class ConversationBox : public Saveable {
public:
	ConversationBox(Resources &resources, Variables &variables,
			Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager);
	~ConversationBox();

	int32 getWidth () const;
	int32 getHeight() const;

	/** Start the specified conversation. */
	bool start(const Common::String &conversation);
	/** Restart the conversation. */
	bool restart();
	/** Stop the currently running conversation. */
	void stop();

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

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	struct BoxProperties {
		int32 width;  ///< The box's width.
		int32 height; ///< The box's height.

		const char *frameFile; ///< File used for the full frame.

		const char *frameTopFile;    ///< File used for the top part of the frame.
		const char *frameBottomFile; ///< File used for the bottom part of the frame.
		const char *frameLeftFile;   ///< File used for the left part of the frame.
		const char *frameRightFile;  ///< File used for the right part of the frame.

		int32 frameLeftRightWidth; ///< The width of the left and right parts of the frame.
		int32 frameTopDownHeight;  ///< The height of the top and bottom parts of the frame.

		int32 scrollUp[4];   ///< The scroll up button's coordinates.
		int32 scrollDown[4]; ///< The scroll down button's coordinates.

		/** File used the sprite that's shown when scrolling up is allowed. */
		const char *scrollUpFile;
		/** File used the sprite that's shown when scrolling down is allowed. */
		const char *scrollDownFile;
		/** File used the sprite that's shown when scrolling up and down is allowed. */
		const char *scrollUpDownFile;

		int32 textAreaWidth;  ///< The width of the raw text area.
		int32 textAreaHeight; ///< The height of the raw text area.
		int32 textHeight;     ///< The height of a text line.
		int32 textMargin;     ///< The maximum width of a text line.

		uint32 numLines; ///< Max number of text lines visible in the box.
	} _boxProps;

	/** A scrolling action. */
	enum ScrollAction {
		kScrollActionUp,   ///< Scroll up.
		kScrollActionDown, ///< Scroll down.
		kScrollActionNone  ///< No scroll.
	};

	/** A box's state. */
	enum State {
		kStateWaitUserAction = 0, ///< Waiting for the user to do something.
		kStatePlayingLine    = 1, ///< Playing an entry's line.
		kStatePlayingReply   = 2  ///< Playign an entry's reply.
	};

	/** A conversation line. */
	struct Line {
		/** The talk line with the sound and text. */
		TalkLine *talk;
		/** The line's text wrapped to the text area. */
		FontManager::TextList texts;
		/** The graphical text lines of the selected wrapped text lines. */
		Common::Array<TextObject *> textObjectsSelected;
		/** The graphical text lines of the unselected wrapped text lines. */
		Common::Array<TextObject *> textObjectsUnselected;

		/** The number within the lines array. */
		uint32 lineNumber;

		Line(TalkLine *line = 0, const FontManager *fontManager = 0,
				uint32 colorSelected = 0, uint32 colorUnselected = 0);
		~Line();

		/** Return the line's name. */
		const Common::String &getName() const;
	};

	/** A reference to a physical line. */
	struct PhysLineRef {
		/** Which real line does it belong to. */
		uint32 n;
		/** Iterator to the real line. */
		Common::Array<Line *>::const_iterator itLine;
		/** Iterator to the line's text part. */
		FontManager::TextList::const_iterator itString;
		/** Iterator to the line's selected graphic part. */
		Common::Array<TextObject *>::iterator itTextSel;
		/** Iterator to the line's unselected graphic part. */
		Common::Array<TextObject *>::iterator itTextUnsel;

		/** Return the line's name. */
		const Common::String &getName() const;
		/** Return the line's selected graphic. */
		TextObject *getSelectedText();
		/** Return the line's unselected graphic. */
		TextObject *getUnselectedText();
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

	const FontManager *_fontMan;

	Common::Rect _area; ///< The area where the box is visible.

	/** The currently running conversation. */
	Conversation *_conversation;

	Sprite *_sprites;     ///< The box part sprites.

	TextObject *_markerSelect;   ///< Marker text of a selected line.
	TextObject *_markerUnselect; ///< Marker text of an unselected line.

	Common::Array<Line *> _lines; ///< All current conversation lines.

	Common::Rect *_textAreas;   ///< Areas of the visible lines.
	Common::Rect _scrollAreas[2]; ///< Areas of the scroll up/down buttons.

	uint32 _physLineCount; ///< Number of physical lines.
	uint32 _physLineTop;   ///< The visible physical line at the top.

	uint32 _selected; ///< The selected physical line.

	Sprite _box; ///< The box's sprite.

	uint32 _colorSelected;   ///< Color index of a selected line.
	uint32 _colorUnselected; ///< Color index of an unselected line.
	uint32 _colorShading;    ///< Color index of the background shading.

	State _state; ///< The current state.

	uint8  _curSpeaker;                     ///< The current playing line's speaker.
	uint16 _curReply;                       ///< The current playing reply.
	Common::Array<TalkLine *> _nextReplies; ///< The replies playing next.

	// For saving/loading
	uint32 _curLineNumber;        ///< The number of the current line.
	Common::String _curReplyName; ///< The name of the current reply.

	/** Fill in the box poperties struct, depending on the game version. */
	void fillInBoxProperties(GameVersion gameVersion);

	/** Load all needed sprites. */
	void loadSprites();

	/** Build the box's sprite. */
	void build();

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

	/** Set the give speaker variable to a specific state. */
	void speakerVariable(uint8 speaker, bool on);
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATIONBOX_H
