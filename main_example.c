#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_gpio.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

int main(void) {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  while (1){  
	GPIOD->BSRRL = GPIO_Pin_14;
	Delay(8000000);
	GPIOD->BSRRH = GPIO_Pin_14;
	Delay(8000000);
  }
}
