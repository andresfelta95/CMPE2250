// implementation file for ETC
// Processor: MC9S12XDP512
// Crystal: 16 MHz
// by Andres Tangarife
// Februaty, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "math.h"
#include "Clock.h"
#include "ECT.h"

unsigned int Prscaleval;

void Timer_Init(Timer_Prescale timerPrescale)
{
    TSCR2 = timerPrescale;                     // set the register
    TSCR1 = TSCR1_TEN_MASK | TSCR1_TFFCA_MASK; // enable the timer
    Prscaleval = (unsigned int)pow_i(2, (int)timerPrescale);
}

void Timer_InitCH0(unsigned long ulBusClock, Timer_Prescale prescale, unsigned int uiOffset, int enableInt, Timer_PinAction pinAction)
{
    TSCR2 = prescale;                          // set the register
    TSCR1 = TSCR1_TEN_MASK | TSCR1_TFFCA_MASK; // enable the timer
    TIOS |= Timer_CH0_BIT;                     // set as output compare
    if (enableInt)
    {
        TIE_C0I = 1;
    }
    switch (pinAction)
    {
    case Timer_Pin_Disco:
        TCTL2_OM0 = 0;
        TCTL2_OL0 = 0;
        break;
    case Timer_Pin_Toggle:
        TCTL2_OM0 = 0;
        TCTL2_OL0 = 1;
        ;
        break;
    case Timer_Pin_Clear:
        TCTL2_OM0 = 1;
        TCTL2_OL0 = 0;
        break;
    case Timer_Pin_Set:
        TCTL2_OM0 = 1;
        TCTL2_OL0 = 1;
        break;
    }
    TC0 = TCNT + uiOffset;
    Prscaleval = (unsigned int)pow_i(2, (int)prescale); // set the prescale value
}

void TimerCH_EnableAsOutput(Timer_CH_BIT timerchannel)
{
    TIOS |= timerchannel;
}

void TimerCH_EnableAsInput(Timer_CH_BIT timerchannel)
{
    TIOS &= ~(timerchannel);
}

void TimerCH_IntEnable(Timer_CH_BIT timerchannel)
{
    TIE |= timerchannel;
}

void Timer_SleepCounts(unsigned int val)
{
    TimerCH_EnableAsOutput(Timer_CH5_BIT);
    TC5 = TCNT + val;
    while (!(TFLG1 & TFLG1_C5F_MASK))
        ;
}

void Timer_SleepCounts2(uint val1, Timer_CH_BIT timerchannel, uint *val2)
{
    TimerCH_EnableAsOutput(timerchannel);
    *val2 = TCNT + val1;
    while (!(TFLG1 & timerchannel))
        ;
}

void Timer_Sleep_ms(unsigned int val)
{
    int iX;
    unsigned long cnt = Clock_GetBusSpeed() / 1000;
    cnt /= Prscaleval;

    for (iX = 0; iX < val; iX++)
    {
        Timer_SleepCounts((unsigned int)cnt);
    }
}

void Timer_Sleep_us(unsigned int val)
{
    int iX;
    unsigned long cnt = Clock_GetBusSpeed() / 1000000;
    cnt /= Prscaleval;
    if (cnt <= 0)
    {
        cnt = 1;
    }
    for (iX = 0; iX < val; iX++)
    {
        Timer_SleepCounts((unsigned int)cnt);
    }
}
