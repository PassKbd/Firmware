#include "buttonpad.h"

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <discoveryf4_led.h>

GPIO_TypeDef* row_gpios[4] = { GPIOD, GPIOD, GPIOD, GPIOD };
uint16_t row_pins[4] = { GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11 };

GPIO_TypeDef* column_gpios[4] = { GPIOC, GPIOC, GPIOB, GPIOB };
uint16_t column_pins[4] = { GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9 };

volatile Buttonpad_Event events[BUTTONPAD_EVENTS];

uint16_t currentReadEvent = 0;
uint16_t currentWriteEvent = 0;

uint16_t keyReleaseCounter[16];
uint16_t keyPressedCounter[16];

void Buttonpad_init()
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic;

	// Enable GPIO peripherals
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// Generic GPIO settings
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Initialize row GPIO
	for (int row = 0; row < 4; ++row) {
		gpio.GPIO_Pin = row_pins[row];
		GPIO_Init(row_gpios[row], &gpio);
		// Set to high -> inactive row
		row_gpios[row]->BSRRL = row_pins[row];
	}
	
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	
	for (int col = 0; col < 4; ++col) {
		gpio.GPIO_Pin = column_pins[col];
		GPIO_Init(column_gpios[col], &gpio);
	}
	
	// Initialize Timer	  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Configure Timer for 5s Ticks
	timer.TIM_ClockDivision = TIM_CKD_DIV2;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = (BUTTONPAD_TIMER_PERIOD - 1);
	timer.TIM_Prescaler = (BUTTONPAD_TIMER_PRESCALER - 1);
	TIM_TimeBaseInit(TIM2, &timer);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// Initialize Timer interrupt on medium-low priority
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x08;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Init(&nvic);
	 
	// Initialize event buffer
	for (int i = 0; i < BUTTONPAD_EVENTS; ++i) {
		events[i].type = Buttonpad_Event_None;
	}
	 
	// And go!
	TIM_Cmd(TIM2, ENABLE);
	
}

Buttonpad_Event Buttonpad_nextEvent()
{
	return events[currentReadEvent];
}

Buttonpad_Event Buttonpad_waitEvent()
{
	while (1) {
		Buttonpad_Event event = Buttonpad_nextEvent();
		if (event.type != Buttonpad_Event_None) {
			Buttonpad_clearEvent();
			return event;
		}
	}
}


void Buttonpad_clearEvent()
{
	if (events[currentReadEvent].type != Buttonpad_Event_None) {
		events[currentReadEvent].type = Buttonpad_Event_None;
		currentReadEvent = (currentReadEvent + 1) & (BUTTONPAD_EVENTS - 1);
	}
}


void TIM2_IRQHandler(void)
{
	static int row = 3;
	
	// Change to next row
	row = (row + 1) & 3;
	
	// Activate row
	row_gpios[row]->BSRRH = row_pins[row];
	
	// Check each column in active row
	for (int col = 0; col < 4; ++col) {
		unsigned char key_index = (row << 2) + col;
		if (GPIO_ReadInputDataBit(column_gpios[col], column_pins[col]) == 0) {
			++keyPressedCounter[key_index];
			keyReleaseCounter[key_index] = 0;
		}
		else {
			++keyReleaseCounter[key_index];
		}
		if (keyReleaseCounter[key_index] > BUTTONPAD_SHORT_THRESHOLD &&
			events[currentWriteEvent].type == Buttonpad_Event_None) 
		{
			if (keyPressedCounter[key_index] >= BUTTONPAD_SHORT_THRESHOLD) {
				events[currentWriteEvent].key = key_index;
				events[currentWriteEvent].type = Buttonpad_Event_ShortPress;
				++currentWriteEvent;
			} 
			else if (keyPressedCounter[key_index] >= BUTTONPAD_LONG_THRESHOLD) {
				events[currentWriteEvent].key = key_index;
				events[currentWriteEvent].type = Buttonpad_Event_LongPress;
				++currentWriteEvent;
			}
			keyPressedCounter[key_index] = 0;
			currentWriteEvent &= (BUTTONPAD_EVENTS - 1);
		}
	}
	
	// Disable row
	row_gpios[row]->BSRRL = row_pins[row];

	// Clear interrupt
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
