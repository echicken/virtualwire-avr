CFLAGS=-g -Wall -Wextra -DTEST -Os -I../src -Imock

SOURCES=$(wildcard ../src/**/*.c ../src/*.c)
OBJECTS=$(patsubst ../src/%.c,build/%.o,$(SOURCES))

TEST_SOURCES=$(wildcard **/*_test.c *_test.c)
TEST_OBJECTS=$(patsubst %.c,build/%.o,$(TEST_SOURCES))
TESTS=$(patsubst %.c,build/%,$(TEST_SOURCES))

all: build $(OBJECTS) $(TEST_OBJECTS) $(TESTS)

build:
	mkdir build

# compile library sources
build/%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# compile test sources
build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# link tests
build/%: build/%.o $(OBJECTS)
	$(CC) $(OBJECTS) $< -o $@

test: build $(TESTS)
	@./runtests.sh

clean:
	rm -rf $(OBJECTS) $(TEST_OBJECTS) $(TESTS) build
