#include "main.h"

int main(void) {
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