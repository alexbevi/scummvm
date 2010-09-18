#include "unity.h"
#include "sprite.h"
#include "common/system.h"
#include "object.h"
#include "sound.h"

namespace Unity {

SpritePlayer::SpritePlayer(Sprite *spr, Object *par, UnityEngine *vm) : sprite(spr), parent(par), _vm(vm) {
	current_entry = ~0;
	current_sprite = NULL;
	current_speechsprite = NULL;
	current_palette = NULL;
	wait_target = 0;

	normal.xpos = normal.ypos = 0;
	normal.xadjust = normal.yadjust = 0;
	speech.xpos = speech.ypos = 0;
	speech.xadjust = speech.yadjust = 0;

	was_speech = false;
	was_marked = false;
}

SpritePlayer::~SpritePlayer() {
}

void SpritePlayer::resetState() {
	normal.xadjust = normal.yadjust = 0;
	speech.xadjust = speech.yadjust = 0;

	wait_target = 0;
}

void SpritePlayer::startAnim(unsigned int a) {
	if (a >= sprite->numAnims()) {
		error("animation %d is too high (only %d animation(s))", a, sprite->numAnims());
	}

	current_entry = sprite->getIndexFor(a);
	assert(current_entry != (unsigned int)~0);

	resetState();
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

byte *SpritePlayer::getPalette() {
	if (!current_palette) return NULL;

	byte *palette = current_palette->palette;
	current_palette = NULL;
	return palette;
}

bool SpritePlayer::playing() {
	SpriteEntry *e = sprite->getEntry(current_entry);
	assert(e);
	return !(e->type == se_Pause || e->type == se_Exit);
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
			was_speech = false;
			current_speechsprite = NULL;

			current_entry++;

			e = sprite->getEntry(current_entry);
			if (e->type == se_Sprite || e->type == se_SpeechSprite) return;
			break;

		case se_SpeechSprite:
			if (!was_marked) {
				error("no marked data during sprite playback");
			}

			current_speechsprite = (SpriteEntrySprite *)e;
			// XXX: don't understand how this works :(
			if (!was_speech) {
				speech = marked;
			}
			was_speech = true;

			current_entry++;
			e = sprite->getEntry(current_entry);
			if (e->type == se_Sprite || e->type == se_SpeechSprite) return;
			break;

		case se_Palette:
			current_palette = (SpriteEntryPalette *)e;
			current_entry++;
			break;

		case se_Position:
			{
				SpriteEntryPosition *p = (SpriteEntryPosition *)e;
				if (was_speech) {
					speech.xpos = p->newx;
					speech.ypos = p->newy;
				} else {
					normal.xpos = p->newx;
					normal.ypos = p->newy;
				}
			}
			current_entry++;
			break;

		case se_RelPos:
			{
				SpriteEntryRelPos *p = (SpriteEntryRelPos *)e;
				if (was_speech) {
					speech.xadjust = p->adjustx;
					speech.yadjust = p->adjusty;
				} else {
					normal.xadjust = p->adjustx;
					normal.yadjust = p->adjusty;
				}
			}
			current_entry++;
			break;

		case se_MouthPos:
			{
				SpriteEntryMouthPos *p = (SpriteEntryMouthPos *)e;
				// TODO: okay, this isn't MouthPos :-) what is it?
				// set on characters, drones, cable, welds..
			}
			current_entry++;
			break;

		case se_Pause:
		case se_Exit:
			return;

		case se_Mark:
			// XXX: do something here
			was_marked = true;
			marked = normal;
			current_entry++;
			break;

		case se_Mask:
			warning("sprite mask mode not implemented");
			current_entry++;
			break;

		case se_Static:
			warning("sprite static mode not implemented");
			current_entry++;
			break;

		case se_RandomWait:
			if (!wait_target) {
				unsigned int wait = _vm->_rnd.getRandomNumberRng(0,
					((SpriteEntryRandomWait *)e)->rand_amt);
				// TODO: is /6 correct? see below
				wait_target = g_system->getMillis() + ((SpriteEntryRandomWait *)e)->base/6 + wait/6;
				return;
			}
			// fall through
		case se_Wait:
			if (!wait_target) {
				// TODO: is /6 correct? just a guess, so almost certainly not
				// example values are 388, 777, 4777, 288, 666, 3188, 2188, 700, 200000, 1088, 272..
				wait_target = g_system->getMillis() + ((SpriteEntryWait *)e)->wait/6;
				return;
			}
			if (wait_target < g_system->getMillis()) {
				wait_target = 0;
				current_entry++;
				continue;
			}
			return;

		case se_Jump:
			current_entry = sprite->getIndexFor(((SpriteEntryJump *)e)->target);
			assert(current_entry != (unsigned int)~0);
			if (old_entry == current_entry) {
				// XXX: work out why this happens: missing handling?
				error("sprite file in infinite loop");
			}
			old_entry = current_entry;
			resetState();
			break;

		case se_Audio:
			_vm->_snd->playAudioBuffer(((SpriteEntryAudio *)e)->length,
					((SpriteEntryAudio *)e)->data);
			current_entry++;
			break;

		case se_WaitForSound:
			warning("sprite waiting for sound not implemented");
			current_entry++;
			break;

		case se_Silent:
			warning("sprite silencing not implemented");
			current_entry++;
			break;

		case se_StateSet:
			warning("sprite state setting not implemented");
			current_entry++;
			break;

		default:
			error("internal error: bad sprite player entry");
		}
	}
}

} // Unity

