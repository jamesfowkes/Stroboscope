# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc

LIBS_FOLDER=$(PROJECTS_PATH)\Libs
DEL=$(LIBS_FOLDER)\del.py

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	UNITY_FOLDER=E:/WorkDir/Unity
else
	TARGET_EXTENSION=.out
	UNITY_FOLDER=/home/james/Documents/Unity
endif

TARGET = $(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES= \
	$(UNITY_FOLDER)/src/unity.c \
	../$(TEST_NAME).c \
	$(TEST_NAME)_test.c \
	$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src -I../ -I$(LIBS_FOLDER)/Utility
SYMBOLS=-DUNITY_TEST -std=c99

CLEANUP = $(DEL) $(TARGET)

.PHONY: strobe

all: default

default:
	@echo "Available tests:"
	@echo "strobe"

strobe: TEST_NAME=$@

strobe:
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET) $(LD_SUFFIX)
	./$(TARGET)

clean:
	$(CLEANUP)
