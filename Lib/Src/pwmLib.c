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
        default:
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
        case PWM_Channel_2:
            //  Select Clock
            PWMCLK_PCLK2 = clock;
            //  Set the polarity
            PWMPOL_PPOL2 = polarity;
            //  Set the period
            PWMPER2 = period;
            //  Set the duty cycle
            PWMDTY2 = duty;
            //  Enable the channel
            PWME_PWME2 = 1;
            break;
        case PWM_Channel_3:
            //  Select Clock
            PWMCLK_PCLK3 = clock;
            //  Set the polarity
            PWMPOL_PPOL3 = polarity;
            //  Set the period
            PWMPER3 = period;
            //  Set the duty cycle
            PWMDTY3 = duty;
            //  Enable the channel
            PWME_PWME3 = 1;
            break;
        case PWM_Channel_6:
            //  Select Clock
            PWMCLK_PCLK6 = clock;
            //  Set the polarity
            PWMPOL_PPOL6 = polarity;
            //  Set the period
            PWMPER6 = period;
            //  Set the duty cycle
            PWMDTY6 = duty;
            //  Enable the channel
            PWME_PWME6 = 1;
            break;
        case PWM_Channel_7:
            //  Select Clock
            PWMCLK_PCLK7 = clock;
            //  Set the polarity
            PWMPOL_PPOL7 = polarity;
            //  Set the period
            PWMPER7 = period;
            //  Set the duty cycle
            PWMDTY7 = duty;
            //  Enable the channel
            PWME_PWME7 = 1;
            break;
        default:
            break;
    }
}

void PWM_16bit(PWM_Channel channel, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period)
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
             PWM_Channel_1:
            //  Select Clock
            PWMCLK_PCLK1 = clock;
            //  Set the polarity
            PWMPOL_PPOL1 = polarity;
            //  Set the period
            PWMPER01 = period;
            //  Set the duty cycle
            PWMDTY01 = duty;
            // set as a 16-bit channel (the actual 16-bit concatenation)
            PWMCTL_CON01 = 1;
            //  Enable the channel
            PWME_PWME1 = 1;
            break;
        case PWM_Channel_4: 
             PWM_Channel_5:
            //  Select Clock
            PWMCLK_PCLK5 = clock;
            //  Set the polarity
            PWMPOL_PPOL5 = polarity;
            //  Set the period
            PWMPER45 = period;
            //  Set the duty cycle
            PWMDTY45 = duty;
            // set as a 16-bit channel (the actual 16-bit concatenation)
            PWMCTL_CON45 = 1;
            //  Enable the channel
            PWME_PWME5 = 1;
            break;
        default:
            break;
    }
}

void PWM_16bit_B(PWM_Channel channel, B_Lower_Nibble upper_nibble, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period)
{
    //  Set the clock B to divide by the nibble value
    PWMCLK &= 0b10001111;
    PWMCLK |= upper_nibble;

    //  Set the clock SA to dive by period divided by 2
    PWMSCLA = period / 2;

    //  Check if we are using channel 2, 3, 6, or 7
    switch (channel)
    {
        case PWM_Channel_2: 
             PWM_Channel_3:
            //  Select Clock
            PWMCLK_PCLK3 = clock;
            //  Set the polarity
            PWMPOL_PPOL3 = polarity;
            //  Set the period
            PWMPER23 = period;
            //  Set the duty cycle
            PWMDTY23 = duty;
            // set as a 16-bit channel (the actual 16-bit concatenation)
            PWMCTL_CON23 = 1;
            //  Enable the channel
            PWME_PWME3 = 1;
            break;
        case PWM_Channel_6: 
             PWM_Channel_7:
            //  Select Clock
            PWMCLK_PCLK7 = clock;
            //  Set the polarity
            PWMPOL_PPOL7 = polarity;
            //  Set the period
            PWMPER67 = period;
            //  Set the duty cycle
            PWMDTY67 = duty;
            // set as a 16-bit channel (the actual 16-bit concatenation)
            PWMCTL_CON67 = 1;
            //  Enable the channel
            PWME_PWME7 = 1;
            break;
        default:
            break;
    }
}

