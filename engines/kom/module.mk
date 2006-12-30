MODULE := engines/kom

MODULE_OBJS := \
	kom.o \
	screen.o \
	database.o \
	actor.o \
	input.o \
	panel.o \
	flicplayer.o \
	game.o \
	font.o \
	debugger.o \
	plugin.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules 
include $(srcdir)/rules.mk

