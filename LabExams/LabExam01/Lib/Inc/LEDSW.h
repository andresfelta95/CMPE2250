//header for switches and Leds
//Processor: MC9S12XDP512
//Crystal: 16 MHz
//by Andres Tangarife
//January, 2022

#define LED_RED     0b10000000  //RED LED pin
#define LED_YELLOW  0b01000000  //YELLOW LED pin 
#define LED_GREEN   0b00100000  //GREEN LED pin 
#define LED_ALL     0b11100000  //ALL LED pins 

//Switch buttons
#define SW_LEFT   0b00001000
#define SW_RIGHT  0b00000010
#define SW_MID    0b00000001
#define SW_UP     0b00010000
#define SW_DOWN   0b00000100
#define SW_ALL    0b00011111

typedef enum SwState
{
  Idle,
  Pressed,
  Held,
  Released,
}SwState;

typedef union __SwLedTypedef
{
  volatile unsigned char Port;
  struct __Bits
  {
    unsigned int SwMid_BIT:1;
    unsigned int SwRight_BIT:1;
    unsigned int SwDown_BIT:1;
    unsigned int SwLeft_BIT:1;
    unsigned int SwUp_BIT:1;
    unsigned int GreenLed_BIT:1;
    unsigned int YellowLed_BIT:1;
    unsigned int RedLed_BIT:1;
  }Bits;
}SwLedTypedef;

void SwLED_Init(void);  //LEDs as outputs, switches as inputs, digital inputs enabled
void LED_On(char);      //accepts LED_RED, LED_GREEN, LED_YELLOW, LED_ALL
void LED_Off(char);     //accepts LED_RED, LED_GREEN, LED_YELLOW, LED_ALL
void LED_Tog(char);     //accepts LED_RED, LED_GREEN, LED_YELLOW, LED_ALL
unsigned char SwCk(void); //Debounced button presses
void Sw_Process(SwState*, unsigned char); //Process state of a swicth in PT1AD1

//Process debounced state of a swicth in PT1AD1
void Sw_ProcessD(unsigned char, SwState*, unsigned char); //port, SwState, PIN

