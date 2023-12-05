#include <stdbool.h>
#include <xc.h>
#include "uart.h"

void uart_start(int baudRate){   
    /**
     * @brief Initializes the UART communication with the specified baud rate.
     * @note This function configures the necessary registers for UART communication, including the direction
     * of the Tx and Rx pins. It calculates the value to be loaded in the SPBRG register based on the specified
     * baud rate and the frequency of the microcontroller. The upper and lower bytes of the SPBRG value are then
     * loaded into the SPBRGH and SPBRGL registers, respectively.
     * @param baudRate The desired baud rate for the communication.
     * @return None.
     * @example uart_start(9600);
     */

    /*Function's Variables*/
    unsigned int SPBRGValue = ((_XTAL_FREQ / (4 * baudRate)) - (5/4));

    /*Function's Code*/
    TxPinDirection  = 0;		                    //Sets Tx pin as output.
    RxPinDirection  = 1;		                    //Sets Rx pin as input.
        
    if (baudRate >= 9600) {
        SPBRG = SPBRGValue;                         //Loads the SPBRG value.
    } 
    else {
        SPBRGH = (SPBRGValue >> 8) & 0xFF;          //Loads the upper byte of the SPBRG value.
        SPBRGL          = SPBRGValue & 0xFF;        //Loads the lower byte of the SPBRG value.
    }
    BAUDCON         = 0b01001000;                   //Configures the baud rate.
    TXSTA           = 0b00100100;                   //Enables TX in asynchronous mode. 
    RCSTA           = 0b10010000;                   //Enables RX and the serial port.
}
void uart_printString(char *inputString, bool endOfLineCharacters){
    /**
     * @brief Transmits a string of characters through UART.
     * @note This function will transmit the characters in the input string one by one, waiting for each one to be transmitted before proceeding to the next one.
     * @param inputString Pointer to the string to be transmitted.
     * @param endOfLineCharacters Flag to indicate if the function should append a carriage return and line feed character at the end of the string.
     * @return none.
     * @example uart_printString("Hello World", No_EOL);
     */

    /*Function's Variables*/
    char i = 0;

    /*Function's Code*/
    while (inputString[i] != '\0'){         //If there are characters left to transmit.
        TXREG = inputString[i];             //Transmits a single character.
        while (TXIF != 1);                  //Waits for the transmission of that character to finish
        i++;                                //Shifts the string's array position for the next character.
    }
    if (endOfLineCharacters){               //If End Of Line characters are enabled.
        TXREG = '\r';                       //Appends the carriage return character.
        while (TXIF != 1);                  //Waits for the transmission of that char to finish
        TXREG = '\n';                       //Appends the line feed character.
    }
}
void uart_receiveString(void){
    /**
     * @brief Receives a string through the UART interface and stores it in a buffer.
     * 
     * This function waits for a new character to be received through the UART interface, 
     * then copies it to a buffer. It continues receiving characters until an End Of Line 
     * character is detected, at which point it adds a null terminator to the last character 
     * position of the string. The received string is stored in a global variable named 
     * 'receiverString'.
     * 
     * @note This function blocks execution until the entire string has been received.
     * 
     * @param None
     *
     * @return None
     */

    /*Function's Variables*/
    char stringIndex = 0;   

    /*Function's Code*/                           
    do {
        while (RCIF != 1);                                  //Wait for a new character to be received.
        if ((RCREG != '\r') && (RCREG != '\n')){            //If the character is not an End Of Line character.
            receiverString[stringIndex] = RCREG;            //Copies the received character to the buffer.
        }
        else {                                              //If the character is an End Of Line character.
            receiverString[stringIndex] = '\0';             //Writes a NULL terminator at the last character position of the string.
        }
        stringIndex++;                                      //Shifts the array position for the next character.
    } while (RCREG != '\n');                                //Continues while the received character is not the newline character.
}
void uart_sendChar(char inputCharacter){
    /**
     * @brief Sends a single character via UART communication.
     * @note The char is sent without ASCII conversion.
     * @param inputCharacter The character to be transmitted.
     * @return None.
     */

    TXREG = inputCharacter;                         //Transmits a single char.
    while (TXIF != 1);                              //Waits for the transmission of that char to finish.
}
void uart_printNumber(int inputNumber, bool endOfLine){
    /**
     * @brief Transmits a number as a string of ASCII characters through the UART interface.
     *
     * This function converts the input number into a string of ASCII characters, and then transmits it
     * through the UART interface. The transmitted string will be terminated by a null character.
     *
     * @param inputNumber The number to be transmitted through the UART interface.
     * @param endOfLine If true, appends a carriage return and a line feed character to the end of the transmitted string.
     * @return none
     */
    /*Function's Variables*/
    char currentCharIndex = 0;
    char temp;
    char transmitterString[MaxStringLength];
    char j;
    char k;

    /*Function's Code*/  
    /*Number Splitting and storing into a string as ASCII characters.*/
    while (inputNumber != 0) {                                          //Divides a number into its characters.
        transmitterString[currentCharIndex] = (inputNumber % 10) + '0'; //Stores each character in a string.
        inputNumber /= 10;
        currentCharIndex++;                                             //Shifts the string position for the next character.
    }
    if (currentCharIndex == 0) {                                        //If the number is a 0.
        transmitterString[currentCharIndex] = '0';                      //Writes the ASCII number 0 to the string.
        currentCharIndex++;                                             //Shifts the string position for the null terminator.
    }

    /*Array characters position reversing*/
    for (j = 0, k = currentCharIndex-1; j < k; j++, k--) {              //Reverses the string's characters. EG. [A][B][C]
        temp = transmitterString[j];                                    //       temp = A
        transmitterString[j] = transmitterString[k];                    //  --->[C][B][C]
        transmitterString[k] = temp;                                    //      [C][B][A]<---
    }

    /*End Of Line characters*/
    if (endOfLine){                                                     //If End Of Line characters are enabled
        transmitterString[currentCharIndex] = '\r';                     //Appends the carriage return character.
        transmitterString[currentCharIndex + 1] = '\n';                 //Appends the line feed character.
        currentCharIndex = (currentCharIndex + 2);                      //Shifts the array position for the null terminator.
    }
    transmitterString[currentCharIndex] = '\0';                         //Appends the null terminator at the end.
    
    /*String transmission*/
    currentCharIndex = 0;                                               //Resets the current char index before transmitting.
    while (transmitterString[currentCharIndex] != '\0'){                //If there are chars left to transmit.
        TXREG = transmitterString[currentCharIndex];                    //Transmits a single char.
        while (TXIF != 1);                                              //Waits for the transmission of that char to finish
        currentCharIndex++;                                             //Shifts the string's array position for the next character.
    }
}
bool compareString(const char *referenceString, const char *inputString) {
    /**
     * @brief Compares two strings to check if they are equal.
     * 
     * This function compares two strings by iterating over their characters
     * until the end of the string literal is reached or until a character 
     * mismatch is found. If both strings have the same length and their 
     * characters are equal, it returns true. Otherwise, it returns false.
     *
     * @param referenceString Pointer to the reference string.
     * @param inputString Pointer to the input string.
     * @return True if the two strings are equal, false otherwise.
     */

    /*Function's Variables*/
    char i = 0;                                                                         

    /*Function's Code*/                                                                 //Verifies that the characters are the same until the ...
    while((referenceString[i] == inputString[i]) && (referenceString[i] != '\0')) {     //...end of the string literal. EG.
        i++;                                                                            // "A   B   C"              "X   Y   Z"
    }                                                                                   // [A] [B] [C]      NOT     [A] [B] [C]
    if((referenceString[i] == '\0') && (inputString[i] == '\0')) {                      //If both strings also have the same length.
        return 1;                                                                       // "A   B   C"              "A   B   C"
    }                                                                                   // [A] [B] [C]      NOT     [A] [B] [C] [D] [E]
    else {                                                                              //If the strings are different.
        return 0;
    }
}
