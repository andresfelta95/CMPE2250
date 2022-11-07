// SCI library
// Last Modified : June 08 2015
// Revision History:
// May 13th 2014 - Initial Port to C - Simon Walker
// June 8th 2015 - Genric BAUD init added, others removed except for IRDA
// May 23rd 2019 - cleanup, real use of derivative file definitions
// Mar 15th 2022 - cleanup, added interrupt (RDRF only) to init
//               - added ISR template to header


//interrupt VectorNumber_Vsci0 void ISR_SCI0(void)
//{
  // only once source of interrupt! RIE == RDRF, no need to check!
  // your blocking read must (will) test RDRF and read data
  // so flag is cleared!
  // x = sci0_bread();  
//}

// sci 0 - normal mode ********************************
// set baud, returns actual baud
unsigned long sci0_Init (unsigned long ulBusClock, unsigned long ulBaudRate, int iRDRF_Interrupt);

// read a byte, non-blocking
// returns 1 if byte read, 0 if not
int sci0_read (unsigned char * pData);

// blocking byte read
// waits for a byte to arrive and returns it
unsigned char sci0_bread (void);

// send a byte over SCI
void sci0_txByte (unsigned char data);

// send a null-terminated string over SCI
void sci0_txStr (char const * straddr);

// receive a string from the SCI
// up to buffer size-1 (string always NULL terminated)
// number of characters is BufferSize minus one for null
// once user enters the max characters, null terminate and return
// if user enters 'enter ('\r')' before-hand, return with current entry (null terminated)
// echo valid characters (non-enter) back to the terminal
// return -1 on any error, otherwise string length
int sci0_rxStr (char * const pTarget, int BufferSize);
// sci 0 - normal mode ********************************

// these items are optional, for other SCI ports
// sci 1 - IRDA mode **********************************
void sci1_Init (void);
void sci1_txByte (unsigned char data);
int sci1_read (unsigned char * pData);
// sci 1 - IRDA mode **********************************

// sci 2 - normal mode - shared with port J interrupts
unsigned long sci2_Init (unsigned long iBaudRate);
int sci2_read (unsigned char * pData);
// sci 2 - normal mode - shared with port J interrupts

