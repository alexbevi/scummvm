MODULE := engines/darkseed2

MODULE_OBJS := \
	darkseed2.o \
	detection.o \
	resources.o

# This module can be built as a plugin
ifeq ($(ENABLE_DARKSEED2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
