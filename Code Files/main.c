#include "configurationBits.c"
#include "uart.h"
#include "bt832.h"
#include <xc.h> 
#include <stdbool.h>
#define _XTAL_FREQ 4000000

#define ledPin LATA2
void ledBlink(void){
    ledPin = 1;              
    __delay_ms(500);
    ledPin = 0;              
}

void main(void) {
    /*Register configuration*/
    OSCCON = 0b11101011;    //Sets the internal oscillator at 4MHz.
    WDTCON = 0b00010100;    //Sets Watchdog interval for 1 second.
    ANSELA = 0;             //Makes all pins digital I/O.
    RXDTSEL = 1;            //RX Function is on RA5.
    TXCKSEL = 1;            //TX Function is on RA4.

    TRISA0 = 1;             //Sets Pin 0 as a digital input (sensor).
    TRISA1 = 0;             //Sets Pin 1 as a digital output (bt832 UART wakeup).
    TRISA2 = 0;             //Sets Pin 2 as a digital output (led).
    LATA1 = 0;              //Pulls the bt832's GPIO26 Pin low to wake up the UART.
    LATA1 = 1;              //Pulls the bt832's GPIO26 Pin high.

    /*Initialization*/
    ledBlink();
    uart_start(9600);                   //Starts the uart at a baud rate of 9600.
    bt832_command("AT");                //Cleans the bt832's receiver buffer.
    bt832_setName("Water");
    bt832_advertisingInterval("0100");  //Sets the advertising interval as 100ms.
    bt832_save();                       //Saves the configuration to the nonvolatile memory.

    while(1){
        bt832_send(RA0);                //Sends the value of the sensor to the antenna.
        bt832_advertisingStart();       //Starts broadcasting.
        __delay_ms(200);                //Broadcasts for 200 ms.
        bt832_sleep(AllOff);            //Stops broadcasting and turns off the bt832's uart.
        SLEEP();                        //Sends the microcontroller to sleep.
        bt832_wakeUp();                 //Reactivates the bt832's uart.
    }
    return;
}