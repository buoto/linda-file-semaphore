C=cc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -Werror -D DEBUG # -O2
LDFLAGS=-g # -O2

SRCDIR=src
OBJDIR=obj

# add all source files below
SRC=
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))

# add all test source files below
TESTS=
TESTS_OBJ=$(addprefix $(OBJDIR)/,$(TESTS:.c=.o))

MAIN_OBJ=$(OBJDIR)/main.o
TEST_OBJ=$(OBJDIR)/test.o

EXEC=linda
TEST_EXEC=tests

.PHONY: all clean run debug clean test

all: $(EXEC)

rebuild: clean all

$(EXEC): $(OBJDIR) $(OBJ) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(OBJ) -o $(EXEC) $(LDFLAGS)

run: $(EXEC)
	./$(EXEC)

$(TEST_EXEC): $(OBJ) $(TEST_OBJ) $(TESTS_OBJ)
	$(CC) $(TEST_OBJ) $(TESTS_OBJ) -o $(TEST_EXEC) $(LDFLAGS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

debug: $(EXEC)
	gdb $(EXEC)

test_debug: $(TEST_EXEC)
	gdb $(TEST_EXEC)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f $(OBJ) $(MAIN_OBJ) $(EXEC) $(TESTS_OBJ) $(TEST_EXEC) $(CLI_OBJ) $(CLI_EXEC)
