/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       ICA02
// Date:          09/18/2020
// Revision History :
//  each revision will have a date + desc. of changes

/////////////////////////////////////////////////////////////////////////////
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

// other system includes or your includes go here
#include "lcd.h"
#include "LEDSW.h"
#include "Clock.h"
#include "ECT.h"
#include "SevSeg.h"
#include "stdlib.h"
#include "stdio.h"

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////
void _LCD(void);
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
char name[] = "A. Tangarife";
char _ticks[] = "Ticks:";
char _loop[] = "Loops:";
char _cnt1[10] = {0};
char _cnt2[10] = {0};
unsigned long loop = 0;
unsigned int ticks = 0;
/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
// #ICA02Part1
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PIT_Init();
  RTI_Init(&_LCD);
  lcd_Init();
  SwLED_Init();
  SevSeg_Init();
  Clock_Set20MHZ();
  //init timer
  Timer_Init(Timer_Prescale_2);
  //set chn0 as output
  TimerCH_EnableAsOutput(Timer_CH0_BIT);
  TimerCH_IntEnable(Timer_CH0_BIT);

  /*****************************************
  Compare result Action - TCTL1/TCTL2
  OMx   OLx
  0     0  Timer Disconnected from pin
  0     1  Toggle OCx output line
  1     0  Clear OCx output lize to zero
  1     1  Set OCx output line to one
  *****************************************/
  TCTL2_OM0 = 0;  
  TCTL2_OL0 = 1;

  TC0 = TCNT + 50000;

  // basic display
  lcd_StringXY(0,0,_ticks);
  lcd_StringXY(1,0,_loop);
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    asm wai;
    //toggle green led
    LED_Tog(LED_GREEN);
    // increment loop counter
    loop++;
    // display name
    lcd_StringXY(3,0,name);
    // display ticks counter
    (void)sprintf(_cnt2,"%4d",ticks);
    lcd_StringXY(0,16,_cnt2);
    // display loop counter
    (void)sprintf(_cnt1,"%5ld",loop);
    lcd_StringXY(1,15,_cnt1);
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void _LCD(void){
  
}
/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vtimch0 void IOC0 (void)
{
  //increment ticks
  ticks++;

  // clear the interrupt flag
  TFLG1 = TFLG1_C0F_MASK;
  // ream for the next event
  TC0 += 50000;
}