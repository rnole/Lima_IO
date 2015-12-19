#ifndef _ANEMOMETER_H
#define _ANEMOMETER_H

#define WIND_SAMPLE_TIME    1.0
#define WIND_SPEED_FACTOR   2.4

#define PCINT20_ENABLE()				\
do{							\
        PCIFR  |= (1 << PCIF2);                         \
        PCICR  |= (1 << PCIE2);                         \
}while(0)

#define PCINT20_DISABLE()				\
do{							\
	PCICR  &= ~(1 << PCIE2);                        \
}while(0)


void  Anem_interruptFunction(void);
float Anem_get_wind_speed(void);


#endif
