/*
   RadioLib Non-Arduino Raspberry Pi Pico Example

   This example shows how to use RadioLib without Arduino.
   In this case, a Raspberry Pi Pico with 

   Can be used as a starting point to port RadioLib to any platform!
   See this API reference page for details on the RadioLib hardware abstraction
   https://jgromes.github.io/RadioLib/class_hal.html

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "PicoHal.h"


PicoHal* hal = new PicoHal(0);

// NSS pin:   14 chip select
// DIO0 pin:  20
// RESET pin: 15 RST
// DIO1 pin:  6
SX1276 radio = new Module(hal, 14, 20, 15, 6);

int main() {
   stdio_init_all();
   sleep_ms(10000);
   printf("[SX1276] Initializing ... ");

   int state = radio.begin();
   if (state != RADIOLIB_ERR_NONE) {
      printf("failed, code %d\n", state);
      return 1;
   }
   printf("success!\n");


   // while (true) {
   //    // send a packet
   //    printf("[SX1276] Transmitting packet ... ");

   //    state = radio.transmit("Hello World!");

   //    if (state == RADIOLIB_ERR_NONE) {
   //       // the packet was successfully transmitted
   //       printf("success!\n");

   //       // wait for a second before transmitting again
   //       hal->delay(1000);

   //    } else {
   //       printf("failed, code %d\n", state);
   //    }

   // }

  return 0;
}
