//Clock System Library
//Processor: MC9S12XDP512
//Crystal: 16 MHz
//by Carlos Estay
//January 2022


#define EnableInterrupts   {__asm CLI;}
#define asm _asm

#define DEF_BUS_CLOCK 8000000


void Clock_Set24MHZ(void); //Set clock to 24MHZ using PLL
void Clock_Set20MHZ(void); //Set clock to 20MHZ using PLL
void Clock_EnableOutput(unsigned char);  //Enable output specifying divider for it
unsigned long Clock_GetBusSpeed(void);	//Get Current Clock Speed
float Clock_GetFactor(void);		//Get Clock factor as a multiplier of the base clock (8MHz)
void PIT_Init(void);
void PIT_Delay_us(unsigned int);
void RTI_Init(void(*function)(void));
void RTI_Delay_ms(int timeout);