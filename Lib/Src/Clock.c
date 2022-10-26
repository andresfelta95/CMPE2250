// implementation file for Clock
// Processor: MC9S12XDP512
// Crystal: 16 MHz
// by Andres Tangarife
// Februaty, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Clock.h"

ulong busspeed = DEF_BUS_CLOCK;
long RTI_mscntr = 0;
void (*RTI_Tick)(void);

void Clock_Set24MHZ(void)
{ // Set clock to 24MHZ using PLL
  // PLLCLK = 2 x OSCCLK x ([SYNR + 1] / [REFDV + 1])
  // PLLCLK = 2 x 16Mhz x ([2 + 1] / [1 + 1])
  // 3/2 (1.5) * 16Mhz * 2 = 48MHz
  // bus is PLLCLOCK / 2, 48MHz / 2 = 24MHz
  SYNR = 2;
  REFDV = 1;

  PLLCTL = PLLCTL_PLLON_MASK | PLLCTL_AUTO_MASK; // PLL ON and AUTO

  while (!(CRGFLG & CRGFLG_LOCK_MASK))
    ; // Wait for PLL to lock into the frequency

  CLKSEL |= CLKSEL_PLLSEL_MASK; // Select PLL as clock source

  // ECLKCTL |= ECLKCTL_EDIV1_MASK | ECLKCTL_EDIV0_MASK;//clock divide by 4
}

void Clock_Set20MHZ(void)
{
  // PLLCLK = 2 x OSCCLK x ([SYNR + 1] / [REFDV + 1])
  // PLLCLK = 2 x 16Mhz x ([4 + 1] / [3 + 1])
  // 5/4 (1.25) * 16Mhz * 2 = 40MHz
  // bus is PLLCLOCK / 2, 40MHz / 2 = 20MHz
  SYNR = 4;
  REFDV = 3;

  PLLCTL = PLLCTL_PLLON_MASK | PLLCTL_AUTO_MASK; // PLL ON and AUTO

  while (!(CRGFLG & CRGFLG_LOCK_MASK))
    ; // Wait for PLL to lock into the frequency

  CLKSEL |= CLKSEL_PLLSEL_MASK; // Select PLL as clock source

  // ECLKCTL |= ECLKCTL_EDIV1_MASK | ECLKCTL_EDIV0_MASK;//clock divide by 4
}

void Clock_EnableOutput(unsigned char value)
{
  ECLKCTL |= value;
}

unsigned long Clock_GetBusSpeed(void)
{
  if (!(CLKSEL & CLKSEL_PLLSEL_MASK))
  {
    return busspeed;
  }
  else
  {
    busspeed = (2 * ((busspeed * 2) * ((SYNR + 1) / (REFDV + 1)))) / 2;
  }

  return busspeed;
}

float Clock_GetFactor(void)
{
  float factor;
  factor = Clock_GetBusSpeed() / (2 * DEF_BUS_CLOCK);
  return factor;
}
void RTI_Init(void (*function)(void))
{
  RTICTL = RTICTL_RTDEC_MASK | RTICTL_RTR4_MASK | 7;
  CRGINT |= CRGINT_RTIE_MASK;
  RTI_Tick = function;
}

void RTI_Delay_ms(int timeout)
{
  RTI_mscntr = timeout;
  while (RTI_mscntr > 0)
    ;
}

interrupt VectorNumber_Vrti void Vrti_Handler(void)
{
  CRGFLG = CRGFLG_RTIF_MASK; // clear the flag
  if (--RTI_mscntr > 0)
    ;
  RTI_Tick();
}