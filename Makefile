CC=cc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -D DEBUG -O2 # -Werror
LDFLAGS=-g -O2

OBJ_DIR=build

EXEC=linda
LIB=linda
LIB_FILE=$(OBJ_DIR)/lib$(LIB).a

SRC=tuple.c node.c linda.c cli.c
SRC_MAIN=main.c
SRC_DIR=src
SRC_OBJ_DIR=$(OBJ_DIR)/obj
SRC_OBJ=$(addprefix $(SRC_OBJ_DIR)/,$(SRC:.c=.o))
SRC_OBJ_MAIN=$(addprefix $(SRC_OBJ_DIR)/,$(SRC_MAIN:.c=.o))

.PHONY: all clean run debug test

all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug: $(EXEC)
	gdb $(EXEC)

$(EXEC): $(SRC_OBJ_DIR) $(SRC_OBJ_MAIN) $(LIB_FILE)
	$(CC) $(LDFLAGS) -o $(EXEC) $(SRC_OBJ_MAIN) -L$(OBJ_DIR) -l$(LIB)

$(LIB_FILE): $(SRC_OBJ_DIR) $(SRC_OBJ)
	ar rcs $(LIB_FILE) $(SRC_OBJ)

$(SRC_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(SRC_OBJ_DIR):
	mkdir -p $(SRC_OBJ_DIR)

TEST_EXEC=$(EXEC)_test

TEST=tests_main.c
TEST_DIR=tests
TEST_OBJ_DIR=$(OBJ_DIR)/test_obj
TEST_OBJ=$(addprefix $(TEST_OBJ_DIR)/,$(TEST:.c=.o))

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJ_DIR) $(TEST_OBJ) $(LIB_FILE)
	$(CC) $(LDFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) -L$(OBJ_DIR) -l$(LIB) -lcheck

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXEC) $(TEST_EXEC)
