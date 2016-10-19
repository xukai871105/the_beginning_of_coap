
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

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

uip_ipaddr_t server_ipaddr;
static struct etimer et;
static int hum = 0;
static int temp = 0;
static int light = 0;

static process_event_t sensor_data_ready;
typedef struct sensor_data_{
  int temp;
  int hum;
  int light;
} sensor_data_t;
sensor_data_t sensor_give;

PROCESS(sensor_process, "sensor monitor");
PROCESS(coap_post_process, "coap client");
PROCESS(hello_world_process, "hello world process");
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

/*---------------------------------------------------------------------------*/
static void
get_hdc_reading()
{
  int value;
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    // temp = value / 100;
    sensor_give.temp = value / 100;
    printf("HDC: Temp = %d.%02d C\n", value / 100, value % 100);
  }

  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMIDITY);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    // hum = value / 100;
    sensor_give.hum = value / 100;
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
    // light = value / 100;
    sensor_give.hum = value / 100;
    printf("OPT: Light = %d.%02d lux\n", value / 100, value % 100);
  }
}

PROCESS_THREAD(sensor_process, ev, data)
{
  PROCESS_BEGIN();
  printf("sensor process start!\n");
  sensor_data_ready = process_alloc_event(); 

  etimer_set(&et, CLOCK_SECOND * 20);

  while(1)  {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &et) {
      printf("start to acquire sensor data\n");
      SENSORS_ACTIVATE(opt_3001_sensor);
      etimer_set(&et, CLOCK_SECOND * 10);
    } else if(ev == sensors_event && data == &opt_3001_sensor) {
        get_light_reading();
        SENSORS_ACTIVATE(hdc_1000_sensor);
    } else if(ev == sensors_event && data == &hdc_1000_sensor) {
        get_hdc_reading();
        process_post(&coap_post_process, sensor_data_ready, (void *)&sensor_give);
    }

  }
  PROCESS_END();
}

/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("[%d]%s\n", len, (char *)chunk);
}


PROCESS_THREAD(coap_post_process, ev, data) 
{
  PROCESS_BEGIN();
  printf("coap post process start!\n");

  static coap_packet_t request[1];      /* This way the packet can be treated as pointer as usual. */

  uip_ip4addr_t ip4addr;
  uip_ipaddr(&ip4addr, 192, 168, 0, 3);

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
      printf("event\n");
      sensor_data_t sensor_take = *(sensor_data_t*)data;
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_token(request, (uint8_t*)"1234", 4);
      coap_set_header_uri_path(request, url);
      coap_set_header_content_format(request, APPLICATION_JSON);

      char payload[32];
      memset(payload, 0x00, 32);
      int len = sprintf(payload, "{\"temp\":%d, \"hum\":%d, \"light\":%d}", 
              sensor_take.temp, sensor_take.hum, sensor_take.light);
      coap_set_payload(request, (uint8_t *)payload, len);

      PRINT6ADDR(&server_ipaddr); PRINTF(" : %u\n", UIP_HTONS(REMOTE_PORT));
      printf("coap response :\n");
      COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request,
                            client_chunk_handler);
    }
  }

  PROCESS_END();
}
