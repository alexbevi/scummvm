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
#include "common/list.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/sortedlist.h"
#include "engines/darkseed2/palette.h"
#include "engines/darkseed2/sprite.h"

namespace Common {
	class SeekableReadStream;
}

namespace DarkSeed2 {

class Resources;
class Variables;
class TalkManager;
class RoomConfigManager;
class Movie;

class ConversationBox;
class InventoryBox;
class Room;

class Resource;
class TextObject;
class SpriteObject;
class Animation;
class Cursors;

class Graphics {
public:
	struct SpriteQueueEntry {
		Animation *anim;
		SpriteObject *object;
		bool persistent;
		uint32 layer;
		int frame;

		SpriteQueueEntry();
		SpriteQueueEntry(Animation &a, uint32 l, bool per);

		bool operator<(const SpriteQueueEntry &right) const;
	};

	typedef SortedList<SpriteQueueEntry> SpriteQueue;

	struct SpriteRef {
		bool empty;
		SpriteQueue::iterator it;

		SpriteRef();
	};

	static const uint32 kScreenWidth  = 640;  ///< The screen width.
	static const uint32 kScreenHeight = 480;  ///< The screen height.

	Graphics(Resources &resources, Variables &variables, Cursors &cursors);
	~Graphics();

	/** Init the graphics subsystem. */
	void init(TalkManager &talkManager, RoomConfigManager &roomConfigManager, Movie &movie);

	/** Get the conversation box. */
	ConversationBox &getConversationBox();
	/** Get the inventory box. */
	InventoryBox &getInventoryBox();
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

	/** Clear all room animations. */
	void clearAnimations();

	/** Add an animation frame to the rendering queue. */
	void addAnimation(Animation &animation, SpriteRef &ref,
			int32 frame, int32 x = -1, int32 y = -1, bool persistent = false);
	/** Remove an animation frame from the rendering queue. */
	void removeAnimation(SpriteRef &ref);

	/** Add a room animation frame to the rendering queue. */
	void addRoomAnimation(const Common::String &animation, SpriteRef &ref,
			int32 frame, int32 x = -1, int32 y = -1, bool persistent = false);

	/** Merge the sprite's palette into the current game palette. */
	void mergePalette(Sprite &sprite);

	/** Get the game palette. */
	const Palette &getPalette() const;

	/** Request a redraw of the whole screen. */
	void requestRedraw();
	/** Request a redraw of that screen area. */
	void requestRedraw(const Common::Rect &rect);

	/** Copy the screen to the ScummVM screen. */
	void retrace();

	/** Register that sprite to be the current background. */
	void registerBackground(const Sprite &background);
	/** Remove the background. */
	void unregisterBackground();

private:
	Resources *_resources;
	Variables *_variables;
	Cursors   *_cursors;
	Movie     *_movie;

	ConversationBox *_conversationBox; ///< The conversation box.
	InventoryBox    *_inventoryBox;    ///< The inventory box.
	Room            *_room;            ///< The current room.

	Palette _gamePalette; ///< The game palette.
	Sprite _screen;       ///< The game screen.

	Common::List<Common::Rect> _dirtyRects; ///< The dirty rectangles.
	bool _dirtyAll;                         ///< Whole screen dirty?

	const Sprite *_background; ///< The current background.

	TextObject *_talk; ///< The currently active speech line.

	/** The animation frame sprites queue. */
	SpriteQueue _spriteQueue;

	/** Initialize the game palette. */
	void initPalette();
	/** Apply the game palette. */
	void applyGamePalette();

	/** Redraw the dirty screen areas. */
	void redraw();
	/** Redraw that area of the game screen. */
	void redraw(Common::Rect rect);

	/** Dirty the whole screen. */
	void dirtyAll();
	/** Add that area to the dirty rectangles. */
	void dirtyRectsAdd(const Common::Rect &rect);
	/** Copy all dirty areas to the screen. */
	bool dirtyRectsApply();
};

} // End of namespace DarkSeed2

#endif // DARKSEED2_GRAPHICS_H
