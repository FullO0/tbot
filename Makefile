
# compiler flags
DEBUG     = -ggdb
OPTIMISE  = -O0
WARNINGS  = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
CFLAGS    = $(DEBUG) $(OPTIMISE) $(WARNINGS)

# commands
CC        = clang
RM        = rm -f
COMPILE   = $(CC) $(CFLAGS) $(DFLAGS)
INSTALL   = install

# directories
SRC       = src
INCLUDE   = include
BUILD     = build
BIN       = bin
LOCALBIN  = ~/.local/bin

OBJECTS =

# Tell make where to find the files
vpath %.c src/
vpath %.h include/

# Lists
EXES = main
MAIN = error.o logging.o

# Executables
$(BIN)/main: main.c $(addprefix $(BUILD)/, $(MAIN)) | $(BIN)
	$(COMPILE) -o main $^

# Units
$(BUILD)/error.o: error.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD)/logging.o: logging.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

# PHONY Targets
.PHONY: all clean

all: $(BIN)/main

clean:
	$(RM) $(foreach EXEFILE, $(EXES), $(BIN)/$(EXEFILE))
	$(RM) $(BUILD)/*.o
	$(RM) -rf $(BIN)/*.dSYM
