#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

namespace Innocent {

class Engine;
class Resources;
class Interpreter;

class Logic {
public:
	Logic(Engine *e);

	void init();

private:
	Engine *_engine;
	Resources *_resources;
	std::auto_ptr<Interpreter> _interpreter;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
