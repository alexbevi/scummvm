MODULE := engines/ring

MODULE_OBJS := \
	base/cursor.o \
	base/file.o \
	base/font.o \
	base/preferences.o \
	base/sound.o \
	base/text.o \
	base/timer.o \
	base/var.o \
	game/application.o \
	game/art.o \
	game/bag.o \
	game/dialog.o \
	game/language.o \
	game/object.o \
	game/puzzle.o \
	graphics/accessibility.o \
	graphics/animation.o \
	graphics/dragControl.o \
	graphics/hotspot.o \
	graphics/image.o \
	graphics/movability.o \
	graphics/presentation.o \
	graphics/rotation.o \
	graphics/video.o \
	debug.o \
	detection.o \
	ring.o

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
