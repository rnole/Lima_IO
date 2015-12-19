/**
Wind vane driver
*/

#include "Definitions.h"
#include <stdint.h>
#include "utils.h"
#include "Wind_vane.h"

float Vane_read(void) {
  uint16_t adc_reading = 0;
  float vane_voltage = 0;
  float direction_deg = 0;

  adc_reading   = (uint16_t)analogRead_average(VANE);
  vane_voltage  = (adc_reading * 5.0) / 1023.0;
  direction_deg = Vane_volt_2_degs(vane_voltage);
  return direction_deg;
}


float Vane_volt_2_degs(float voltage) {

  if (check_accuracy(3.84, voltage))
    return 0.0;
  if (check_accuracy(1.98, voltage))
    return 22.5;
  if (check_accuracy(2.25, voltage))
    return 45.0;
  if (check_accuracy(0.41, voltage))
    return 67.5;
  if (check_accuracy(0.45, voltage))
    return 90.0;
  if (check_accuracy(0.32, voltage))
    return 112.5;
  if (check_accuracy(0.90, voltage))
    return 135.0;
  if (check_accuracy(0.62, voltage))
    return 157.5;
  if (check_accuracy(1.40, voltage))
    return 180.0;
  if (check_accuracy(1.19, voltage))
    return 202.5;
  if (check_accuracy(3.08, voltage))
    return 225.0;
  if (check_accuracy(2.93, voltage))
    return 247.5;
  if (check_accuracy(4.62, voltage))
    return 270.0;
  if (check_accuracy(4.04, voltage))
    return 292.5;
  if (check_accuracy(4.34, voltage))
    return 315.0;
  if (check_accuracy(3.43, voltage))
    return 337.5;

  return 0.0;
}
