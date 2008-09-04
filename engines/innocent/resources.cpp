#include "resources.h"

namespace Innocent {

Resources::Resources() : _main(new Resources::Main(this)) {
}

void Resources::load() {
	_main->load();
}

} // End of namespace Innocent
