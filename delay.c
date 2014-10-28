#include "delay.h"

void _delay_ms(uint16_t time){
	uint32_t delay = (F_CPU / 1000) * time;
	while(delay--);
}
