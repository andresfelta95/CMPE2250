// Timer Library
// Revision History:
// Created - Unknown
// Dec 2020 - Simplified function names, added prescale enum
// Jan 2022 - Added Timer_Init_ms for prescale/count automation

// not specifically useful, but documents code a little better
typedef enum Timer_Prescale
{
  Timer_Prescale_1 = 0,
  Timer_Prescale_2 = 1,
  Timer_Prescale_4 = 2,
  Timer_Prescale_8 = 3,
  Timer_Prescale_16 = 4,
  Timer_Prescale_32 = 5,
  Timer_Prescale_64 = 6,
  Timer_Prescale_128 = 7
} Timer_Prescale;

// actions pins can take on compare event
typedef enum Timer_PinAction
{
  Timer_Pin_Disco = 0,
  Timer_Pin_Toggle = 1,
  Timer_Pin_Clear = 2,
  Timer_Pin_Set = 3
} Timer_PinAction;

// timer general (uses OC0)
// prescale value as 2^N (0 - 7 as argument = bus / (1 to 128)) in general, 7 would be used (div by 128)
//  now uses enum, so division value is readily documented
// uiOffset is initial offset from TCNT (first event time)
// enableInt is flag for enabling interrupts on OC0
// pin action is action pin should take on event
void Timer_Init (unsigned long ulBusClock, Timer_Prescale prescale, unsigned int uiOffset, int enableInt, Timer_PinAction pinAction);

// this version will determine the prescale and return zero on error
// note, looks for perfect fit, unlike Sleep
// return >0 == offset for next arming.
unsigned int Timer_Init_ms (
  unsigned long ulBusClock,   // current bus rate (saved for Timer_Sleep use)
  unsigned int interval_ms,   // requested delay in ms
  int enableInt,              // enable interrupts?
  Timer_PinAction pinAction);  // event action on OC0 pin (9)

// blocking delay for x ms - requires previous InitTimer call
// calculates values required to generate 1ms delay (may not be perfectly accurate)
//  then loops ms times
// return error code (-1) on error
// uses OC6, no pin action for timing
int Timer_Sleep (unsigned int ms);

