
uint8_t seven_seg_mask[11];

void seven_seg_init()
{
	DDRD = 0xff;
	PORTD = 0xff;

	seven_seg_mask[0] = 0xc0;
	seven_seg_mask[1] = 0xf9;
	seven_seg_mask[2] = 0xa4;
	seven_seg_mask[3] = 0xb0;
	seven_seg_mask[4] = 0x99;
	seven_seg_mask[5] = 0x92;
	seven_seg_mask[6] = 0x82;
	seven_seg_mask[7] = 0xf8;
	seven_seg_mask[8] = 0x80;
	seven_seg_mask[9] = 0x98;
	seven_seg_mask[10] = 0x7f;
}

void seven_seg_digit(uint8_t value, uint16_t duration, uint16_t blank)
{
	PORTD = seven_seg_mask[value];
	sleep(duration);
	PORTD = 0xff;
	sleep(blank);
}

void seven_seg_display(uint8_t value, uint8_t end)
{
	uint8_t m;

	// display hundreds place
	uint8_t display = 0;
	if(value >= 100)
	{
		m = value / 100;
		value -= (m * 100);
		seven_seg_digit(m, 100, 10);
		display = 1;
	}

	// display tens place
	if(display || (value >= 10))
	{
		m = value / 10;
		value -= (m * 10);
		seven_seg_digit(m, 100, 10);
	}

	// display remainder
	seven_seg_digit(value, 100, 10);

	// flash decimal point twice
	if(end)
	{
		seven_seg_digit(10, 25, 10);
		seven_seg_digit(10, 25, 10);

		// sleep a bit before continuing
		sleep(250);
	}
	// sleep a bit before continuing
	else
		sleep(100);
}

