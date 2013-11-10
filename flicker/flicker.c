#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

static void delayMillis(uint16_t aMillis) {
	while (aMillis--) {
		_delay_ms(1);
	}
}

static uint8_t flicker(uint8_t bit, uint8_t state, uint8_t factor) {
	state = (factor * state) + 129;
	if(state > 50)
		PORTB |= _BV(bit);
	else
		PORTB &= ~_BV(bit);
	return state;
}

int main() {
	DDRB = 0xff;
	PORTB = 0;

	uint8_t s0 = 0;
	uint8_t s1 = 0;
	uint8_t s2 = 0;
	uint8_t s3 = 0;
	uint8_t s4 = 0;
	uint8_t s5 = 0;
	while (true) {
		s0 = flicker(0, s0, 3);
		s1 = flicker(1, s1, 5);
		s2 = flicker(2, s2, 7);
		s3 = flicker(3, s3, 13);
		s4 = flicker(4, s4, 17);
		s5 = flicker(5, s5, 19);

		delayMillis(50);
	}
	
	return 0;
}
