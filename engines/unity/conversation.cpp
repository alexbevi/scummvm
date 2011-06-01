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

#include "conversation.h"
#include "sound.h"
#include "unity.h"
#include "common/str.h"

namespace Unity {

// TODO: these are 'sanity checks' for verifying global state
unsigned int g_debug_conv_response, g_debug_conv_state;

void WhoCanSayBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	whocansay = readObjectID(stream);
	// XXX
	byte unknown1 = stream->readByte();
	byte unknown2 = stream->readByte();
	byte unknown3 = stream->readByte();
	byte unknown4 = stream->readByte();
	debugN(3, "WhoCanSayBlack::readFrom()");
	debugN(3, " unknown1: %d", unknown1);
	debugN(3, " unknown2: %d", unknown2);
	debugN(3, " unknown3: %d", unknown3);
	debug(3, " unknown4: %d", unknown4);
}

void TextBlock::readFrom(Common::SeekableReadStream *stream) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 0x10d);

	char buf[256];
	stream->read(buf, 255);
	buf[255] = 0;
	text = buf;

	// make sure it's all zeros?
	unsigned int i = 255;
	while (buf[i] == 0) i--;
	//assert(strlen(buf) == i + 1); XXX: work out what's going on here

	stream->read(buf, 4);

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();

	/*if (text.size()) {
		debugN("text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			debugN(" (%02x??%02x%02x.vac)", voice_group, voice_subgroup, voice_id);
		}
		debugN("\n");
	} else {
		// these fields look like they could mean something else?
		debugN("no text: %x, %x, %x\n", voice_group, voice_subgroup, voice_id);
	}*/

	// XXX: work out what's going on here
	if (!(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0))
		warning("text is weird: %02x, %02x, %02x, %02x", buf[0], buf[1], buf[2], buf[3]);
}

void TextBlock::execute(UnityEngine *_vm, Object *speaker) {
	if (text.size()) {
		_vm->_dialog_text = text;

		_vm->setSpeaker(speaker->id);
		debug(1, "%s says '%s'", speaker->identify().c_str(), text.c_str());

		Common::String file = _vm->voiceFileFor(voice_group, voice_subgroup,
			speaker->id, voice_id);
		_vm->_snd->playSpeech(file);

		_vm->runDialog();
	}
}

const char *change_actor_names[4] = { "disable", "set response", "enable", "unknown2" };

void ChangeActionBlock::readFrom(Common::SeekableReadStream *stream, int _type) {
	uint16 unknown = stream->readUint16LE();
	assert(unknown == 8);

	assert(_type >= BLOCK_CONV_CHANGEACT_DISABLE &&
		_type <= BLOCK_CONV_CHANGEACT_UNKNOWN2);
	type = _type;

	response_id = stream->readUint16LE();
	state_id = stream->readUint16LE();

	byte unknown4 = stream->readByte();
	byte unknown5 = stream->readByte();
	uint16 unknown6 = stream->readUint16LE();
	assert(unknown6 == 0 || (unknown6 >= 7 && unknown6 <= 12));

	debug(4, "%s: response %d, state %d; unknowns: %x, %x, %x",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_DISABLE],
		response_id, state_id, unknown4, unknown5, unknown6);
}

void ChangeActionBlock::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	debug(1, "ChangeActionBlock::execute: %s on %d,%d",
		change_actor_names[type - BLOCK_CONV_CHANGEACT_DISABLE],
		response_id, state_id);

	Response *resp = src->getResponse(response_id, state_id);
	if (!resp) error("ChangeAction couldn't find state %d of situation %d", state_id, response_id);

	// TODO: terrible hack
	if (type == BLOCK_CONV_CHANGEACT_ENABLE) {
		resp->response_state = RESPONSE_ENABLED;
	} else {
		resp->response_state = RESPONSE_DISABLED;
	}
}

void ResultBlock::readFrom(Common::SeekableReadStream *stream) {
	entries.readEntryList(stream);
}

void ResultBlock::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	debug(1, "ResultBlock::execute");
	(void)src;

	// TODO: what are we meant to provide for context here?
	Action context;
	context.action_type = ACTION_TALK;
	context.target = speaker;
	context.who = speaker->id;
	context.other = objectID();
	context.x = 0xffffffff;
	context.y = 0xffffffff;
	entries.execute(_vm, &context);
}

Response::Response() {
}

Response::~Response() {
	for (unsigned int i = 0; i < blocks.size(); i++)
		delete blocks[i];
	for (unsigned int i = 0; i < textblocks.size(); i++)
		delete textblocks[i];
	for (unsigned int i = 0; i < whocansayblocks.size(); i++)
		delete whocansayblocks[i];
}

void Response::readFrom(Common::SeekableReadStream *stream) {
	id = stream->readUint16LE();
	state = stream->readUint16LE();

	g_debug_conv_state = state;
	g_debug_conv_response = id;

	char buf[256];
	stream->read(buf, 255);
	buf[255] = 0;
	text = buf;

	response_state = stream->readByte();
	assert(response_state == RESPONSE_ENABLED || response_state == RESPONSE_DISABLED ||
		response_state == RESPONSE_UNKNOWN1);

	uint16 unknown1 = stream->readUint16LE();
	uint16 unknown2 = stream->readUint16LE();
	debugN(5, "(%04x, %04x), ", unknown1, unknown2);
	// the 0xfffb is somewhere after a USE on 070504, see 7/4
	assert(unknown2 == 0 || (unknown2 >= 6 && unknown2 <= 15) || unknown2 == 0xffff || unknown2 == 0xfffb);

	unknown1 = stream->readUint16LE();
	debugN(5, "(%04x), ", unknown1);

	next_situation = stream->readUint16LE();
	debugN(5, "(next %04x), ", next_situation);

	for (unsigned int i = 0; i < 5; i++) {
		uint16 unknown01 = stream->readUint16LE();
		uint16 unknown02 = stream->readUint16LE();
		debugN(5, "(%04x, %04x), ", unknown01, unknown02);
		assert(unknown02 == 0 || (unknown02 >= 6 && unknown02 <= 15) || unknown02 == 0xffff);
	}

	target = readObjectID(stream);

	uint16 unknown3 = stream->readUint16LE();
	uint16 unknown4 = stream->readUint16LE();
	uint16 unknown5 = stream->readUint16LE();
	uint16 unknown6 = stream->readUint16LE();
	uint16 unknown7 = stream->readUint16LE();

	voice_id = stream->readUint32LE();
	voice_group = stream->readUint32LE();
	voice_subgroup = stream->readUint16LE();

	debugN(5, "%x: ", response_state);
	debugN(5, "%04x -- (%04x, %04x, %04x, %04x)\n", unknown3, unknown4, unknown5, unknown6, unknown7);
	debugN(5, "response %d, %d", id, state);
	if (text.size()) {
		debugN(5, ": text '%s'", text.c_str());
		if (voice_id != 0xffffffff) {
			debugN(5, " (%02x??%02x%02x.vac)", voice_group, voice_subgroup, voice_id);
		}
	}
	debugN(5, "\n");

	int type;
	while ((type = readBlockHeader(stream)) != -1) {
		switch (type) {
			case BLOCK_END_BLOCK:
				debugN(5, "\n");
				return;

			case BLOCK_CONV_WHOCANSAY:
				while (true) {
					WhoCanSayBlock *block = new WhoCanSayBlock();
					block->readFrom(stream);
					whocansayblocks.push_back(block);

					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != BLOCK_CONV_WHOCANSAY)
						error("bad block type %x encountered while parsing whocansay", type);
				}
				break;

			case BLOCK_CONV_TEXT:
				while (true) {
					TextBlock *block = new TextBlock();
					block->readFrom(stream);
					textblocks.push_back(block);

					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != BLOCK_CONV_TEXT)
						error("bad block type %x encountered while parsing text", type);
				}
				break;

			case BLOCK_CONV_CHANGEACT_DISABLE:
			case BLOCK_CONV_CHANGEACT_ENABLE:
			case BLOCK_CONV_CHANGEACT_UNKNOWN2:
			case BLOCK_CONV_CHANGEACT_SET:
				while (true) {
					ChangeActionBlock *block = new ChangeActionBlock();
					block->readFrom(stream, type);
					blocks.push_back(block);

					int oldtype = type; // XXX
					type = readBlockHeader(stream);
					if (type == BLOCK_END_BLOCK)
						break;
					if (type != oldtype)
						error("bad block type %x encountered while parsing changeaction", type);
				}
				break;

			case BLOCK_CONV_RESULT:
				{
				ResultBlock *block = new ResultBlock();
				block->readFrom(stream);
				blocks.push_back(block);
				break;
				}

			default:
				error("bad block type %x encountered while parsing response", type);
		}
	}

	error("didn't find a BLOCK_END_BLOCK for a BLOCK_CONV_RESPONSE");
}

Conversation::Conversation() {
}

Conversation::~Conversation() {
	for (unsigned int i = 0; i < responses.size(); i++)
		delete responses[i];
}

void Conversation::loadConversation(UnityData &data, unsigned int world, unsigned int id) {
	assert(!responses.size());
	our_world = world;
	our_id = id;

	Common::String filename = Common::String::format("w%03xc%03d.bst", world, id);
	Common::SeekableReadStream *stream = data.openFile(filename);

	int blockType;
	while ((blockType = readBlockHeader(stream)) != -1) {
		assert(blockType == BLOCK_CONV_RESPONSE);
		Response *r = new Response();
		r->readFrom(stream);
		responses.push_back(r);
	}

	delete stream;
}

bool WhoCanSayBlock::match(UnityEngine *_vm, objectID speaker) {
	if (speaker.world != whocansay.world) return false;
	if (speaker.screen != whocansay.screen) return false;

	if (whocansay.world == 0 && whocansay.screen == 0 && whocansay.id == 0x10) {
		// any away team member
		// TODO: do we really need to check this?
		if (Common::find(_vm->_away_team_members.begin(),
			_vm->_away_team_members.end(),
			_vm->data.getObject(speaker)) == _vm->_away_team_members.end()) {
			return false;
		}
		return true;
	}

	if (speaker.id != whocansay.id) return false;

	return true;
}

bool Response::validFor(UnityEngine *_vm, objectID speaker) {
	for (unsigned int i = 0; i < whocansayblocks.size(); i++) {
		if (whocansayblocks[i]->match(_vm, speaker)) return true;
	}

	return false;
}

void Response::execute(UnityEngine *_vm, Object *speaker, Conversation *src) {
	if (text.size()) {
		// TODO: response escape strings
		// (search backwards between two '@'s, format: '0' + %c, %1x, %02x)

		_vm->_dialog_text = text;

		// TODO: this is VERy not good
		_vm->setSpeaker(speaker->id);
		debug(1, "%s says '%s'", "Picard", text.c_str());

		// TODO: 0xcc some marks invalid entries, but should we check something else?
		// (the text is invalid too, but i display it, to make it clear we failed..)

		if (voice_group != 0xcc) {
			Common::String file = _vm->voiceFileFor(voice_group, voice_subgroup,
				speaker->id, voice_id);

			_vm->_snd->playSpeech(file);
		}

		_vm->runDialog();
	}

	Object *targetobj = speaker;
	if (target.id != 0xff) {
		if (target.world == 0 && target.screen == 0 && (target.id >= 0x20 && target.id <= 0x28)) {
			// TODO: super hack
			warning("weird target: this is meant to be actors on the Enterprise?");
			target.id -= 0x20;
		}
		targetobj = _vm->data.getObject(target);
	}

	assert(textblocks.size() == 1); // TODO: make this not an array?
	textblocks[0]->execute(_vm, targetobj);

	debug(1, "***begin execute***");
	for (unsigned int i = 0; i < blocks.size(); i++) {
		blocks[i]->execute(_vm, targetobj, src);
	}
	debug(1, "***end execute***");
}

Response *Conversation::getEnabledResponse(UnityEngine *_vm, unsigned int response, objectID speaker) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id != response) continue;
		if (responses[i]->response_state != RESPONSE_ENABLED) continue;
		if (speaker.id != 0xff && !responses[i]->validFor(_vm, speaker)) continue;
		return responses[i];
	}

	return NULL;
}

Response *Conversation::getResponse(unsigned int response, unsigned int state) {
	for (unsigned int i = 0; i < responses.size(); i++) {
		if (responses[i]->id == response) {
			if (responses[i]->state == state) {
				return responses[i];
			}
		}
	}

	return NULL;
}

void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int situation) {
	debug(1, "running situation (%02x) @%d,%d", situation, our_world, our_id);

	uint16 state = 0xffff;
	while (situation != 0xffff) {
		Response *resp;
		if (state == 0xffff) {
			resp = getEnabledResponse(_vm, situation, speaker->id);
		} else {
			resp = getResponse(situation, state);
		}
		if (!resp) error("couldn't find active response for situation %d", situation);

		resp->execute(_vm, speaker, this);

		_vm->_dialog_choice_states.clear();
		if (resp->next_situation != 0xffff) {
			situation = resp->next_situation;
			_vm->_dialog_choice_situation = situation; // XXX: hack

			for (unsigned int i = 0; i < responses.size(); i++) {
				if (responses[i]->id == situation) {
					if (responses[i]->response_state == RESPONSE_ENABLED) {
						if (responses[i]->validFor(_vm, speaker->id)) {
							_vm->_dialog_choice_states.push_back(responses[i]->state);
						}
					}
				}
			}

			if (!_vm->_dialog_choice_states.size()) {
				// see first conversation in space station
				warning("didn't find a next situation");
				return;
			}

			if (_vm->_dialog_choice_states.size() > 1) {
				debug(1, "continuing with conversation, using choices");
				state = _vm->runDialogChoice(this);
			} else {
				debug(1, "continuing with conversation, using single choice");
				state = _vm->_dialog_choice_states[0];
			}
		} else {
			debug(1, "end of conversation");
			return;
		}
	}
}

/*void Conversation::execute(UnityEngine *_vm, Object *speaker, unsigned int response, unsigned int state) {
	debug("1, running situation (%02x) @%d,%d,%d", our_world, our_id, response, state);

	Response *resp = getResponse(response, state);
	if (!resp) error("couldn't find response %d/%d", response, state);
	resp->execute(_vm, speaker);
}*/

} // Unity

