MODULE := engines/glk

MODULE_OBJS := \
	blorb.o \
	conf.o \
	detection.o \
	events.o \
	fonts.o \
	glk.o \
	glk_api.o \
	glk_dispa.o \
	pc_speaker.o \
	picture.o \
	raw_decoder.o \
	screen.o \
	selection.o \
	sound.o \
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
	alan2/alan2.o \
	alan2/decode.o \
	alan2/detection.o \
	alan2/execute.o \
	alan2/interpreter.o \
	alan2/parse.o \
	alan2/rules.o \
	alan2/saveload.o \
	frotz/bitmap_font.o \
	frotz/config.o \
	frotz/detection.o \
	frotz/frotz.o \
	frotz/glk_interface.o \
	frotz/mem.o \
	frotz/pics.o \
	frotz/pics_decoder.o \
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
	frotz/processor_windows.o \
	frotz/quetzal.o \
	frotz/screen.o \
	frotz/sound_folder.o \
	frotz/windows.o \
	glulxe/accel.o \
	glulxe/detection.o \
	glulxe/exec.o \
	glulxe/float.o \
	glulxe/funcs.o \
	glulxe/gestalt.o \
	glulxe/glkop.o \
	glulxe/glulxe.o \
	glulxe/heap.o \
	glulxe/operand.o \
	glulxe/search.o \
	glulxe/serial.o \
	glulxe/string.o \
	glulxe/vm.o \
	hugo/detection.o \
	hugo/heexpr.o \
	hugo/heglk.o \
	hugo/hemedia.o \
	hugo/hemisc.o \
	hugo/heobject.o \
	hugo/heparse.o \
	hugo/heres.o \
	hugo/herun.o \
	hugo/heset.o \
	hugo/htokens.o \
	hugo/hugo.o \
	hugo/stringfn.o \
	magnetic/detection.o \
	magnetic/emu.o \
	magnetic/graphics.o \
	magnetic/magnetic.o \
	magnetic/sound.o \
	scott/detection.o \
	scott/scott.o \
	tads/detection.o \
	tads/os_banners.o \
	tads/os_buffer.o \
	tads/os_glk.o \
	tads/os_frob_tads.o \
	tads/os_parse.o \
	tads/tads.o \
	tads/tads2/built_in.o \
	tads/tads2/character_map.o \
	tads/tads2/command_line.o \
	tads/tads2/data.o \
	tads/tads2/debug.o \
	tads/tads2/error.o \
	tads/tads2/error_handling.o \
	tads/tads2/error_message.o \
	tads/tads2/execute_command.o \
	tads/tads2/file_io.o \
	tads/tads2/get_string.o \
	tads/tads2/line_source_file.o \
	tads/tads2/list.o \
	tads/tads2/ltk.o \
	tads/tads2/memory_cache.o \
	tads/tads2/memory_cache_heap.o \
	tads/tads2/memory_cache_swap.o \
	tads/tads2/object.o \
	tads/tads2/os.o \
	tads/tads2/output.o \
	tads/tads2/play.o \
	tads/tads2/post_compilation.o \
	tads/tads2/qa_scriptor.o \
	tads/tads2/regex.o \
	tads/tads2/run.o \
	tads/tads2/runstat.o \
	tads/tads2/runtime_app.o \
	tads/tads2/runtime_driver.o \
	tads/tads2/tads2.o \
	tads/tads2/tokenizer.o \
	tads/tads2/tokenizer_hash.o \
	tads/tads2/vocabulary.o \
	tads/tads2/vocabulary_parser.o \
	tads/tads3/tads3.o

# This module can be built as a plugin
ifeq ($(ENABLE_GLK), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
