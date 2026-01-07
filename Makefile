
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

# Tell make where to find the files
# vpath %.c src/
# vpath %.h include/
# vpath %.o build/

# Executables
all: $(SRC)/main.c $(BUILD)/error.o $(BUILD)/logging.o | $(BIN)
	$(COMPILE) -o $(BIN)/main $^

# Units
$(BUILD)/error.o: $(SRC)/error.c $(INCLUDE)/error.h | $(INCLUDE)
	$(COMPILE) -c $< -o $@

$(BUILD)/logging.o: $(SRC)/logging.c $(INCLUDE)/error.h | $(INCLUDE)
	$(COMPILE) -c $< -o $@
