#include "../../5-LIB/ERROR_STATE.h"
#include "../../5-LIB/TYPEDEF.h"
#include "../../1-MCAL/DIO/DIO_interface.h"
#include "Switch_Config.h"
#include "Switch_Private.h"



SW_t switches[NUM_OF_SWITCHS]={ {DIO_U8_PORTA,DIO_U8_PIN1,INPUT_FLOATING},
                                {DIO_U8_PORTC,DIO_U8_PIN5,INPUT_FLOATING}

};
