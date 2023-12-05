#include <stdbool.h>
#include <xc.h>
#include "uart.h"
#include "bt832.h"


void bt832_setName(char* name){
    /**
     * @brief Sets the name of the BT832 module through the UART interface.
     * 
     * This function sets the name of the BT832 module to the given string through the UART interface. 
     * It sends the "AT+NAME=" command followed by the given name and the end of line character, 
     * then waits for a response through the UART interface before returning.
     * 
     * @note This function blocks execution until a response is received.
     * 
     * @param name: A pointer to a string containing the desired name for the BT832 module.
     * 
     * @return None.
     */
    uart_printString("AT+NAME=",No_EOL);
    uart_printString(name,EOL);
    uart_receiveString();                   //Waits for a response from the module to ensure that the operation was successful.
}
void bt832_save(void){
    /**
     * @brief Saves the current settings to non-volatile memory.
     * 
     * This function sends the "AT+SAVE=1" command to the BT832 module to save the current settings 
     * to non-volatile memory. It waits for a response from the module to ensure that the operation 
     * was successful.
     * 
     * @param None
     *
     * @return None
     */
    uart_printString("AT+SAVE=1",EOL);  //Sends the command to save the current settings to non-volatile memory.
    uart_receiveString();               //Waits for a response from the module to ensure that the operation was successful.
}
void bt832_advertisingInterval(char* interval){
    /**
     * @brief Sets the advertising interval of the BT832 module.
     *
     * This function sets the advertising interval of the BT832 module by sending the "AT+ADINTERVAL" command
     * with the desired interval as an argument.
     *
     * @param interval A string containing the desired advertising interval. Example "0100" for 100ms.
     *
     * @return None
     */
    uart_printString("AT+ADINTERVAL=",No_EOL);  //Sends the command to set the advertising interval without an end of line.
    uart_printString(interval,EOL);             //Sends the desired interval as an argument with an end of line.
    uart_receiveString();                       //Waits for a response from the module to ensure that the operation was successful.
}
void bt832_advertisingStart(void){
    uart_printString("AT+ADSTART=",No_EOL);
    uart_sendChar(5);                           //Length: (always 5)
    uart_sendChar(255);                         //Timeout: 255 = run forever
    uart_sendChar(1);                           //Name: Advertising with name
    uart_sendChar(1);                           //Phy: BLE GAP PHY 1MBPS
    uart_sendChar(255);                         //Channel: Use channels 37, 38, & 39.
    uart_sendChar(50);                          //Advertisement type: Nonconnectable & scannable
    uart_receiveString();
}
void bt832_sleep(char* configuration){
    /**
     * @brief Sends a command to put the BT832 module to sleep with the specified configuration.
     * 
     * This function sends a command to the BT832 module to put it to sleep with the specified configuration. 
     * The possible configurations are:
     * - UARTOn: Stops advertisement and scanning, but keeps UART communication active.
     * - AllOff: Stops advertisement, scanning, and UART communication.
     * - AdvOn: Stops UART communication and scanning, but keeps advertisement active.
     * 
     * The function blocks execution until the BT832 module responds to the command.
     * 
     * @note This function only sends the command to the BT832 module, it does not perform any sleep operation itself.
     * 
     * @param configuration A string indicating the desired configuration for the BT832 module sleep mode. 
     *                      Valid values are: UARTOn, AllOff, and AdvOn.
     *
     * @return None
     */
    uart_printString("AT+STOP=",No_EOL);    //Sends the command to the BT832 module to put it to sleep.
    uart_printString(configuration,EOL);    //Sends the specified configuration option.
    uart_receiveString();                   //Waits for a response from the BT832 module to confirm the operation's success.
}
void bt832_send(bool package){
    uart_printString("AT+ADVMANU=",No_EOL);
        uart_sendChar(2);
        uart_sendChar(!package);
        uart_sendChar(0);
        uart_receiveString();
}
void bt832_command(char* command){
    uart_printString(command,EOL);
    uart_receiveString();
}
void bt832_wakeUp(void){
    bt832_WakeUpPin = 0;    //Pulls the bt832's GPIO26 Pin low to wake up the UART.
    uart_receiveString();
    bt832_WakeUpPin = 1;    //Pulls the bt832's GPIO26 Pin high.
}

