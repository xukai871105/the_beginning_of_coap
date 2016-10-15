
#include "contiki.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/leds.h"
#include <stdio.h>

static struct etimer et;
static struct ctimer ct;

/* A timeout used when waiting to connect to a network */
#define ER_COAP_DEMO_NET_CONNECT_PERIODIC        (CLOCK_SECOND >> 3)
/* Provide visible feedback via LEDS while searching for a network */
#define ER_COAP_NO_NET_LED_DURATION             (ER_COAP_DEMO_NET_CONNECT_PERIODIC >> 1)

static void
net_led_off(void *d)
{
  leds_off(LEDS_RED);
}

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello World Process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{

  PROCESS_BEGIN();

  etimer_set(&et, ER_COAP_DEMO_NET_CONNECT_PERIODIC);

  while(1) 
  {
		PROCESS_YIELD();

		if(ev == PROCESS_EVENT_TIMER && etimer_expired(&et)) {
			if(uip_ds6_get_global(ADDR_PREFERRED) == NULL) {
				leds_on(LEDS_RED);
				ctimer_set(&ct, ER_COAP_NO_NET_LED_DURATION, net_led_off, NULL);
				etimer_set(&et, ER_COAP_DEMO_NET_CONNECT_PERIODIC);
			}
			else
			{
				leds_on(LEDS_GREEN);
				printf("device join in the network!\n");
				printf("Hello CC2538\n");       
			}
		}
	}

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

