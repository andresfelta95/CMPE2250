/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       Bring up the PIT at the desired interval, channel 0
// Revision History
//      Created:  OCT 2021
/////////////////////////////////////////////////////////////////////////////

// requires interrupt handler for PIT Channel 0:
/*
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action!
}
*/

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// ulBusRate      : bus frequency, in Hz
// ulInterval_us  : desired interval in us
// iDebugSegs     : option, debug info placed in segs (computed clocking values for PIT)
int PIT_Init0 (unsigned long ulBusRate, unsigned long ulInterval_us, int iDebugSegs);

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////