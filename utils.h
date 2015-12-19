/**
  Utilities
*/

#ifndef UTILS_H_
#define UTILS_H_

uint8_t check_accuracy(float truth_value, float measurement);
uint16_t analogRead_average(int pin_to_read);
void    split_float_number(float value, int *int_part, int *frac_part);


#endif
