#ifndef __DISCOVERYF4_BUTTON_H__
#define __DISCOVERYF4_BUTTON_H__

typedef enum {
	DiscoveryF4_Button_GPIO = 0,
	DiscoveryF4_Button_INT = 1
} DiscoveryF4_Button_Mode;

void DiscoveryF4_Button_init(DiscoveryF4_Button_Mode mode);
int DiscoveryF4_Button_isPressed();

#endif
