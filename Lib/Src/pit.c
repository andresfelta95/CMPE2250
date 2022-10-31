// implementation file for PIT
// Processor: MC9S12XDP512
// Crystal: 16 MHz
// by Andres Tangarife
// October, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "pit.h"
#include "Clock.c"
//  Global Variables
unsigned int factor8Bit = 1;

int PIT_Init0(unsigned long ulInterval_us)
{
    unsigned long counts = Clock_GetBusSpeed() / (1E6f / ulInterval_us);
    // enable interrupt on chan 0
    PITINTE = PIT0; // 13.3.0.5
    PITMUX &= 0b11111110; //
    // Get a factor to obtain the biggest 16 bit and the smallest 8 bit as factors for the interval
    while (factor8Bit < 256 && ((counts % factor8Bit != 0) || (counts / factor8Bit > 65535)))
    {
        factor8Bit++;
    }
    if (factor8Bit > 256)
    {
        return 0;
    }
    else
    {
        // Set the interval fro the 8 bit and 16 bit factors
        PITMTLD0 = factor8Bit - 1;
        PITLD0 = (counts / factor8Bit) - 1;
        // enable periodic interrupt, normal in wait, PIT stalled in freeze
        PITCFLMT = PITCFLMT_PITE_MASK | PITCFLMT_PITFRZ_MASK; //
        // enable chan 0
        PITCE = 0b00000001; // 13.3.0.3
        return 0;
    }
}

int PIT_Inits(unsigned long ulInterval_us, PIT_Channel eChannel, PIT_Int eInt)
{
    unsigned long counts = Clock_GetBusSpeed() * ulInterval_us / 1000000;
    // enable interrupt if selected on channel selected
    if (eInt == 1)
    {
        PITINTE = eChannel; //
    }
    // Get a factor to obtain the biggest 16 bit and the smallest 8 bit as factors for the interval
    while (factor8Bit < 256 && ((counts % factor8Bit != 0) || (counts / factor8Bit > 65535)))
    {
        factor8Bit++;
    }
    if (factor8Bit > 256)
    {
        return 0;
    }
    // Set the interval fro the 8 bit and 16 bit factors in the channel selected
    switch (eChannel)
    {
    case 1:
        PITMTLD0 = factor8Bit - 1;
        PITLD0 = (counts / factor8Bit) - 1;
        break;
    case 2:
        PITMTLD0 = factor8Bit - 1;
        PITLD1 = (counts / factor8Bit) - 1;
        break;
    case 4:
        PITMTLD0 = factor8Bit - 1;
        PITLD2 = (counts / factor8Bit) - 1;
        break;
    case 8:
        PITMTLD0 = factor8Bit - 1;
        PITLD3 = (counts / factor8Bit) - 1;
        break;
    default:
        break;
    }
    // enable periodic interrupt, normal in wait, PIT stalled in freeze
    PITCFLMT = 0b10100000; //
    // enable channel selected
    PITCE = eChannel; //
    return 0;
}

void PIT_Init(void)
{
    // PIT Timer Initialization
    // PIT0 and PIT1 connected to micro Timer 0
    PITMTLD0 = 0;                                       // Micro Timer base 0, Mod-1 (bypass)
    PITMUX &= ~(PITMUX_PMUX0_MASK | PITMUX_PMUX1_MASK); // Connect PIT0 and PIT1 to Micro-Timer 0
    PITCFLMT |= PITCFLMT_PITE_MASK;                     // Enable General PIT
    PITCE |= PITCE_PCE0_MASK;                           // enable PIT 0, used for us delays
}
//-----------------------------------------------------------------
void PIT_Delay_us(unsigned int usDelay)
{
    unsigned long counts = Clock_GetBusSpeed() / 1000000;
    if (PITCE & PITCE_PCE0_MASK) // Check that PIT0 is enabled
    {
        PITLD0 = (unsigned int)(counts * usDelay); // timer Set for us Counter
        PITTF = PITTF_PTF0_MASK;                   // clear flag PIT0, safety practice
        PITFLT |= PITFLT_PFLT0_MASK;               // Force load register into PIT0
        while (!(PITTF & PITTF_PTF0_MASK))
            ;                    // wait until flag gets active
        PITTF = PITTF_PTF0_MASK; // clear flag PIT channel 0
    }
}

void PIT_Delay_ms(unsigned int msDelay)
{
    unsigned long counts = Clock_GetBusSpeed() / 1000;
    if (PITCE & PITCE_PCE0_MASK) // Check that PIT0 is enabled
    {
        PITLD0 = (unsigned int)(counts * msDelay); // timer Set for ms Counter
        PITTF = PITTF_PTF0_MASK;                   // clear flag PIT0, safety practice
        PITFLT |= PITFLT_PFLT0_MASK;               // Force load register into PIT0
        while (!(PITTF & PITTF_PTF0_MASK))
            ;                    // wait until flag gets active
        PITTF = PITTF_PTF0_MASK; // clear flag PIT channel 0
    }
}
