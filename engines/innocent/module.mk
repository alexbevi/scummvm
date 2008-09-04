MODULE := engines/innocent

MODULE_OBJS = \
	detection.o \
	innocent.o

# This module can be built as a plugin
ifeq ($(ENABLE_MADE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
