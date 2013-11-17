# Example Makefile for demo project
# Copied from http://www.enteract.com/~rneswold/avr/x421.html
# with "clean" added.

CC=avr-gcc
OBJCOPY=avr-objcopy

MEGA8_CFLAGS=-g -mmcu=atmega8 -O0
TINY85_CFLAGS=-g -mmcu=attiny85 -O0

all: spi-pong.hex usi-slave.hex

clean:
	rm -f *.o *.out *.map *.hex

spi-pong.hex : spi-pong.out
	$(OBJCOPY) -j .text -O ihex spi-pong.out spi-pong.hex

spi-pong.out : spi-pong.o
	$(CC) $(MEGA8_CFLAGS) -o spi-pong.out -Wl,-Map,spi-pong.map spi-pong.o

spi-pong.o : spi-pong.c ../shared/common.h
	$(CC) $(MEGA8_CFLAGS) -Os -c spi-pong.c

usi-slave.hex : usi-slave.out
	$(OBJCOPY) -j .text -O ihex usi-slave.out usi-slave.hex

usi-slave.out : usi-slave.o
	$(CC) $(TINY85_CFLAGS) -o usi-slave.out -Wl,-Map,usi-slave.map usi-slave.o

usi-slave.o : usi-slave.c ../shared/common.h
	$(CC) $(TINY85_CFLAGS) -Os -c usi-slave.c

