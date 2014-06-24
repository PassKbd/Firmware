#ifndef __BUTTONPAD_H__
#define __BUTTONPAD_H__

#include <stm32f4xx_tim.h>

// Configure Timer Settings and Threshold values here

// Timer Clock Prescaler
#define BUTTONPAD_TIMER_PRESCALER 42000

// Timer Clock Ticks
#define BUTTONPAD_TIMER_PERIOD 20

// short press threshold in ms
#define BUTTONPAD_SHORT_THRESHOLD_MS 200

// long press threshold in ms
#define BUTTONPAD_LONG_THRESHOLD_MS 2000

// event buffer length power -> 2^n events are stored
#define BUTTONPAD_EVENTS_POWER 2

// --------------------------
// NO changes after this line

typedef enum {
	Buttonpad_Key_1 = 0,
	Buttonpad_Key_2,
	Buttonpad_Key_3,
	Buttonpad_Key_A,
	Buttonpad_Key_4,
	Buttonpad_Key_5,
	Buttonpad_Key_6,
	Buttonpad_Key_B,
	Buttonpad_Key_7,
	Buttonpad_Key_8,
	Buttonpad_Key_9,
	Buttonpad_Key_C,
	Buttonpad_Key_Star,
	Buttonpad_Key_0,
	Buttonpad_Key_Hash,
	Buttonpad_Key_D
} Buttonpad_Key;

typedef enum {
	Buttonpad_Event_None,
	Buttonpad_Event_ShortPress,
	Buttonpad_Event_LongPress
} Buttonpad_EventType;

typedef struct {
	Buttonpad_Key key;
	Buttonpad_EventType type;
} Buttonpad_Event;

// ms for once checking the whole button pad (4 rows * time per row)
#define BUTTONPAD_TIMER_PERIOD_MS (4 * (1000 / (84000000 / 2 / BUTTONPAD_TIMER_PRESCALER / BUTTONPAD_TIMER_PERIOD)))

// threshold values. might be inaccurate due to rounding errors
#define BUTTONPAD_SHORT_THRESHOLD (BUTTONPAD_SHORT_THRESHOLD_MS / BUTTONPAD_TIMER_PERIOD_MS)
#define BUTTONPAD_LONG_THRESHOLD (BUTTONPAD_LONG_THRESHOLD_MS / BUTTONPAD_TIMER_PERIOD_MS)

// event buffer length
#define BUTTONPAD_EVENTS (1 << BUTTONPAD_EVENTS_POWER)

void Buttonpad_init();
Buttonpad_Event Buttonpad_nextEvent();
Buttonpad_Event Buttonpad_waitEvent();
void Buttonpad_clearEvent();

#endif
