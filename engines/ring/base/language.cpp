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
 */

#include "ring/base/language.h"

#include "ring/helpers.h"

namespace Ring {

#pragma region Language

Language::Language() : BaseObject(kLanguageInvalid), _channel(0) {
}

Language::Language(LanguageId id, Common::String name, Common::String folder, uint channel) :
	BaseObject(id), _name(name), _folder(folder), _channel(channel) {
}

#pragma endregion

#pragma region LanguageHandler

LanguageHandler::LanguageHandler() : _language(kLanguageInvalid) {
}

LanguageHandler::~LanguageHandler() {
	CLEAR_ARRAY(Language, _languages);
}

void LanguageHandler::add(LanguageId id, Common::String name, Common::String folder, uint channel) {
	if (_languages.has(id))
		error("[LanguageHandler::add] ID already exists (%d)", id);

	_languages.push_back(new Language(id, name, folder, channel));
}

void LanguageHandler::setActiveLanguage(Common::String name) {
	if (_languages.empty())
		error("[LanguageHandler::setActiveLanguage] No language available");

	for (AssociativeArray<Language *>::iterator i = _languages.begin(); i != _languages.end(); i++) {
		if ((*i)->getName() == name) {
			_language = (LanguageId)(*i)->getId();
			return;
		}
	}

	error("[LanguageHandler::setActiveLanguage] Language doesn't exist (%s)", name.c_str());
}

Common::String LanguageHandler::getFolder(LanguageId id) {
	if (!_languages.has(id))
		error("[LanguageHandler::getFolder] ID doesn't exist (%d)", id);

	return _languages.get(id)->getFolder();
}

uint32 LanguageHandler::getChannel(LanguageId id) {
	if (!_languages.has(id))
		error("[LanguageHandler::getChannel] ID doesn't exist (%d)", id);

	return _languages.get(id)->getChannel();
}

#pragma endregion

} // End of namespace Ring
