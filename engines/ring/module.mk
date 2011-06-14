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
	base/puzzle.o \
	base/rotation.o \
	base/saveload.o \
	base/stream.o \
	base/text.o \
	base/timer.o \
	base/var.o \
	base/zone.o \
	graphics/animation.o \
	graphics/aquator.o \
	graphics/dragControl.o \
	graphics/hotspot.o \
	graphics/image.o \
	graphics/imageLoader.o \
	graphics/movie.o \
	graphics/screen.o \
	graphics/visual.o \
	sound/sound.o \
	debug.o \
	detection.o \
	ring.o

ifdef ENABLE_RING1
MODULE_OBJS += \
	game/application_ring.o \
	game/event_ring.o \
	game/visual_ring.o
endif

ifdef ENABLE_FAUST
MODULE_OBJS += \
	game/application_faust.o \
	game/event_faust.o
endif

ifdef ENABLE_POMPEII
MODULE_OBJS += \
	game/application_pompeii.o \
	game/event_pompeii.o
endif

ifdef ENABLE_PILGRIM2
MODULE_OBJS += \
	game/application_pilgrim2.o \
	game/event_pilgrim2.o
endif

ifdef ENABLE_PILGRIM3
MODULE_OBJS += \
	game/application_pilgrim3.o \
	game/event_pilgrim3.o
endif

ifdef ENABLE_JERUSALEM
MODULE_OBJS += \
	game/application_jerusalem.o \
	game/event_jerusalem.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
