// implementation file for ETC
//Processor: MC9S12XDP512
//Crystal: 16 MHz
//by Andres Tangarife
//Februaty, 2022
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "SevSeg.h"
uint i;
uint value;
SevSegTypedef *portB;
//portB = &PORTB;

void SevSeg_Init(void){
    //PA0 and PA1 to outputs (DDR register)
    DDRA |= PORTA_PA1_MASK | PORTA_PA0_MASK; 
    //Set Entire port B to output
    DDRB = 0xFF;
    SevSeg_ClearAll();
}

void SevSeg_Char(unsigned char add, unsigned char value){
    
    PORTB = 0b01011000;
/*          ||||||||_____Addr
            |||||________Bank A
            ||||_________Normal
            |||__________Decode  
            ||___________Decode as HEX
            |____________No Data coming
*/
    PORTB |= (add &  0x7F);
    //Port A
    PORTA |= PORTA_PA1_MASK;
    //LATCH
    PORTA &= ~PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
    //Send DATA BYTE

    //Port B
    PORTB = value;
    PORTB |= 0x80;
    

    
    //Port A
    PORTA &= ~PORTA_PA1_MASK; //Set to Data Mode

    //LATCH
    PORTA &= ~ PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
}

void SevSeg_CharD(unsigned char add, unsigned char value){
    PORTB = 0b01011000;
/*          ||||||||_____Addr
            |||||________Bank A
            ||||_________Normal
            |||__________Decode  
            ||___________Decode as HEX
            |____________No Data coming
*/
    PORTB |= (add &  0x7);
    //Port A
    PORTA |= PORTA_PA1_MASK;
    //LATCH
    PORTA &= ~PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
    //Send DATA BYTE

    //Port B
    PORTB = value;
    PORTB &= 0x7F; //No decimal point


    //Port A
    PORTA &= ~PORTA_PA1_MASK; //Set to Data Mode

    //LATCH
    PORTA &= ~ PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
}

void SevSeg_ClearDigit(unsigned char add){
    PORTB = 0b01111000;
/*           |||||||_____Addr
             ||||||________Bank A
             |||||_________Normal
             ||||__________No Decode  
             |||___________Decode as HEX
             ||____________No Data coming
*/
    PORTB |= (add &  0x7);
    //Port A
    PORTA |= PORTA_PA1_MASK;

    //LATCH
    PORTA &= ~PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;

    //Send DATA BYTE

    //Port B
    PORTB = 0x80;
    

    
    //Port A
    PORTA &= ~PORTA_PA1_MASK; //Set to Data Mode

    //LATCH
    PORTA &= ~ PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
}
void SevSeg_ClearTop4(void){
    for (i = 0; i < 4; i++){
        SevSeg_ClearDigit(i);
    }
}
void SevSeg_ClearBot4(void){
    for (i = 4; i < 8; i++){
        SevSeg_ClearDigit(i);
    }
}

void SevSeg_ClearAll(void){
    for (i = 0; i < 8; i++){
        SevSeg_ClearDigit(i);
    }
}

void SevSeg_Top4(unsigned int var){
    
    for ( i = 0; i < 4; i++)
    {
        value = var >> ((3-i)*4);
        SevSeg_Char( i, value);
    }
    
}
void SevSeg_Bot4(unsigned int var){
    
    for ( i = 0; i < 4; i++)
    {
        value = var >> ((3-i)*4);
        SevSeg_Char( (i+4), value);
    }
}

void SevSeg_Cust(unsigned char add, unsigned char segm){
    PORTB = 0b01111000;
/*           |||||||_____Addr
             ||||||________Bank A
             |||||_________Normal
             ||||__________No Decode  
             |||___________Decode as HEX
             ||____________No Data coming
*/
    PORTB |= (add &  0x7);
    //Port A
    PORTA |= PORTA_PA1_MASK;

    //LATCH
    PORTA &= ~PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;

    //Send DATA BYTE

    //Port B
    PORTB = segm;
    
    
    

    
    //Port A
    PORTA &= ~PORTA_PA1_MASK; //Set to Data Mode

    //LATCH
    PORTA &= ~ PORTA_PA0_MASK;
    //Add delay for faster than 8MHZ
    PORTA |= PORTA_PA0_MASK;
}

unsigned int BCDToHex(unsigned int uBcd)
{
    unsigned char i;
    unsigned int result = 0, pow = 1;
    
    for (i = 0; i < 4; i++)
    {
        result += (uBcd % 16) * pow;
        uBcd /= 16; //uBcd >>= 4;
        pow *= 10;
    }
    return result;
}
unsigned int HexToBCD(unsigned int uHex)
{
    unsigned char i;
    unsigned int result = 0, pow = 1;
    
    for (i = 0; i < 4; i++)
    {
        result += (uHex % 10) * pow;
        uHex /= 10;
        pow *= 16;
    }
    return result;
}

void SevSeg_Top4D(unsigned int var, unsigned char add){
    
    for ( i = 0; i < 4; i++)
    {
        value = var >> ((3-i)*4);
        if (i == add)
        {
            SevSeg_CharD( i, value);
        }
        else
        {
            SevSeg_Char( i, value);
        }
    }
    
}
void SevSeg_Bot4D(unsigned int var, unsigned char add){
    
    for ( i = 0; i < 4; i++)
    {
        value = var >> ((3-i)*4);
        if ((i + 4) == add)
        {
            SevSeg_CharD( (i + 4), value);
        }
        else
        {
            SevSeg_Char( (i+4), value);
        }
        
    }
}