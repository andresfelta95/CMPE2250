//Seven Segment Display Controller Library
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by Carlos Estay
//September 2020


#define DEGREE_SIGN SEG_A | SEG_B | SEG_F | SEG_G
#define SEG_DP 0b00000000

#define SEG_A 0b01000000
#define SEG_B 0b00100000 
#define SEG_C 0b00010000 
#define SEG_E 0b00001000 
#define SEG_F 0b00000010 
#define SEG_G 0b00000100 
#define SEG_D 0b00000001 

#define NO_DP 0b10000000


typedef union __SevSegTypedef
{
  unsigned char Byte;
  struct SevenSeg_Bits
  {
    unsigned char ADDR:3;
    unsigned char BANK:1;
    unsigned char SDWN:1;
    unsigned char DEC:1;
    unsigned char HEX_B:1;
    unsigned char S_M:1;
  }Bits;
}SevSegTypedef;

typedef union __Seg16Typedef
{
  unsigned int Word;

  struct Seg16_Byte
  {//This is because our m,icro is Little Endian (LSB first)
    unsigned int Byte1:8;
    unsigned int Byte0:8;  
  }Byte;

  struct Seg16_Nibble
  {//This is because our m,icro is Little Endian (LSB first)
    unsigned int Nibble2:4;
    unsigned int Nibble3:4;
    unsigned int Nibble0:4;
    unsigned int Nibble1:4;
  }Nibble;
  
}Seg16Typedef;


void SevSeg_Init(void); //Initialize the ports to operate with the 7-Seg Display
void SevSeg_Char(unsigned char, unsigned char);		//Digit address, character
void SevSeg_CharD(unsigned char, unsigned char);	//Digit address, character with decimal point 
void SevSeg_ClearDigit(unsigned char);			   //Digit address
void SevSeg_ClearTop4(void); 
void SevSeg_ClearBot4(void);
void SevSeg_ClearAll(void);                        //Clear all digits
void SevSeg_Top4(unsigned int);						//Four chars compressed as four nibbles in an int
void SevSeg_Bot4(unsigned int);						//Four chars compressed as four nibbles in an int
void SevSeg_Cust(unsigned char, unsigned char);		//Digit address, selected segments
unsigned int HexToBCD(unsigned int uHex);
unsigned int BCDToHex(unsigned int uBcd);
char TryBCDToHex(unsigned int src, unsigned int* dest);
char TryHexToBCD(unsigned int src, unsigned int* dest);
void SevSeg_Top4D(unsigned int, unsigned char);					
void SevSeg_Bot4D(unsigned int, unsigned char);	



