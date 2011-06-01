/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef UNITY_CONVERSATION_H
#define UNITY_CONVERSATION_H

#include "object.h"

#include "common/array.h"
#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Unity {

class UnityEngine;
class UnityData;
class Conversation;

class ResponseBlock {
public:
	virtual void execute(UnityEngine *_vm, Object *speaker, Conversation *src) = 0;
	virtual ~ResponseBlock() { }
};

class WhoCanSayBlock {
protected:
	objectID whocansay;

public:
	bool match(UnityEngine *_vm, objectID speaker);
	void readFrom(Common::SeekableReadStream *stream);
};

class ChangeActionBlock : public ResponseBlock {
public:
	int type;
	uint16 response_id, state_id;

	void readFrom(Common::SeekableReadStream *stream, int _type);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class ResultBlock : public ResponseBlock {
public:
	EntryList entries;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class TextBlock {
public:
	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker);
};

class Response {
public:
	Response();
	~Response();

	uint16 id, state;
	Common::Array<ResponseBlock *> blocks;
	Common::Array<TextBlock *> textblocks;
	Common::Array<WhoCanSayBlock *> whocansayblocks;

	byte response_state;

	uint16 next_situation;
	objectID target;

	Common::String text;
	uint32 voice_id, voice_group;
	uint16 voice_subgroup;

	bool validFor(UnityEngine *_vm, objectID speaker);
	void readFrom(Common::SeekableReadStream *stream);
	void execute(UnityEngine *_vm, Object *speaker, Conversation *src);
};

class Conversation {
public:
	Conversation();
	~Conversation();

	Common::Array<Response *> responses;
	unsigned int our_world, our_id;

	void loadConversation(UnityData &data, unsigned int world, unsigned int id);
	Response *getResponse(unsigned int response, unsigned int state);
	Response *getEnabledResponse(UnityEngine *_vm, unsigned int response, objectID speaker);
	void execute(UnityEngine *_vm, Object *speaker, unsigned int situation);
	//void execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state);
};

} // Unity

#endif

