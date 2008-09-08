MODULE := engines/innocent

MODULE_OBJS = \
	detection.o \
	innocent.o \
	resources.o \
	datafile.o \
	main_dat.o \
	graph_dat.o \
	prog_dat.o \
	graphics.o \
	logic.o \
	inter.o \
	program.o \
	debugger.o

# This module can be built as a plugin
ifeq ($(ENABLE_MADE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
