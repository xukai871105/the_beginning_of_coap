
#include "contiki.h"
#include "dev/leds.h"
#include "net/ipv6/uip-ds6.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "hello world process");
PROCESS(simple_process, "simple process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer et_red;
  PROCESS_BEGIN();

  etimer_set(&et_red, CLOCK_SECOND / 8);
  printf("hello world!\n");
  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_red)) {
      if(uip_ds6_get_global(ADDR_PREFERRED) != NULL) {
        leds_off(LEDS_RED);
        printf("device has joined the net\n");
        process_start(&simple_process, NULL);
      } else {
        leds_toggle(LEDS_RED);
        etimer_set(&et_red, CLOCK_SECOND / 8);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(simple_process, ev, data)
{
  static struct etimer et_green;
  PROCESS_BEGIN();

  printf("simple process\n");
  etimer_set(&et_green, CLOCK_SECOND / 4);

  while(1) {
    PROCESS_YIELD();
    if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et_green)) {
      leds_toggle(LEDS_GREEN);
      etimer_set(&et_green, CLOCK_SECOND / 4);
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

