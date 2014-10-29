#ifndef __DELAY_H__
#define __DELAY_H__

#ifndef F_CPU
#error "CPU frequency not defined(F_CPU)"
#endif

#include <stm32f4xx.h>

void _delay_ms(uint32_t time);
void _delay_us(uint32_t time);

#endif