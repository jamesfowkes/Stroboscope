include $(PROJECTS_PATH)/Libs/standard.mk

NAME=stroboscope

MCU_TARGET=atmega328p
AVRDUDE_PART=m328p

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
	
EXTRA_FLAGS = \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DENCODER_PCINT1 \
	-DSUPPRESS_PCINT1
	
include $(LIBS_DIR)/AVR/make_avr.mk

