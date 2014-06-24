#include <discoveryf4_led.h>

void DiscoveryF4_LED_init(uint32_t leds)
{
	/* Enable GPIO Port D peripheral */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure LED pins */
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if (leds & DiscoveryF4_LED_3) {
		GPIO_InitStructure.GPIO_Pin = DiscoveryF4_LED_3;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	
	if (leds & DiscoveryF4_LED_4) {
		GPIO_InitStructure.GPIO_Pin = DiscoveryF4_LED_4;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	
	if (leds & DiscoveryF4_LED_5) {
		GPIO_InitStructure.GPIO_Pin = DiscoveryF4_LED_5;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	
	if (leds & DiscoveryF4_LED_6) {
		GPIO_InitStructure.GPIO_Pin = DiscoveryF4_LED_6;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
}

void DiscoveryF4_LED_on(uint32_t leds)
{
	leds &= DiscoveryF4_LED_All;
	GPIOD->BSRRL = leds;
}

void DiscoveryF4_LED_off(uint32_t leds)
{
	leds &= DiscoveryF4_LED_All;
	GPIOD->BSRRH = leds;
}

void DiscoveryF4_LED_toggle(uint32_t leds)
{
	leds &= DiscoveryF4_LED_All;
	GPIOD->ODR ^= leds;
}
