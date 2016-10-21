#ifndef __SENSOR_PROCESS_H__
#define __SENSOR_PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-engine.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "board-peripherals.h"

extern process_event_t sensor_data_ready;
typedef struct sensor_data_{
  int temp;
  int hum;
  int light;
} sensor_data_t;

PROCESS_NAME(sensor_process);
#endif

