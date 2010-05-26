#include "unity.h"
#include "sound/mixer.h"

namespace Unity {

class Sound {
public:
	Sound(UnityEngine *engine);
	~Sound();

	void init();
	void playSpeech(Common::String name);

protected:
	UnityEngine *_vm;
	Audio::SoundHandle *_soundHandle;
};

}

