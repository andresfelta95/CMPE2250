/////////////////////////////////////////////////////////////////////////////
// 9S12X Program: SCI Demo
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       Demo of SCI interrupts, operations, libs, and escape sequences
// Date:          Nov 8 2022
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>
#include "derivative.h"
#include "pll.h"
#include "segs.h"

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes (these would be in your SCI.H file)
/////////////////////////////////////////////////////////////////////////////
void sci0_Init(void);

// send a byte over SCI
void sci0_txByte(unsigned char data);

// blocking read!
unsigned char sci0_bread(void);

// read a byte, non-blocking
// returns 1 if byte read, 0 if not
int sci0_read(unsigned char *pData);

// send a null-terminated string over SCI
void sci0_txStr (char const * straddr);
/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile unsigned char __RXVal;

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
// see the link to the VT100 terminal escape sequences in Moodle!

// mark constant strings as const (target, not pointer)
//  otherwise the string will go into RAM (see Project.map)
char const GoYellow [] = "\x1B[33m";

/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
void main(void)
{
  //unsigned char var = 'A';

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  sci0_Init();
  Segs_Init();
  
  // set the terminal colour to yellow
  sci0_txStr (GoYellow);

  // turn on interrupt for RX on SCI0
  SCI0CR2_RIE = 1;

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // wait for the only interrupt we asked for (RDRF)
    // note: this program behaviour would be quite different
    //  if we added a timer or pit interrupt
    asm wai;

    // show the received character on the segs
    Segs_8H(0, __RXVal);
    
    // echo it back to the terminal so we see it
    sci0_txByte(__RXVal);                       

    // always send the string Hello! too!
    sci0_txStr ("Hello!");

    // sci0_txByte (var++);
    // if (var > 'Z')
    //   var = 'A';

    {
      // create target for receive
      // unsigned char rec;

      // if no error reported, then use target
      // if (!sci0_read (&rec))
      //  Segs_8H (0, rec);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

int sci0_read(unsigned char *pData)
{
  // no byte to read?
  if (!SCI0SR1_RDRF)
    return -1;

  // return the data through the pointer
  *pData = SCI0DRL;

  return 0;
}

unsigned char sci0_bread(void)
{
  // wait for RDRF! (Could be forever!)
  while (!SCI0SR1_RDRF)
    ;

  // read and return the byte
  return SCI0DRL;
}

void sci0_txStr (char const * straddr)
{
  // whilst the pointer not at NUL point
  while (*straddr)
  {
    // fire out the target of the pointer (each character)
    sci0_txByte (*straddr); 
    
    // move on to the next character by advancing pointer
    straddr++; 
  }  
}

void sci0_txByte(unsigned char data)
{
  // wait for TDRE to be high (transmit buffer ready for data)
  while (!SCI0SR1_TDRE)
    ;

  // send the byte
  SCI0DRL = data;
}

void sci0_Init(void)
{
  // 20E6 / 16 / 9600 = ~130
  // you would do calcs to get this value
  // you get bus + desired baud and calculate SCI0BD value 

  // set the BAUD rate (assume 9600 BAUD at 20MHz bus)
  SCI0BD = 130;

  // turn on TX and RX module 11.3.2.6
  SCI0CR2 = 0b00001100;
  // same as:
  SCI0CR2_TE = 1;
  SCI0CR2_RE = 1;
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vsci0 void ISR_SCI0(void)
{
  // we need to see RDRF set
  // we need to pull data
  //  :: in order to clear the RDRF interrupt
  
  // place in volatile global, but use WAI to control when it is read
  __RXVal = sci0_bread();
}
