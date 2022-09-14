/////////////////////////////////////////////////////////////////////////////
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       ICA01
// Date:          2022-09-13
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
void binaer(void);
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
char name[] = "Tangarife";
char course[40] = {0};
char cntDisplay[40] = {0};
char binaryCnt[40] = {0};
char regCnt[40] = {0};
int j = 0;
uint  msCount = 0;
uint count = 0;
uint loopVal;

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
SwState midS, rightS, leftS, upS, downS;
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
  PIT_Init();
  RTI_Init(&_LCD);
  lcd_Init();
  SwLED_Init();
  SevSeg_Init();
  //init timer
  Timer_Init(Timer_Prescale_32);
  //set chn0 as output
  TimerCH_EnableAsOutput(Timer_CH0_BIT);

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

  TC0 = TCNT + 3125; //5ms

  lcd_Clear();
  //Count in hex on LCD
  (void)sprintf(cntDisplay, "%04x", count);
  lcd_StringXY(0,0,cntDisplay);
  //Regresive count on LCD
  (void)sprintf(regCnt, "%04d", (9999 - count));
  lcd_StringXY(0,16,regCnt);
  //Name on LCD  
  lcd_StringXY(1, (21 - sizeof(name)), name);
  //Course on LCD
  (void)sprintf(course, "CMPE2250");
  lcd_StringXY(2,0,course);
  //Binary on LCD
  binaer();
  (void)sprintf(binaryCnt, "%.16s", binaryCnt);
  lcd_StringXY(3,2,binaryCnt);

  lcd_DispControl(0,0);
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    //Switches states
    Sw_Process(&upS, SW_UP);
    Sw_Process(&downS, SW_DOWN);
    Sw_Process(&midS, SW_MID);
    Sw_Process(&leftS, SW_LEFT);
    Sw_Process(&rightS, SW_RIGHT);

    // check the timer every 5ms
    if (TFLG1 & TFLG1_C0F_MASK)
    {
      TC0 += 3125;//clear flag
      //increase count every 100 cycles
      if(++msCount > 99)
      {
        msCount = 0;
        if(++count > 9999)
        {
          count = 0;
        }
      }       
    }

    //check left button being pressed
    if (leftS == Held)
    {
      SevSeg_Top4(count);
    }
    //check mid button being pressed
    else if(midS == Held)
    {
      SevSeg_Top4(HexToBCD(9999 - count));
    }
    else    
    {
      SevSeg_ClearTop4();
    }

    
    //Count in hex on LCD
    (void)sprintf(cntDisplay, "%04X", count);
    lcd_StringXY(0,0,cntDisplay);
    //Regresive count on LCD
    (void)sprintf(regCnt, "%04d", (9999 - count));
    lcd_StringXY(0,16,regCnt);
    //Count on SevSeg
    SevSeg_Bot4(HexToBCD(count));
    //Binary on LCD
    binaer();
    (void)sprintf(binaryCnt, "%.16s", binaryCnt);
    lcd_StringXY(3,2,binaryCnt);
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
//
void binaer(void){
  if(0 <= count && count <= 65535) 
  {
    loopVal = count;
    for( j = 0; j < 16 ; j++) 
    {
      if (loopVal % 2 == 0) 
      {
        binaryCnt[(15 - j)] = '0';
      }
      else {
        binaryCnt[(15 -j)] = '1';
      }
      loopVal = loopVal / 2;
    }
  }    
}

void _LCD(void){
  
}
/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////