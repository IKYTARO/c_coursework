C_FLAGS         = -std=c11 -Wall -Wextra -Werror -pedantic
SANITIZER_FLAGS = -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover \
	              -fstack-protector -fanalyzer

BINARY_TREE_DIR = BinaryTree
HASH_TABLE_DIR  = HashTable
LIST_DIR        = LinkedList
STACK_DIR       = Stack

BINARY_TREE_FILES = $(wildcard $(BINARY_TREE_DIR)/*.c $(BINARY_TREE_DIR)/*.h)
HASH_TABLE_FILES  = $(wildcard $(HASH_TABLE_DIR)/*.c $(HASH_TABLE_DIR)/*.c)
LIST_FILES        = $(wildcard $(LIST_DIR)/*.c $(LIST_DIR)/*.c)
STACK_FILES       = $(wildcard $(STACK_DIR)/*.c $(STACK_DIR)/*.c)

ALL_SOURCES = $(BINARY_TREE_FILES) $(HASH_TABLE_FILES) $(LIST_FILES) $(STACK_FILES)

test-bst:
	gcc $(C_FLAGS) $(SANITIZER_FLAGS) $(STACK_DIR)/stack.c $(BINARY_TREE_DIR)/check_bst.c $(BINARY_TREE_DIR)/test.c

format:
	clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 4}" $(ALL_SOURCES)

VALGRIND_OPTS = \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \