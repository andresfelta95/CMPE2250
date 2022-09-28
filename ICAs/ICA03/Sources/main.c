/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        This B. You
// Details:       A more detailed explanation of the program is entered here
// Date:          Date Created
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
#include "PortJ.h"
#include "stdlib.h"
#include "stdio.h"

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
#define ICA03_PART_A
#define ICA03_PART_B
#define ICA03_PART_C
char name[] = "A. Tangarife";
char _Time[] = "Time:";
char _TimeF[] = "H:M:S:";
char _TimeFormat[] = {0};
char _TimeS[10] = {0};
unsigned int cntr = 0;
int _ActvStWtch = 0;
int _TimeCnt = 0;
int _TimeStpW = 0;
int _TimeMin = 0;
int _TimeHr = 0;
int _Flag = 0;
int _FlagJ = 0;
float _TimeSec = 0;
float _TimeCntF = 0;
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
  #ifdef ICA03_PART_A
  Clock_Set20MHZ(); // set the clock to 20 MHz
  PIT_Init();       // initialize the PIT
  lcd_Init();       // initialize the LCD
  SwLED_Init();     // initialize the switches and LEDs
  SevSeg_Init();    // initialize the 7-segment display
  PortJ_Init();     // initialize the port J
  // Set Timer at 50ms interval
  Timer_InitCH0(20E6, Timer_Prescale_32, 31250, 1, Timer_Pin_Toggle);
  // add Name to LCD
  lcd_StringXY(0, 0, name);
  // add Time to LCD
  lcd_StringXY(3, 0, _Time);
  // add Time Format to LCD
  lcd_StringXY(2, 0, _TimeF);
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // wait for interrupts
    asm wai;
    if(_Flag == 1)
    {
      _Flag = 0;
      // increment the counter
      if (cntr++ > 9999)
      {
        cntr = 0;
      }
      // increment Stopwatch
      if (_ActvStWtch)
      {
        // increment stopwatch every 50ms
        _TimeCnt += 1;
      }
    }
      // if flag is set increment counter
    // update the 7-segment display with the current count
    SevSeg_Top4(HexToBCD(cntr));
    // Convert count into seconds
    _TimeCntF = 0.05 * _TimeCnt;
    
    // Convert seconds into hours, minutes, and seconds
    _TimeSec = _TimeCntF;
    if (_TimeSec >= 60)
    {
      _TimeMin = _TimeSec / 60;
      _TimeSec = _TimeSec - (_TimeMin * 60);
      if (_TimeMin >= 60)
      {
        _TimeHr = _TimeMin / 60;
        _TimeMin = _TimeMin - (_TimeHr * 60);
        if (_TimeHr >= 24)
        {
          _TimeHr = 0;
        }
      }
    }

    // update the LCD with the current Stopwatch time
    (void)sprintf(_TimeS, "%5.1f", _TimeCntF);
    lcd_StringXY(3, 6, _TimeS);
    // update the LCD with the time in hours, minutes, and seconds in one line
    (void)sprintf(_TimeFormat, "%02d:%02d:%04.1f", _TimeHr, _TimeMin, _TimeSec);
    lcd_StringXY(2, 6, _TimeFormat);
    LED_Tog(LED_RED);
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines (ISR)
/////////////////////////////////////////////////////////////////////////////
//timer interrupt
interrupt VectorNumber_Vtimch0 void Timer0_ISR(void)
{
  // Set flag in main
  _Flag = 1;
  // clear the interrupt flag
  TFLG1 = TFLG1_C0F_MASK;
  // ream the next event
  TC0 += 31250;
}
// Port J interrupt
interrupt VectorNumber_Vportj void PortJ_ISR(void)
{
  // check if the interrupt is from the switch 1
  if (PIFJ_PIFJ1) // read of PIFJ (22.3.2.61)
  {
    // ack interrupt
    //PIFJ_PIFJ0 = 1; // can't do R/W clearing (preserves other bits, so clears other flags!)
    PIFJ = PIFJ_PIFJ1_MASK; // write only clear (just this ONE bit)
    // Check if the stopwatch is active and if it is, stop it
    if(!_ActvStWtch)
    {
      // start the stopwatch      
      _ActvStWtch = 1;
    }
    // show something happened:
    LED_Tog(LED_GREEN);
  }
  // check if the interrupt is from the switch 2
  if (PIFJ_PIFJ0)
  {
    if(_ActvStWtch)
    {
      // stop the stopwatch
      _ActvStWtch = 0;
    }
    else // reset the stopwatch
    {
      _TimeCnt = 0;
      _TimeCntF = 0;
      _TimeSec = 0;
      _TimeMin = 0;
      _TimeHr = 0;
    }
    // ack interrupt
    PIFJ = PIFJ_PIFJ0_MASK; // write only clear
    // show something happened:
    LED_Tog(LED_YELLOW);
  } 

}
#endif