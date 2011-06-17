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
	game/ring/ring_application.o \
	game/ring/ring_visual.o \
	game/ring/ring_zonesystem.o \
	game/ring/ring_zoneni.o \
	game/ring/ring_zonerh.o \
	game/ring/ring_zonefo.o \
	game/ring/ring_zonero.o \
	game/ring/ring_zonewa.o \
	game/ring/ring_zoneas.o \
	game/ring/ring_zonen2.o
endif

ifdef ENABLE_FAUST
MODULE_OBJS += \
	game/faust/faust_animation.o \
	game/faust/faust_application.o \
	game/faust/faust_bag.o \
	game/faust/faust_button.o \
	game/faust/faust_init.o \
	game/faust/faust_input.o \
	game/faust/faust_ride.o \
	game/faust/faust_setup.o \
	game/faust/faust_sound.o \
	game/faust/faust_timer.o \
	game/faust/faust_visual.o \
	game/faust/faust_zone.o
endif

ifdef ENABLE_POMPEII
MODULE_OBJS += \
	game/pompeii/pompeii_animation.o \
	game/pompeii/pompeii_application.o \
	game/pompeii/pompeii_bag.o \
	game/pompeii/pompeii_button.o \
	game/pompeii/pompeii_init.o \
	game/pompeii/pompeii_input.o \
	game/pompeii/pompeii_ride.o \
	game/pompeii/pompeii_setup.o \
	game/pompeii/pompeii_sound.o \
	game/pompeii/pompeii_timer.o \
	game/pompeii/pompeii_visual.o \
	game/pompeii/pompeii_zone.o
endif

ifdef ENABLE_PILGRIM2
MODULE_OBJS += \
	game/pilgrim2/pilgrim2_animation.o \
	game/pilgrim2/pilgrim2_application.o \
	game/pilgrim2/pilgrim2_bag.o \
	game/pilgrim2/pilgrim2_button.o \
	game/pilgrim2/pilgrim2_init.o \
	game/pilgrim2/pilgrim2_input.o \
	game/pilgrim2/pilgrim2_ride.o \
	game/pilgrim2/pilgrim2_setup.o \
	game/pilgrim2/pilgrim2_sound.o \
	game/pilgrim2/pilgrim2_timer.o \
	game/pilgrim2/pilgrim2_visual.o \
	game/pilgrim2/pilgrim2_zone.o
endif

ifdef ENABLE_PILGRIM3
MODULE_OBJS += \
	game/pilgrim3/pilgrim3_animation.o \
	game/pilgrim3/pilgrim3_application.o \
	game/pilgrim3/pilgrim3_bag.o \
	game/pilgrim3/pilgrim3_button.o \
	game/pilgrim3/pilgrim3_init.o \
	game/pilgrim3/pilgrim3_input.o \
	game/pilgrim3/pilgrim3_ride.o \
	game/pilgrim3/pilgrim3_setup.o \
	game/pilgrim3/pilgrim3_sound.o \
	game/pilgrim3/pilgrim3_timer.o \
	game/pilgrim3/pilgrim3_visual.o \
	game/pilgrim3/pilgrim3_zone.o
endif

ifdef ENABLE_JERUSALEM
MODULE_OBJS += \
	game/jerusalem/jerusalem_animation.o \
	game/jerusalem/jerusalem_application.o \
	game/jerusalem/jerusalem_bag.o \
	game/jerusalem/jerusalem_button.o \
	game/jerusalem/jerusalem_init.o \
	game/jerusalem/jerusalem_input.o \
	game/jerusalem/jerusalem_ride.o \
	game/jerusalem/jerusalem_setup.o \
	game/jerusalem/jerusalem_sound.o \
	game/jerusalem/jerusalem_timer.o \
	game/jerusalem/jerusalem_visual.o \
	game/jerusalem/jerusalem_zone.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
