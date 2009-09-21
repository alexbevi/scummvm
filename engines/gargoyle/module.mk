MODULE := engines/gargoyle

MODULE_OBJS = \
	blorb.o \
	detection.o \
	frotz_debugger.o \
	frotz_funcs.o \
	frotz_games.o \
	frotz_gfx.o \
	frotz_interp.o \
	frotz_os.o \
	frotz_sound.o \
	frotz_saveload.o \
	frotz_static.o \
	frotz_zcode.o \
	gargoyle.o \
	graphics.o \
staticres.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
