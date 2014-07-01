MCPU    = atmega8
F_CPU   = 1000000
CC      = avr-gcc
CFLAGS  = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCPU) -I../../src
OBJCOPY = avr-objcopy

OBJECTS = main.o uart.o libvirtualwire.a

default: compile

libvirtualwire.a:
	make clean -C ../..
	make -C ../.. F_CPU=$(F_CPU) MCPU=$(MCPU) 
	cp ../../build/libvirtualwire.a .

compile: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o main.elf
	rm -f main.hex
	$(OBJCOPY) -j .text -j .data -O ihex main.elf main.hex
	rm -f main.elf

clean:
	rm -f *.o *.elf *.hex *.a

%.o: %.c
	$(CC) $(CFLAGS) -c $<

fuse:
	# set 1Mhz
	avrdude -p m8 -U lfuse:w:0xe1:m -U hfuse:w:0xd9:m

flash:
	avrdude -p m8 -U flash:w:main.hex
