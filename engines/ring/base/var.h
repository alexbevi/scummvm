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

#ifndef RING_VAR_H
#define RING_VAR_H

#include "ring/shared.h"

namespace Ring {

template<class T>
class VarEntry : public BaseObject, Common::Serializable {
public:
	VarEntry() : BaseObject(0) {}
	VarEntry(Id id, T value) : BaseObject(id), _value(value) {}

	void set(T value) { _value = value; }
	T get() { return _value; }

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	T _value;
};

class Var : Common::Serializable {
public:
	Var();
	~Var();

	void removeAll();

	// Accessors
#define DEFINE_FUNCTIONS(name, type, storage) \
	void define##name(Id id, type value) { define(&storage, id, value); } \
	type get##name(Id id) { return get(&storage, id); } \
	void set##name(Id id, type value) { set(&storage, id, value); }

	DEFINE_FUNCTIONS(Byte,   int8,           _bytes);
	DEFINE_FUNCTIONS(Word,   int16,          _words);
	DEFINE_FUNCTIONS(Dword,  int32,          _dwords);
	DEFINE_FUNCTIONS(String, Common::String, _strings);
	DEFINE_FUNCTIONS(Float,  float,         _floats);

#undef DEFINE_FUNCTIONS

	// Serializable
	void saveLoadWithSerializer(Common::Serializer &s);

private:
	AssociativeArray<VarEntry<int8> *>           _bytes;
	AssociativeArray<VarEntry<int16> *>          _words;
	AssociativeArray<VarEntry<int32> *>          _dwords;
	AssociativeArray<VarEntry<Common::String> *> _strings;
	AssociativeArray<VarEntry<float> *>          _floats;

	template<class T>
	void define(AssociativeArray<VarEntry<T> *> *array, Id id, T value);

	template<class T>
	T get(AssociativeArray<VarEntry<T> *> *array, Id id);

	template<class T>
	void set(AssociativeArray<VarEntry<T> *> *array, Id id, T value);
};

template<typename T>
inline void Var::define(AssociativeArray<VarEntry<T> *> *array, Id id, T value) {
	if (array->has(id))
		error("[Var::define] ID already exists (%d)", id);

	array->push_back(new VarEntry<T>(id, value));
}

template<typename T>
inline T Var::get(AssociativeArray<VarEntry<T> *> *array, Id id) {
	if (!array->has(id))
		error("[Var::get] ID doesn't exists (%d)", id);

	return array->get(id)->get();
}

template<typename T>
inline void Var::set(AssociativeArray<VarEntry<T> *> *array, Id id, T value) {
	if (!array->has(id))
		error("[Var::set] ID doesn't exists (%d)", id);

	array->get(id)->set(value);
}

} // End of namespace Ring

#endif // RING_VAR_H
