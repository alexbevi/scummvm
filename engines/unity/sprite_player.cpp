#include "sprite.h"
#include "common/system.h"

namespace Unity {

SpritePlayer::SpritePlayer(Sprite *spr) : sprite(spr) {
	current_entry = ~0;
	current_sprite = 0;
	current_speechsprite = 0;
	wait_start = 0;

	next_xpos = xpos = 0;
	next_ypos = ypos = 0;
	next_m_xpos = m_xpos = 0;
	next_m_ypos = m_ypos = 0;
}

SpritePlayer::~SpritePlayer() {
}

void SpritePlayer::startAnim(unsigned int a) {
	current_entry = sprite->getIndexFor(a);
	assert(current_entry != ~0);
	//current_sprite = 0; XXX: animations which are just speech sprites cause issues
	current_speechsprite = 0; // XXX: good?
	wait_start = 0;
	next_xpos = xpos; next_ypos = ypos;
}

unsigned int SpritePlayer::getCurrentWidth() {
	assert(current_sprite);
	return current_sprite->width;
}

unsigned int SpritePlayer::getCurrentHeight() {
	assert(current_sprite);
	return current_sprite->height;
}

byte *SpritePlayer::getCurrentData() {
	assert(current_sprite);
	return current_sprite->data;
}

bool SpritePlayer::speaking() {
	return current_speechsprite != NULL;
}

unsigned int SpritePlayer::getSpeechWidth() {
	assert(current_speechsprite);
	return current_speechsprite->width;
}

unsigned int SpritePlayer::getSpeechHeight() {
	assert(current_speechsprite);
	return current_speechsprite->height;
}

byte *SpritePlayer::getSpeechData() {
	assert(current_speechsprite);
	return current_speechsprite->data;
}


void SpritePlayer::update() {
	unsigned int old_entry = ~0;
	while (true) {
		SpriteEntry *e = sprite->getEntry(current_entry);
		assert(e);
		switch (e->type) {
		case se_None:
			current_entry++;
			break;
		case se_Sprite:
			current_sprite = (SpriteEntrySprite *)e;
			// XXX: don't understand how this works either
			xpos = next_xpos;
			ypos = next_ypos;
			current_entry++;
			break;
		case se_SpeechSprite:
			current_speechsprite = (SpriteEntrySprite *)e;
			// XXX: don't understand how this works :(
			m_xpos = next_m_xpos;
			m_ypos = next_m_ypos;
			current_entry++;
			break;
		case se_RelPos:
			{
				SpriteEntryRelPos *p = (SpriteEntryRelPos *)e;
				next_xpos = p->adjustx;
				next_ypos = p->adjusty;
			}
			current_entry++;
			break;
		case se_MouthPos:
			{
				SpriteEntryMouthPos *p = (SpriteEntryMouthPos *)e;
				next_m_xpos = p->adjustx;
				next_m_ypos = p->adjusty;
			}
			current_entry++;
			break;
		case se_Pause:
			return;
		case se_RandomWait:
			// XXX: need to make this work
			current_entry++;
			return;
		case se_Wait:
			if (!wait_start) {
				wait_start = g_system->getMillis();
				return;
			}
			// TODO: is /4 correct?
			// example values are 388, 777, 4777, 288, 666, 3188, 2188, 700, 200000, 1088, 272..
			if (wait_start + ((SpriteEntryWait *)e)->wait/4 < g_system->getMillis()) {
				wait_start = 0;
				current_entry++;
				continue;
			}
			return;
		case se_Jump:
			current_entry = sprite->getIndexFor(((SpriteEntryJump *)e)->target);
			assert(current_entry != ~0);
			if (old_entry == current_entry) {
				// XXX: work out why this happens: missing handling?
				error("sprite file in infinite loop");
			}
			old_entry = current_entry;
			break;
		case se_Exit:
			// XXX: check this
			error("reached EXIT in a sprite");
			return;
		default:
			assert(false);
		}
	}
}

} // Unity

