#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
// #include "sys/ctimer.h"
#include "dev/leds.h"
// #include "dev/watchdog.h"
// #include "random.h"
#include "board-peripherals.h"
// #include "rf-core/rf-ble.h"

// #include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>

static struct etimer et;

PROCESS(hello_world_process, "hello world process");
PROCESS(sensor_process, "sensor monitoring");
AUTOSTART_PROCESSES(&hello_world_process);

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
      } else {
        leds_toggle(LEDS_RED);
        etimer_set(&et_red, CLOCK_SECOND / 8);
      }
    }
  }

  PROCESS_END();
}


static void
get_hdc_reading()
{
  int value;
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("HDC: Temp=%d.%02d C\n", value / 100, value % 100);
  } 

  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMIDITY);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("HDC: Humidity=%d.%02d %%RH\n", value / 100, value % 100);
  }
}

static void
get_light_reading()
{
  int value;
  value = opt_3001_sensor.value(0);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    printf("OPT: Light=%d.%02d lux\n", value / 100, value % 100);
  }
}


PROCESS_THREAD(sensor_process, ev, data)
{

  PROCESS_BEGIN();
  printf("sensor process start!\n");

  // init_sensors();
  // SENSORS_ACTIVATE(batmon_sensor);

  /* Init the BLE advertisement daemon */
  // rf_ble_beacond_config(0, BOARD_STRING);
  // rf_ble_beacond_start();

  etimer_set(&et, CLOCK_SECOND * 20);
  // get_sync_sensor_readings();
  // init_sensor_readings();

  while(1) {

    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &et) {
        etimer_set(&et, CLOCK_SECOND * 20);
        SENSORS_ACTIVATE(hdc_1000_sensor);
    } else if(ev == sensors_event && data == &hdc_1000_sensor) {
        get_hdc_reading();
        SENSORS_ACTIVATE(opt_3001_sensor);
    }else if(ev == sensors_event && data == &opt_3001_sensor) {
        get_light_reading();
    } 
  }

  PROCESS_END();
}
