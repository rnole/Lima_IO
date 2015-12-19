/**
Anemometer driver
*/

#include "Definitions.h"
#include <stdint.h>
#include "Anemometer.h"


float Anem_get_wind_speed(void) {
  uint32_t cont = 0;
  anem_counter = 0;
  last_wind_time = micros();
  enableInterrupt(ANEN, Anem_interruptFunction, FALLING);
  delay(1000);
  disableInterrupt(ANEN);
  return WIND_SPEED_FACTOR * anem_counter / WIND_SAMPLE_TIME; 
}


void  Anem_interruptFunction(void) {
  current_time = micros() - last_wind_time;
  last_wind_time = micros();

  if (current_time > 1000) {                             // checking if interval > 1ms, debouncing
    anem_counter++;
  }
}
