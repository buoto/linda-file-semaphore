CC=cc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -D DEBUG -O2 # -Werror
LDFLAGS=-g -O2
TESTFLAGS=`pkg-config --libs --cflags check`

SRCDIR=src
OBJDIR=obj
TESTDIR=tests

EXEC=linda
TESTEXEC=$(EXEC)_test

SRC=tuple.c node.c linda.c cli.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
OBJ_MAIN=$(OBJDIR)/main.o

.PHONY: all clean run debug test

all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug: $(EXEC)
	gdb $(EXEC)

$(EXEC): $(OBJDIR) $(OBJ_MAIN) $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJ_MAIN) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

TEST_OBJ=$(OBJDIR)/tests_main.o

test: $(TESTEXEC)
	./$(TESTEXEC)

$(TESTEXEC): $(OBJDIR) $(TEST_OBJ) $(OBJ)
	$(CC) $(LDFLAGS) -o $(TESTEXEC) $(TEST_OBJ) $(OBJ) $(TESTFLAGS)

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC) $(TESTEXEC)
