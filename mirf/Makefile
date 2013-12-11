CC=avr-gcc
OBJCOPY=avr-objcopy
CHIP=atmega8

CFLAGS=-g -mmcu=$(CHIP) -O0

all: mirf_pong.hex

clean:
	rm -f *.o *.out *.map *.hex

mirf_pong.hex : mirf_pong.out
	$(OBJCOPY) -j .text -O ihex mirf_pong.out mirf_pong.hex

mirf_pong.out : mirf_pong.o mirf.o spi.o
	$(CC) $(CFLAGS) -o mirf_pong.out -Wl,-Map,mirf_pong.map mirf_pong.o mirf.o spi.o

mirf_pong.o : mirf_pong.c
	$(CC) $(CFLAGS) -Os -c mirf_pong.c

mirf.o : mirf.c mirf.h nRF24L01.h
	$(CC) $(CFLAGS) -Os -c mirf.c

spi.o : spi.c spi.h
	$(CC) $(CFLAGS) -Os -c spi.c


