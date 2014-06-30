CC=avr-gcc
AR=avr-ar
RANLIB=avr-ranlib

MCPU=attiny85
F_CPU=1000000
CFLAGS=-Wall -Os -mmcu=$(MCPU) -DF_CPU=$(F_CPU)

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst src/%.c,build/%.o,$(SOURCES))

TARGET=build/libvirtualwire.a

all: $(TARGET)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)
	$(RANLIB) $@

.PHONY: tests
tests:
	@make test -C tests

clean:
	rm -rf $(TARGET) $(OBJECTS) $(TESTS)
	@make clean -C tests
