NAME = stroboscope_test
CC = gcc 
FLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS -DF_CPU=8000000 -DMEMORY_POOL_BYTES=512 -DENCODER_PCINT0 -DSUPPRESS_PCINT0 -std=c99  -Wfatal-errors
MMCU = iom328p

AVR_DIR = $(AVR32_HOME)\avr\include\avr
HARNESS_ROOT_DIR = $(PROJECTS_PATH)\Libs\AVR\Harness
HARNESS_AVR_DIR = $(HARNESS_ROOT_DIR)/AVR
LIBS_DIR = $(PROJECTS_PATH)/Libs

DEL = python $(LIBS_DIR)/del.py

INCLUDE_DIRS = \
	-I$(HARNESS_ROOT_DIR) \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/TestHarness \
	-I$(LIBS_DIR)/Utility \
	-I$(LIBS_DIR)/Format

CFILES = \
	app.c \
	app_test_harness.c \
	strobe.c \
	ui.c \
	ui_lcd.c \
	$(HARNESS_ROOT_DIR)/lib_io_harness.c \
	$(HARNESS_ROOT_DIR)/lib_pcint_harness.c \
	$(HARNESS_ROOT_DIR)/lib_tmr8_tick_harness_functions.c \
	$(HARNESS_ROOT_DIR)/lib_tmr16_harness_functions.c \
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
	$(LIBS_DIR)/TestHarness/test_harness.c \
	$(LIBS_DIR)/Format/format.c

OBJDEPS=$(CFILES:.c=.o)

all:
	$(DEL) $(OBJDEPS)
	python $(HARNESS_ROOT_DIR)/generate_test_io.py $(MMCU) $(AVR_DIR) $(HARNESS_ROOT_DIR)\avr
	$(CC) $(FLAGS) $(INCLUDE_DIRS) $(OPTS) $(CFILES) -o $(NAME).exe
	$(NAME).exe
