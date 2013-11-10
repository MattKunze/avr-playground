/**
 * Tests the 7 segment LED display functions
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

	uint8_t test[5];
	test[0] = 100;
	test[1] = 50;
	test[2] = 6;
	test[3] = 237;
	test[4] = 99;

	int8_t offset = 0;
	for( ;; )
	{
		seven_seg_display(test[offset], 1);

		offset++;
		if(offset >= 50)
			offset = 0;
	}

	return 0;
}

