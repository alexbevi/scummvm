#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

#include "config.h"

#include "common/list.h"

#include "innocent/value.h"

namespace Innocent {
//

class Animation;
class Engine;
class Resources;
class Interpreter;
class Program;

class Logic {
public:
	Logic(Engine *e);
	~Logic();

	void init();

	// set actor# of the protagonist
	void setProtagonist(uint16);

	void changeRoom(uint16);

	Engine *engine() { return _engine; }

	void tick();

	void addAnimation(Animation *anim);
	void setRoomLoop(const CodePointer &code);

	const Common::List<Animation *> animations() const { return _animations; }
	Program *blockProgram() const { return _blockProgram.get(); }
	Interpreter *mainInterpreter() const { return _toplevelInterpreter.get(); }

private:
	Engine *_engine;
	Resources *_resources;
	std::auto_ptr<Interpreter> _toplevelInterpreter, _blockInterpreter;
	uint16 _protagonist;
	uint16 _currentRoom;
	uint16 _currentBlock;
	std::auto_ptr<Program> _blockProgram;
	Common::List<Animation *> _animations;
	std::auto_ptr<CodePointer> _roomLoop;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
