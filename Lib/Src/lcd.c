// implementation file for lcd
// Processor: MC9S12XDP512
// Crystal: 16 MHz
// by Andres Tangarife
// March, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Clock.h"
#include "ECT.h"
#include "lcd.h"
#include "pit.h"

void lcd_Init(void)
{
    // Port Initialization-------------------------------------------
    // PH7-PH0 -> D7 - D0 (data Lines)
    DDRH = 0xFF;
    // PK2-PK0 -> Control Lines (output)
    DDRK |= 0b00000111;
    // 1st delay - Delay 40+ [ms]
    PIT_Delay_ms(45);
    // Timer_Sleep_ms(45);
    // Present Data on PTH
    PTH = 0b00111000; // Function Set
    /*             ||||||_____(don't care)
                   |||||______(don't care)
                   ||||_______font:  5x8 matrix (LOW)
                   |||________lines: 2 (HIGH)
                   ||_________data:  8-bit (HIGH)
                   |__________function set of commands
    */
    // Write Operation - Instruction Register
    // PK1->R/W->0  PK2->RS->0
    PORTK_PK1 = 0;
    PORTK_PK2 = 0;
    // Latch Instruction
    PORTK_PK0 = 1;
    PIT_Delay_us(2);
    PORTK_PK0 = 0;
    // 2nd Delay, 4.1ms+
    PIT_Delay_ms(5);
    // Timer_Sleep_ms(5);
    // Latch same Instruction again
    PORTK_PK0 = 1;
    PIT_Delay_us(2);
    PORTK_PK0 = 0;
    // third Delay  100uS+
    PIT_Delay_us(150); // third Delay  100uS+
    // Latch same Instruction again
    PORTK_PK0 = 1;
    PIT_Delay_us(2);
    PORTK_PK0 = 0;
    // Busy flag is active now**********************
    // lcd_Ctrl(0b00111000); //Function set
    //     //We can check for busy flag now**********************
    //     //5x8 dots, 2 lines
    lcd_Ctrl(0b00111000);

    lcd_Ctrl(0b00001110); // display controls
    /*                 ||||_____Blink:   LOW for off
                       |||______Cursor:  HIGH for on
                       ||_______Display: HIGH for on
                       |________Display Control commands
    */
    lcd_Ctrl(0b00000001); // clear display, home position
    lcd_Ctrl(0b00000110); // mode controls
    /*                  |||_____Shift:   LOW for no display shift
                        ||______Inc/Dec: HIGH for increment (to the left)
                        |_______Entry Mode commands
    */
}

void lcd_Ctrl(unsigned char val)
{
    while (lcd_Busy())
        ;
    PTH = val;
    PORTK_PK1 = 0; // Write
    PORTK_PK2 = 0; // Command

    // latch
    PORTK_PK0 = 1; // E ->  1
    PIT_Delay_us(2);
    PORTK_PK0 = 0; // E ->  0
}

char lcd_Busy(void)
{
    unsigned char inVal = 0;
    DDRH = 0x00;   // To read
    PORTK_PK1 = 1; // R/W   ->  1
    PORTK_PK2 = 0; // RS    ->  0

    // latch
    PORTK_PK0 = 1; // E ->  1
    PIT_Delay_us(2);
    PORTK_PK0 = 0; // E ->  0

    inVal = PTH & PTH_PTH7_MASK;
    DDRH = 0xFF;
    return inVal;
}

void lcd_Data(unsigned char data)
{
    while (lcd_Busy())
        ;

    PTH = data;
    // PORTK &= ~PORTK_PK1_MASK;
    // PORTK |= PORTK_PK2_MASK;
    PORTK_PK1 = 0; // Write
    PORTK_PK2 = 1; //

    // latch
    PORTK_PK0 = 1; // E ->  1
    PIT_Delay_us(2);
    PORTK_PK0 = 0; // E ->  0
}
void lcd_Addr(unsigned char addr)
{
    addr |= 0b10000000;
    lcd_Ctrl(addr);
}

void lcd_AddrXY(unsigned char ix, unsigned char iy)
{
    if (ix > 3 || iy > 19)
    {
        lcd_Addr(0);
    }
    else
    {
        switch (ix)
        {
        case 0:
            lcd_Addr(iy);
            break;
        case 1:
            lcd_Addr(iy + 64);
            break;
        case 2:
            lcd_Addr(iy + 20);
            break;
        case 3:
            lcd_Addr(iy + 84);

        default:
            break;
        }
    }
}

void lcd_String(char const *straddr)
{
    while (*straddr != 0)
    {
        lcd_Data(*straddr++);
    }
}

void lcd_StringXY(unsigned char ix, unsigned char iy, char const *const straddr)
{
    lcd_AddrXY(ix, iy);
    lcd_String(straddr);
}

void lcd_DispControl(unsigned char curon, unsigned char blinkon)
{
    if (curon == 1 && blinkon == 1)
    {
        lcd_Ctrl(0b00001111);
    }
    else if (curon == 1 && blinkon == 0)
    {
        lcd_Ctrl(0b00001110);
    }
    else if (curon == 0 && blinkon == 0)
    {
        lcd_Ctrl(0b00001100);
    }
    else if (curon == 0 && blinkon == 1)
    {
        lcd_Ctrl(0b00001101);
    }
}

void lcd_Clear(void)
{
    lcd_Ctrl(0b00000001);
}

void lcd_Home(void)
{
    lcd_AddrXY(0, 0);
}

void lcd_SmatStringXY(unsigned char ix, unsigned char iy, char const *straddr, int delay)
{
    lcd_AddrXY(ix, iy);
    while (*straddr != 0)
    {
        if (++iy > 19)
        {
            iy = 0;
            if (++ix > 3)
            {
                ix = 0;
            }
        }
        lcd_Data(*straddr++);
        lcd_AddrXY(ix, iy);
        PIT_Delay_ms(delay);
    }
}