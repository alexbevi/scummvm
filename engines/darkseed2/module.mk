MODULE := engines/darkseed2

MODULE_OBJS := \
	conversation.o \
	conversationbox.o \
	conversationboxsaturn.o \
	conversationboxwindows.o \
	cursors.o \
	darkseed2.o \
	datfile.o \
	detection.o \
	events.o \
	font.o \
	graphicalobject.o \
	graphics.o \
	imageconverter.o \
	inter.o \
	inventory.o \
	inventorybox.o \
	mike.o \
	movie.o \
	music.o \
	objects.o \
	options.o \
	palette.o \
	pathfinder.o \
	resources.o \
	room.o \
	roomconfig.o \
	saveable.o \
	saveload.o \
	script.o \
	sound.o \
	sprite.o \
	talk.o \
	variables.o \
	versionformats.o

# This module can be built as a plugin
ifeq ($(ENABLE_DARKSEED2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
