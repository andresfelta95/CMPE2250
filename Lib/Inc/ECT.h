//Enhanced Capture Timer (ECT) ILibrary
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by Carlos Estay, based of Simon's library
//October 2021
//Edited January, 2022

#include "derivative.h"


typedef enum __Timer_Prescale
{
  Timer_Prescale_1 = 0,
  Timer_Prescale_2 = 1,
  Timer_Prescale_4 = 2,
  Timer_Prescale_8 = 3,
  Timer_Prescale_16 = 4,
  Timer_Prescale_32 = 5,
  Timer_Prescale_64 = 6,
  Timer_Prescale_128 = 7
}Timer_Prescale;


typedef enum __Timer_CH_BIT
{
  Timer_CH0_BIT = 0b00000001, //1
  Timer_CH1_BIT = 0b00000010, //2
  Timer_CH2_BIT = 0b00000100, //4
  Timer_CH3_BIT = 0b00001000, //8
  Timer_CH4_BIT = 0b00010000, //16
  Timer_CH5_BIT = 0b00100000, //32
  Timer_CH6_BIT = 0b01000000, //64
  Timer_CH7_BIT = 0b10000000 //128
}Timer_CH_BIT;


//Initializes the timer using the input prescale
// REGISTERS:
// TSCR2, TSCR1
void Timer_Init(Timer_Prescale);

//Set channel as Output Compare(1)
// REGISTERS:
// TIOS
void TimerCH_EnableAsOutput(Timer_CH_BIT);

//Set channel as Input Capture(0)
// REGISTERS:
// TIOS
void TimerCH_EnableAsInput(Timer_CH_BIT);

//Enable interrupt for channel(1)
// REGISTERS:
// TIE
void TimerCH_IntEnable(Timer_CH_BIT);

//Blocking delay for x counts - requires previous InitTimer call
//A specific channel, neither TC0 nor TC7, has to be dedicated to this.
void Timer_SleepCounts(unsigned int);

void Timer_SleepCounts2(uint, Timer_CH_BIT, uint*);

//blocking delay for x [ms], no matter what prescale or BUS speed is active
void Timer_Sleep_ms(unsigned int);



