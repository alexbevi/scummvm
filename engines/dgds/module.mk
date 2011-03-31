MODULE := engines/dgds

MODULE_OBJS = \
	compression.o \
	detection.o \
	dgds.o \
	game.o \
	game_willy.o \
	resource.o \
	resourcemanager.o \
	resources/font.o \
	resources/movie.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
