/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       In this lab you will use the timer module to create a timer that supports calibration.
//                You will additionally explore some ancillary issues with switch debounce,
//                and program execution time
//                you will show elapsed time as DDDDD HH:MM:SS:h since reset
// Date:          11/07/2022
// Revision History :
//  each revision will have a date + desc. of changes

/////////////////////////////////////////////////////////////////////////////
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

// other system includes or your includes go here
#include "lcd.h"    //LCD
#include "LEDSW.h"  //LEDs and Switches
#include "Clock.h"  //Clock
#include "ECT.h"    //Enhanced Capture Timer
#include "SevSeg.h" //Seven Segment
#include "PortJ.h"  //Port J
#include "stdlib.h" //Standard Library
#include "stdio.h"  //Standard I/O
#include "pit.h"    //PIT

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////
void Timer_ConvertToClockVals(unsigned long ulTimeVal);
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
typedef struct SClockVals
{
  int iThow;          // thousandths of a second 0-999
  char cSec;          // seconds 0-59
  char cMin;          // minutes 0-59
  char cHour;         // hours 0-23
  unsigned int uiDay; // days 0-9999
} SClockVals;
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  // other initialization code goes here
  Clock_Set20MHZ(); // Set the clock to 20 MHz
  Timer_InitCH0(20E6, Timer_Prescale_1, 2000, 1, 1);  // 1ms
  lcd_Init(); //LCD
  
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
  }
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
// Function:    Timer_ConvertToClockVals
// Description: This function converts the timer value to a clock value
// Inputs:      unsigned long ulTimerVal - the timer value
// Outputs:     SClockVals - the clock values
void Timer_ConvertToClockVals(unsigned long ulTimerVal)
{
  SClockVals sClockVals;
  unsigned long ulTemp;
  ulTemp = ulTimerVal / 1000;
  sClockVals.iThow = ulTimerVal - (ulTemp * 1000);
  ulTemp = ulTemp / 60;
  sClockVals.cSec = ulTemp - (ulTemp * 60);
  ulTemp = ulTemp / 60;
  sClockVals.cMin = ulTemp - (ulTemp * 60);
  ulTemp = ulTemp / 24;
  sClockVals.cHour = ulTemp - (ulTemp * 24);
  sClockVals.uiDay = ulTemp;
  return sClockVals;
}
/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
