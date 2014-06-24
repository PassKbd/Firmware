#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <discoveryf4_led.h>
#include <discoveryf4_button.h>

int main(void)
{
	// Init DiscoveryF4 Buttons and LEDs
	DiscoveryF4_LED_init(DiscoveryF4_LED_All);
	DiscoveryF4_Button_init(DiscoveryF4_Button_GPIO);
	
	// Init Timer
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Configure Timer for 5s Ticks
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 9999;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 41999;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);
	 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// Enable Timer interrupt on lowest priority
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Init(&NVIC_InitStructure);
	 
	TIM_Cmd(TIM2, ENABLE);
	
	
	// Toggle Blue LED on Button press
	while (1){  
		while(!DiscoveryF4_Button_isPressed());
		DiscoveryF4_LED_toggle(DiscoveryF4_LED_Blue);
		while(DiscoveryF4_Button_isPressed());	
	}
}

// Toggle Red, Green and Orange LEDs on Timer (every 5s)
void TIM2_IRQHandler(void){
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	DiscoveryF4_LED_toggle(DiscoveryF4_LED_Red | DiscoveryF4_LED_Green | DiscoveryF4_LED_Orange);
}
