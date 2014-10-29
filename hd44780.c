#include "hd44780.h"

// From D0 to D7
GPIO_TypeDef* data_gpios[8] = { GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE };
uint16_t data_pins[8] = { GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

// E, RS, RW
GPIO_TypeDef* ctrl_gpio[3] = { GPIOE, GPIOE, GPIOE };
uint16_t ctrl_pins[3] = { GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4 };

static inline void lcd_EN_low(){
	ctrl_gpio[0]->BSRRH = ctrl_pins[0];
}

static inline void lcd_EN_high(){
	ctrl_gpio[0]->BSRRL = ctrl_pins[0];
}

static inline void lcd_RW_low(){
	ctrl_gpio[2]->BSRRH = ctrl_pins[2];
}

static inline void lcd_RW_high(){
	ctrl_gpio[2]->BSRRL = ctrl_pins[2];
}

static inline void lcd_EN_toggle(){
	lcd_EN_high();
	
	_delay_us(200);//TODO: timing ok?
	
	lcd_EN_low();
}

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

void lcd_outbyte(uint8_t out){
	lcd_EN_low();
	lcd_RW_low();
	
	data_as_output();
	
	// Set Data
	for (int pin = 0; pin < 8; ++pin) {
		// Pin auf 0 resetten
		data_gpios[pin]->BSRRH = data_pins[pin];
		// Pin ggf. setzen
		if (out & 0x01) {
			data_gpios[pin]->BSRRL = data_pins[pin];
		}
		out >>= 1;
	}
	
	_delay_ms(2);//TODO: timing ok?
	
	lcd_EN_high();
	
	_delay_ms(2);//TODO: timing ok?
	
	lcd_EN_low();
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
		
		data_gpios[pin]->BSRRL = data_pins[pin];//Set to high -> inactive row
	}
}

uint8_t lcd_inbyte(void) {
	data_as_input();
	
	lcd_RW_high();
	lcd_EN_high();
	
	// Read Data
	uint8_t tmp = 0;
	for (int pin = 0; pin < 8; ++pin) {
		if (GPIO_ReadInputDataBit(data_gpios[pin], data_pins[pin]) == 1) {
			tmp |= 0x80;
		}
		tmp >>= 1;
	}
	
	lcd_EN_low();
}

uint8_t lcd_isBusy() {
	uint8_t tmp = lcd_inbyte();
	if (tmp & 0x80) {
		return 1;
	}
	return 0;
}

void lcd_selectData() {
	ctrl_gpio[1]->BSRRL = ctrl_pins[1];//RS to high
}

void lcd_selectInstruction() {
	ctrl_gpio[1]->BSRRH = ctrl_pins[1];//RS to low
}

void lcd_command(uint8_t command) {
	lcd_selectInstruction();
	lcd_outbyte(command);
	
	_delay_ms(2);//TODO: timing ok?
}

void lcd_data(uint8_t data) {
	lcd_selectData();
	lcd_outbyte(data);
	
	_delay_ms(2);//TODO: timing ok?
}

void lcd_init() {
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
		
		ctrl_gpio[pin]->BSRRH = ctrl_pins[pin];//Initialize to low
	}
	
	_delay_ms(20);//wait 16ms or more after power-on
	
	//begin init sequence
	data_as_output();
	
	uint8_t out = LCD_INIT_CMD;
	for (int pin = 0; pin < 8; ++pin) {
		// Pin auf 0 resetten
		data_gpios[pin]->BSRRH = data_pins[pin];
		// Pin ggf. setzen
		if (out & 0x01) {
			data_gpios[pin]->BSRRL = data_pins[pin];
		}
		out >>= 1;
	}
	
	lcd_EN_toggle();
	_delay_ms(5);
	
	lcd_EN_toggle();
	_delay_ms(5);
	
	lcd_EN_toggle();
	_delay_ms(5);
	
	//end init sequence
	
	// configure display!
	lcd_command(LCD_SET_FUNCTION |
		  LCD_FUNCTION_8BIT |
		  LCD_FUNCTION_2LINE |
		  LCD_FUNCTION_5X7 );
	
	// Display ein / Cursor aus / Blinken aus
	lcd_command(LCD_SET_DISPLAY |
		  LCD_DISPLAY_ON |
		  LCD_CURSOR_ON |
		  LCD_BLINKING_ON); 
	
	// Cursor inkrement / kein Scrollen
	lcd_command(LCD_SET_ENTRY |
		  LCD_ENTRY_INCREASE |
		  LCD_ENTRY_NOSHIFT );
}