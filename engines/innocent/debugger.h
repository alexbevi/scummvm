#ifndef INNOCENT_DEBUGGER_H
#define INNOCENT_DEBUGGER_H

#include "gui/debugger.h"

namespace Innocent {

class Engine;

class Debugger : public ::GUI::Debugger {
public:
	Debugger(Engine *vm);

private:
	Engine *_vm;

	bool cmd_setBackdrop(int argc, const char **argv);
	bool cmd_paintText(int argc, const char **argv);
};

} // End of namespace Innocent

#endif // defined INNOCENT_DEBUGGER_H
