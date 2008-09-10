#ifndef INNOCENT_DEBUGGER_H
#define INNOCENT_DEBUGGER_H

#include "common/singleton.h"
#include "gui/debugger.h"

namespace Innocent {

class Engine;
class Logic;

class Debugger : public ::GUI::Debugger, public Common::Singleton<Debugger> {
public:
	Debugger();
	void setEngine(Engine *vm);

	inline void opcodeStep() { if (_stepOpcodes) { _stepOpcodes = false; attach(); onFrame(); } }
	void clickHandler();

private:
	Logic *logic() const;

	Engine *_vm;

	#define CMD(name) bool cmd_##name(int argc, const char **argv);
	bool cmd_setBackdrop(int argc, const char **argv);
	bool cmd_paintText(int argc, const char **argv);
	bool cmd_paintSprite(int argc, const char **argv);
	bool cmd_listExits(int argc, const char **argv);
	bool cmd_showClickable(int argc, const char **argv);
	bool cmd_break(int argc, const char **argv);
	CMD(step)
	CMD(setVar)
	#undef CMD

	bool _stepOpcodes;
	bool _breakOnClickHandler;
};

#define Debug Debugger::instance()

} // End of namespace Innocent

#endif // defined INNOCENT_DEBUGGER_H
