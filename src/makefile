TARGET := ../walking_on_graphs
TEST := ../test
CC := gcc
CFLAGS := -O1    -g -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=gnu17

SRCS := misc.c tactics.c walker.c  graph_table.c movement.c restart_sim.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)


$(TEST): $(SRCS) arena.c main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) $(CLIBS)
 
	
.PHONY: clean
