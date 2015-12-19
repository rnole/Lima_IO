#ifndef _RAIN_GAUGE_H
#define _RAIN_GAUGE_H

extern uint16_t precipitation_counter;

void RainG_init(void);
void RainG_interruptFunction(void);
float RainG_read(void);


#endif
