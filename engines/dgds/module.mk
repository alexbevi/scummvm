MODULE := engines/dgds

MODULE_OBJS = \
	compression.o \
	detection.o \
	dgds.o \
	game.o \
	game_willy.o \
	resource.o \
	managers/resman.o \
	resources/font.o \
	resources/movie.o \
	resources/moviegroup.o

# This module can be built as a plugin
ifeq ($(ENABLE_DGDS), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
