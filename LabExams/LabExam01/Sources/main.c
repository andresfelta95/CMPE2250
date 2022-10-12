/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       Lab Exam 01
// Date:          10/05/2022
// Revision History :

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

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
int _Pj1Flag = 0;             // Flag for PJ1
int _Pj0Flag = 0;             // Flag for PJ0
int _GCntr = 10;              // Global Counter
int _TimeCnt = 0;             // Time Counter
int _LedCase = 0;             // LED Case
int _LockoutSw = 0;           // Lockout Switch
int _LockCnt = 0;             // Lockout Counter
char name[] = "A. Tangarife"; // Name
char _DecHex[18] = {0};       // Decimal and Hex values to display
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

  // Initilize elements
  Clock_Set20MHZ();                           // set the clock to 20 MHz
  PIT_Init();                                 // initialize the PIT
  lcd_Init();                                 // initialize the LCD
  SwLED_Init();                               // initialize the switches and LEDs
  SevSeg_Init();                              // initialize the 7-segment display
  PortJ_Init();                               // initialize the port J
  SevSeg_Top4(HexToBCD(_GCntr));              // Display the counter
  SevSeg_Bot4(HexToBCD(_TimeCnt));            // Display the Timer Counter
  lcd_DispControl(0, 0);                      // Turn off the cursor
  lcd_StringXY(0, (21 - sizeof(name)), name); // Display the name in the first line
  // Create a string with the decimal and hex values of the Time Counter
  (void)sprintf(_DecHex, "Dec:%04d,Hex:%04X", HexToBCD(_TimeCnt), BCDToHex(_TimeCnt));
  lcd_StringXY(1, 0, _DecHex); // Display the string in the second line
  // Set Timer ch0 at 240ms interval
  Timer_InitCH0(20E6, Timer_Prescale_128, 37500, 1, Timer_Pin_Toggle);
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    asm wai; // wait for an interrupt
    // Check for pj1 flag
    if (_Pj1Flag)
    {
      _Pj1Flag = 0;      // Reset flag
      if (_GCntr-- <= 0) // Check if counter is less than 0
      {
        _GCntr = 9999; // Reset counter
      }
    }
    // If the flag0 is set then display the counter in the bottom 4 digits
    if (_Pj0Flag)
    {
      SevSeg_Top4(HexToBCD(_TimeCnt)); // Display the Timer Counter into the top 4 digits
      SevSeg_Bot4(HexToBCD(_GCntr));   // Display the counter in the bottom 4 digits
    }
    else
    {
      SevSeg_Bot4(HexToBCD(_TimeCnt)); // Display the Timer Counter into the bottom 4 digits
      SevSeg_Top4(HexToBCD(_GCntr));   // Display 0 in the top 4 digits
    }
    // Create a counter to lockout the switch
    if (_LockoutSw)
    {
      if (_LockCnt++ >= 1)
      {
        _LockCnt = 0;   // Reset the counter
        _LockoutSw = 0; // Reset the lockout switch
      }
    }
    // Check for the LED case
    switch (_LedCase)
    {
    case 0:               // Case 0
      LED_Off(LED_GREEN); // Turn off the green LED
      LED_On(LED_RED);    // Turn on the red LED
      break;
    case 1:               // Case 1
      LED_Off(LED_RED);   // Turn off the red LED
      LED_On(LED_YELLOW); // Turn on the green LED
      break;
    case 2:                // Case 2
      LED_Off(LED_YELLOW); // Turn off the yellow LED
      LED_On(LED_GREEN);   // Turn on the green LED
      break;
    }
    // Create a string with the decimal and hex values of the Time Counter
    (void)sprintf(_DecHex, "Dec:%04d,Hex:%04X", HexToBCD(_TimeCnt), BCDToHex(_TimeCnt));
    lcd_StringXY(1, 0, _DecHex); // Display the string in the second line
  }
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
// timer interrupt
interrupt VectorNumber_Vtimch0 void Timer0_ISR(void)
{
  // Increase LED case at a maximum of 2
  if (_LedCase++ >= 2)
  {
    _LedCase = 0;
  }
  // Increase the time counter at a maximum of 9999
  if (_TimeCnt++ >= 9999)
  {
    _TimeCnt = 0;
  }
  // clear the interrupt flag
  TFLG1 = TFLG1_C0F_MASK;
  // ream the next event
  TC0 += 37500;
}
// Port J Interrupt
interrupt VectorNumber_Vportj void PortJ_ISR(void)
{
  // Check if PJ1 is pressed and the lockout switch is not active
  if (PIFJ_PIFJ1)
  {
    PIFJ_PIFJ1 = 1;  // Clear the interrupt flag
    if (!_LockoutSw) // Check if the lockout switch is not active
    {
      _Pj1Flag = 1;   // Set the flag
      _LockoutSw = 1; // Set the lockout switch
    }
  }

  // Check if PJ0 is pressed and the lockout switch is not active
  if (PIFJ_PIFJ0)
  {
    PIFJ = PIFJ_PIFJ0_MASK; // write only clear (just this ONE bit)
    if (!_LockoutSw)        // Check if the lockout switch is not active
    {
      _Pj0Flag = !_Pj0Flag; // Toggle the flag
      _LockoutSw = 1;       // Set the lockout switch
    }
  }
}
