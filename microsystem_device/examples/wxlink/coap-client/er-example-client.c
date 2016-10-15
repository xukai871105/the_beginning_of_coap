
/**
* \file
*      Erbium (Er) CoAP client example.
* \author
*      Matthias Kovatsch <kovatsch@inf.ethz.ch>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-engine.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "ip64-addr.h"

#define COAP_HEART 1
#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

PROCESS(er_example_client, "Erbium Example Client");
AUTOSTART_PROCESSES(&er_example_client);

uip_ipaddr_t server_ipaddr;


/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;

  int len = coap_get_payload(response, &chunk);

  printf("[%d]%s\n", len, (char *)chunk);
}

PROCESS_THREAD(er_example_client, ev, data)
{
  PROCESS_BEGIN();

  static coap_packet_t request[1];      /* This way the packet can be treated as pointer as usual. */

  uip_ip4addr_t ip4addr;
  uip_ipaddr(&ip4addr, 192, 168, 0, 3);

  ip64_addr_4to6(&ip4addr, &server_ipaddr);
  PRINTF("Server address: ");
  PRINT6ADDR(&server_ipaddr);
  PRINTF("\n");

  /* receives all CoAP messages */
  coap_init_engine();

  //etimer_set(&et, ER_COAP_DEMO_NET_CONNECT_PERIODIC);

  while(1)  {
    PROCESS_YIELD();

    if(ev == sensors_event && data == &button_left_sensor) {
      printf("left button press\n");
      // send_packet();

        coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
        coap_set_header_uri_path(request, "time");
        PRINT6ADDR(&server_ipaddr);
        PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
        printf("uri hello coap response :\n");
        COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                              client_chunk_handler);
    }

  }
  PROCESS_END();
}
