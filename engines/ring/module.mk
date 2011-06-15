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
	sound/soundentry.o \
	sound/soundhandler.o \
	sound/soundloader.o \
	sound/soundmanager.o \
	debug.o \
	detection.o \
	ring.o

ifdef ENABLE_RING1
MODULE_OBJS += \
	game/ring/ring.o \
	game/ring/event.o \
	game/ring/event_animation.o \
	game/ring/event_bag.o \
	game/ring/event_button.o \
	game/ring/event_init.o \
	game/ring/event_input.o \
	game/ring/event_ride.o \
	game/ring/event_setup.o \
	game/ring/event_sound.o \
	game/ring/event_timer.o \
	game/ring/event_zone.o \
	game/ring/visual.o
endif

ifdef ENABLE_FAUST
MODULE_OBJS += \
	game/faust/init.o \
	game/faust/faust.o \
	game/faust/event.o
endif

ifdef ENABLE_POMPEII
MODULE_OBJS += \
	game/pompeii/pompeii.o \
	game/pompeii/event.o
endif

ifdef ENABLE_PILGRIM2
MODULE_OBJS += \
	game/pilgrim2/pilgrim2.o \
	game/pilgrim2/event.o
endif

ifdef ENABLE_PILGRIM3
MODULE_OBJS += \
	game/pilgrim3/pilgrim3.o \
	game/pilgrim3/event.o
endif

ifdef ENABLE_JERUSALEM
MODULE_OBJS += \
	game/jerusalem/jerusalem.o \
	game/jerusalem/event.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
