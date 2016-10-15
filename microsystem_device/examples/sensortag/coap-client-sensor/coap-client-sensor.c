
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

#define COAP_HEART 1

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

static process_event_t event_data_ready;

PROCESS(er_example_client, "CoAP Client");
PROCESS(coap_post_sensor, "CoAP Client");
AUTOSTART_PROCESSES(&er_example_client, &coap_post_sensor);

uip_ipaddr_t server_ipaddr;
static struct etimer et;
static int hum = 0;
static int temp = 0;
static int light = 0;

/*---------------------------------------------------------------------------*/
static void
get_hdc_reading()
{
  int value;
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    temp = value / 100;
    printf("HDC: Temp = %d.%02d C\n", value / 100, value % 100);
  }

  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMIDITY);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    hum = value / 100;
    printf("HDC: Humidity = %d.%02d %%RH\n", value / 100, value % 100);
  }
}
/*---------------------------------------------------------------------------*/
static void
get_light_reading()
{
  int value;
  value = opt_3001_sensor.value(0);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    light = value / 100;
    printf("OPT: Light = %d.%02d lux\n", value / 100, value % 100);
  }
}

static void
init_sensor_readings(void)
{
  SENSORS_ACTIVATE(hdc_1000_sensor);
  SENSORS_ACTIVATE(opt_3001_sensor);
}

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

  event_data_ready = process_alloc_event(); 

  etimer_set(&et, CLOCK_SECOND * 10);

  while(1)  {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &et) {
      printf("time out\n");
      // init_sensor_readings();
      SENSORS_ACTIVATE(opt_3001_sensor);
      etimer_set(&et, CLOCK_SECOND * 10);
    } else if(ev == sensors_event && data == &opt_3001_sensor) {
        get_light_reading();
        SENSORS_ACTIVATE(hdc_1000_sensor);
    } else if(ev == sensors_event && data == &hdc_1000_sensor) {
        get_hdc_reading();
        process_post(&coap_post_sensor, event_data_ready, NULL);
    }

  }
  PROCESS_END();
}

PROCESS_THREAD(coap_post_sensor, ev, data) {
  PROCESS_BEGIN();

  static coap_packet_t request[1];      /* This way the packet can be treated as pointer as usual. */

  uip_ip4addr_t ip4addr;
  uip_ipaddr(&ip4addr, 192, 168, 0, 3);

  ip64_addr_4to6(&ip4addr, &server_ipaddr);
  PRINTF("Server address: ");
  PRINT6ADDR(&server_ipaddr);
  PRINTF("\n");

  // init_sensor_readings();
  /* receives all CoAP messages */
  coap_init_engine();

  while(1) {
    PROCESS_YIELD();
    if (ev == event_data_ready) {
      printf("event");
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "devices/CD12");
      coap_set_header_content_format(request, APPLICATION_JSON);

      char payload[32];
      memset(payload, 0x00, 32);
      int len = sprintf(payload, "{\"temp\":%d, \"hum\":%d, \"light\":%d}", temp, hum, light);
      coap_set_payload(request, (uint8_t *)payload, len);

      PRINT6ADDR(&server_ipaddr); PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
      printf("coap response :\n");
      COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                            client_chunk_handler);
    }
  }

  PROCESS_END();
}
