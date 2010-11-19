MODULE := engines/liath

MODULE_OBJS := \
	data/archive.o \
	data/resource.o \
	data/segment.o \
	game/action.o \
	game/array.o \
	game/expression.o \
	game/game.o \
	game/graphics.o \
	game/hero.o \
	game/interpreter.o \
	game/mouse.o \
	game/position.o \
	game/progress.o \
	game/savegame.o \
	game/sound.o \
	game/text.o \
	game/work.o \
	debug.o \
	detection.o \
	liath.o

# This module can be built as a plugin
ifeq ($(ENABLE_LIATH), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
