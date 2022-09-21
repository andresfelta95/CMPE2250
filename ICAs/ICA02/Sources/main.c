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
  Clock_Set20MHZ();
  PIT_Init();  
  lcd_Init();
  SwLED_Init();
  SevSeg_Init();
  //init timer
  Timer_InitCH0(20E6, Timer_Prescale_32, 3125, 1, Timer_Pin_Toggle);
  // basic display
  lcd_StringXY(0,0,_ticks);
  lcd_StringXY(1,0,_loop);
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // wait for timer interrupt
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

    Timer_SleepCounts(31250);
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

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
  TC0 += 3125;
}