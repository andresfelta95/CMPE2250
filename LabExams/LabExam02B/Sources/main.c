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
#include "sci.h"    //SCI


/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////
void ShowOutput(void);
void TreeKhz(void);
#define PartB
#define PartC
#define PartD
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile unsigned int pitCnt = 0;
volatile unsigned int bntNum = 0;
volatile unsigned char __RXVal;
volatile unsigned long lDuty = 3333;
volatile int _ActvStWtch = 0;
volatile int iPolarity = 1;
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
SwState upSwitch; //up Switch
SwState downSwitch; //down Switch
SwState leftSwitch; //left Switch
SwState rightSwitch; //right Switch
char const GoYellow [] = "\x1B[33m"; //Yellow
char const GoRed [] = "\x1B[31m"; //Red
char const GoGreen [] = "\x1B[32m"; //Green
char const GoBlue [] = "\x1B[34m"; //Blue
/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  Clock_Set20MHZ();                           // set the clock to 20 MHz
  (void)PIT_Init0(125000);                    //Initialize PIT0 with 20ms interval
  SwLED_Init();                               // initialize the switches and LEDs
  (void)sci0_Init(20E6, 19200, 0);            //Initialize SCI
  PortJ_Init();                               // initialize the port J
  
  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  (void)TreeKhz();
  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    asm wai; //wait for interrupt
    #ifdef PartB
    Sw_Process(&upSwitch, SW_UP); //Process up switch
    Sw_Process(&downSwitch, SW_DOWN); //Process down switch
    Sw_Process(&leftSwitch, SW_LEFT); //Process left switch
    Sw_Process(&rightSwitch, SW_RIGHT); //Process right switch
    if (upSwitch == Released) //if up switch is released
    {
      sci0_txStr(GoRed); //send red
      bntNum = 1; //set button number to 1
    }
    if (downSwitch == Released) //if down switch is released
    {
      sci0_txStr(GoGreen); //send green
      bntNum = 2; //set button number to 2
    }
    if (leftSwitch == Released) //if left switch is released
    {
      sci0_txStr(GoYellow); //send Yellow
      bntNum = 3; //set button number to 3
    }
    if (rightSwitch == Released) //if right switch is released
    {
      sci0_txStr(GoBlue); //send Blue
      bntNum = 4; //set button number to 4
    }
    (void)ShowOutput(); //show output
    #endif
    #ifdef PartC
    // Pressing a specific numeric key in the terminal will change the duty cycle of the PWM signal
    //  0 = 0%
    //  1 = 10%
    //  2 = 60%
    //  3 = 93.2%
    //  4 = 100%
    __RXVal = sci0_bread(); //read value
    switch (__RXVal) //switch on value
    {
      case '0': //if 0
        lDuty = 0; //set duty cycle to 0
        break;
      case '1': //if 1
        lDuty = 66; //set duty cycle to 10%
        break;
      case '2': //if 2
        lDuty = 3999; //set duty cycle to 60%
        break;
      case '3': //if 3
        lDuty = 6217; //set duty cycle to 93.2%
        break;
      case '4': //if 4
        lDuty = 6666; //set duty cycle to 100%
        break;
      default: //default
        break;
    }    
    #endif
    #ifdef PartD
    //  Presing PJ1 will use a positive polarity PWM signal and turn on Green LED
    //  Pressing PJ2 will use a negative polarity PWM signal and turn on Red LED
    if (_ActvStWtch == 1) //if PJ0 is pressed
    {
      iPolarity = 1; //set polarity to positive
      LED_Off(LED_GREEN); //turn on green LED
      LED_On(LED_RED); //turn off red LED
      // (void)TreeKhz(); //Reset PWM
    }
    if (_ActvStWtch == 2) //if PJ1 is pressed
    {
      iPolarity = 0; //set polarity to negative
      LED_On(LED_GREEN); //turn off green LED
      LED_Off(LED_RED); //turn on red LED
    }
    #endif
    (void)TreeKhz(); //Reset PWM
  }                   
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////
void ShowOutput(void)
{
  switch(bntNum)
  {
    case 0:
      sci0_txStr("None");
      break;
    case 1:
      sci0_txStr("Up");
      break;
    case 2:
      sci0_txStr("Down");
      break;
    case 3:
      sci0_txStr("Left");
      break;
    case 4:
      sci0_txStr("Right");
      break;
    default:
      break;
  }
}
void TreeKhz(void)
{
  // channel 7 is a B channel!
  // set Clock B to divide by 1 (8.3.2.4)
  PWMPRCLK &= 0b10001111; // 2^0 = 1
  // don't use scaled clock
  // select Clock B for channel 7 (8.3.2.3)
  PWMCLK_PCLK7 = 0; // 0 means use clock B
  // set polarity as start high, go low (does not matter here) (8.3.2.2)
  PWMPOL_PPOL7 = iPolarity; // 0 means start high, go low
  // program period (8.3.2.13)
  PWMPER67 = 6666; // nice, support file has 16-bit registers defined
  // program duty (8.3.2.14)
  PWMDTY67 = lDuty; // 16-bit values provide *much* higher control over period and duty ratio
  // set as a 16-bit channel (the actual 16-bit concatenation)
  PWMCTL_CON67 = 1;
  // enable channel (8.3.2.1)
  PWME_PWME7 = 1;
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
// Port J interrupt
interrupt VectorNumber_Vportj void PortJ_ISR(void)
{
  // check if the interrupt is from the switch 1
  if (PIFJ_PIFJ1) // read of PIFJ (22.3.2.61)
  {
    // ack interrupt
    //PIFJ_PIFJ0 = 1; // can't do R/W clearing (preserves other bits, so clears other flags!)
    PIFJ = PIFJ_PIFJ1_MASK; // write only clear (just this ONE bit)
    
    // show something happened:
    _ActvStWtch = 2; // set the active switch to 2
  }
  // check if the interrupt is from the switch 2
  if (PIFJ_PIFJ0)
  {    
    // ack interrupt
    PIFJ = PIFJ_PIFJ0_MASK; // write only clear
    // show something happened:
    _ActvStWtch = 1;
  } 

}