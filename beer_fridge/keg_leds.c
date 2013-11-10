#include "../shared/common.h"

#define LED_COUNT	4

uint8_t levels[LED_COUNT];

void clear()
{
	// turn everything off
	DDRB = _BV(4);
	PORTB = 0;
}

void set_bank(uint8_t bank, uint8_t level)
{
	// led 10 is on B4
	uint8_t bValue = 0;
	if(level >= 10)
	{
		bValue = _BV(4);
		level = 9;
	}

	// led 8 and 9 are on port A0-1
	uint8_t aValue = 0;
	switch(level)
	{
		case 9:
			aValue |= _BV(1);
		case 8:
			aValue |= _BV(0);
	}

	// now handle lower values on port d
	uint8_t dValue = 0;
	if(level > 7)
		level = 7;
	switch(level)
	{
		case 7:
			dValue |= _BV(6);
		case 6:
			dValue |= _BV(5);
		case 5:
			dValue |= _BV(4);
		case 4:
			dValue |= _BV(3);
		case 3:
			dValue |= _BV(2);
		case 2:
			dValue |= _BV(1);
		case 1:
			dValue |= _BV(0);
	}

	// now set the actual output
	clear();

	PORTA = aValue;
	PORTB = bValue;
	PORTD = dValue;

	DDRB |= _BV(bank);
}

void debug_flash(uint8_t bank, uint8_t level, uint8_t times)
{
	uint8_t count;
	for(count = 0; count < times; count++)
	{
		set_bank(bank, level);
		sleep(20);
		clear();
		sleep(20);
	}
}

int main()
{
	uint8_t bank;
	for(bank = 0; bank < LED_COUNT; bank++)
		levels[bank] = 0;

	// port b controls the led bank - sink current
	DDRB = 0;
	PORTB = 0;

	// ports a and d control the pin - source current
	// D controls leds 1-7
	// A controls leds 8 and 9
	DDRD = 0x7f;
	PORTD = 0;
	DDRA = 0x03;
	PORTA = 0;

	// A2 is also reset and it doesn't seem to source an LED right, 
	// so the only other available pin is B4 to use for led 10
	DDRB = _BV(4);

	// temp
	uint16_t wait = 0;
	uint8_t offset = 0;

	for( ;; )
	{
		// temp
		if(++wait >= 0x0fff)
		{
			wait = 0;

			debug_flash(offset % 4, offset % 10 + 1, 3);

			for(bank = 0; bank < 4; bank++)
			{
				uint8_t t = (bank + offset) % 4;
				if(bank == 0)
					levels[t] = 10;
				else
					levels[t] = bank;
			}
			offset++;
		}

		for(bank = 0; bank < 4; bank++)
		{
			if(levels[bank] > 0)
				set_bank(bank, levels[bank]);
		}
	}

	return 0;
}
