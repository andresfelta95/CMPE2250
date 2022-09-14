/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       LCD Library
// Revision History :
// Created : Unknown
//  Dec 07 2020 - Modified Documentation
//  Dec    2020 - Modified names, modified to use timer for delays
//  Nov 2021, Added PIT Timer for delays, by Carlos
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
// 8-Bit interface init on LCD
// LCD is wired to PTH for data, PK0:2 for control :
// 2     1     0     PTK 
// A     R/W*  E     LCD 
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// LCD Address Scheme (HEX):
// 00 01 ... 12 13
// 40 41 ... 52 53
// 14 15 ... 26 27
// 54 55 ... 66 67
////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////
#define lcd_RWUp DDHR = 0; PORTK |= 2;

#define lcd_RWDown PORTK &= (~2); DDHR = 0XFF;

#define lcd_EUp PORTK |= 1;
#define lcd_EDown PORTK &= (~1);
#define lcd_RUp PORTK |= 4;
#define lcd_RDown PORTK &= (~4);

void lcd_Init (void);
void lcd_Ctrl (unsigned char); //LCD_Inst
char lcd_Busy (void); //LCD_Inst
void lcd_Data (unsigned char val);
void lcd_Addr (unsigned char addr);
void lcd_AddrXY (unsigned char ix, unsigned char iy);
void lcd_String (char const * straddr);
void lcd_StringXY (unsigned char ix, unsigned char iy, char const * const straddr);
void lcd_SmatStringXY(unsigned char ix, unsigned char iy, char const * straddr, int delay);

void lcd_DispControl (unsigned char curon, unsigned char blinkon);
void lcd_Clear (void);
void lcd_Home (void);
void lcd_ShiftL (void);
void lcd_ShiftR (void);

void lcd_CGAddr (unsigned char addr);
void lcd_CGChar (unsigned char cgAddr, unsigned char const * const cgData);
