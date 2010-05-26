#include "sound.h"
#include "common/system.h"
#include "sound/audiostream.h"
#include "sound/decoders/adpcm.h"

namespace Unity {

Sound::Sound(UnityEngine *_engine) : _vm(_engine) {
}

Sound::~Sound() {
	delete _soundHandle;
}

void Sound::init() {
	_soundHandle = new Audio::SoundHandle();
}

void Sound::playSpeech(Common::String name) {
	Common::SeekableReadStream *audioFileStream = _vm->openFile(name);
	Audio::AudioStream *sampleStream = Audio::makeADPCMStream(
		audioFileStream, DisposeAfterUse::YES, 0, Audio::kADPCMIma,
		22050, 1);
	if (!sampleStream) error("couldn't make sample stream");
	_vm->_mixer->playStream(Audio::Mixer::kSpeechSoundType, _soundHandle, sampleStream);
}

}

