/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andres Tangarife
// Details:       Bring up the PIT at the desired interval, in any channel
// Revision History
//      Created:  OCT 2022
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
typedef enum PIT_Channel 
{
  PIT0 = 1, 
  PIT1 = 2, 
  PIT2 = 4, 
  PIT3 = 8
} PIT_Channel;

typedef enum PIT_Int 
{
  PITInt_Disabled = 0, 
  PITInt_Enabled = 1
} PIT_Int;
/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// ulBusRate      : bus frequency, in Hz
// ulInterval_us  : desired interval in us
// iDebugSegs     : option, debug info placed in segs (computed clocking values for PIT)
int PIT_Init0 (unsigned long ulInterval_us, int iDebugSegs);


// ulBusRate      : bus frequency, in Hz
// ulInterval_us  : desired interval in us
// eChannel       : PIT channel to use
// eInt           : enable interrupt
// iDebugSegs     : option, debug info placed in segs (computed clocking values for PIT)
int PIT_Inits (unsigned long ulBusRate, unsigned long ulInterval_us, PIT_Channel eChannel, PIT_Int eInt, int iDebugSegs);
void PIT_Init(void);
void PIT_Delay_us(unsigned int);
void PIT_Delay_ms(unsigned int);
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////