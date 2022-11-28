// implementation file for DCI
// Processor: MC9S12XDP512
// Crystal: 16 MHz
// by Andres Tangarife
// November, 2022

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "math.h"
#include "Clock.h"
#include "ECT.h"
#include "sci.h"

unsigned long sci0_Init (unsigned long ulBusClock, unsigned long ulBaudRate, int iRDRF_Interrupt)
{
    //  Get the SCI0 baud rate divisor
    unsigned long  uBaudValue = ulBusClock/16/ulBaudRate;

    // set the BAUD rate (assume 9600 BAUD at 20MHz bus)
    SCI0BD = uBaudValue;
    // turn on TX and RX module 11.3.2.6
    SCI0CR2 = 0b00001100;
    // same as:
    SCI0CR2_TE = 1;
    SCI0CR2_RE = 1;
    return uBaudValue;  
}

// read a byte, non-blocking
// returns 1 if byte read, 0 if not
int sci0_read (unsigned char * pData)
{
    // no byte to read?
    if (!SCI0SR1_RDRF)
        return -1;

    // return the data through the pointer
    *pData = SCI0DRL;

    return 0;
}

// blocking byte read
// waits for a byte to arrive and returns it
unsigned char sci0_bread (void)
{
    // wait for RDRF! (Could be forever!)
    while (!SCI0SR1_RDRF)
        ;

    // read and return the byte
    return SCI0DRL;
}

// send a byte over SCI
void sci0_txByte (unsigned char data)
{
    // wait for TDRE to be high (transmit buffer ready for data)
    while (!SCI0SR1_TDRE);

    // send the byte
    SCI0DRL = data;
}

// send a null-terminated string over SCI
void sci0_txStr (char const * straddr)
{
    // whilst the pointer not at NUL point
    while (*straddr)
    {
        // fire out the target of the pointer (each character)
        sci0_txByte (*straddr);

        // move on to the next character by advancing pointer
        straddr++;
    }
}

// receive a string from the SCI
// up to buffer size-1 (string always NULL terminated)
// number of characters is BufferSize minus one for null
// once user enters the max characters, null terminate and return
// if user enters 'enter ('\r')' before-hand, return with current entry (null terminated)
// echo valid characters (non-enter) back to the terminal
// return -1 on any error, otherwise string length
int sci0_rxStr (char * const pTarget, int BufferSize)
{
    int i = 0;
    char c;

    // loop until we get a carriage return or buffer is full
    while (i < BufferSize-1)
    {
        // read a byte
        c = sci0_bread();

        // if it's a carriage return, we're done
        if (c == '\r')
            break;

        // echo the character back to the terminal
        sci0_txByte(c);

        // store the character in the buffer
        pTarget[i++] = c;
    }

    // null terminate the string
    pTarget[i] = '\0';

    // return the string length
    return i;
}

