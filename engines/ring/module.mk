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
	sound/soundstream.o \
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
	game/faust/faust_application.o \
	game/faust/faust_zonesystem.o \
	game/faust/faust_zone2.o \
	game/faust/faust_zone3.o \
	game/faust/faust_zone4.o \
	game/faust/faust_zone5.o \
	game/faust/faust_zone6.o \
	game/faust/faust_zone7.o \
	game/faust/faust_zone8.o \
	game/faust/faust_zone9.o \
	game/faust/faust_zone10.o \
	game/faust/faust_zone11.o \
	game/faust/faust_zone12.o \
	game/faust/faust_zone13.o \
	game/faust/faust_zone14.o \
	game/faust/faust_zone15.o \
	game/faust/faust_zone16.o \
	game/faust/faust_zone17.o
endif

ifdef ENABLE_POMPEII
MODULE_OBJS += \
	game/pompeii/pompeii_application.o \
	game/pompeii/pompeii_zonesystem.o \
	game/pompeii/pompeii_zone1.o \
	game/pompeii/pompeii_zone2.o \
	game/pompeii/pompeii_zone3.o \
	game/pompeii/pompeii_zone4.o \
	game/pompeii/pompeii_zone5.o \
	game/pompeii/pompeii_zone6.o \
	game/pompeii/pompeii_zone7.o \
	game/pompeii/pompeii_zone8.o \
	game/pompeii/pompeii_zone9.o \
	game/pompeii/pompeii_zone10.o \
	game/pompeii/pompeii_zone11.o \
	game/pompeii/pompeii_zone12.o
endif

ifdef ENABLE_PILGRIM2
MODULE_OBJS += \
	game/pilgrim2/pilgrim2_application.o \
	game/pilgrim2/pilgrim2_zonesystem.o \
	game/pilgrim2/pilgrim2_zone1.o \
	game/pilgrim2/pilgrim2_zone2.o \
	game/pilgrim2/pilgrim2_zone3.o \
	game/pilgrim2/pilgrim2_zone4.o \
	game/pilgrim2/pilgrim2_zone5.o
endif

ifdef ENABLE_PILGRIM3
MODULE_OBJS += \
	game/pilgrim3/pilgrim3_application.o \
	game/pilgrim3/pilgrim3_zonesystem.o \
	game/pilgrim3/pilgrim3_zone6.o \
	game/pilgrim3/pilgrim3_zone7.o \
	game/pilgrim3/pilgrim3_zone8.o \
	game/pilgrim3/pilgrim3_zone9.o \
	game/pilgrim3/pilgrim3_zone10.o
endif

ifdef ENABLE_JERUSALEM
MODULE_OBJS += \
	game/jerusalem/jerusalem_application.o \
	game/jerusalem/jerusalem_zonesystem.o \
	game/jerusalem/jerusalem_zone1.o \
	game/jerusalem/jerusalem_zone2.o \
	game/jerusalem/jerusalem_zone3.o \
	game/jerusalem/jerusalem_zone4.o \
	game/jerusalem/jerusalem_zone5.o \
	game/jerusalem/jerusalem_zone6.o \
	game/jerusalem/jerusalem_zone7.o \
	game/jerusalem/jerusalem_zone8.o \
	game/jerusalem/jerusalem_zone9.o \
	game/jerusalem/jerusalem_zone10.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_RING), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
