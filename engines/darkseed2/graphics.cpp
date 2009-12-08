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

#include "common/stream.h"

#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/variables.h"
#include "engines/darkseed2/resources.h"
#include "engines/darkseed2/talk.h"
#include "engines/darkseed2/roomconfig.h"
#include "engines/darkseed2/movie.h"
#include "engines/darkseed2/cursors.h"
#include "engines/darkseed2/conversationbox.h"
#include "engines/darkseed2/inventorybox.h"
#include "engines/darkseed2/room.h"
#include "engines/darkseed2/graphicalobject.h"

namespace DarkSeed2 {

static const uint32 kConversationX =   0;
static const uint32 kConversationY = 410;
static const uint32 kInventoryX    =   0;
static const uint32 kInventoryY    = 410;

Graphics::SpriteRef::SpriteRef() {
	layer = -1;
	anim  =  0;
	frame =  0;
}


Graphics::Graphics(Resources &resources, Variables &variables, Cursors &cursors) {
	_resources = &resources;
	_variables = &variables;
	_cursors   = &cursors;
	_movie     = 0;

	clearPalette();

	_screen.create(kScreenWidth, kScreenHeight);

	_dirtyAll = false;

	_background = 0;

	_talk = 0;

	_conversationBox = 0;
	_room            = 0;
}

Graphics::~Graphics() {
	delete _room;
	delete _inventoryBox;
	delete _conversationBox;
	delete _talk;
}

void Graphics::init(TalkManager &talkManager, RoomConfigManager &roomConfigManager, Movie &movie) {
	_movie = &movie;

	_conversationBox = new ConversationBox(*_resources, *_variables, *this, talkManager);
	_conversationBox->move(kConversationX, kConversationY);

	_inventoryBox = new InventoryBox(*_resources, *_variables, *this, talkManager, *_cursors);
	_inventoryBox->move(kInventoryX, kInventoryY);

	_room = new Room(*_variables, *this);
	_room->registerConfigManager(roomConfigManager);

	_screen.clear();

	initPalette();
	dirtyAll();
}

ConversationBox &Graphics::getConversationBox() {
	assert(_conversationBox);

	return *_conversationBox;
}

InventoryBox &Graphics::getInventoryBox() {
	assert(_inventoryBox);

	return *_inventoryBox;
}

Room &Graphics::getRoom() {
	assert(_room);

	return *_room;
}

void Graphics::updateStatus() {
	assert(_conversationBox);
	assert(_inventoryBox);

	_conversationBox->updateStatus();
	_inventoryBox->updateStatus();
}

void Graphics::clearPalette() {
	_gamePalette.clear();
	applyGamePalette();
}

void Graphics::setPalette(const Palette &pal) {
	_gamePalette = pal;

	initPalette();
}

void Graphics::enterMovieMode() {
}

void Graphics::leaveMovieMode() {
	applyGamePalette();
	requestRedraw();
}

void Graphics::assertPalette0() {
	byte index0[4];

	g_system->grabPalette(index0, 0, 1);

	if ((index0[0] == 0) && (index0[1] == 0) && (index0[2] == 0))
		return;

	index0[0] = 0;
	index0[1] = 0;
	index0[2] = 0;

	g_system->setPalette(index0, 0, 1);
	g_system->updateScreen();
}

void Graphics::initPalette() {
	// Palette entry 0 is transparent
	_gamePalette[0] = 0;
	_gamePalette[1] = 0;
	_gamePalette[2] = 0;

	// Some standard colors
	_gamePalette.addColor(255, 255, 255, true);
	_gamePalette.addColor(  0,   0,   0, true);
	_gamePalette.addColor(239, 167, 127, true);

	applyGamePalette();
}

void Graphics::applyGamePalette() {
	byte pal[1024];

	_gamePalette.makeSystemCompatible(pal);

	g_system->setPalette(pal, 0, 256);
}

void Graphics::talk(TextObject *talkObject) {
	talkEnd();

	_talk = talkObject;

	requestRedraw(_talk->getArea());
}

void Graphics::talkEnd() {
	if (!_talk)
		return;

	requestRedraw(_talk->getArea());

	delete _talk;
	_talk = 0;
}

void Graphics::clearRoomAnimations() {
	for (int i = 0; i < kLayerCount; i++)
		_spriteQueue[i].clear();
}

void Graphics::addRoomAnimation(const Common::String &animation, SpriteRef &ref,
		int32 frame, int layer, int32 x, int32 y) {

	assert(_room);

	Animation *anim = _room->getAnimation(animation);
	if (!anim)
		// No animation
		return;

	if ((ref.layer != -1) && (ref.anim == anim) && (ref.frame == frame))
		// The animation is already at that frame
		return;

	// Remove the old frame
	removeRoomAnimation(ref);

	if ((frame < 0) || (frame > 99))
		// Broken frame number
		return;

	anim->setFrame(frame);

	// Set the layer and flip the order
	layer = (kLayerCount - 1) - CLIP<int>(layer, 0, (kLayerCount - 1));

	if ((x < 0) || (y < 0))
		(*anim)->move();
	else
		(*anim)->moveFeet((uint32) x, (uint32) y);

	// Push it into the queue
	_spriteQueue[layer].push_back(&**anim);

	// We need to redraw that area
	requestRedraw((*anim)->getArea());

	// Refresh reference
	ref.layer = layer;
	ref.it    = _spriteQueue[layer].reverse_begin();
	ref.anim  = anim;
	ref.frame = frame;
}

void Graphics::removeRoomAnimation(SpriteRef &ref) {
	if (ref.layer < 0)
		// Empty reference
		return;

	// We need to redraw that area
	requestRedraw((*ref.it)->getArea());

	// Remove the animation frame from the queue
	_spriteQueue[ref.layer].erase(ref.it);

	// Marking reference as empty
	ref.layer = -1;
}

void Graphics::mergePalette(Sprite &from) {
	debugC(2, kDebugGraphics, "Merging palettes");

	Common::Array<byte> changeSet = _gamePalette.merge(from.getPalette(), true);

	from.applyChangeSet(changeSet);
	applyGamePalette();

	dirtyAll();
}

const Palette &Graphics::getPalette() const {
	return _gamePalette;
}

void Graphics::retrace() {
	redraw();

	if (dirtyRectsApply())
		g_system->updateScreen();
}

void Graphics::dirtyAll() {
	_dirtyAll = true;
	_dirtyRects.clear();
}

void Graphics::dirtyRectsAdd(const Common::Rect &rect) {
	if (_dirtyAll)
		return;

	if ((rect.left == 0) && (rect.top == 0) &&
	    (rect.right >= ((int) kScreenWidth)) && (rect.bottom >= ((int) kScreenHeight))) {

		dirtyAll();
		return;
	}

	// Upper limit. Dirty rectangle overhead isn't that great either :P
	if (_dirtyRects.size() >= 30) {
		dirtyAll();
		return;
	}

/*
	// If the rectangle intersects with an already existing one, merge them
	for (Common::List<Common::Rect>::iterator it = _dirtyRects.begin(); it != _dirtyRects.end(); ++it) {
		if(it->intersects(rect)) {
			it->extend(rect);
			return;
		}
	}
*/

	// Otherwise, add it as a separate one
	_dirtyRects.push_back(rect);
}

bool Graphics::dirtyRectsApply() {
	if (_dirtyAll) {
		debugC(5, kDebugGraphics, "Refreshing the whole screen");
		// Everything is dirty, copy the whole screen

		g_system->copyRectToScreen(_screen.getData(), _screen.getWidth(),
					0, 0, _screen.getWidth(), _screen.getHeight());
		_dirtyAll = false;
		return true;
	}

	if (_dirtyRects.empty())
		return false;

	debugC(5, kDebugGraphics, "Refreshing %d rectangle(s)", _dirtyRects.size());

	int screenWidth  = _screen.getWidth();
	int screenHeight = _screen.getHeight();

	Common::Rect screenArea(screenWidth, screenHeight);

	Common::List<Common::Rect>::iterator it;
	for (it = _dirtyRects.begin(); it != _dirtyRects.end(); ++it) {
		it->clip(screenArea);

		if (it->isEmpty())
			continue;

		const byte *data = _screen.getData() + it->top * screenWidth + it->left;

		g_system->copyRectToScreen(data, screenWidth, it->left, it->top, it->width(), it->height());
	}

	_dirtyRects.clear();
	return true;
}

void Graphics::registerBackground(const Sprite &background) {
	debugC(-1, kDebugGraphics, "New background");

	assert(_conversationBox);
	assert(_inventoryBox);

	_background = &background;

	setPalette(_background->getPalette());

	_conversationBox->newPalette();
	_inventoryBox->newPalette();

	requestRedraw();
}

void Graphics::unregisterBackground() {
	_background = 0;
	requestRedraw();
}

void Graphics::requestRedraw() {
	requestRedraw(Common::Rect(0, 0, kScreenWidth, kScreenHeight));
}

void Graphics::requestRedraw(const Common::Rect &rect) {
	dirtyRectsAdd(rect);
}

void Graphics::redraw() {
	if (_dirtyAll) {
		redraw(Common::Rect(0, 0, kScreenWidth, kScreenHeight));
		return;
	}

	for (Common::List<Common::Rect>::iterator it = _dirtyRects.begin(); it != _dirtyRects.end(); ++it)
		redraw(*it);
}

void Graphics::redraw(const Common::Rect &rect) {
	if (rect.isEmpty())
		return;

	if (_movie->isPlaying()) {
		_movie->redraw(_screen, rect);
		return;
	}

	if (_background)
		_screen.blit(*_background, rect, rect.left, rect.top, false);

	// Clip the area for animation sprite redraw to the room area
	Common::Rect spriteArea = rect;
	_room->clipToRoom(spriteArea);
	for (int i = 0; i < kLayerCount; i++) {
		for (SpriteQueue::iterator it = _spriteQueue[i].begin(); it != _spriteQueue[i].end(); ++it) {
			(*it)->redraw(_screen, spriteArea);
		}
	}

	if (_talk)
		_talk->redraw(_screen, rect);

	if (_conversationBox && _conversationBox->isActive())
		_conversationBox->redraw(_screen, rect);

	if (_inventoryBox && _inventoryBox->isVisible())
		_inventoryBox->redraw(_screen, rect);
}

} // End of namespace DarkSeed2
