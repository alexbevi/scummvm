MODULE := engines/kom

MODULE_OBJS := \
	kom.o \
	screen.o \
	database.o \
	plugin.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules 
include $(srcdir)/rules.mk

