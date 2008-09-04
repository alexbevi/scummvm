#ifndef INNOCENT_LOGIC_H
#define INNOCENT_LOGIC_H

namespace Innocent {

class Engine;

class Logic {
public:
	Logic(Engine *e) : _engine(e) {}

	void init();
	void start();
};

} // End of namespace Innocent

#endif // INNOCENT_LOGIC_H
