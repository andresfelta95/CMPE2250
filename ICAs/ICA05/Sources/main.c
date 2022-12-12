/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       In this ICA you will use the PWM module in 16-bit mode on channel 7. 
//                You will use the up and down switches to select a duty value for the output waveform.
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
void PWM_5kHz(void);  //5kHz PWM
void Display_lcd(int iSeq); //Display LCD
void Up_Function(int iSeq); //Function to process up switch events depending on sequence
void Down_Function(int iSeq); //Function to process down switch events depending on sequence
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile long lDuty = 500;
volatile int iAdd = 1;
volatile int iPolarity = 1;
volatile int iEnable = 1;
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
  int iSeq = 0;
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  Clock_Set20MHZ();                           // set the clock to 20 MHz
  Timer_Init(Timer_Prescale_64);              // initialize the timer
  lcd_Init();                                 // initialize the LCD
  PWM_5kHz();                                 // initialize the PWM
  SwLED_Init();                               // initialize the LEDs and switches.
  lcd_DispControl(0,0);                       // turn off the cursor
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    Sw_Process(&upSwitch, SW_UP); //Process up switch
    Sw_Process(&downSwitch, SW_DOWN); //Process down switch
    Sw_Process(&leftSwitch, SW_LEFT); //Process left switch
    Sw_Process(&rightSwitch, SW_RIGHT); //Process right switch
    Sw_Process(&centerSwitch, SW_MID); //Process center switch
    //  Check if middle switch is released
    if (centerSwitch == Released)
    {
      //  Toggle channel 7
      iEnable = !iEnable;
      lcd_Clear();
    }
    //  Check if left switch is released
    if (leftSwitch == Released)
    {
      //  Change to previous sequence
      if (iSeq-- < 0)
      {
        iSeq = 3;
      }
    }
    //  Check if right switch is released
    if (rightSwitch == Released)
    {
      //  Change to next sequence
      if (iSeq++ > 3)
      {
        iSeq = 0;
      }
    }
    //  Check if up switch is released
    if (upSwitch == Released)
    {
      Up_Function(iSeq);  //Call up function
      lcd_Clear();
    }
    //  Check if down switch is released
    if (downSwitch == Released)
    {
      Down_Function(iSeq);  //Call down function
      lcd_Clear();
    }
    //  Call PWM_5kHz() to update the output
    PWM_5kHz();
    //  Call Display_lcd() to update the LCD
    Display_lcd(iSeq);
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void PWM_5kHz(void)
{
  // channel 7 is a B channel!
  // set Clock B to divide by 1 (8.3.2.4)
  PWMPRCLK &= 0b10001111; // 2^0 = 1
  PWMPRCLK |= PWM_Channel_B_Two; // 2^1 = 2
  // set Clock B to divide by 250
  PWMSCLB = 1; // remember it does x2
  // select Clock B for channel 7 (8.3.2.3)
  PWMCLK_PCLK7 = 1; // 0 means use clock scale B
  // set polarity as start high, go low (does not matter here) (8.3.2.2)
  PWMPOL_PPOL7 = iPolarity; // 0 means start high, go low
  // program period (8.3.2.13)
  PWMPER67 = 1000; // nice, support file has 16-bit registers defined
  // program duty (8.3.2.14)
  PWMDTY67 = lDuty; // 16-bit values provide *much* higher control over period and duty ratio
  // set as a 16-bit channel (the actual 16-bit concatenation)
  PWMCTL_CON67 = iEnable;
  // enable channel (8.3.2.1)
  PWME_PWME7 = iEnable; // 0 means disable, 1 means enable
}
void Display_lcd( int iSeq)
{
  // Diplay:
  // 1. Adjust: 0.1% or 1% or 10%
  // 2. Duty: percentage
  // 3. Polarity: L or H
  // 4. PWM Chan 7: On or Off
  char cAdjust[14];
  char cDuty[18];
  char cPolarity[18];
  char cEnable[18];
  char cSeq[4];
  float fDuty = (float)(lDuty / 1000.0)*100.0;  
  //  Create adjust string
  if (iAdd == 1)
  {
    (void)sprintf(cAdjust, "Adjust: 0.1%%");
  }
  else if (iAdd == 10)
  {
    (void)sprintf(cAdjust, "Adjust: 1%%");
  }
  else if (iAdd == 100)
  {
    (void)sprintf(cAdjust, "Adjust: 10%%");
  }
  //  Create duty string
  (void)sprintf(cDuty, "Duty: %3.1f%%", fDuty);
  //  Create polarity string
  if (iPolarity == 1)
  {
    (void)sprintf(cPolarity, "Polarity: H");
  }
  else if (iPolarity == 0)
  {
    (void)sprintf(cPolarity, "Polarity: L");
  }
  //  Create enable string
  if (iEnable == 1)
  {
    (void)sprintf(cEnable, "PWM Chan 7: On");
  }
  else if (iEnable == 0)
  {
    (void)sprintf(cEnable, "PWM Chan 7: Off");
  }
  //  Create sequence string
  if (iSeq == 0)
  {
    (void)sprintf(cSeq, "S:A");
  }
  else if (iSeq == 1)
  {
    (void)sprintf(cSeq, "S:D");
  }
  else if (iSeq == 2)
  {
    (void)sprintf(cSeq, "S:P");
  }
  else if (iSeq == 3)
  {
    (void)sprintf(cSeq, "S:R");
  }
  //  Display strings
  lcd_StringXY(0, 0, cAdjust);
  lcd_StringXY(1, 0, cDuty);
  lcd_StringXY(2, 0, cPolarity);
  lcd_StringXY(3, 0, cEnable);
  lcd_StringXY(0, 16, cSeq);

}
void Up_Function(int iSeq)
{
  // Check which sequence is active
  switch (iSeq)
  {
    //  Adjust
    case 0:
      //  Increment adjust
      if (iAdd == 1)
      {
        iAdd = 10;  //  0.1% to 1%
      }
      else if (iAdd == 10)
      {
        iAdd = 100; //  1% to 10%
      }
      else if (iAdd == 100)
      {
        iAdd = 1;  //  10% to 0.1%
      }
      break;
    //  Duty
    case 1:
      //  Increment duty
      lDuty += iAdd;
      //  Check if duty is greater than 1000
      if (lDuty > 1000)
      {
        //  Set duty to 1000
        lDuty = 1000;
      }
      break;
    //  Polarity
    case 2:
      //  Set polarity to 1
      iPolarity = 1;
      break;
    //  PWM Chan 7
    case 3:
      //  Reset default values
      lDuty = 500;  //  50%
      iPolarity = 1; // High
      iAdd = 1;  //  0.1%
      break;
      default:
      break;
  }
}
void Down_Function(int iSeq)
{
  // Check which sequence is active
  switch (iSeq)
  {
    //  Adjust
    case 0:
      //  Decrement adjust
      if (iAdd == 1)
      {
        iAdd = 100;  //  0.1% to 10%
      }
      else if (iAdd == 10)
      {
        iAdd = 1; //  1% to 0.1%
      }
      else if (iAdd == 100)
      {
        iAdd = 10;  //  10% to 1%
      }
      break;
    //  Duty
    case 1:
      //  Decrement duty
      lDuty -= iAdd;
      //  Check if duty is less than 0
      if (lDuty < 0)
      {
        //  Set duty to 0
        lDuty = 0;
      }
      break;
    //  Polarity
    case 2:
      //  Set polarity to 0
      iPolarity = 0;
      break;
    //  PWM Chan 7
    case 3:
      //  Reset default values
      lDuty = 500;  //  50%
      iPolarity = 1; // High
      iAdd = 1;  //  0.1%
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
  // disable the PIT
  // PITCE = 0;
  // LED_Tog(LED_RED); // toggle the green LED
  
}
