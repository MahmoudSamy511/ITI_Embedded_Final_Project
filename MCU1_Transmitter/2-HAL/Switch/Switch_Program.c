#include "../../1-MCAL/DIO/DIO_interface.h"

#include "../../5-LIB/ERROR_STATE.h"
#include "../../5-LIB/TYPEDEF.h"
#include<util/delay.h>
#include "Switch_Config.h"
#include "Switch_Private.h"


ES_t SW_enu_Init(SW_t * Copy_Au8_Switchs)
{

    for(u8 i = 0 ; i < NUM_OF_SWITCHS;i++ ){
    DIO_enu_SetPinDirection(Copy_Au8_Switchs[i].SW_Port, Copy_Au8_Switchs[i].SW_Pin, DIO_U8_INPUT);
    DIO_enu_SetPinValue(Copy_Au8_Switchs[i].SW_Port, Copy_Au8_Switchs[i].SW_Pin, Copy_Au8_Switchs[i].SW_Initial_State);
    }

}

ES_t SW_enu_getValue(u8 * Copy_u8_Value,SW_t * Copy_Au8_Switchs)
{

    DIO_enu_GetPinValue(Copy_Au8_Switchs->SW_Port, Copy_Au8_Switchs->SW_Pin,Copy_u8_Value);
    _delay_ms(25);
    DIO_enu_GetPinValue(Copy_Au8_Switchs->SW_Port, Copy_Au8_Switchs->SW_Pin,Copy_u8_Value);
    
}
