CC=avr-gcc
OBJCOPY=avr-objcopy
CHIP=attiny85
F_CPU=1000000 # 1 MHz

CFLAGS=-g -mmcu=$(CHIP) -Os -DF_CPU=$(F_CPU) -Wall

all: mirf_tx_test.hex

clean:
	rm -f *.o *.elf *.map *.hex

mirf_tx_test.hex : mirf_tx_test.elf
	$(OBJCOPY) -j .text -O ihex mirf_tx_test.elf mirf_tx_test.hex

mirf_tx_test.elf : mirf_tx_test.o mirf.o spi.o
	$(CC) $(CFLAGS) -o mirf_tx_test.elf -lm -Wl,-Map,mirf_tx_test.map mirf_tx_test.o # mirf.o spi.o

mirf_tx_test.o : mirf_tx_test.c
	$(CC) $(CFLAGS) -I. -c mirf_tx_test.c

mirf.o : mirf.c mirf.h nRF24L01.h
	$(CC) $(CFLAGS) -c mirf.c

spi.o : spi.c spi.h
	$(CC) $(CFLAGS) -c spi.c


