#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "AtoD.h"
#include "pit.h"

// other includes, as *required* for this implementation

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////
void AtoDInit(int iEnableInterrupt)
{
    ATD0CTL2 = 0b11000000; //Turn on AtoD
    ATD0CTL2_ASCIE = iEnableInterrupt; //Enable AtoD Interrupt
    PIT_Delay_us(50); //Delay 50us
    //  configure the A/D to complete 8 conversions per sequence (scan all channels). 
    //  This is done with the AD0CTL3 register (5.3.2.4). 
    //  We also don’t want to use the FIFO feature, 
    //  and we want the conversion to continue in freeze.
    ATD0CTL3 = 0b01000000;
    //  The next register (ATD0CTL4) determines clock parameters for the A/D. 
    //  The A/D can only run at 2MHz max, 
    //  so we need a prescale of 10 to clock our 20MHz bus down to 2MHz.
    ATD0CTL4 = 0b00000100;
    //  The last register (ATD0CTL5) determines how the data is presented, 
    //  and how the conversion operates
    ATD0CTL5 = 0b10110000;
}