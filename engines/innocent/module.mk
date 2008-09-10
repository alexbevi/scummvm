MODULE := engines/innocent

MODULE_OBJS = \
	inter.o \
	detection.o \
	innocent.o \
	resources.o \
	datafile.o \
	main_dat.o \
	graph_dat.o \
	prog_dat.o \
	graphics.o \
	logic.o \
	program.o \
	debugger.o \
	animation.o \
	value.o \
	sprite.o \
	actor.o \
	exit.o \
	room.o \
	eventmanager.o \
	debug.o \
	movie.o

inter.o: opcode_handlers.cpp

# This module can be built as a plugin
ifeq ($(ENABLE_MADE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
