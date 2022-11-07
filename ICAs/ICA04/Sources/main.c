/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       PIT Interrupt Service Routine
// Date:          10/26/2022
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
void ShowCount(void); //Show PIT Count
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile unsigned int pitCnt = 0;
volatile unsigned int timCnt = 0;
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
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  Clock_Set20MHZ();                           // set the clock to 20 MHz
  Timer_InitCH0(20000000, Timer_Prescale_64, 3125, 1, Timer_Pin_Toggle); // initialize timer channel 0
  (void)PIT_Init0(10000); //Initialize PIT0 with 10ms interval
  SwLED_Init();                               // initialize the switches and LEDs
  SevSeg_Init();                              // initialize the 7-segment display
  PortJ_Init(); 
  ShowCount();
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    #ifdef PART_A
    asm wai; //wait for interrupt
    //The delay between the PIT and the timer is 146us
    //The difference between the two is that the PIT has a priority over the timer
    #endif  //PART_A
    #ifdef PART_B
    //asm sei; //enable interrupts
    //Timer_Sleep_ms(20); //Sleep for 20ms
    //asm cli; //disable interrupts
    // Answers:
    // The delay betweem PIT and Timer was >5 seconds.
    // 1. The PIT should occur first and it can be proved by the fact that 
    //    the PIT count is incremented first.
    // 2. We can prove it checking waveforms from the LEDs and verifying 
    //    the time between the Yellow LED and the Green LED.
    // 3. The reason is because of the blocking delay the timer skips some cycles and the PIT is not affected by this.
    #endif  //PART_B
    
    ShowCount(); // display the PIT count on the 7-segment display
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void ShowCount(void)
{
  //  display the Timer count on the 7-segment display top row
  SevSeg_Top4(HexToBCD(timCnt));
  //  display the PIT count on the 7-segment display bottom row
  SevSeg_Bot4(HexToBCD(pitCnt));
}
/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vtimch0 void IOC0 (void)
{
  // interrupt service routine for channel 0
  TC0 += 3125; // set the next interrupt time
  #ifdef PART_C
  TC0 = TCNT + 3125; // set the next interrupt time
  //  Answers:
  //  1. The PIT keeps occuring first but the difference is smaller.
  //     However, at the beginning the diference is smaller and then it increases.
  //  2. If it is set in the first line it helps to keep the rearm time of the timer closer 
  //     to the PIT.
  #endif  //PART_C
  // take action!
  if (timCnt++ > 9999)
  {
    timCnt = 0;
    LED_Tog(LED_YELLOW); // toggle the yellow LED
  }
}
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action!
  if (pitCnt++ > 9999)
  {
    pitCnt = 0;
    // disable the PIT
    // PITCE = 0;
    LED_Tog(LED_GREEN); // toggle the green LED
  }
}