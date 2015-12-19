
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#define F_CPU 8000000UL

/* PORTB  */
#define  ANEN     8            // Arduino D8 = PB0

/* PORTC  */
#define UV_SENSOR 0
#define VANE      2

/* PORTD  */
#define  INT0     2
#define  RAIN_G   3

/* RESPONSE CODES */


#define EEPROM_ADDR                 0x50      // Address of 24LC512 eeprom chip
#define HTTP_REQUEST_INTERVAL_MS   20000      // Units: ms ... 900 000ms = 15mins
#define MAX_QUERY_SIZE               130
#define NUMBER_OF_READINGS            10
#define ERROR_ACCURACY              0.05

#endif
