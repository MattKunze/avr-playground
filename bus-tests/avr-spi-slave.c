// enable debug_flash in common header
#define DEBUG_LED_PORT	PORTC
#define DEBUG_LED_PIN	1

#include "../shared/common.h"

#define DD_MOSI    PINB3
#define DD_MISO    PINB4
#define DD_SCK     PINB5
#define DDR_SPI    DDRB
#define PORT_SPI   PORTB

void SPI_SlaveInit(void) {
  DDR_SPI = _BV(DD_MISO);
  SPCR = _BV(SPE);
}

char SPI_SlaveReceive() {
  while(!(SPSR & _BV(SPIF)));
  return SPDR;
}

int main(void) {
  debug_led_init();
  SPI_SlaveInit();

  DDRB &= ~_BV(1);

  debug_flash(4, 100, 50);
  for(;;) {
    char data = SPI_SlaveReceive();
    if(data)
      debug_flash_pin(data, 100, 50, 1);
    else
      debug_flash_pin(2, 100, 50, 2);
  }
}

