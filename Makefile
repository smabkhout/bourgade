MAX_X ?= 4
MAX_Y ?= 3
NUM_RESOURCES ?= 5
BOURGADE_FLAGS = -DMAX_X=$(MAX_X) -DMAX_Y=$(MAX_Y) -DNUM_RESOURCES=$(NUM_RESOURCES)
CFLAGS = -Wall -Wextra -std=c99 -g3 $(BOURGADE_FLAGS) -I src

all: project

%.o:
	$(CC) -c $(CFLAGS) $< -o $@

project: project.o
	$(CC) $(CFLAGS) project.o -o project

test:
	@echo "To be done"

clean:
	@rm -f project test_project *.o *~ src/*~ test/*~

project.o: src/project.c

.PHONY: all clean test
