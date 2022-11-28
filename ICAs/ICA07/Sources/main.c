/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       The goal for your program is to display a repeating pattern of ‘a’ through ‘z’ then repeat, 
//                terminating each sequence with a carriage return + line feed pair. 
//                Every 1s you will toggle the case of the output. 
//                In other words, you will see a repeating pattern of ‘a’ through ‘z’ for 1s, 
//                then a repeating pattern of ‘A’ through ‘Z’ for 1s, and repeat.
//                Because the output will be saturated at 9600 BAUD,
//                the point at which the case flips will be ‘random’
// Date:          Date Created
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
#include "sci.h"    //SCI

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile unsigned char gCase = 0; // 0 = lower, 1 = upper
volatile unsigned char gChar = 'a'; // current character to print
volatile unsigned int gCount = 0; // count of characters printed
volatile unsigned int gPITCount = 0; // count of PIT interrupts
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
  Clock_Set20MHZ(); //Set Clock to 20MHz
  (void)sci0_Init(20E6, 9600, 0); //Initialize SCI0
  SevSeg_Init(); //Initialize Seven Segment
  Timer_Init(Timer_Prescale_64); //Initialize Timer
  (void)PIT_Init0(100000);//Initialize PIT0 to 100ms
  lcd_Init(); //Initialize LCD

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    if(gPITCount == 10) //If 1s has passed
    {
      gPITCount = 0; //Reset PIT Count
      gCase = !gCase; //Toggle Case
    }

    if(gCase == 0) //If Lower Case
    {
      sci0_OutChar(gChar); //Print Character
      gChar++; //Increment Character
      if(gChar > 'z') //If Character is 'z'
      {
        gChar = 'a'; //Reset Character
        sci0_OutChar(0x0D); //Print Carriage Return
        sci0_OutChar(0x0A); //Print Line Feed
      }
    }
    else //If Upper Case
    {
      sci0_OutChar(gChar); //Print Character
      gChar++; //Increment Character
      if(gChar > 'Z') //If Character is 'Z'
      {
        gChar = 'A'; //Reset Character
        sci0_OutChar(0x0D); //Print Carriage Return
        sci0_OutChar(0x0A); //Print Line Feed
      }
    }
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only
  gPITCount++; //Increment PIT Count
}