#include "sensor-process.h"
#include "coap-post-process.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

static struct etimer et;
process_event_t sensor_data_ready;
sensor_data_t sensor_give;
/*---------------------------------------------------------------------------*/
static void get_hdc_reading()
{
  int value;
  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    sensor_give.temp = value / 100;
    printf("HDC: Temp = %d.%02d C\n", value / 100, value % 100);
  }

  value = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMIDITY);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    sensor_give.hum = value / 100;
    printf("HDC: Humidity = %d.%02d %%RH\n", value / 100, value % 100);
  }
}
/*---------------------------------------------------------------------------*/
static void get_light_reading()
{
  int value;
  value = opt_3001_sensor.value(0);
  if(value != CC26XX_SENSOR_READING_ERROR) {
    sensor_give.light = value / 100;
    printf("OPT: Light = %d.%02d lux\n", value / 100, value % 100);
  }
}
PROCESS(sensor_process, "sensor monitoring");
PROCESS_THREAD(sensor_process, ev, data)
{
  PROCESS_BEGIN();
  printf("sensor process start!\n");
  sensor_data_ready = process_alloc_event(); 

  etimer_set(&et, CLOCK_SECOND * 60);

  while(1)  {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &et) {
      printf("start to acquire sensor data\n");
      SENSORS_ACTIVATE(opt_3001_sensor);
      etimer_set(&et, CLOCK_SECOND * 60);
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
