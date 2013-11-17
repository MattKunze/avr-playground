// enable debug_flash in common header
#define DEBUG_LED_PORT	PORTC
#define DEBUG_LED_PIN	1

#include "../shared/common.h"

#define DD_SS      PINB2
#define DD_MOSI    PINB3
#define DD_MISO    PINB4
#define DD_SCK     PINB5
#define DDR_SPI    DDRB
#define PORT_SPI   PORTB

void SPI_MasterInit(void) {
  uint8_t temp;

  // Set MOSI and SCK output, all others input
  DDR_SPI = _BV(DD_MOSI) | _BV(DD_SCK);
  // Enable SPI, Master, interrupts, set clock rate fck/16
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPIE) | _BV(SPR0);

  // clear registers
  temp = SPSR;
  temp = SPDR;
}
void SPI_SlaveInit(void) {
  uint8_t temp;

  // set MISO output, all others input
  DDR_SPI = _BV(DD_MISO);
  // enable SPI, interrupts
  SPCR = _BV(SPE) | _BV(SPIE);

  // clear registers
  temp = SPSR;
  temp = SPDR;
}

char SPI_MasterTransmit(char data)
{
  // Start transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & _BV(SPIF)));

  return SPDR;
}

char SPI_SlaveReceive(char response) {
  // need to write the byte sent back to master first
  SPDR = response;
  // wait for receive to complete
  while(!(SPSR & _BV(SPIF)));
  return SPDR;
}

/* interrupt placeholder
ISR(SPI_STC_vect) {
  char data = SPDR;
  debug_flash_pin(data, 100, 50, 3);
}
*/

int main(void) {
  debug_led_init();

  // use slave select to enable as master or slave
  DDRB &= ~_BV(DD_SS);
  uint8_t master = 0;
  if(PINB & _BV(DD_SS)) {
    SPI_MasterInit();
    debug_flash(1, 100, 50);
    master = 1;
  }
  else {
    SPI_SlaveInit();
    debug_flash(2, 100, 50);
  }

  DDRC &= ~_BV(0);
  /* sei(); */

  char count = 2;
  for(;;) {
    char data = 0;
    if(master && (PINC & _BV(0))) {
      data = SPI_MasterTransmit(count++);
      debug_flash_pin(2, 100, 50, 2);
    }
    else if(!master) {
      data = SPI_SlaveReceive(1 + count++ % 2);
    }
    if(data)
      debug_flash_pin(data, 100, 50, 3);
  }
}

