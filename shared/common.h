#define F_CPU 3686400

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#ifndef __AVR_COMMON_H__
#define __AVR_COMMON_H__

void sleep(uint16_t millis) {
  while ( millis ) {
    _delay_ms(1);
    millis--;
  }
}

uint8_t readLine(char* buffer, uint8_t max)
{
	uint8_t pos = 0;
	while(pos < (max - 1))
	{
		buffer[pos + 1] = '\0';
		char c = getchar();

		switch(c)
		{
			case '\r': 
				break; // ignore
			case '\n': 
				return pos;
			default:
				buffer[pos++] = c;
				break;
		}
	}

	return pos;
}

#ifdef DEBUG_LED_PORT

#ifndef DEBUG_LED_PIN
#error "DEBUG_LED_PIN must be defined along with DEBUG_LED_PORT"
#endif

void debug_led_init()
{
	DDRB |= _BV(DEBUG_LED_PIN);
	DEBUG_LED_PORT &= ~_BV(DEBUG_LED_PIN);
}

void debug_flash( 
			uint8_t times, 
			uint8_t pulse, 
			uint8_t width )
{
	while(times)
	{
		DEBUG_LED_PORT |= _BV(DEBUG_LED_PIN);
		sleep(pulse);
		DEBUG_LED_PORT &= ~_BV(DEBUG_LED_PIN);

		times--;
		if(times)
			sleep(width);
	}
}

#endif // DEBUG_LED_PORT

#endif // __AVR_COMMON_H__
