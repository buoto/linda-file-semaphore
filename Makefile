make: *** No rule to make target `testsobj/node_tests.o', needed by `tests'.  Stop.

C=cc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -Werror -D DEBUG # -O2
LDFLAGS=-g # -O2

SRCDIR=src
OBJDIR=obj

TESTSDIR=tests
TESTSOBJDIR=testsobj

# add all source files below
SRC=tuple.c node.c
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))

# add all test source files below
TESTS=node_tests.c
OBJ_TESTS=$(addprefix $(TESTSOBJDIR)/,$(TESTS:.c=.o))

MAIN_OBJ=$(OBJDIR)/main.o
TEST_OBJ=$(TESTSOBJDIR)/node_tests.o

EXEC=linda
TEST_EXEC=tests

.PHONY: all clean run debug clean test

all: $(EXEC)

rebuild: clean all

$(EXEC): $(OBJDIR) $(OBJ) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(OBJ) -o $(EXEC) $(LDFLAGS)

run: $(EXEC)
	./$(EXEC)

$(TEST_EXEC): $(TESTSOBJDIR) $(OBJ_TESTS) $(TEST_OBJ)
	$(CC) $(TEST_OBJ) $(OBJ_TESTS) -o $(TEST_EXEC) $(LDFLAGS) -lcheck

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TESTSOBJDIR)/%.o: $(TESTSDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

debug: $(EXEC)
	gdb $(EXEC)

test_debug: $(TEST_EXEC)
	gdb $(TEST_EXEC)

$(OBJDIR):
	mkdir $(OBJDIR)

$(TESTSOBJDIR):
	mkdir $(TESTSOBJDIR)

clean:
	rm -f $(OBJ) $(MAIN_OBJ) $(EXEC) $(OBJ_TESTS) $(TEST_EXEC) $(CLI_OBJ) $(CLI_EXEC)
