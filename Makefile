############################################################
#General purpose makefile
#
#Works for all C - projects where
#- binaries are compiled into sub - dir bin
#- binaries are created from a multiple c - sources
#and depend on all headers and object files in./
#
#Note : due to the dependencies encoded multiple targets
#are not sensible
#

#Please add all header files in./ here
FILES += prep
FILES += worm
FILES += worm_model
FILES += messages
FILES += board_model

#Please add THE target in./ bin here
TARGET += $(BIN_DIR)/worm
 
#################################################
#There is no need to edit below this line
#################################################

MACHINE := $(shell uname -m)
$(info $$MACHINE is $(MACHINE))
ifeq ($(MACHINE), i686)
  CFLAGS = -g -Wall
  LDLIBS = -lncurses
else ifeq ($(MACHINE), armv7l)
  CFLAGS = -g -Wall
  LDLIBS = -lncurses
else ifeq ($(MACHINE), arm64)
  CFLAGS = -g -Wall
  LDLIBS = -lncurses
else ifeq ($(MACHINE), x86_64)
  CFLAGS = -g -Wall
  LDLIBS = -lncurses
endif

#### Fixed variable definitions
CC = gcc
RM_DIR = rm -rf
MKDIR = mkdir
SHELL = /bin/bash
BIN_DIR = bin
OBJ_DIR = out

OBJECTS=$(patsubst %, $(OBJ_DIR)/%.o, $(FILES))

#### Default target
all: $(BIN_DIR) $(TARGET)

#### Fixed build rules for binaries with multiple object files

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

#### Binaries
$(TARGET) : $(OBJ_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

.PHONY: clean
clean :
	$(RM_DIR) $(BIN_DIR) $(OBJ_DIR)
