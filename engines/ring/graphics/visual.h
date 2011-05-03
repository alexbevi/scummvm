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

#ifndef RING_VISUAL_H
#define RING_VISUAL_H

#include "ring/shared.h"

#include "common/keyboard.h"
#include "common/rect.h"
#include "common/serializer.h"

namespace Ring {

class Hotspot;
class ImageHandle;
class Object;
class Text;

class Visual : public BaseObject, public Common::Serializable {
public:
	Visual(Id id);
	virtual ~Visual();

	virtual void draw() = 0;
	virtual uint32 handleLeftButtonUp(const Common::Point &point) = 0;
	virtual uint32 handleUpdate(const Common::Point &point) = 0;
	virtual uint32 handleLeftButtonDown(Common::Point point) { return 0; }
	virtual bool handleKey(Common::KeyCode key) { return false; }
	virtual void alloc() = 0;
	virtual void dealloc() = 0;

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s) {}

	// Accessors
	void setField8(uint32 val) { _field_8 = val; }
	void setVisible(bool state) { _visible = state; }

protected:
	uint32 _field_8;
	bool _visible;
};

class VisualObjectList : public Visual {
public:
	VisualObjectList(Id id);
	~VisualObjectList();

	virtual void draw();
	virtual uint32 handleLeftButtonUp(const Common::Point &point);
	virtual uint32 handleUpdate(const Common::Point &point);
	virtual void alloc();
	virtual void dealloc();

	// Init
	void init(uint32 a1,
	          Common::String imagePath, Common::String iconPath, Common::String filename3, Common::String filename4, Common::String filename5,
	          Common::String filename6, Common::String filename7, Common::String filename8, Common::String filename9, Common::String filename10,
	          Common::String filename11, Common::String filename12, Common::String filename13,
	          DrawType drawType, ArchiveType archiveType);
	void setOrigin(const Common::Point &point) { _origin = point; }
	void setBackgroundOffset(const Common::Point &point) { _backgroundOffset = point; }
	void sub_46DD30(uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6);
	void setUpOffset(const Common::Point &point) { _upOffset = point; }
	void setDownOffset(const Common::Point &point) { _downOffset = point; }
	void sub_46DDA0(uint32 a1, uint32 a2, uint32 a3, uint32 a4);
	void sub_46DDD0(uint32 a1, uint32 a2, uint32 a3, uint32 a4);
	void setImageInfo(const Common::Point &point, ImageType imageType, DrawType drawType);
	void sub_46DE30(uint32 a1, uint32 a2);
	void initHotspots();
	void sub_46E330(uint32 a1);
	void setTextForegroundColor(const Color &foreground, const Color &foregroundSelected);
	void setTextBackgroundColor(const Color &background);
	void setFontId(FontId fontId);
	void setIconDirectory(const Common::String &directory);

	// Management
	void add(ObjectId objectId);
	void remove(ObjectId objectId, bool removeObject);
	void removeAll(bool removeObject);
	bool isIn(const Common::String &description);
	void resetObjectClicked();

	// Accessors
	uint32 getItemCount() { return _itemCount; }
	ObjectId getObjectIdClicked() { return _objectIdClicked; }
	int32 getImageIndexClicked() { return _imageIndexClicked; }
	int32 getObjectIndexClicked() { return _objectIndexClicked; }

private:
	Common::String _iconPath;
	AssociativeArray<Object *> _objects;
	Common::Array<ImageHandle *> _images;
	Common::Array<Hotspot *> _hotspots;
	ImageHandle *_backgroundImage;
	ImageHandle *_upGun;
	ImageHandle *_upGur;
	ImageHandle *_upGus;
	ImageHandle *_upGua;
	ImageHandle *_downGun;
	ImageHandle *_downGur;
	ImageHandle *_downGus;
	ImageHandle *_downGua;
	ImageHandle *_cliImageP;
	ImageHandle *_cliImageA;
	Common::Point _origin;
	Common::Point _backgroundOffset;
	uint32 _field_59;
	uint32 _field_5D;
	uint32 _field_61;
	uint32 _field_65;
	uint32 _field_69;
	uint32 _field_6D;
	Common::Point _upOffset;
	Common::Point _downOffset;
	uint32 _field_81;
	uint32 _field_85;
	uint32 _field_89;
	uint32 _field_8D;
	uint32 _field_91;
	uint32 _field_95;
	uint32 _field_99;
	uint32 _field_9D;
	Common::Point _imageCoords;
	ImageType _imageType;
	DrawType _drawType;
	uint32 _field_B1;
	uint32 _field_B5;
	uint32 _field_B9;
	uint32 _field_BD;
	uint32 _objectIndex;
	uint32 _itemCount;
	ObjectId _objectIdClicked;
	int32 _objectIndexClicked;
	int32 _imageIndexClicked;
	ArchiveType _archiveType;
	Text  *_text1;
	Text  *_text2;
	FontId _fontId;
	Color  _foregroundColor;
	Color  _foregroundColorSelected;
	Color  _backgroundColor;
	bool   _allocated;

	Hotspot *getHotspot(const Common::Point &point);
	void loadImage(ImageHandle *image) const;
};

} // End of namespace Ring

#endif // RING_VISUAL_H
