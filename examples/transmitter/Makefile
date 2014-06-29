MCPU    = attiny85
F_CPU   = 1000000
CC      = avr-gcc
CFLAGS  = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCPU) -I../../src
OBJCOPY = avr-objcopy

OBJECTS = main.o libvirtualwire.a

default: compile

libvirtualwire.a:
	make -C ../..
	ln -sv ../../build/libvirtualwire.a .

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
	avrdude -p t85 -U lfuse:w:0x62:m -U hfuse:w:0xdf:m

flash:
	avrdude -p t85 -U flash:w:main.hex
