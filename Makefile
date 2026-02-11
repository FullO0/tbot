
# compiler flags
MODE ?= debug

CPPFLAGS  = -Iinclude

WARNINGS  = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
MACRO     = -fmacro-prefix-map=src/=
CFLAGS    = $(WARNINGS) $(MACRO)

ifeq ($(MODE), release)
	OPTIMIZE   = -Ofast -march=native
	CPPFLAGS  += -DNDEBUG
	CC         = gcc
else
	OPTIMIZE   = -O0 -ggdb
	CPPFLAGS  += -DDEBUG
	CC         = clang
endif

COMPILE    = $(CC) $(CFLAGS) $(OPTIMIZE) $(CPPFLAGS)


# commands
RM         = rm -f
INSTALL    = install

# directories
SRC       = src
INCLUDE   = include
BUILD     = build
BIN       = bin
TEST      = tests
LOCALBIN  = ~/.local/bin

# Tell make where to find the files
vpath %.c src/
vpath %.h include/

# Lists
EXES   = main test_error test_logger test_matrix
MAIN   = error.o logging.o
ERROR  = error.o
LOGGER = error.o logging.o
MATRIX = error.o logging.o matrix.o

# Executables
$(BIN)/main: main.c $(addprefix $(BUILD)/, $(MAIN)) | $(BIN)
	$(COMPILE) -o $@ $^

$(BIN)/test_error: test_error.c $(addprefix $(BUILD)/, $(ERROR)) | $(BIN)
	$(COMPILE) -o $@ $^

$(BIN)/test_logger: test_logger.c $(addprefix $(BUILD)/, $(LOGGER)) | $(BIN)
	$(COMPILE) -o $@ $^

$(BIN)/test_matrix: test_matrix.c  $(INCLUDE)/test_data_matrix.h \
                    $(addprefix $(BUILD)/, $(MATRIX)) | $(BIN)
	$(COMPILE) -o $@ $(filter %.c %.o, $^)

$(BIN):
	@mkdir -p bin

# Units
$(BUILD)/error.o: error.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD)/logging.o: logging.c error.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD)/matrix.o: matrix.c matrix.h error.h logging.h | $(BUILD)
	$(COMPILE) -c $< -o $@

$(BUILD):
	@mkdir -p build

# Extras
$(INCLUDE)/test_data_matrix.h: $(TEST)/matrix/gen_test_data.py $(PYTHON_EXE)
	$(PYTHON_EXE) $(TEST)/matrix/gen_test_data.py

# PYTHON
PYTHON_EXE=.venv/bin/python
VENV_EXISTS = $(wildcard $(PYTHON_EXE))

$(PYTHON_EXE):
ifeq ($(VENV_EXISTS),)
	@echo "Venv not found. Setting it up now..."
	make setup_venv
endif

setup_venv:
	@command -v python3 >/dev/null 2>&1 || { echo "python3 not found. Please install it."; exit 1; }
	python3 -m venv .venv
	$(PYTHON_EXE) -m pip install -r requirements.txt

# PHONY Targets
.PHONY: all clean test_error test_logger test_matrix lsp

all: $(BIN)/main

test_error: $(BIN)/test_error

test_logger: $(BIN)/test_logger
	$(TEST)/logger/test_logger.sh $(TEST)/logger 

test_matrix: $(BIN)/test_matrix
	$(BIN)/test_matrix

lsp:
	compiledb -n make

clean:
	$(RM) $(foreach EXEFILE, $(EXES), $(BIN)/$(EXEFILE))
	$(RM) $(BUILD)/*.o
	$(RM) -rf $(BIN)/*.dSYM
