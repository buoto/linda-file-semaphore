C=cc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -Werror -D DEBUG -O2
LDFLAGS=-g -O2

EXEC=linda
LIB=linda
LIB_FILE=lib$(LIB).a

OBJ_DIR=obj

SRC=tuple.c node.c
SRC_MAIN=main.c
SRC_DIR=src
SRC_OBJ_DIR=$(OBJ_DIR)/src
SRC_OBJ=$(addprefix $(SRC_OBJ_DIR)/,$(SRC:.c=.o))
SRC_OBJ_MAIN=$(addprefix $(SRC_OBJ_DIR)/,$(SRC_MAIN:.c=.o))

TEST=node_tests.c
TEST_DIR=tests
TEST_OBJ_DIR=$(OBJ_DIR)/test
TEST_OBJ=$(addprefix $(TEST_OBJ_DIR)/,$(TEST:.c=.o))

all: $(EXEC)

$(EXEC): $(SRC_OBJ_DIR) $(SRC_OBJ_MAIN) $(LIB_FILE)
	$(CC) -lm -o $(EXEC) $(SRC_OBJ_MAIN) -L. -l$(LIB)

$(LIB_FILE): $(SRC_OBJ)
	ar rcs $(LIB_FILE) $(SRC_OBJ)

$(SRC_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(SRC_OBJ_DIR):
	mkdir -p $(SRC_OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

#
# CC=cc
# CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -Werror -D DEBUG # -O2
# LDFLAGS=-g # -O2
#
# SRCDIR=src
# OBJDIR=obj
#
# TESTSDIR=tests
# TESTSOBJDIR=testsobj
#
# # add all source files below
# SRC=tuple.c node.c
# OBJ=$(addprefix $(OBJDIR)/,$(SRC:.c=.o))
#
#
# # add all test source files below
# TESTS=node_tests.c
# OBJ_TESTS=$(addprefix $(TESTSOBJDIR)/,$(TESTS:.c=.o))
#
# MAIN_OBJ=$(OBJDIR)/main.o
# TEST_OBJ=$(TESTSOBJDIR)/node_tests.o
#
# EXEC=linda
# TEST_EXEC=tests
#
# .PHONY: all clean run debug clean test
#
# all: $(EXEC)
#
# rebuild: clean all
#
# $(EXEC): $(OBJDIR) $(OBJ) $(MAIN_OBJ)
# 	$(CC) $(MAIN_OBJ) $(OBJ) -o $(EXEC) $(LDFLAGS)
#
# run: $(EXEC)
# 	./$(EXEC)
#
# $(TEST_EXEC): $(TESTSOBJDIR) $(OBJ_TESTS) $(TEST_OBJ)
# 	$(CC) $(TEST_OBJ) $(OBJ_TESTS) -o $(TEST_EXEC) $(LDFLAGS) -lcheck
#
# test: $(TEST_EXEC)
# 	./$(TEST_EXEC)
#
# $(OBJDIR)/%.o: $(SRCDIR)/%.c
# 	$(CC) -c $(CFLAGS) $< -o $@
#
# $(TESTSOBJDIR)/%.o: $(TESTSDIR)/%.c
# 	$(CC) -c $(CFLAGS) $< -o $@
#
# debug: $(EXEC)
# 	gdb $(EXEC)
#
# test_debug: $(TEST_EXEC)
# 	gdb $(TEST_EXEC)
#
# $(OBJDIR):
# 	mkdir $(OBJDIR)
#
# $(TESTSOBJDIR):
# 	mkdir $(TESTSOBJDIR)
#
# clean:
# 	rm -f $(OBJ) $(MAIN_OBJ) $(EXEC) $(OBJ_TESTS) $(TEST_EXEC) $(CLI_OBJ) $(CLI_EXEC)
