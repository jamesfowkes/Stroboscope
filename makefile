NAME=stroboscope

AVR_DIR=$(AVR32_HOME)/bin

CC=$(AVR_DIR)/avr-gcc
MCU_TARGET=atmega328p
LIBS_DIR = $(PROJECTS_PATH)\Libs

DEL = python $(LIBS_DIR)\del.py

OPT_LEVEL=3

INCLUDE_DIRS = \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Utility

CFILES = \
	app.c \
	strobe.c \
	ui.c \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/AVR/lib_encoder.c \
	$(LIBS_DIR)/Generics/button.c \
	$(LIBS_DIR)/Generics/memorypool.c
	
OPTS = \
	-g \
	-Wall \
	-Wextra \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DENCODER_PCINT0
	
LDFLAGS = \
	-Wl

OBJDEPS=$(CFILES:.c=.o)

all: $(NAME).elf

$(NAME).elf: $(OBJDEPS)
	$(CC) $(INCLUDE_DIRS) $(OPTS) $(LDFLAGS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -o $@ $^

%.o:%.c
	$(CC) $(INCLUDE_DIRS) $(OPTS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -c $< -o $@

clean:
	$(DEL) $(NAME).elf
	$(DEL) $(OBJDEPS)