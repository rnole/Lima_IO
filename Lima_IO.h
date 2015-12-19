#ifndef _LIMA_IO_H
#define _LIMA_IO_H

#include <stdint.h>
#include <stdio.h>
#include "Definitions.h"
#include "EEPROM.h"
#include "err.h"
#include "RTC.h"
#include "Rain_gauge.h"
#include "Anemometer.h"
#include "Wind_vane.h"
#include "BMP180.h"
#include "utils.h"

typedef struct{
  char day_str[6];
  char time_str[6];
  float uv;
  float vane;
  float anem;
  float rain_gauge;
  float humidity;
  float temp;
  float pressure;
}struct_weather;



void Ports_init(void);
void ENC28J60_init(void);
void Temp_sensor_init(void);

float UV_sensor_read(void);

err_t    Send_http_post_request(char *param_str);
void     Save_weather_parameters(struct_weather *weather_ptr);

#endif
