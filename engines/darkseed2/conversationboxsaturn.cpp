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

#include "common/textconsole.h"

#include "darkseed2/conversationbox.h"
#include "darkseed2/imageconverter.h"
#include "darkseed2/graphics.h"

namespace DarkSeed2 {

// Size
static const int32 kWidth  = 320;
static const int32 kHeight =  48;

// Sprites
static const char *kFileFrame[4]    = { "DLG_L"   , "DLG_TOP" , "DLG_R"    , "DLG_BTM"   };
static const int32 kSizeFrame[4][2] = { { 40, 48 }, { 240, 7 }, {  40, 48 }, { 240,  7 } };
static const int32 kPosFrame [4][2] = { {  0,  0 }, {  40, 0 }, { 280,  0 }, {  40, 41 } };

static const char *kFileButton[8]    = { "TEXT_U"  , "TEXT_D"   , "TEXT_UD"  , "TEXT_FIN",
                                         "TEXT_BLK", "TEXT_L"   , "TEXT_R"   , "TEXT_LR"   };
static const int32 kSizeButton[8][2] = { { 40, 48 }, {  40, 48 }, {  40, 48 }, {  40, 48 },
                                         { 40, 48 }, {  40, 48 }, {  40, 48 }, {  40, 48 } };
static const int32 kPosButton [8][2] = { {  0,  0 }, {   0,  0 }, {   0,  0 }, {   0,  0 },
                                         {  0,  0 }, { 280,  0 }, { 280,  0 }, { 280,  0 } };

// Scroll button coordinates
static const int32 kScrollUp   [4] = {  15,  24,  34,  40};
static const int32 kScrollDown [4] = {  15,  41,  34,  57};
static const int32 kScrollLeft [4] = {  15,  24,  34,  40};
static const int32 kScrollRight[4] = {  15,  41,  34,  57};

// Area coordinates
static const int32 kTextAreaWidth  = 250;
static const int32 kTextAreaHeight =  34;
static const int32 kTextHeight     =  12;
static const int32 kTextMargin     =  45;
static const int32 kTextLineWidth  = 208;

// Colors
static const byte kColorText      [3] = {255, 255, 255};
static const byte kColorBackground[3] = {  0,   0,   0};

ConversationBoxSaturn::ConversationBoxSaturn(Resources &resources, Variables &variables,
		Graphics &graphics, TalkManager &talkManager, const FontManager &fontManager) :
		ConversationBox(resources, variables, graphics, talkManager, fontManager) {

	_frameSprites  = 0;
	_buttonSprites = 0;
}

ConversationBoxSaturn::~ConversationBoxSaturn() {
	delete[] _frameSprites;
	delete[] _buttonSprites;
}

int32 ConversationBoxSaturn::getWidth() const {
	return kWidth;
}

int32 ConversationBoxSaturn::getHeight() const {
	return kHeight;
}

void ConversationBoxSaturn::talk(const TextLine &textLine) {
	_graphics->talk(textLine);
}

void ConversationBoxSaturn::notifyMouseMove(int32 x, int32 y) {
	if (!_inited)
		return;

}

void ConversationBoxSaturn::notifyClicked(int32 x, int32 y) {
	if (!_inited)
		return;

	stop();
}

void ConversationBoxSaturn::updateStatus() {
	if (!_inited)
		return;

}

bool ConversationBoxSaturn::loadSprites() {
	Palette palette;

	if (!palette.loadFromPAL555(*_resources, "PARTS")) {
		warning("ConversationBoxSaturn::loadSprites(): Failed to load PARTS.PAL");
		return false;
	}

	ImgConv.registerStandardPalette(palette);

	_frameSprites = new Sprite[3];
	_frameSprites[2].create(kWidth, kHeight);

	// Building the frame
	for (int i = 0; i < 4; i++) {
		Sprite frame;

		if (!frame.loadFromBoxImage(*_resources, kFileFrame[i], kSizeFrame[i][0], kSizeFrame[i][1])) {
			warning("ConversationBoxSaturn::loadSprites(): Failed to load sprite \"%s\"", kFileFrame[i]);
			return false;
		}

		_frameSprites[2].blit(frame, kPosFrame[i][0], kPosFrame[i][1]);
	}

	_buttonSprites = new Sprite[8];
	for (int i = 0; i < 8; i++) {
		if (!_buttonSprites[i].loadFromBoxImage(*_resources, kFileButton[i], kSizeButton[i][0], kSizeButton[i][1])) {
			warning("ConversationBoxSaturn::loadSprites(): Failed to load sprite \"%s\"", kFileButton[i]);
			return false;
		}
	}

	ImgConv.unregisterStandardPalette();

	return true;
}

void ConversationBoxSaturn::build() {
	_colorBackground =   ImgConv.getColor(kColorBackground[0], kColorBackground[1], kColorBackground[2]);
	_colorText.push_back(ImgConv.getColor(kColorText      [0], kColorText      [1], kColorText      [2]));

	_box = new Sprite;
	_box->create(kWidth, kHeight);

	// The background
	_frameSprites[1].create(kTextAreaWidth, kTextAreaHeight);
	_frameSprites[1].fill(_colorBackground);

	_frameSprites[0].create(kWidth, kHeight);

	// Put the background
	_frameSprites[0].blit(_frameSprites[1], (kWidth  - kTextAreaWidth)  / 2,
	                                        (kHeight - kTextAreaHeight) / 2, true);
	// Put the frame
	_frameSprites[0].blit(_frameSprites[2], 0, 0, true);

	_box->blit(_frameSprites[0], 0, 0, false);
}

void ConversationBoxSaturn::updateLines() {
}

void ConversationBoxSaturn::updateScroll() {
	_box->blit(_frameSprites[0], 0, 0, false);
}

void ConversationBoxSaturn::drawLines() {
	updateScroll();

	_graphics->requestRedraw(_area);
}

void ConversationBoxSaturn::redrawLines() {
	_box->blit(_frameSprites[0], 0, 0, false);

	drawLines();
}

} // End of namespace DarkSeed2
