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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "hdb/hdb.h"

namespace HDB {

bool Input::init() {
	_stylusDown = false;

	warning("STUB: Input::init: Set the default key values");

	_mouseX = kScreenWidth / 2;
	_mouseY = kScreenHeight / 2;

	_mouseLButton = _mouseMButton = _mouseRButton = 0;

	return true;
}

void Input::setButtons(uint16 b) {
	warning("STUB: Input: setButtons required");
}

uint16 Input::getButtons() {
	warning("STUB: Input: getButtons required");
	return 0;
}

void Input::stylusDown(int x, int y) {
	int worldX, worldY;
	GameState gs;
	static uint32 delay = 0, time;

	// Don't let the screen get clicked too fast
	time = g_system->getMillis();
	if (time - delay < 100)
		return;
	time = delay;

	_stylusDown = true;
	_stylusDownX = x;
	_stylusDownY = y;
	gs = g_hdb->getGameState();

	switch (gs) {
	case GAME_TITLE:
		warning("STUB: Menu: changeToMenu required");
		g_hdb->changeGameState();
		break;
	case GAME_MENU:
		warning("STUB: Menu: processInput required");
		break;
	case GAME_PLAY:
		// Is Player Dead? Click on TRY AGAIN
		if (g_hdb->_ai->playerDead()) {
			warning("STUB: TRY AGAIN is onscreen");
			return;
		}

		// Is Dialog Active?
		if (g_hdb->_window->dialogActive()) {
			g_hdb->_window->closeDialog();
			if (!g_hdb->_ai->cinematicsActive())
				return;
		}

		// Is a Choice Dialog Active?
		warning("STUB: stylusDown: Check Choice Dialog Active");

		// Is MessageBar active?
		warning("STUB: stylusDown: Check Message Bar Active");

		// In a cinematic?
		if (g_hdb->_ai->playerLocked())
			return;

		// Check for map dragging in debug Mode and place player there
		warning("STUB: stylusDown: Check for Map dragging in Debug Mode");

		// Clicked in the world
		g_hdb->_map->getMapXY(&worldX, &worldY);
		worldX = ((worldX + x) / kTileWidth) * kTileWidth;
		worldY = ((worldY + y) / kTileHeight) * kTileHeight;

		// Don't allow a click into INV/DELIVERIES area to go into the world
		if (x >= (kScreenWidth - 32 * 5))
			return;

		// Toggle Walk Speed if we clicked Player
		int nx, ny;
		static uint32 lastRunning = g_system->getMillis();
		g_hdb->_ai->getPlayerXY(&nx, &ny);
		if (nx == worldX && ny == worldY) {
			if (lastRunning > g_system->getMillis())
				return;
			lastRunning = g_system->getMillis() + 1000 * kRunToggleDelay;
			g_hdb->_ai->togglePlayerRunning();
			if (g_hdb->_ai->playerRunning())
				g_hdb->_window->centerTextOut("Running Speed", kScreenHeight - 32, kRunToggleDelay * kGameFPS);
			else
				g_hdb->_window->centerTextOut("Walking Speed", kScreenHeight - 32, kRunToggleDelay * kGameFPS);
			warning("STUB: Play SND_SWITCH_USE");
		}

		g_hdb->setTargetXY(worldX, worldY);
		break;
	case GAME_LOADING:
		debug(9, "stylusDown: GAME_LOADING found");
		break;
	}
}

void Input::stylusUp(int x, int y) {
	_stylusDown = false;
}

void Input::stylusMove(int x, int y) {
	// In a cinematic?
	if (g_hdb->_ai->playerLocked() || g_hdb->_ai->playerDead())
		return;

	switch (g_hdb->getGameState()) {
	case GAME_PLAY:
		warning("STUB: stylusMove: Add GetDebug() check");
		break;
	case GAME_MENU:
		warning("STUB: stylusMove: Menu::processInput() required");
		break;
	default:
		debug(9, "stylusMove: Unintended GameState");
		break;
	}
}

void Input::updateMouse(int newX, int newY) {
	_mouseX = newX;
	_mouseY = newY;

	if (_mouseX < 0)
		_mouseX = 0;
	else if (_mouseX >= kScreenWidth)
		_mouseX = kScreenWidth - 1;

	if (_mouseY < 0)
		_mouseY = 0;
	else if (_mouseY >= kScreenHeight)
		_mouseY = kScreenHeight - 1;

	// Turn Cursor back on?
	if (!g_hdb->_drawMan->getPointer()) {
		g_hdb->_drawMan->showPointer(true);
	}

	// Check if LButton is being dragged
	if (_mouseLButton) {
		stylusMove(_mouseX, _mouseY);
	}
}

void Input::updateMouseButtons(int l, int m, int r) {
	_mouseLButton += l;
	_mouseMButton += m;
	_mouseRButton += r;

	// Check if LButton has been pressed
	// Check if LButton has been lifted
	if (_mouseLButton) {
		if (_mouseX > (kScreenWidth - 32 * 5) && _mouseY < 240) {
			g_hdb->_window->checkInvSelect(_mouseX, _mouseY);
		} else if (_mouseX > (kScreenWidth - 32 * 5) && _mouseY >= 240) {
			warning("STUB: updateMouseButtons: checkDeliveriesSelect() required");
		} else {
			warning("STUB: updateMouseButtons: Add pause check");
			stylusDown(_mouseX, _mouseY);
		}
	} else if (!_mouseLButton) {
		stylusUp(_mouseX, _mouseY);
	}

	// Check if MButton has been pressed
	if (_mouseMButton) {
		warning("STUB: updateMouseButtons: Add pause check");
		g_hdb->_ai->clearWaypoints();
		warning("STUB: Play SND_POP");
	}

	// Check if RButton has been pressed
	if (_mouseRButton) {
		warning("STUB: updateMouseButtons: Add pause check");
		uint16 buttons = getButtons() | kButtonB;
		setButtons(buttons);
	}
}

} // End of Namespace
