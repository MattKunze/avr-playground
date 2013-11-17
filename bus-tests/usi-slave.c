// enable debug_flash in common header
#define DEBUG_LED_PORT	PORTB
#define DEBUG_LED_PIN	3

#include "../shared/common.h"

#define DD_MOSI    PINB3
#define DD_MISO    PINB4
#define DD_SCK     PINB5
#define DDR_SPI    DDRB
#define PORT_SPI   PORTB

void USI_SlaveInit(void) {
  DDR_SPI = _BV(DD_MISO);
  USICR = _BV(USIWM0) | _BV(USICS1);
}

char USI_SlaveReceive() {
  // wait for byte to be received
  while(!(USISR & _BV(USIOIF)));

  // clear the counter overflow flag
  USISR = _BV(USIOIF);

  return USIDR;
}

int main(void) {
  debug_led_init();
  USI_SlaveInit();

  DDRB &= ~_BV(1);

  debug_flash(2, 100, 50);
  for(;;) {
    char data = USI_SlaveReceive();
    if(data && (data != 0xff)) {
      debug_flash_pin(1, 100, 50, 4);
      debug_flash_pin(data, 100, 50, 3);
    }
    /*
    else {
      debug_flash(1, 100, 50);
      sleep(1000);
    }
    */
  }
}

