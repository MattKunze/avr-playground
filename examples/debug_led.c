/**
 * Tests the debug LED flash function. Flashes B0 intermittently, and tying
 * B1 to VCC will increment/decrement the flash count.
 */

// enable debug_flash in common header
#define DEBUG_LED_PORT	PORTB
#define DEBUG_LED_PIN	0	

#include "../shared/common.h"

int main(void)
{
	uint8_t count = 1;
	uint8_t up = 1;

	debug_led_init();

	// read pin 1 to change count
	DDRB &= ~_BV(1);

	for( ;; )
	{
		debug_flash(count, 10, 50);

		// sleep one second and listen for pin 1 to change the debug count
		uint16_t s = 1000;
		while(s)
		{
			sleep(100);
			s -= 100;

			if(PINB & _BV(1))
			{
				debug_flash(1, 100, 50);
				if(up)
				{
					count++;
					if(count == 5)
						up = 0;
				}
				else
				{
					count--;
					if(count == 1)
						up = 1;
				}
			}
		}
		if(s)
			sleep(s);
	}

	return 0;
}

