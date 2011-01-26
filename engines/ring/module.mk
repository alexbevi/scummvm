MODULE := engines/ring

MODULE_OBJS := \
	data/archive.o \
	game/application.o \
	game/art.o \
	game/dialog.o \
	game/language.o \
	game/object.o \
	debug.o \
	detection.o \
	ring.o

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
