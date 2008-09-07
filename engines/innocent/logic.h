#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

#include "config.h"

namespace Innocent {

class Engine;
class Resources;
class Interpreter;
class Program;

class Logic {
public:
	Logic(Engine *e);

	void init();

	// set actor# of the protagonist
	void setProtagonist(uint16);

	void changeRoom(uint16);

	Engine *engine() { return _engine; }

	void tick();

private:
	Engine *_engine;
	Resources *_resources;
	std::auto_ptr<Interpreter> _toplevelInterpreter, _blockInterpreter;
	uint16 _protagonist;
	uint16 _currentRoom;
	uint16 _currentBlock;
	std::auto_ptr<Program> _blockProgram;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
