
#include "coap-post-process.h"
#include "sensor-process.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

uip_ipaddr_t server_ipaddr;

/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("coap response payload:\n");
  printf("[%d]%s\n", len, (char *)chunk);
}
PROCESS(coap_post_process, "coap client");
PROCESS_THREAD(coap_post_process, ev, data) 
{
  PROCESS_BEGIN();
  printf("coap post process start!\n");

  static coap_packet_t request[1]; 

  uip_ip4addr_t ip4addr;
  uip_ipaddr(&ip4addr, 192, 168, 0, 9);

  ip64_addr_4to6(&ip4addr, &server_ipaddr);
  PRINTF("Server address: ");
  PRINT6ADDR(&server_ipaddr);
  PRINTF("\n");

  uint8_t dev_addr[8];
  ieee_addr_cpy_to(dev_addr, 8);

  static char url[32];
  sprintf(url, "devices/%02X%02X", dev_addr[6], dev_addr[7]);
  PRINTF("device url: %s\n", url);
  /* receives all CoAP messages */
  coap_init_engine();

  while(1) {
    PROCESS_YIELD();
    if (ev == sensor_data_ready) {
      printf("take sensor date ready event\n");
      sensor_data_t sensor_take = *(sensor_data_t*)data;
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      // coap_set_token(request, (uint8_t*)"1234", 4);
      coap_set_header_uri_path(request, url);
      coap_set_header_content_format(request, APPLICATION_JSON);

      char payload[32];
      memset(payload, 0x00, 32);
      int len = sprintf(payload, "{\"temp\":%d, \"hum\":%d, \"light\":%d}", 
              sensor_take.temp, sensor_take.hum, sensor_take.light);
      coap_set_payload(request, (uint8_t *)payload, len);

      printf("coap request payload: %s\n", payload);
      PRINT6ADDR(&server_ipaddr); PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
      
      COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                            client_chunk_handler);
    }
  }

  PROCESS_END();
}
