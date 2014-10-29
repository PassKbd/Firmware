#include "delay.h"

void _delay_ms(uint32_t time){
	uint32_t delay = (F_CPU / 1000) * time;
	while(delay--);
}

void _delay_us(uint32_t time){
	uint32_t delay = (F_CPU / 1000000) * time;
	while(delay--);
}
