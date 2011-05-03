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

#include "darkseed2/darkseed2.h"
#include "darkseed2/saveable.h"
#include "darkseed2/versionformats.h"
#include "darkseed2/sprite.h"
#include "darkseed2/font.h"

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
	virtual ~ConversationBox();

	virtual int32 getWidth () const = 0;
	virtual int32 getHeight() const = 0;

	bool init();

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

	/** Speak that text. */
	virtual void talk(const TextLine &textLine) = 0;

	/** Notify that the mouse was moved. */
	virtual void notifyMouseMove(int32 x, int32 y) = 0;
	/** Notify that the mouse clicked. */
	virtual void notifyClicked(int32 x, int32 y) = 0;

	/** Check for changes in the box's status. */
	virtual void updateStatus() = 0;

protected:
	/** A box's state. */
	enum State {
		kStateWaitUserAction = 0, ///< Waiting for the user to do something.
		kStatePlayingLine    = 1, ///< Playing an entry's line.
		kStatePlayingReply   = 2, ///< Playign an entry's reply.
		kStateWaitEndTalk    = 3  ///< Wait for a talk line to end.
	};

	/** A conversation line. */
	struct Line {
		/** The talk line with the sound and text. */
		TalkLine *talk;
		/** The line's text wrapped to the text area. */
		FontManager::TextList texts;
		/** The graphical text lines. */
		Common::Array< Common::Array<TextObject *> > textObjects;

		/** The number within the lines array. */
		uint32 lineNumber;

		Line(TalkLine *line = 0, const FontManager *fontManager = 0,
				const Common::Array<uint32> *colors = 0, int32 maxWidth = 0);
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
		/** Iterator to the line's graphic parts. */
		Common::Array< Common::Array<TextObject *> >::iterator itText;

		/** Return the line's name. */
		const Common::String &getName() const;
		/** Return the line's graphics. */
		const Common::Array<TextObject *> &getText() const;
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

	bool _inited;

	Common::Rect _area; ///< The area where the box is visible.

	/** The currently running conversation. */
	Conversation *_conversation;

	Common::Array<Line *> _lines; ///< All current conversation lines.

	uint32 _physLineCount; ///< Number of physical lines.
	uint32 _physLineTop;   ///< The visible physical line at the top.

	uint32 _selected; ///< The selected physical line.

	Sprite *_box; ///< The box's sprite.

	State _state; ///< The current state.

	uint8  _curSpeaker;                     ///< The current playing line's speaker.
	uint16 _curReply;                       ///< The current playing reply.
	Common::Array<TalkLine *> _nextReplies; ///< The replies playing next.

	// For saving/loading
	uint32 _curLineNumber;        ///< The number of the current line.
	Common::String _curReplyName; ///< The name of the current reply.

	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

	/** Load all needed sprites. */
	virtual bool loadSprites() = 0;

	/** Build the box's sprite. */
	virtual void build() = 0;

	// Update helpers
	void clearLines();
	void clearReplies();

	virtual void updateLines()  = 0;
	virtual void updateScroll() = 0;
	virtual void drawLines()    = 0;
	virtual void redrawLines()  = 0;

	/** Translate the physical line number to a real line number. */
	uint32 physLineNumToRealLineNum(uint32 physLineNum) const;

	/** Find the nth physical line. */
	bool findPhysLine(uint32 n, PhysLineRef &ref) const;
	/** Find the next physical line. */
	bool nextPhysLine(PhysLineRef &ref) const;
	/** Helper method for nextPhysLine. */
	bool nextPhysRealLine(PhysLineRef &ref) const;

	/** Get the currently selected line. */
	Line *getSelectedLine();

	/** Speak that line. */
	void speakLine(TalkLine &line);

	/** Set the give speaker variable to a specific state. */
	void speakerVariable(uint8 speaker, bool on);
};

class ConversationBoxWindows : public ConversationBox {
public:
	ConversationBoxWindows(Resources &resources, Variables &variables,
			Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager);
	~ConversationBoxWindows();

	int32 getWidth () const;
	int32 getHeight() const;

	void talk(const TextLine &textLine);

	void notifyMouseMove(int32 x, int32 y);
	void notifyClicked(int32 x, int32 y);

	void updateStatus();

protected:
	/** A scrolling action. */
	enum ScrollAction {
		kScrollActionUp,   ///< Scroll up.
		kScrollActionDown, ///< Scroll down.
		kScrollActionNone  ///< No scroll.
	};

	/** Color value used for the background shading. */
	uint32 _colorShading;
	/** Color values used for the the text. */
	Common::Array<uint32> _colorText;

	Sprite *_sprites; ///< The box part sprites.

	Common::Array<TextObject *> _marker; ///< Line marker texts

	Common::Rect *_textAreas;   ///< Areas of the visible lines.
	Common::Rect _scrollAreas[2]; ///< Areas of the scroll up/down buttons.

	bool loadSprites();

	void build();

	void updateLines();
	void updateScroll();
	void drawLines();
	void redrawLines();

	bool canScroll()     const; ///< Is scrolling possible?
	bool canScrollUp()   const; ///< Is scrolling up possible?
	bool canScrollDown() const; ///< Is scrolling down possible?

	/** Get the text area the coordinates are in. */
	int getTextArea(int32 x, int32 y);
	/** Get the scroll action area the coordinates are in. */
	ScrollAction getScrollAction(int32 x, int32 y);

	/** Scroll the lines. */
	void doScroll(ScrollAction scroll);
	/** Pick a line. */
	void pickLine(Line *line);
};

class ConversationBoxSaturn : public ConversationBox {
public:
	ConversationBoxSaturn(Resources &resources, Variables &variables,
			Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager);
	~ConversationBoxSaturn();

	int32 getWidth () const;
	int32 getHeight() const;

	void talk(const TextLine &textLine);

	void notifyMouseMove(int32 x, int32 y);
	void notifyClicked(int32 x, int32 y);

	void updateStatus();

protected:
	/** Color values used for the background. */
	uint32 _colorBackground;    ///< Color index of the background.
	/** Color values used for the the text. */
	Common::Array<uint32> _colorText;

	Sprite *_frameSprites;  ///< The box frame sprites.
	Sprite *_buttonSprites; ///< The box button sprites.

	bool loadSprites();

	void build();

	void updateLines();
	void updateScroll();
	void drawLines();
	void redrawLines();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_CONVERSATIONBOX_H
