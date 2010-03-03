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
#include "common/frac.h"

#include "engines/darkseed2/darkseed2.h"
#include "engines/darkseed2/saveable.h"
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

class Graphics : public Saveable {
public:
	/** An entry in the sprite queue. */
	struct SpriteQueueEntry {
		bool persistent; ///< Is the sprite persistent or should it be removed on room change?

		Animation    *anim;    ///< The animation the sprite is from.
		SpriteObject *object; ///< The object the sprite belongs to.

		int32 layer; ///< The drawing layer the sprite is on.
		int   frame; ///< The current frame within the animation.

		SpriteQueueEntry();
		SpriteQueueEntry(Animation &a, int32 l, bool per);

		/** SpriteQueueEntrys are sortable by layer. */
		bool operator<(const SpriteQueueEntry &right) const;
	};

	/** A sprite queue. */
	typedef SortedList<SpriteQueueEntry> SpriteQueue;

	/** A reference to a specific sprite within the sprite queue. */
	struct SpriteRef {
		bool empty; ///< Empty reference?

		SpriteQueue::iterator it; ///< The iterator within the sprite queue the reference refers to.

		SpriteRef();

		/** Clear the sprite reference. */
		void clear();

		/** Is the sprite up-to-date with the information in the parameters? */
		bool isUpToDate(int32 frame, int32 x, int32 y, frac_t scale) const;
	};

	Graphics(int32 width, int32 height, Resources &resources, Variables &variables, Cursors &cursors);
	~Graphics();

	/** Init the graphics subsystem. */
	void init(TalkManager &talkManager, ScriptRegister &scriptRegister,
			RoomConfigManager &roomConfigManager, Movie &movie);

	/** Get the conversation box. */
	ConversationBox &getConversationBox();
	/** Get the inventory box. */
	InventoryBox &getInventoryBox();
	/** Get the current room. */
	Room &getRoom();

	int32 getScreenWidth()  const;
	int32 getScreenHeight() const;

	/** Check for status changes. */
	void updateStatus();

	/** Fill the whole screen with palette entry 0. */
	void clearScreen();

	/** Clear the game palette. */
	void clearPalette();
	/** Change the game palette. */
	void setPalette(const Palette &pal);

	/** Enter a special mode for movie playback. */
	void enterMovieMode();

	/** Leave the special movie mode. */
	void leaveMovieMode();

	/** Assert that palette entry 0 is black, after the palette
	 *  has been changed from "the outside", e.g. AVI decoder.
	 */
	void assertPalette0();

	/** Speak that text. */
	void talk(TextObject *talkObject);
	/** End the current talk. */
	void talkEnd();

	/** Clear all room animations. */
	void clearAnimations();

	/** Add an animation frame to the rendering queue. */
	void addAnimation(Animation &animation, SpriteRef &ref, bool persistent = false);
	/** Remove an animation frame from the rendering queue. */
	void removeAnimation(SpriteRef &ref);

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

protected:
	bool saveLoad(Common::Serializer &serializer, Resources &resources);
	bool loading(Resources &resources);

private:
	Resources *_resources;
	Variables *_variables;
	Cursors   *_cursors;
	Movie     *_movie;

	ConversationBox *_conversationBox; ///< The conversation box.
	InventoryBox    *_inventoryBox;    ///< The inventory box.
	Room            *_room;            ///< The current room.

	int32 _screenWidth;
	int32 _screenHeight;

	Palette _gamePalette; ///< The game palette.
	Sprite  _screen;      ///< The game screen.

	bool                       _dirtyAll;   ///< Whole screen dirty?
	Common::List<Common::Rect> _dirtyRects; ///< The dirty rectangles.

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
