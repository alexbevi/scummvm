#include "sound.h"
#include "common/system.h"
#include "sound/audiostream.h"
#include "sound/decoders/adpcm.h"

namespace Unity {

Sound::Sound(UnityEngine *_engine) : _vm(_engine) {
	_speechSoundHandle = NULL;
	_sfxSoundHandle = NULL;
}

Sound::~Sound() {
	delete _speechSoundHandle;
	delete _sfxSoundHandle;
}

void Sound::init() {
	_sfxSoundHandle = new Audio::SoundHandle();
	_speechSoundHandle = new Audio::SoundHandle();
}

void Sound::playSpeech(Common::String name) {
	debug(1, "playing speech: %s", name.c_str());
	stopSpeech();
	Common::SeekableReadStream *audioFileStream = _vm->data.openFile(name);
	Audio::AudioStream *sampleStream = Audio::makeADPCMStream(
		audioFileStream, DisposeAfterUse::YES, 0, Audio::kADPCMIma,
		22050, 1);
	if (!sampleStream) error("couldn't make sample stream");
	_vm->_mixer->playStream(Audio::Mixer::kSpeechSoundType, _speechSoundHandle, sampleStream);
}

bool Sound::speechPlaying() {
	return _vm->_mixer->isSoundHandleActive(*_speechSoundHandle);
}

void Sound::stopSpeech() {
	_vm->_mixer->stopHandle(*_speechSoundHandle);
}

void Sound::playAudioBuffer(unsigned int length, byte *data) {
	Common::MemoryReadStream *audioFileStream = new Common::MemoryReadStream(data, length);
	Audio::AudioStream *sampleStream = Audio::makeADPCMStream(
		audioFileStream, DisposeAfterUse::YES, 0, Audio::kADPCMIma,
		22050, 1);
	if (!sampleStream) error("couldn't make sample stream");
	_vm->_mixer->playStream(Audio::Mixer::kSFXSoundType, _sfxSoundHandle, sampleStream);
}

}

