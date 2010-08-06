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

#include "common/system.h"

#include "graphics/thumbnail.h"

#include "engines/darkseed2/saveload.h"

namespace DarkSeed2 {

SaveMetaInfo::SaveMetaInfo() {
	saveDate = 0;
	saveTime = 0;
	playTime = 0;
}

void SaveMetaInfo::fillWithCurrentTime(uint32 startTime, uint32 prevPlayTime) {
	TimeDate curTime;
	g_system->getTimeAndDate(curTime);

	saveDate = (((curTime.tm_year + 1900) & 0xFFFF) << 16) |
	           (((curTime.tm_mon  +    1) & 0x00FF) <<  8) |
	            ((curTime.tm_mday         & 0x00FF));
	saveTime =  ((curTime.tm_hour         & 0x00FF) <<  8) |
	             (curTime.tm_min          & 0x00FF);

	uint32 playHour;
	uint32 playMin = ((g_system->getMillis() - startTime) / 60000) + prevPlayTime;

	playHour = playMin / 60;
	playMin %= 60;

	playTime = ((playHour & 0xFF) << 8) | (playMin & 0xFF);
}

int SaveMetaInfo::getSaveYear() const {
	return saveDate >> 16;
}

int SaveMetaInfo::getSaveMonth()  const {
	return (saveDate & 0x0000FF00) >> 8;
}

int SaveMetaInfo::getSaveDay() const {
	return saveDate & 0x000000FF;
}

int SaveMetaInfo::getSaveHour() const {
	return saveTime >> 8;
}

int SaveMetaInfo::getSaveMinute() const {
	return saveTime & 0x00FF;
}

int SaveMetaInfo::getPlayHour() const {
	return playTime >> 8;
}

int SaveMetaInfo::getPlayMinute() const {
	return playTime & 0x00FF;
}

uint32 SaveMetaInfo::getPlayTime() const {
	return getPlayHour() * 60 + getPlayMinute();
}


bool SaveLoad::syncMetaInfo(Common::Serializer &serializer, SaveMetaInfo &meta) {
	if (!serializer.syncVersion(1))
		return false;

	if (!serializer.matchBytes("SVMDARKSEED2", 12))
		return false;

	serializer.syncString(meta.description);

	serializer.syncAsUint32LE(meta.saveDate);
	serializer.syncAsUint32LE(meta.saveTime);
	serializer.syncAsUint32LE(meta.playTime);

	return true;
}

bool SaveLoad::loadMetaInfo(Common::SeekableReadStream *stream, SaveMetaInfo &meta) {
	Common::Serializer serializer(stream, 0);

	return syncMetaInfo(serializer, meta);
}

Common::String SaveLoad::createFileName(const Common::String &base, int slot) {
	if ((slot < 0) || (slot > kMaxSlot))
		return "";

	char buffer[3];
	snprintf(buffer, 3, "%02d", slot);

	return base + ".s" + buffer;
}

Common::OutSaveFile *SaveLoad::openForSaving(const Common::String &file) {
	if (file.empty())
		return 0;

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();

	return saveMan->openForSaving(file);
}

Common::InSaveFile *SaveLoad::openForLoading(const Common::String &file) {
	if (file.empty())
		return 0;

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();

	return saveMan->openForLoading(file);
}

bool SaveLoad::getState(SaveStateDescriptor &state, const Common::String &target, int slot) {
	Common::InSaveFile *file = openForLoading(createFileName(target, slot));
	if (!file)
		return false;

	Graphics::Surface *thumbnail = new Graphics::Surface;

	if (!Graphics::loadThumbnail(*file, *thumbnail)) {
		delete file;
		return false;
	}

	SaveMetaInfo meta;
	if (!loadMetaInfo(file, meta)) {
		delete file;
		return false;
	}

	state = SaveStateDescriptor(slot, meta.description);

	state.setThumbnail(thumbnail);
	state.setDeletableFlag(true);
	state.setWriteProtectedFlag(false);
	state.setSaveDate(meta.getSaveYear(), meta.getSaveMonth(), meta.getSaveDay());
	state.setSaveTime(meta.getSaveHour(), meta.getSaveMinute());
	state.setPlayTime(meta.getPlayHour(), meta.getPlayMinute());

	delete file;
	return true;
}

bool SaveLoad::getStates(SaveStateList &list, const Common::String &target) {
	for (int i = 0; i <= kMaxSlot; i++) {
		SaveStateDescriptor state;
		if (getState(state, target, i))
			list.push_back(state);
	}

	return true;
}

bool SaveLoad::removeSave(const Common::String &base, int slot) {
	Common::String file = createFileName(base, slot);
	if (file.empty())
		return false;

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();

	return saveMan->removeSavefile(file);
}

bool SaveLoad::skipThumbnail(Common::SeekableReadStream &in) {
	return ::Graphics::skipThumbnail(in);
}

bool SaveLoad::saveThumbnail(Common::WriteStream &out) {
	return ::Graphics::saveThumbnail(out);
}

void SaveLoad::syncTimestamp(Common::Serializer &serializer, uint32 &time) {
	uint32 now = g_system->getMillis();

	if (serializer.isSaving()) {
		uint32 t = (time < now) ? 0xFFFFFFFF : (time - now);

		serializer.syncAsUint32LE(t);
	} else {
		serializer.syncAsUint32LE(time);

		time = (time == 0xFFFFFFFF) ? 0 : (time + now);
	}
}

template<>
void SaveLoad::sync<uint16>(Common::Serializer &serializer, uint16 &var) {
	serializer.syncAsUint16LE(var);
}

template<>
void SaveLoad::sync<uint32>(Common::Serializer &serializer, uint32 &var) {
	serializer.syncAsUint32LE(var);
}

template<>
void SaveLoad::sync<int16>(Common::Serializer &serializer, int16 &var) {
	serializer.syncAsSint16LE(var);
}

template<>
void SaveLoad::sync<int32>(Common::Serializer &serializer, int32 &var) {
	serializer.syncAsSint32LE(var);
}

template<>
void SaveLoad::sync<Common::String>(Common::Serializer &serializer, Common::String &var) {
	serializer.syncString(var);
}

template<>
void SaveLoad::sync<byte>(Common::Serializer &serializer, byte &var) {
	serializer.syncAsByte(var);
}

template<>
void SaveLoad::sync<bool>(Common::Serializer &serializer, bool &var) {
	byte b = var;

	serializer.syncAsByte(b);

	var = b;
}

template<>
void SaveLoad::sync<Common::Rect>(Common::Serializer &serializer, Common::Rect &var) {
	serializer.syncAsSint16LE(var.left);
	serializer.syncAsSint16LE(var.top);
	serializer.syncAsSint16LE(var.right);
	serializer.syncAsSint16LE(var.bottom);
}

} // End of namespace DarkSeed2
