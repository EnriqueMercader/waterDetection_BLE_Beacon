#ifndef UART_H
#define	UART_H

#include <stdbool.h>

/*Required Definitions*/
#define TxPinDirection  TRISA4
#define RxPinDirection  TRISA5
#define MaxStringLength 10                  //Maximum number of characters that can be received or send at a once.
#define _XTAL_FREQ 4000000

/*Definitions*/
#define EOL             1                   //Enables End Of Line characters
#define No_EOL          0                   //Disables End Of Line characters

/*Global Variables*/
char receiverString[MaxStringLength];       //Maximum length of a received package.

/*Function's prototypes*/
void uart_receiveString(void);
void uart_start(int baudRate);
void uart_printString(char *inputString, bool endOfLineCharacters);
void uart_sendChar(char inputCharacter);
void uart_printNumber(int inputNumber, bool endOfLine);
bool compareString(const char *referenceString, const char *inputString);

#endif	/* UART_H */

