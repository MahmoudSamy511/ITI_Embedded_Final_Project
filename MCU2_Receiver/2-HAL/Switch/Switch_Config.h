
#ifndef SWITCH_CONFIG_H
#define SWITCH_CONFIG_H

#include "../../5-LIB/TYPEDEF.h"


/*Options for Ports: 1- DIO_U8_PORTA
                     2- DIO_U8_PORTB
                     3- DIO_U8_PORTC
                     4- DIO_U8_PORTD 
*/

/*Options For Pins:  1- DIO_U8_PIN0
                     2- DIO_U8_PIN1
                     3- DIO_U8_PIN2
                     4- DIO_U8_PIN3     
                     5- DIO_U8_PIN4    
                     6- DIO_U8_PIN5
                     7- DIO_U8_PIN6
                     8- DIO_U8_PIN7 
*/  

/*Options For State:  1- INPUT_FLOATING
                      2- INPUT_PULLUP
                     
*/  
#define SW_PORT                         DIO_U8_PORTA
#define SW_PIN                          DIO_U8_PIN0
#define SW_INITIAL_STATE                INPUT_PULLUP

#define NUM_OF_SWITCHS                      2

typedef struct {
    u8 SW_Port;
    u8 SW_Pin ;
    u8 SW_Initial_State ;
}SW_t;



#endif