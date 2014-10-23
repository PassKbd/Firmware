#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <discoveryf4_led.h>
#include <discoveryf4_button.h>
#include "buttonpad.h"
#include "hd44780.h"

int main(void)
{
	// Init DiscoveryF4 Buttons and LEDs
	DiscoveryF4_LED_init(DiscoveryF4_LED_All);
	DiscoveryF4_Button_init(DiscoveryF4_Button_GPIO);
	
	DiscoveryF4_LED_on(DiscoveryF4_LED_Blue);
	lcd_init();
	DiscoveryF4_LED_on(DiscoveryF4_LED_Green);
	
	while(1);
	
	Buttonpad_init();
	
	while (1){  
		Buttonpad_Event evt = Buttonpad_nextEvent();
		if (evt.type != Buttonpad_Event_None) {
			DiscoveryF4_LED_on(evt.key << 12);
		}
		if (DiscoveryF4_Button_isPressed()) {
			Buttonpad_clearEvent();
			DiscoveryF4_LED_off(DiscoveryF4_LED_All);
			while (DiscoveryF4_Button_isPressed());
		}
	}
}

// Toggle Red, Green and Orange LEDs on Timer (every 5s)
