
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"

#include "er-coap-engine.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "board-peripherals.h"
#include "ip64-addr.h"
#include "ieee-addr.h"

#include "sensor-process.h"
#include "coap-post-process.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

PROCESS(start_process, "start process");
AUTOSTART_PROCESSES(&start_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(start_process, ev, data)
{
  static struct etimer et_red;
  PROCESS_BEGIN();

  etimer_set(&et_red, CLOCK_SECOND / 8);
  printf("system start!\n");
  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_red)) {
      if(uip_ds6_get_global(ADDR_PREFERRED) != NULL) {
        leds_off(LEDS_RED);
        printf("device has joined the net\n");
        process_start(&sensor_process, NULL);
        process_start(&coap_post_process, NULL);
      } else {
        leds_toggle(LEDS_RED);
        etimer_set(&et_red, CLOCK_SECOND / 8);
      }
    }
  }

  PROCESS_END();
}
