/**********************RECEIVER********************/
#ifndef APP_H
#define APP_H

#include "../1-MCAL/UART/UART_Interface.h"
#include "../1-MCAL/Timers/Timer_Interface.h"
#include "../1-MCAL/GI/GI_Interface.h"
#include "../1-MCAL/DIO/DIO_interface.h"
#include "../1-MCAL/ADC/ADC_Interface.h"

#include "../2-HAL/Servo_Motor/Servo_Interface.h"
#include "../2-HAL/DC_Motor/DC_Motor_Interface.h"
#include "../2-HAL/LCD/LCD_Interface.h"

extern volatile u8  strFlag;
extern u8 received_msg[4];

 #define  F_CPU   16000000UL
 #include <util/delay.h>

void APP_Init(void);
void Set_Machine_speed(void);
void Stop_Machine(void);
void Normal_state(void);
void Emeregency_state(void);
void Abnormal_state_5Sec(void);
void StringInterruptHandler(void);
void Timer2_InterruptHandler(void);
u8 strCmp(u8 *str1, u8 *str2);




#endif
