MODULE := engines/ring

MODULE_OBJS := \
	base/accessibility.o \
	base/application.o \
	base/art.o \
	base/bag.o \
	base/cursor.o \
	base/dialog.o \
	base/font.o \
	base/language.o \
	base/object.o \
	base/movability.o \
	base/preferences.o \
	base/presentation.o \
	base/puzzle.o \
	base/rotation.o \
	base/saveload.o \
	base/sound.o \
	base/text.o \
	base/timer.o \
	base/var.o \
	game/application_ring.o \
	game/event_ring.o \
	graphics/animation.o \
	graphics/aquator.o \
	graphics/dragControl.o \
	graphics/hotspot.o \
	graphics/image.o \
	graphics/movie.o \
	graphics/screen.o \
	graphics/visual.o \
	debug.o \
	detection.o \
	ring.o

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
