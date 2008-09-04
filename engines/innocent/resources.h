#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

namespace Innocent {

class MainDat;
class GraphicsMap;
class ProgDat;

class Resources {
public:
	Resources();
	void load();

	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }
	ProgDat *progDat() const { return _progDat.get(); }

private:
	void loadGraphicFiles();

	std::auto_ptr<MainDat> _main;
	std::auto_ptr<GraphicsMap> _graphicsMap;
	std::auto_ptr<ProgDat> _progDat;
};

} // End of namespace Innocent

#endif
