#ifndef INNOCENT_RESOURCES_H
#define INNOCENT_RESOURCES_H

#include "common/util.h"

namespace Innocent {

class Resources {
public:
	class Main;
	class GraphicsMap;

	Resources();
	void load();

private:
	class DataFile;

	Main *_main;
	GraphicsMap *_graphicsMap;

	class Datafile {
	public:
		Datafile(Resources *resources) : _resources(resources) {}
		virtual ~Datafile() {}

		virtual void load() { error("loading file %s not implemented", filename()); }
		virtual const char *filename() const = 0;

	private:
		Resources *_resources;
	};

public:
	class Main : public Datafile {
	public:
		Main(Resources *resources) : Datafile(resources) {}

		const char *filename() const { return "iuc_main.dat"; }
	};

};


} // End of namespace Innocent

#endif
