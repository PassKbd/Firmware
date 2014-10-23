#include "hd44780.h"

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <discoveryf4_led.h>

GPIO_TypeDef* data_gpios[8] = { GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE };
// From D0 to D7
uint16_t data_pins[8] = { GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

// E, RS, RW
GPIO_TypeDef* ctrl_gpio[3] = { GPIOE, GPIOE, GPIOE };
uint16_t ctrl_pins[3] = { GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4 };

void data_as_output() {
	GPIO_InitTypeDef gpio;
	// Generic GPIO settings
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Initialize row GPIO
	for (int pin = 0; pin < 8; ++pin) {
		gpio.GPIO_Pin = data_pins[pin];
		GPIO_Init(data_gpios[pin], &gpio);
		// Set to high -> inactive row
		data_gpios[pin]->BSRRL = data_pins[pin];
	}
}

void lcd_outbyte(uint8_t out)
{	
	// EN to low
	ctrl_gpio[0]->BSRRH = ctrl_pins[0];
	
	// RW to low
	ctrl_gpio[2]->BSRRH = ctrl_pins[2];

	data_as_output();

	// Set Data
	for (int pin = 0; pin < 8; ++pin)
	{
		// Pin auf 0 resetten
		data_gpios[pin]->BSRRH = data_pins[pin];
		// Pin ggf. setzen
		if (out & 0x01) {
			data_gpios[pin]->BSRRL = data_pins[pin];
		}
		out >>= 1;
	}
	
	volatile uint32_t delay = 8400000;
	while (delay--);
	
	// EN to high
	ctrl_gpio[0]->BSRRL = ctrl_pins[0];	

	volatile uint32_t delay = 8400000;
	while (delay--);

	// EN to low
	ctrl_gpio[0]->BSRRH = ctrl_pins[0];	
}

void data_as_input() {
	GPIO_InitTypeDef gpio;
	// Generic GPIO settings
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Initialize row GPIO
	for (int pin = 0; pin < 8; ++pin) {
		gpio.GPIO_Pin = data_pins[pin];
		GPIO_Init(data_gpios[pin], &gpio);
		// Set to high -> inactive row
		data_gpios[pin]->BSRRL = data_pins[pin];
	}
}

uint8_t lcd_inbyte(void) {
	data_as_input();

	// RW to high
	ctrl_gpio[2]->BSRRL = ctrl_pins[2];
	
	// EN to high
	ctrl_gpio[0]->BSRRL = ctrl_pins[0];

	// Read Data
	uint8_t tmp;
	for (int pin = 0; pin < 8; ++pin)
	{
		if (GPIO_ReadInputDataBit(data_gpios[pin], data_pins[pin]) == 1) {
			tmp |= 0x80;
		}
		tmp >>= 1;
	}
	
	// EN to low
	ctrl_gpio[0]->BSRRH = ctrl_pins[0];	
}

uint8_t lcd_isBusy() {
	uint8_t tmp = lcd_inbyte();
	if (tmp & 0x80) {
		return 1;
	}
	return 0;
}

void lcd_selectData() {
	// RS to high
	ctrl_gpio[1]->BSRRL = ctrl_pins[1];
}

void lcd_selectInstruction() {
	// RS to low
	ctrl_gpio[1]->BSRRH = ctrl_pins[1];
}

void lcd_command(uint8_t command) {
	lcd_selectInstruction();
	lcd_outbyte(command);
	
	volatile  uint32_t delay = 16800000;
	while (delay--);
}

void lcd_data(uint8_t data) {
	lcd_selectData();
	lcd_outbyte(data);

	volatile uint32_t delay = 16800000;
	while (delay--);
}

void lcd_init()
{
	// Enable GPIO peripherals
	// Change accordingly to header
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// Initialize control pins	
	GPIO_InitTypeDef gpio;
	// Generic GPIO settings
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	
	for (int pin = 0; pin < 3; ++pin) {
		gpio.GPIO_Pin = ctrl_pins[pin];
		GPIO_Init(ctrl_gpio[pin], &gpio);		
		// Initialize to low
		ctrl_gpio[pin]->BSRRH = ctrl_pins[pin];
	}
	
	// Initialize display!
	lcd_command( LCD_SET_FUNCTION |
                 LCD_FUNCTION_8BIT |
                 LCD_FUNCTION_2LINE |
                 LCD_FUNCTION_5X7 );
 
    // Display ein / Cursor aus / Blinken aus
    lcd_command( LCD_SET_DISPLAY |
                 LCD_DISPLAY_ON |
                 LCD_CURSOR_ON |
                 LCD_BLINKING_ON); 
 
    // Cursor inkrement / kein Scrollen
    lcd_command( LCD_SET_ENTRY |
                 LCD_ENTRY_INCREASE |
                 LCD_ENTRY_NOSHIFT );
}

/*
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
*/
