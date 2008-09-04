#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include <memory>

namespace Innocent {

class MainDat;
class GraphicsMap;

class Resources {
public:
	Resources();
	void load();

	MainDat *mainDat() const { return _main.get(); }
	GraphicsMap *graphicsMap() const { return _graphicsMap.get(); }

private:
	void loadGraphicFiles();

	std::auto_ptr<MainDat> _main;
	std::auto_ptr<GraphicsMap> _graphicsMap;
};

} // End of namespace Innocent

#endif
