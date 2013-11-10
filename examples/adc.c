/**
 * Tests analog to digital conversion
 */

// enable debug_flash in common header
#define DEBUG_LED_PORT	PORTB
#define DEBUG_LED_PIN	0	

#include "../shared/common.h"
#include "../shared/7segment.h"

int main(void)
{
	debug_led_init();
	seven_seg_init();

	// VCC reference, left align ouput
	ADMUX |= _BV(REFS0) | _BV(REFS1);
	// VCC reference
	ADMUX &= ~_BV(ADLAR);
	// 128 prescaler
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	// free running mode
	ADCSRA |= _BV(ADFR);
	// enable ADC
	ADCSRA |= _BV(ADEN);
	// start conversion
	ADCSRA |= _BV(ADSC);

	uint8_t h;
	while(1)
	{
		seven_seg_display(ADCL, 1);
		h = ADCH;
		sleep(50);
	}

	return 0;
}

