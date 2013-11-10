# Example Makefile for demo project
# Copied from http://www.enteract.com/~rneswold/avr/x421.html
# with "clean" added.

CC=avr-gcc
OBJCOPY=avr-objcopy
CHIP=atmega8
#CHIP=atmega16
#CHIP=atmega8515
#CHIP=attiny13

CFLAGS=-g -mmcu=$(CHIP) -O0

all: debug_led.hex 7segment.hex adc.hex

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

debug_led.hex : debug_led.out
	$(OBJCOPY) -j .text -O ihex debug_led.out debug_led.hex

debug_led.out : debug_led.o
	$(CC) $(CFLAGS) -o debug_led.out -Wl,-Map,debug_led.map debug_led.o

debug_led.o : debug_led.c ../shared/common.h
	$(CC) $(CFLAGS) -Os -c debug_led.c
	$(CC) $(CFLAGS) -o debug_led.elf debug_led.c

bam.hex : bam.out
	$(OBJCOPY) -j .text -O ihex bam.out bam.hex

bam.out : bam.o
	$(CC) $(CFLAGS) -o bam.out -Wl,-Map,bam.map bam.o

bam.o : bam.c ../shared/common.h ../shared/bam.h
	$(CC) $(CFLAGS) -Os -c bam.c

test.hex : test.out
	$(OBJCOPY) -j .text -O ihex test.out test.hex

test.out : test.o
	$(CC) $(CFLAGS) -o test.out -Wl,-Map,test.map test.o

test.o : test.c
	$(CC) $(CFLAGS) -Os -c test.c

7segment.hex : 7segment.out
	$(OBJCOPY) -j .text -O ihex 7segment.out 7segment.hex

7segment.out : 7segment.o
	$(CC) $(CFLAGS) -o 7segment.out -Wl,-Map,7segment.map 7segment.o

7segment.o : 7segment.c ../shared/common.h ../shared/7segment.h
	$(CC) $(CFLAGS) -Os -c 7segment.c
	$(CC) $(CFLAGS) -o 7segment.elf 7segment.c

adc.hex : adc.out
	$(OBJCOPY) -j .text -O ihex adc.out adc.hex

adc.out : adc.o
	$(CC) $(CFLAGS) -o adc.out -Wl,-Map,adc.map adc.o

adc.o : adc.c ../shared/common.h ../shared/7segment.h
	$(CC) $(CFLAGS) -Os -c adc.c
	$(CC) $(CFLAGS) -o adc.elf adc.c

