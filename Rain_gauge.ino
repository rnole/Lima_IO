/**
Rain gauge driver
DUDA: ¿Como sabes de qué tiempo a qué tiempo se va a medir?
*/
#include "Definitions.h"
#include <avr/interrupt.h>
#include "Rain_gauge.h"

extern uint16_t precipitation_counter = 0;

void RainG_interruptFunction(void) {
  current_time = micros() - last_rain_time;
  last_rain_time = micros();

  if (current_time > 500)  // checking if interval > 500us, debouncing
    precipitation_counter++;
}


float RainG_read(void){
  
  float rain_amount = precipitation_counter * 0.2794;
  precipitation_counter = 0;
  return rain_amount;
}

void RainG_reset(void){
    precipitation_counter = 0;
}

