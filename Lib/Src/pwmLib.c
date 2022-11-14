#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "pwmLib.h"

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
void PWM_8bit_A(PWM_Channel channel, A_Lower_Nibble lowerNibble, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period)
{
    //  Set the clock A to divide by the nibble value
    PWMCLK &= 0b11110000;
    PWMCLK |= lowerNibble;

    //  Set the clock SA to dive by period divided by 2
    PWMSCLA = period / 2;

    //  Check if we are using channel 0, 1, 4, or 5
    switch (channel)
    {
        case PWM_Channel_0:
            //  Select Clock
            PWMCLK_PCLK0 = clock;
            //  Set the polarity
            PWMPOL_PPOL0 = polarity;
            //  Set the period
            PWMPER0 = period;
            //  Set the duty cycle
            PWMDTY0 = duty;
            //  Enable the channel
            PWME_PWME0 = 1;
            break;
        case PWM_Channel_1:
            //  Select Clock
            PWMCLK_PCLK1 = clock;
            //  Set the polarity
            PWMPOL_PPOL1 = polarity;
            //  Set the period
            PWMPER1 = period;
            //  Set the duty cycle
            PWMDTY1 = duty;
            //  Enable the channel
            PWME_PWME1 = 1;
            break;
        case PWM_Channel_4:
            //  Select Clock
            PWMCLK_PCLK4 = clock;
            //  Set the polarity
            PWMPOL_PPOL4 = polarity;
            //  Set the period
            PWMPER4 = period;
            //  Set the duty cycle
            PWMDTY4 = duty;
            //  Enable the channel
            PWME_PWME4 = 1;
            break;
        case PWM_Channel_5:
            //  Select Clock
            PWMCLK_PCLK5 = clock;
            //  Set the polarity
            PWMPOL_PPOL5 = polarity;
            //  Set the period
            PWMPER5 = period;
            //  Set the duty cycle
            PWMDTY5 = duty;
            //  Enable the channel
            PWME_PWME5 = 1;
            break;
    }
}

void PWM_8bit_B(PWM_Channel number, B_Lower_Nibble upper_nibble, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period)
{
    //  Set the clock B to divide by the nibble value
    PWMCLK &= 0b10001111;
    PWMCLK |= upper_nibble;

    //  Set the clock SA to dive by period divided by 2
    PWMSCLA = period / 2;

    //  Check if we are using channel 2, 3, 6, or 7
    switch (channel)
    {
        case PWM_Channel_0:
            //  Select Clock
            PWMCLK_PCLK0 = clock;
            //  Set the polarity
            PWMPOL_PPOL0 = polarity;
            //  Set the period
            PWMPER0 = period;
            //  Set the duty cycle
            PWMDTY0 = duty;
            //  Enable the channel
            PWME_PWME0 = 1;
            break;
        case PWM_Channel_1:
            //  Select Clock
            PWMCLK_PCLK1 = clock;
            //  Set the polarity
            PWMPOL_PPOL1 = polarity;
            //  Set the period
            PWMPER1 = period;
            //  Set the duty cycle
            PWMDTY1 = duty;
            //  Enable the channel
            PWME_PWME1 = 1;
            break;
        case PWM_Channel_4:
            //  Select Clock
            PWMCLK_PCLK4 = clock;
            //  Set the polarity
            PWMPOL_PPOL4 = polarity;
            //  Set the period
            PWMPER4 = period;
            //  Set the duty cycle
            PWMDTY4 = duty;
            //  Enable the channel
            PWME_PWME4 = 1;
            break;
        case PWM_Channel_5:
            //  Select Clock
            PWMCLK_PCLK5 = clock;
            //  Set the polarity
            PWMPOL_PPOL5 = polarity;
            //  Set the period
            PWMPER5 = period;
            //  Set the duty cycle
            PWMDTY5 = duty;
            //  Enable the channel
            PWME_PWME5 = 1;
            break;
    }
}