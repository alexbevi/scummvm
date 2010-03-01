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
#include "common/serializer.h"

#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/imageconverter.h"
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

static const int32 kConversationX =   0;
static const int32 kConversationY = 410;
static const int32 kInventoryX    =   0;
static const int32 kInventoryY    = 410;

Graphics::SpriteQueueEntry::SpriteQueueEntry() {
	anim       = 0;
	object     = 0;
	persistent = false;
	layer      = -1;
	frame      = -1;
}

Graphics::SpriteQueueEntry::SpriteQueueEntry(Animation &a, int32 l, bool per) {
	anim       = &a;
	object     = &*a;
	persistent = per;
	layer      = l;
	frame      = a.currentFrame();
}

bool Graphics::SpriteQueueEntry::operator<(const SpriteQueueEntry &right) const {
	return layer < right.layer;
}


Graphics::SpriteRef::SpriteRef() {
	empty = true;
}

void Graphics::SpriteRef::clear() {
	empty = true;
}

bool Graphics::SpriteRef::isUpToDate(int32 frame, int32 x, int32 y, frac_t scale) const {
	if (empty)
		// Empty reference => Not up-to-date
		return false;

	if ((frame >= 0) && (it->frame != frame))
		// Frames don't match => Not up-to-date
		return false;

	if (it->object->getX() != x)
		// X positions don't match => Not up-to-date
		return false;
	if (it->object->getY() != y)
		// Y positions don't match => Not up-to-date
		return false;

	if (it->object->getScale() != scale)
		// Scalings don't match => Not up-to-date
		return false;

	// Must be up-to-date then
	return true;
}


Graphics::Graphics(Resources &resources, Variables &variables, Cursors &cursors) {
	_resources = &resources;
	_variables = &variables;
	_cursors   = &cursors;
	_movie     = 0;

	clearPalette();
	ImgConv.registerStandardPalette(_gamePalette);

	_screen.create(kScreenWidth, kScreenHeight);

	_dirtyAll = false;

	_background = 0;

	_talk = 0;

	_conversationBox = 0;
	_inventoryBox    = 0;
	_room            = 0;
}

Graphics::~Graphics() {
	ImgConv.unregisterStandardPalette();

	delete _room;
	delete _inventoryBox;
	delete _conversationBox;
	delete _talk;
}

void Graphics::init(TalkManager &talkManager, ScriptRegister &scriptRegister,
		RoomConfigManager &roomConfigManager, Movie &movie) {

	_movie = &movie;

	// Init conversation box
	_conversationBox = new ConversationBox(*_resources, *_variables, *this, talkManager);
	_conversationBox->move(kConversationX, kConversationY);

	// Init inventory box
	_inventoryBox = new InventoryBox(*_resources, *_variables, scriptRegister, *this, talkManager, *_cursors);
	_inventoryBox->move(kInventoryX, kInventoryY);

	// Init room
	_room = new Room(*_variables, scriptRegister, *this);
	_room->registerConfigManager(roomConfigManager);

	_screen.darken();

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

void Graphics::clearScreen() {
	_screen.darken();
	dirtyAll();
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
	dirtyAll();
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

void Graphics::clearAnimations() {
	// Remove all non-persistent sprites
	SpriteQueue::iterator sprite = _spriteQueue.begin();
	while (sprite != _spriteQueue.end()) {
		if (!sprite->persistent)
			sprite = _spriteQueue.erase(sprite);
		else
			++sprite;
	}
}

void Graphics::addAnimation(Animation &animation, SpriteRef &ref, bool persistent) {
	if (!ref.empty && (ref.it->anim == &animation) && (ref.it->frame == animation.currentFrame())) {
		// The animation is already at that frame
		return;
	}

	// Remove the old frame
	removeAnimation(ref);

	// The vertical position of the feet dictates the layer, and therefore the drawing order
	uint32 layer = animation->getFeetY();

	// Push it into the queue
	ref.it    = _spriteQueue.insert(SpriteQueueEntry(animation, layer, persistent));
	ref.empty = false;

	// We need to redraw that area
	requestRedraw(animation->getArea());
}

void Graphics::removeAnimation(SpriteRef &ref) {
	if (ref.empty)
		// Nothing to do
		return;

	if (ref.it->object)
		// Redraw the area
		requestRedraw(ref.it->object->getArea());

	// Remove the sprite from the queue
	_spriteQueue.erase(ref.it);

	// Marking reference as empty
	ref.empty = true;
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

		const ::Graphics::Surface &surface = _screen.getTrueColor();

		g_system->copyRectToScreen((const byte *) surface.pixels, surface.pitch, 0, 0, surface.w, surface.h);

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

		const ::Graphics::Surface &surface = _screen.getTrueColor();
		const byte *data = (const byte *) surface.getBasePtr(it->left, it->top);

		g_system->copyRectToScreen(data, surface.pitch, it->left, it->top, it->width(), it->height());
	}

	_dirtyRects.clear();
	return true;
}

void Graphics::registerBackground(const Sprite &background) {
	debugC(-1, kDebugGraphics, "New background");

	assert(_conversationBox);
	assert(_inventoryBox);

	_screen.darken();

	_background = &background;

	setPalette(_background->getPalette());

	requestRedraw();
}

void Graphics::unregisterBackground() {
	_background = 0;
	dirtyAll();
}

void Graphics::requestRedraw() {
	dirtyAll();
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

void Graphics::redraw(Common::Rect rect) {
	rect.clip(Common::Rect(0, 0, kScreenWidth, kScreenHeight));

	if (rect.isEmpty())
		return;

	if (_movie->isPlaying()) {
		_movie->redraw(_screen, rect);
		return;
	}

	if (_background)
		_screen.blit(*_background, rect, rect.left, rect.top, true);

	// Clip the area for animation sprite redraw to the room area
	Common::Rect spriteArea = rect;
	_room->clipToRoom(spriteArea);

	for (SpriteQueue::iterator it = _spriteQueue.begin(); it != _spriteQueue.end(); ++it)
		it->object->redraw(_screen, spriteArea);

	if (_talk)
		_talk->redraw(_screen, rect);

	if (_conversationBox && _conversationBox->isActive())
		_conversationBox->redraw(_screen, rect);

	if (_inventoryBox && _inventoryBox->isVisible())
		_inventoryBox->redraw(_screen, rect);
}

bool Graphics::saveLoad(Common::Serializer &serializer, Resources &resources) {
	if (serializer.isLoading()) {
		_spriteQueue.clear();

		delete _talk;
		_talk = 0;

		unregisterBackground();
		dirtyAll();
	}

	assert(_conversationBox);
	assert(_inventoryBox);
	assert(_room);

	if (!_conversationBox->doSaveLoad(serializer, resources))
		return false;
	if (!_inventoryBox->doSaveLoad(serializer, resources))
		return false;
	if (!_room->doSaveLoad(serializer, resources))
		return false;

	return true;
}

bool Graphics::loading(Resources &resources) {
	registerBackground(_room->getBackground());
	dirtyAll();
	return true;
}

} // End of namespace DarkSeed2
