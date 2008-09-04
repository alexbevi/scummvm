#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

#include <memory>

#include "config.h"

namespace Innocent {

class Engine;
class Interpreter;

class Logic {
public:
	Logic(Engine *e);

//	void init();
	void start();

	std::auto_ptr<Interpreter> _interpreter;

	uint16 _status;
	enum Status {
		kStatusOk		= 0,
		kInvalidOpcode	= 1
	};

	void setProtagonistId(uint16 id) { _protagonistId = id; }

private:
	Engine *_engine;
	uint16 _protagonistId;
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
