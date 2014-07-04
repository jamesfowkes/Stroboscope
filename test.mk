include $(PROJECTS_PATH)/Libs/standard.mk

NAME = stroboscope_test
CC = gcc 
EXTRA_FLAGS = -DF_CPU=8000000 -DMEMORY_POOL_BYTES=512 -DENCODER_PCINT0 -DSUPPRESS_PCINT0
MMCU = iom328p

EXTRA_INCLUDE_DIRS = \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/TestHarness \
	-I$(LIBS_DIR)/Utility \
	-I$(LIBS_DIR)/Format

EXTRA_CFILES = \
	app.c \
	app_test_harness.c \
	strobe.c \
	ui.c \
	ui_lcd.c \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_pcint.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/AVR/lib_encoder.c \
	$(LIBS_DIR)/AVR/lib_tmr16.c \
	$(LIBS_DIR)/AVR/lib_tmr16_pwm.c \
	$(LIBS_DIR)/Devices/lcd/lcd.c \
	$(LIBS_DIR)/Generics/button.c \
	$(LIBS_DIR)/Generics/memorypool.c \
	$(LIBS_DIR)/Format/format.c

include $(LIBS_DIR)/AVR/Harness/make_test_harness.mk
