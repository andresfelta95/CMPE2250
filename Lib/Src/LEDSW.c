// implementation file for switches and LEDs
//Processor: MC9S12XDP512
//Crystal: 16 MHz
//by Andres Tangarife
//January, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "LEDSW.h"

void SwLED_Init(void){
    PT01AD1 = 0;//clear prot
    DDR01AD1 = 0b11100000;//LEDs as outputs, buttons as inputs
    ATD1DIEN1 |= 0b00011111;//Enable GPIOs for buttons
}
void LED_On(char _led){
    PT01AD1 |= _led;
}
void LED_Off(char _led){
    PT01AD1 &= ~(_led);
}
void LED_Tog(char _led){
    PT01AD1 ^= _led;
}

void Sw_Process(SwState* _State, unsigned char _mask){
    if(PT01AD1 & _mask)
    {
        if (*_State == Idle)
        {
            *_State = Pressed;
        }
        else
        {
            *_State = Held;
        }
    }
    else
    {
        if (*_State == Held)
        {
            *_State = Released;            
        }
        else
        {
            *_State = Idle;
        }                
    }
    
}