#include <string.h>

// enable debug_flash in common header
#define DEBUG_LED_PORT  PORTC
#define DEBUG_LED_PIN   1

#include "../shared/common.h"

#include "nRF24L01.h"
#include "mirf.h"

void main() {
  mirf_init();
  sleep(50);
  mirf_config();

  uint8_t buffer[5] = {14, 123, 63, 23, 54};
  uint8_t buffersize = 5;

  while(1) {
    mirf_send(buffer, buffersize);
    sleep(1000);

    debug_flash(2, 100, 50);

    /*
    // If maximum retries were reached, reset MAX_RT
    if (mirf_max_rt_reached()) {
      mirf_config_register(STATUS, _BV(MAX_RT));

      debug_flash(5, 50, 10);
    }
    */
  }
}

