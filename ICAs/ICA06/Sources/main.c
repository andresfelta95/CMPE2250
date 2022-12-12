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
#include "lcd.h"    //LCD
#include "LEDSW.h"  //LEDs and Switches
#include "Clock.h"  //Clock
#include "ECT.h"    //Enhanced Capture Timer
#include "SevSeg.h" //Seven Segment
#include "PortJ.h"  //Port J
#include "stdlib.h" //Standard Library
#include "stdio.h"  //Standard I/O
#include "pit.h"    //PIT
#include "pwmLib.h" //PWM Library

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////
void PWM_Ch3(void); //PWM Channel 3
void PWM_RGB(void); //Use PWM to control RGB LED
void LCD_Display(void); //LCD Display
void Part_A(void); //Part A
void Part_B(void); //Part B
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile int iDuty = 200;
volatile int iRDuty = 255;
volatile int iGDuty = 255;
volatile int iBDuty = 255;
volatile int iPitCount = 0;
volatile int flag = 0;
volatile int iSec = 0;
volatile int iSeq = 0;
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
SwState upSwitch; //up Switch
SwState downSwitch; //down Switch
SwState leftSwitch; //left Switch
SwState rightSwitch; //right Switch
SwState centerSwitch; //center Switch
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
  (void)PIT_Inits(100000, PIT0, 1);           // initialize the PIT0
  Timer_Init(Timer_Prescale_64);              // initialize the timer
  lcd_Init();                                 // initialize the LCD
  PWM_Ch3();                                  // initialize PWM channel 3
  lcd_DispControl(0,0);                       // turn off the cursor
  SwLED_Init();                               // initialize the LEDs and switches.
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    asm WAI; // wait for interrupt
    Sw_Process(&upSwitch, SW_UP); //Process up switch
    Sw_Process(&downSwitch, SW_DOWN); //Process down switch
    Sw_Process(&leftSwitch, SW_LEFT); //Process left switch
    Sw_Process(&rightSwitch, SW_RIGHT); //Process right switch
    Sw_Process(&centerSwitch, SW_MID); //Process center switch
    Part_A(); //Part A
    Part_B(); //Part B
    PWM_Ch3(); //PWM Channel 3
    PWM_RGB(); //Use PWM to control RGB LED
    LCD_Display(); //Display on LCD
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void PWM_Ch3(void)
{
  // channel 3 is a B channel!
  // set Clock B to divide by 2 (8.3.2.4) 
  PWMPRCLK &= 0b10001111; 
  PWMPRCLK |= 0b00010000; // 2^1 = 2 (upper nibble for B) 
  // set Clock SB to divide by 50 (8.3.2.10) 
  PWMSCLB = 25; // remember: it does x 2 
  // select Clock SB for channel 2 (8.3.2.3) 
  PWMCLK_PCLK3 = 1; // 1 means use clock SB 
  // set polarity as start high, go low (does not matter here) (8.3.2.2) 
  PWMPOL_PPOL3 = 1; // program period (8.3.2.13) 
  PWMPER3 = 200; // program duty (8.3.2.14) 
  PWMDTY3 = iDuty; // half period, so 50% duty 
  // enable channel (8.3.2.1) 
  PWME_PWME3 = 1;
}
void PWM_RGB(void)
{
  // channel 0, 1 and 4 are A channels!
  // set Clock A to divide by 16
  PWMPRCLK &= 0b11111000; // clear bits 0, 1, 2
  PWMPRCLK |= PWM_Channel_A_Two; // 2^1 = 2 (lower nibble for A) 
  // set Clock SB to divide by 50  
  PWMSCLA = 4; // remember: it does x 2 
  // select Clock SB for channel 2  
  PWMCLK_PCLK0 = 1; // 1 means use clock SB
  PWMCLK_PCLK1 = 1; // 1 means use clock SB
  PWMCLK_PCLK4 = 1; // 1 means use clock SB 
  // set polarity as start high, go low (does not matter here)  
  PWMPOL_PPOL0 = 1; // program period
  PWMPOL_PPOL1 = 1; // program period 
  PWMPOL_PPOL4 = 1; // program period 
  PWMPER0 = 255; // program period  
  PWMPER1 = 255; // program period 
  PWMPER4 = 255; // program period 
  PWMDTY0 = iBDuty; // Blue duty
  PWMDTY1 = iGDuty; // Green duty
  PWMDTY4 = iRDuty; // Red duty
  // enable channels
  PWME_PWME0 = 1;
  PWME_PWME1 = 1;
  PWME_PWME4 = 1;
}
void LCD_Display(void)
{
  char sCount[18]; //string count
  char sRGB[20]; //string RGB
  (void)sprintf(sCount, "Count: %04d", iPitCount); //print count
  (void)sprintf(sRGB, "R:%03d G:%03d B:%03d", iRDuty, iGDuty, iBDuty); //print RGB
  lcd_StringXY(3, 20-strlen(sCount), sCount); //display count
  lcd_StringXY(0, 0, sRGB); //display RGB
}
void Part_A(void)
{  
  //   check if switch is Released
  if (upSwitch == Released || downSwitch == Released || leftSwitch == Released || rightSwitch == Released || centerSwitch == Released)
  {
    //   Set flag to 0
    flag = 0;
    //   Set iSec to 0
    iSec = 0;
    //  Set duty cycle to 200
    iDuty = 200;
  }
  //  If iSec is greater than 20 decrement iDuty by 10
  if (iSec++ > 20)
  {
    if (iDuty <= 50)
    {
      iDuty = 50;
    }
    else
    {
      iDuty -= 10;
    }
  }
}
void Part_B(void)
{
  //  Left and right switches will change between red, green and blue
  //  Up and down switches will increase and decrease the duty cycle of the selected color
  //  If the center switch is pressed at the same time as the up or down switch, the duty cycle will change by 20
  if (leftSwitch == Released)
  {
    if (iSeq-- <= 0)
    {
      iSeq = 2;
    }
  }
  if (rightSwitch == Released)
  {
    if (iSeq++ >= 2)
    {
      iSeq = 0;
    }
  }
  switch(iSeq)
  {
    case 0:
      if (upSwitch == Released)
      {
        if(iRDuty++ >= 255)
        {
          iRDuty = 255;
        }
      }
      if (upSwitch == Held && centerSwitch == Released)
      {
        if (iRDuty >= 235)
        {
          iRDuty = 255;
        }
        else
        {
          iRDuty += 20;
        }
      }
      if (downSwitch == Released)
      {
        if (iRDuty-- <= 0)
        {
          iRDuty = 0;
        }
      }
      if (downSwitch == Held && centerSwitch == Released)
      {
        if (iRDuty <= 20)
        {
          iRDuty = 0;
        }
        else
        {
          iRDuty -= 20;
        }
      }
      break;
    case 1:
      if (upSwitch == Released)
      {
        if(iGDuty++ >= 255)
        {
          iGDuty = 255;
        }
      }
      if (upSwitch == Held && centerSwitch == Released)
      {
        if (iGDuty >= 235)
        {
          iGDuty = 255;
        }
        else
        {
          iGDuty += 20;
        }
      }
      if (downSwitch == Released)
      {
        if (iGDuty-- <= 0)
        {
          iGDuty = 0;
        }
      }
      if (downSwitch == Held && centerSwitch == Released)
      {
        if (iGDuty <= 20)
        {
          iGDuty = 0;
        }
        else
        {
          iGDuty -= 20;
        }
      }
      break;
    case 2:
      if (upSwitch == Released)
      {
        if(iBDuty++ >= 255)
        {
          iBDuty = 255;
        }
      }
      if (upSwitch == Held && centerSwitch == Released)
      {
        if (iBDuty >= 235)
        {
          iBDuty = 255;
        }
        else
        {
          iBDuty += 20;
        }
      }
      if (downSwitch == Released)
      {
        if (iBDuty-- <= 0)
        {
          iBDuty = 0;
        }
      }
      if (downSwitch == Held && centerSwitch == Released)
      {
        if (iBDuty <= 20)
        {
          iBDuty = 0;
        }
        else
        {
          iBDuty -= 20;
        }
      }
      break;
    default:
      break;
  }
}
/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action!
  if(iPitCount++ > 9999)
  {
    iPitCount = 0;
  }
  LED_Tog(LED_RED); // toggle the green LED
  
}