#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

#include "config.h"

namespace Innocent {

class Engine;
class Resources;
class Interpreter;

class Logic {
public:
	Logic(Engine *e);

	void init();

	// set actor# of the protagonist
	void setProtagonist(uint16);

	Engine *engine() { return _engine; }

private:
	Engine *_engine;
	Resources *_resources;
	std::auto_ptr<Interpreter> _interpreter;
	uint16 _protagonist;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
