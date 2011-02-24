MODULE := engines/dgds

MODULE_OBJS = \
	detection.o \
	dgds.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
