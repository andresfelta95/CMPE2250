/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        This B. You
// Details:       The purpose of the PWM module is to create continuous waveforms with programmable duty rates.
//                There are eight 8-bit channels available for use (may be configured as four 16-bit channels). 
//                There are many clocking options which allow a wide range of frequencies.
//                You will use PWM to create fixed frequencies suitable for audio tone generation, 
//                LCD backlight control, or as a simple square-wave generator. 
//                You may also use actual modulation with PWM to control servos,
//                operate a Class D amplifier, or generate RGB colors with an RGB LED.
//                The module consists of eight channels, but effectively, they are internally organized as two groups of four channels, 
//                each group based on a dedicated clock.
//                Channels 0, 1, 4, and 5 are part of group A
//                Channels 2, 3, 6, and 7 are part of group B
//                Each group may also select between a scaled and unscaled clock source. 
//                This flexible clocking scheme allows for great control of the frequencies generated but makes operation a little more complex than
//                other clock-based modules we have seen thus far. 
//                The fact that each group shares a clock source also limits how each group may be used.
// Revision History
//      Created: 
//      each revision will have a date + desc. of changes
/////////////////////////////////////////////////////////////////////////////
//  Includes
typedef enum PWM_Channel    // PWM Channel
{
    PWM_Channel_0 = 0,
    PWM_Channel_1,
    PWM_Channel_2,
    PWM_Channel_3,
    PWM_Channel_4,
    PWM_Channel_5,
    PWM_Channel_6,
    PWM_Channel_7
} PWM_Channel;            // PWM Channel

typedef enum A_Lower_Nibble // PWM Channel A Lower Nibble
{
    PWM_Channel_A_One = 0,
    PWM_Channel_A_Two = 0b00000001,
    PWM_Channel_A_Four = 0b00000010,
    PWM_Channel_A_Eight = 0b00000100,
    PWM_Channel_A_Sixteen = 0b00000011,
    PWM_Channel_A_ThirtyTwo = 0b00000101,
    PWM_Channel_A_SixtyFour = 0b00000110,
    PWM_Channel_A_OneTwentyEight = 0b00000111
} A_Lower_Nibble;            // PWM Channel A Lower Nibble

typedef enum B_Lower_Nibble // PWM Channel B Lower Nibble
{
    PWM_Channel_B_One = 0,
    PWM_Channel_B_Two = 0b00010000,
    PWM_Channel_B_Four = 0b00100000,
    PWM_Channel_B_Eight = 0b01000000,
    PWM_Channel_B_Sixteen = 0b00110000,
    PWM_Channel_B_ThirtyTwo = 0b01010000,
    PWM_Channel_B_SixtyFour = 0b01100000,
    PWM_Channel_B_OneTwentyEight = 0b01110000
} B_Lower_Nibble;            // PWM Channel B Lower Nibble

typedef enum PWM_Clock      // PWM Clock
{
    PWM_Clock_X = 0,
    PWM_Clock_SX
} PWM_Clock;                // PWM Clock

typedef enum PWM_Polarity   // PWM Polarity
{
    PWM_Polarity_Low = 0,
    PWM_Polarity_High
} PWM_Polarity;             // PWM Polarity

//  PWM upper nibble of the PWM Control Register (PWMCR) is used to select the clock source for each group

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////
//  PWM_8bit_A
//  Purpose:    This function configures the PWM module for 8-bit operation
//  Arguments:  PWM_Channel number, A_Lower_Nibble, PWM_Clock, PWM_Polarity,
//              unsigned int duty, unsigned int period.
//              

void PWM_8bit_A(PWM_Channel number, A_Lower_Nibble lower_nibble, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period);

//  PWM_8bit_B
//  Purpose:    This function configures the PWM module for 8-bit operation
//  Arguments:  PWM_Channel number, B_Lower_Nibble, PWM_Clock, PWM_Polarity,
//              unsigned int duty, unsigned int period.
// 

void PWM_8bit_B(PWM_Channel number, B_Lower_Nibble upper_nibble, PWM_Clock clock, PWM_Polarity polarity, unsigned int duty, unsigned int period);
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////