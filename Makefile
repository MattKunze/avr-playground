# Example Makefile for demo project
# Copied from http://www.enteract.com/~rneswold/avr/x421.html
# with "clean" added.

CC=avr-gcc
OBJCOPY=avr-objcopy
CHIP=atmega8515
#CHIP=atmega16

CFLAGS=-g -mmcu=$(CHIP) -O0

nop:

clean:
	rm -f *.o *.out *.map *.hex

#hello.hex : hello.out
#	$(OBJCOPY) -j .text -O ihex hello.out hello.hex
#
#hello.out : hello.o
#	$(CC) $(CFLAGS) -o hello.out -Wl,-Map,hello.map hello.o
#
#hello.o : hello.c common.h
#	$(CC) $(CFLAGS) -Os -c hello.c

