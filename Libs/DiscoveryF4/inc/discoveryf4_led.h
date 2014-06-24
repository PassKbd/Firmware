#ifndef __DISCOVERYF4_LED_H__
#define __DISCOVERYF4_LED_H__

#include <stm32f4xx.h>

typedef enum {
	DiscoveryF4_LED_All = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
	DiscoveryF4_LED_3 = GPIO_Pin_13,
	DiscoveryF4_LED_4 = GPIO_Pin_12,
	DiscoveryF4_LED_5 = GPIO_Pin_14,
	DiscoveryF4_LED_6 = GPIO_Pin_15,
	DiscoveryF4_LED_Orange = DiscoveryF4_LED_3,
	DiscoveryF4_LED_Green = DiscoveryF4_LED_4,
	DiscoveryF4_LED_Red = DiscoveryF4_LED_5,
	DiscoveryF4_LED_Blue = DiscoveryF4_LED_6
} DiscoveryF4_LED;

void DiscoveryF4_LED_init(uint32_t leds);
void DiscoveryF4_LED_on(uint32_t leds);
void DiscoveryF4_LED_off(uint32_t leds);
void DiscoveryF4_LED_toggle(uint32_t leds);

#endif
