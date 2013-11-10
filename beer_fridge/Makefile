# Example Makefile for demo project
# Copied from http://www.enteract.com/~rneswold/avr/x421.html
# with "clean" added.

CC=avr-gcc
OBJCOPY=avr-objcopy
CHIP=attiny2313
#CHIP=atmega16

CFLAGS=-g -mmcu=$(CHIP) -O0

all: keg_leds.hex

clean:
	rm -f *.o *.out *.map *.hex

keg_leds.hex : keg_leds.out
	$(OBJCOPY) -j .text -O ihex keg_leds.out keg_leds.hex

keg_leds.out : keg_leds.o
	$(CC) $(CFLAGS) -o keg_leds.out -Wl,-Map,keg_leds.map keg_leds.o

keg_leds.o : keg_leds.c ../shared/common.h
	$(CC) $(CFLAGS) -Os -c keg_leds.c

