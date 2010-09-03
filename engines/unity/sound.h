#include "unity.h"
#include "sound/mixer.h"

namespace Unity {

class Sound {
public:
	Sound(UnityEngine *engine);
	~Sound();

	void init();
	void playAudioBuffer(unsigned int length, byte *data);
	void playSpeech(Common::String name);
	bool speechPlaying();
	void stopSpeech();

protected:
	UnityEngine *_vm;
	Audio::SoundHandle *_sfxSoundHandle;
	Audio::SoundHandle *_speechSoundHandle;
};

}

