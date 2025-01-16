.PHONY: check debug clean grade profile all

CFLAGS := -std=c18
CFLAGS += -Wall -Wextra -Wpedantic -g
CFLAGS += -Wall -Wextra -Wpedantic  -Wwrite-strings -Wvla -Wfloat-equal -Waggregate-return  -std=c18 -Wvla
CFLAGS += -D _DEFAULT_SOURCE 
LDLIBS += -lm  -pthread

SRC_DIR := src/c
OBJ_DIR := obj
TST_DIR := test/src/c_test

SRCS := $(wildcard $(SRC_DIR)/*.c) 
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := test_tutelary
CHECK := $(BIN)_check

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS := $(filter-out $(OBJ_DIR)/tutelary.o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit 

all: $(BIN)


debug: CFLAGS += -g
debug: $(BIN)

check: $(CHECK)

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(GRADE) $(CHECK) 

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	./$(CHECK)
