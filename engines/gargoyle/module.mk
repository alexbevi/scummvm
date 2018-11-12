MODULE := engines/gargoyle

MODULE_OBJS := \
	blorb.o \
	conf.o \
	detection.o \
	events.o \
	fonts.o \
	gargoyle.o \
	glk.o \
	picture.o \
	screen.o \
	selection.o \
	streams.o \
	time.o \
	unicode.o \
	unicode_gen.o \
	utils.o \
	windows.o \
	window_graphics.o \
	window_pair.o \
	window_text_buffer.o \
	window_text_grid.o \
	frotz/detection.o \
	frotz/detection_tables.o \
	frotz/err.o \
	frotz/frotz.o \
	frotz/glk_interface.o \
	frotz/mem.o \
	frotz/processor.o \
	frotz/processor_buffer.o \
	frotz/processor_input.o \
	frotz/processor_maths.o \
	frotz/processor_mem.o \
	frotz/processor_objects.o \
	frotz/processor_screen.o \
	frotz/processor_streams.o \
	frotz/processor_table.o \
	frotz/processor_text.o \
	frotz/processor_variables.o \
	frotz/quetzal.o \
	scott/detection.o \
	scott/scott.o

# This module can be built as a plugin
ifeq ($(ENABLE_GARGOYLE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
