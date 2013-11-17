# Example Makefile for demo project
# Copied from http://www.enteract.com/~rneswold/avr/x421.html
# with "clean" added.

CC=avr-gcc
OBJCOPY=avr-objcopy

MEGA_CFLAGS=-g -mmcu=atmega8 -O0

all: spi-pong.hex

clean:
	rm -f *.o *.out *.map *.hex

spi-pong.hex : spi-pong.out
	$(OBJCOPY) -j .text -O ihex spi-pong.out spi-pong.hex

spi-pong.out : spi-pong.o
	$(CC) $(MEGA_CFLAGS) -o spi-pong.out -Wl,-Map,spi-pong.map spi-pong.o

spi-pong.o : spi-pong.c ../shared/common.h
	$(CC) $(MEGA_CFLAGS) -Os -c spi-pong.c

