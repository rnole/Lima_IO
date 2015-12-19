/**
  Utilities
*/

#include "Definitions.h"
#include <stdint.h>
#include "utils.h"

uint8_t check_accuracy(float truth_value, float measurement) {

  if (((truth_value * (1 - ERROR_ACCURACY)) < measurement) &&
      (measurement < (truth_value * (1 + ERROR_ACCURACY))))
  {
    return 1;
  }
  else
    return 0;
}

void split_float_number(float value, int *int_part, int *frac_part){
 
 *int_part = value;
 float fractional_part = value - *int_part;
 *frac_part = trunc(fractional_part * 100); 
     
}

uint16_t analogRead_average(int pin_to_read) {
  uint16_t sample_value = 0;

  for (int i = 0; i < NUMBER_OF_READINGS; i++)
    sample_value += (uint16_t)analogRead(pin_to_read);

  sample_value /=  NUMBER_OF_READINGS;
  return sample_value;
}
