#ifndef BT832_H
#define	BT832_H

#include <stdbool.h>

/*Required Definitions*/
#define bt832_WakeUpPin LATA1

/*Definitions*/
#define UARTOn  "1" //Advertisement and scan stop; UART stays On.
#define AllOff  "2" //Advertisement, scan, and UART Stop.
#define AdvOn   "3" //UART and scan stop; Advertisement stays On.

/*Function's prototypes*/
void bt832_setName(char* name);
void bt832_save(void);
void bt832_advertisingInterval(char* interval);
void bt832_advertisingStart(void);
void bt832_sleep(char* configuration);
void bt832_send(bool package);
void bt832_command(char* command);
void bt832_wakeUp(void);

#endif	/* BT832_H */

