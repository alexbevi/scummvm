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

#include "ultima/ultima8/misc/pent_include.h"
#include "ultima/ultima8/conf/config_file_manager.h"

namespace Ultima {
namespace Ultima8 {

using Std::string;

ConfigFileManager *ConfigFileManager::_configFileManager = nullptr;

ConfigFileManager::ConfigFileManager() {
	debugN(MM_INFO, "Creating ConfigFileManager...\n");

	_configFileManager = this;
}

ConfigFileManager::~ConfigFileManager() {
	debugN(MM_INFO, "Destroying ConfigFileManager...\n");

	clear();
	_configFileManager = nullptr;
}

bool ConfigFileManager::readConfigFile(string fname, istring root) {
	INIFile *inifile = new INIFile();
	inifile->clear(root);
	if (!inifile->readConfigFile(fname)) {
		delete inifile;
		return false;
	}

	_iniFiles.push_back(inifile);
	return true;
}

void ConfigFileManager::clear() {
	for (Std::vector<INIFile *>::iterator i = _iniFiles.begin();
	        i != _iniFiles.end(); ++i) {
		delete(*i);
	}
	_iniFiles.clear();
}

void ConfigFileManager::clearRoot(istring root) {
	Std::vector<INIFile *>::iterator i = _iniFiles.begin();

	while (i != _iniFiles.end()) {
		if ((*i)->checkRoot(root)) {
			delete(*i);
			i = _iniFiles.erase(i);
		} else {
			++i;
		}
	}
}

bool ConfigFileManager::exists(istring key) {
	return (findKeyINI(key) != nullptr);
}

bool ConfigFileManager::get(istring key, string &ret) {
	INIFile *ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}


bool ConfigFileManager::get(istring key, int &ret) {
	INIFile *ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}

bool ConfigFileManager::get(istring key, bool &ret) {
	INIFile *ini = findKeyINI(key);
	if (!ini) return false;

	ini->value(key, ret);
	return true;
}

Std::vector<istring> ConfigFileManager::listSections(istring root,
        bool longformat) {
	Std::vector<istring> sections;

	Std::set<istring> sectionset;
	Std::set<istring>::iterator iter;

	for (Std::vector<INIFile *>::iterator i = _iniFiles.begin();
	        i != _iniFiles.end(); ++i) {
		if ((*i)->checkRoot(root)) {
			(*i)->listSections(sectionset, longformat);
		}
	}

	for (iter = sectionset.begin(); iter != sectionset.end(); ++iter) {
		sections.push_back(*iter);
	}

	return sections;
}

KeyMap ConfigFileManager::listKeyValues(istring section,
        bool longformat) {
	KeyMap values;

	for (Std::vector<INIFile *>::iterator i = _iniFiles.begin();
	        i != _iniFiles.end(); ++i) {
		if ((*i)->checkRoot(section)) {
			(*i)->listKeyValues(values, section, longformat);
		}
	}

	return values;
}


INIFile *ConfigFileManager::findKeyINI(istring key) {
	for (Std::vector<INIFile *>::reverse_iterator i = _iniFiles.rbegin();
	        i != _iniFiles.rend(); ++i) {
		if ((*i)->hasKey(key))
			return (*i);
	}

	return nullptr;
}

} // End of namespace Ultima8
} // End of namespace Ultima
