
# compiler flags
DEBUG     = -ggdb
OPTIMISE  = -O0
WARNINGS  = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
REMSRC    = -fmacro-prefix-map=src/=
CFLAGS    = $(DEBUG) $(OPTIMISE) $(WARNINGS) $(REMSRC)

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
TEST      = tests
LOCALBIN  = ~/.local/bin

OBJECTS =

# Tell make where to find the files
vpath %.c src/
vpath %.h include/

# Lists
EXES = main test_error test_logger
MAIN = error.o logging.o
ERROR = error.o
LOGGER = error.o logging.o
MATRIX = error.o logging.o matrix.o

# Executables
$(BIN)/main: main.c $(addprefix $(BUILD)/, $(MAIN)) | $(BIN)
	$(COMPILE) -o $@ $^

$(BIN)/test_error: test_error.c $(addprefix $(BUILD)/, $(ERROR)) | $(BIN)
	$(COMPILE) -o $@ $^

$(BIN)/test_logger: test_logger.c $(addprefix $(BUILD)/, $(LOGGER)) | $(BIN)
	$(COMPILE) -o $@ $^
	$(TEST)/logger/test_logger.sh $(TEST)/logger $@

$(BIN)/test_matrix: test_matrix.c $(addprefix $(BUILD)/, $(MATRIX)) | $(BIN)
	$(COMPILE) -o $@ $^

# Units
$(BUILD)/error.o: error.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD)/logging.o: logging.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD)/matrix.o: matrix.c matrix.h error.h logging.h | $(BUILD)
	$(COMPILE) -c $< -o $@

# PHONY Targets
.PHONY: all clean test_error test_logger test_matrix

all: $(BIN)/main
test_error: $(BIN)/test_error
test_logger: $(BIN)/test_logger
test_matrix: $(BIN)/test_matrix

clean:
	$(RM) $(foreach EXEFILE, $(EXES), $(BIN)/$(EXEFILE))
	$(RM) $(BUILD)/*.o
	$(RM) -rf $(BIN)/*.dSYM
