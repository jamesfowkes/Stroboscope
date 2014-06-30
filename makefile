NAME=stroboscope

CC=avr-gcc
MCU_TARGET=atmega328p
LIBS_DIR = $(PROJECTS_PATH)/Libs

DEL = python $(LIBS_DIR)/del.py

OPT_LEVEL=3

INCLUDE_DIRS = \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Utility \
	-I$(LIBS_DIR)/Format

CFILES = \
	app.c \
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
	
OPTS = \
	-g \
	-Wall \
	-Wextra \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DENCODER_PCINT0 \
	-DSUPPRESS_PCINT0 \
	-std=c99
	
LDFLAGS =

OBJDEPS=$(CFILES:.c=.o)

all: $(NAME).elf

$(NAME).elf: $(OBJDEPS)
	$(CC) $(INCLUDE_DIRS) $(OPTS) $(LDFLAGS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -o $@ $^

%.o:%.c
	$(CC) $(INCLUDE_DIRS) $(OPTS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -c $< -o $@

clean:
	$(DEL) $(NAME).elf
	$(DEL) $(OBJDEPS)
