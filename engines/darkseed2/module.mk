MODULE := engines/darkseed2

MODULE_OBJS := \
	darkseed2.o \
	detection.o \
	options.o \
	resources.o \
	palette.o \
	sprite.o \
	graphics.o \
	graphicalobject.o \
	neresources.o \
	cursors.o \
	sound.o \
	music.o \
	variables.o \
	datfile.o \
	script.o \
	objects.o \
	room.o \
	roomconfig.o \
	talk.o \
	conversation.o \
	conversationbox.o \
	inventory.o \
	inventorybox.o \
	movie.o \
	pathfinder.o \
	mike.o \
	inter.o \
	events.o

# This module can be built as a plugin
ifeq ($(ENABLE_DARKSEED2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
