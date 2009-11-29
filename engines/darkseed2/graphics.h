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

#ifndef DARKSEED2_GRAPHICS_H
#define DARKSEED2_GRAPHICS_H

#include "common/rect.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/palette.h"
#include "engines/darkseed2/sprite.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resources;
class Variables;
class TalkManager;

class ConversationBox;
class Room;

class Resource;
class TextObject;

/** A screen part. */
enum ScreenPart {
	kScreenPartPlayArea,    ///< The play area.
	kScreenPartConversation ///< The conversation box.
};

class Graphics {
public:
	static const uint32 _screenWidth  = 640;  ///< The screen width.
	static const uint32 _screenHeight = 480;  ///< The screen height.
	static const uint32 _conversationX = 0;   ///< The conversation box's x coordinate.
	static const uint32 _conversationY = 410; ///< The conversation box's y coordinate.

	Graphics(Resources &resources, Variables &variables);
	~Graphics();

	/** Init the graphics subsystem. */
	void init(TalkManager &talkManager);

	/** Get the conversation box. */
	ConversationBox &getConversationBox();
	/** Get the current room. */
	Room &getRoom();

	/** Check for status changes. */
	void updateStatus();

	/** Clear the game palette. */
	void clearPalette();
	/** Change the game palette. */
	void setPalette(const Palette &pal);

	void enterMovieMode();

	void leaveMovieMode();

	/** Assert that palette entry 0 is black, after the palette
	  * has been changed from "the outside", e.g. AVI decoder.
	  */
	void assertPalette0();

	/** Speak that text. */
	void talk(TextObject *talkObject);
	/** End the current talk. */
	void talkEnd();

	/** Blit the sprite to the screen. */
	void blitToScreen(const Sprite &from, Common::Rect area,
			uint32 x, uint32 y, bool transp = false);
	/** Blit the sprite to the screen. */
	void blitToScreen(const Sprite &from, uint32 x, uint32 y, bool transp = false);

	void blitToScreenDouble(const Sprite &from, Common::Rect area,
			uint32 x, uint32 y, bool transp = false);
	void blitToScreenDouble(const Sprite &from, uint32 x, uint32 y, bool transp = false);

	/** Merge the sprite's palette into the current game palette. */
	void mergePalette(Sprite &sprite);

	/** Get the game palette. */
	const Palette &getPalette() const;

	/** Redraw that screen part. */
	void redraw(ScreenPart part);

	/** Copy the screen to the ScummVM screen. */
	void retrace();

	/** Register that sprite to be the current background. */
	void registerBackground(const Sprite &background);
	/** Remove the background. */
	void unregisterBackground();

private:
	Resources *_resources;
	Variables *_variables;

	ConversationBox *_conversationBox; ///< The conversation box.
	Room *_room;                       ///< The current room.

	Palette _gamePalette; ///< The game palette.
	Sprite _screen;       ///< The game screen.

	Common::List<Common::Rect> _dirtyRects; ///< The dirty rectangle.
	bool _dirtyAll;                         ///< Whole screen dirty?

	const Sprite *_background; ///< The current background.

	TextObject *_talk; ///< The currently active speech line.

	bool _movieMode;

	/** Initialize the game palette. */
	void initPalette();
	/** Apply the game palette. */
	void applyGamePalette();

	/** Redraw that area of the game screen. */
	void redrawScreen(const Common::Rect &rect);

	/** Dirty the whole screen. */
	void dirtyAll();
	/** Add that area to the dirty rectangles. */
	void dirtyRectsAdd(const Common::Rect &rect);
	/** Copy all dirty areas to the screen. */
	bool dirtyRectsApply();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_GRAPHICS_H
