CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g -D DEBUG# -O2 -Werror
LDFLAGS=-g -lpthread -lm # -O2
TESTFLAGS=`pkg-config --libs --cflags check`

SRCDIR=src
OBJDIR=obj
TESTDIR=tests
FTESTEXEC=functional_tests/run.sh

EXEC=linda
TESTEXEC=$(EXEC)_test

SRC=tuple.c node.c linda.c cli.c parser.c file.c store.c time_utils.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
HEADERS=$(addprefix $(SRCDIR)/,$(SRC:.c=.h))
MAINOBJ=$(OBJDIR)/main.o

.PHONY: all clean run debug test FORCE

all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug: $(EXEC)
	gdb $(EXEC)

functional_test: $(EXEC)
	./$(FTESTEXEC)

$(EXEC): $(OBJDIR) $(MAINOBJ) $(OBJ) $(SRCHEADERS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(MAINOBJ) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

TEST_OBJ=$(OBJDIR)/tests_main.o

$(TEST_OBJ): FORCE

test: $(TESTEXEC)
	./$(TESTEXEC)

$(TESTEXEC): $(OBJDIR) $(TEST_OBJ) $(OBJ)
	$(CC) $(LDFLAGS) -o $(TESTEXEC) $(TEST_OBJ) $(OBJ) $(TESTFLAGS)

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC) $(TESTEXEC)
