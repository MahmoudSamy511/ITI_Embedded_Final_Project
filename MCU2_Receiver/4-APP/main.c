#include "../1-MCAL/UART/UART_Interface.h"
#include "../1-MCAL/Timers/Timer_Interface.h"
#include "../1-MCAL/DIO/DIO_interface.h"
#include "../1-MCAL/ADC/ADC_Interface.h"
#include "../2-HAL/Servo_Motor/Servo_Interface.h"
#include "../2-HAL/DC_Motor/DC_Motor_Interface.h"
#include "../2-HAL/LCD/LCD_Interface.h"
#include "../1-MCAL/Timer2/timer2.h"

#include <string.h>

void application_init(void);
void Abnormal_state_5Sec(void);
void Emeregency_state(void);
void Normal_state(void);
void checkStringAvalabile(void);
void Timer2_InterruptHandler(void);
void Set_Machine_speed(void);

u16 digitalValue = 0;
u8 received_msg[4];

u8 volatile Flag = 0;
volatile u8 Timer2_Falg;

int main()
{
	application_init();

	while(1)
	{
		UART_enu_receiveStringWithInterrrupt(received_msg,&checkStringAvalabile,3);

		if(Flag == 1)
		{
			if(!strcmp(received_msg,"EM\r"))
			{
				Emeregency_state();
			}
			else if(!strcmp(received_msg,"SM\r"))
			{
				/*  stop the machine */
				LCD_enu_Clear();
				LCD_enu_SendString("Stop Machine");
				DC_Motor_enu_Stop(MOTOR1);
			}
			else if(!strcmp(received_msg,"AB\r"))
			{
				Abnormal_state_5Sec();
			}
			else
			{
				Normal_state();
			}
			Flag = 0;
		}
		else{/* Nothing */}
	}
	return 0;
}

void application_init(void)
{
	DIO_enu_SetPinDirection(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_OUTPUT);
	DIO_enu_SetPinDirection(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_OUTPUT);
	DIO_enu_SetPinDirection(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_OUTPUT);
	DIO_enu_SetPinDirection(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_OUTPUT);
	DIO_enu_SetPinDirection(DIO_U8_PORTB,DIO_U8_PIN3,DIO_U8_OUTPUT);

	Timers_enu_Init(TIMER0,FAST_PWM_NON_INVERTING,CLK_8);
	Timer2_voidInit(Timer2_InterruptHandler);
	UART_enu_Init();
	ADC_enu_Init();

	Servo_enu_Init(1,0);
	DC_Motor_enu_Init(1,0);

	LCD_enu_Init();
	GI_enu_Enable();
	DC_Motor_enu_Rotate(MOTOR1,CW);
}

void checkStringAvalabile(void)
{
	Flag = 1;
}

void Timer2_InterruptHandler(void)
{
	Timer2_Falg = 1;
}

void Set_Machine_speed(void)
{
	u8 speed = 0;
	ADC_enu_GetDigitalValueSynchNonBlocking(ADC_U8_CHANNEL0,&digitalValue);
	speed =(digitalValue * 255UL)/1023;
	Timers_enu_Timer0SetCompareValue(speed);
}

void Abnormal_state_5Sec(void)
{
	LCD_enu_Clear();
	LCD_enu_SendString("Abnormal State");
    /* Count 5 Seconds */
	Timer2_voidStartCounting();
	/* lighten red LED */
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_HIGH);
	/* run the buzzer */
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_HIGH);
	/* open servo with 90 degrees */
	Servo_enu_SetAngle(MOTOR_1, 90);
	/*  stop the machine */
	DC_Motor_enu_Stop(MOTOR1);

	/* Wait 5 Seconds */
	while(0 == Timer2_Falg);
	Timer2_Falg = 0;
	Timer2_voidDisableCounting();

	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_LOW);
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_LOW);
	Servo_enu_SetAngle(MOTOR_1, 0);
}

void Emeregency_state(void)
{
	LCD_enu_Clear();
	LCD_enu_SendString("Emergency State");
	/* lighten red LED & run buzzer */
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_HIGH);
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_HIGH);
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
	DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_LOW);
}

void Normal_state(void)
{
	u8 i = 0;
	u8 temp = 0;
	while(received_msg[i] != '\r'){
		temp = (temp*10) + (received_msg[i] - '0');
		i++;
	}
	LCD_enu_Clear();
	LCD_enu_SendString("Normal State");
	LCD_enu_SetCursor(1, 0);
	LCD_enu_SendString("Temp: ");
	LCD_enu_SetCursor(1, 6);
	LCD_enu_SendNum(temp);

	if(temp < 20)
	{
		/* turn on green led */
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_HIGH);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_LOW);
	}
	else if((temp >= 20)&&(temp <= 40))
	{
		/* turn on yellow led */
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_HIGH);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_LOW);
	}
	else if ((temp > 40)&&(temp <= 50))
	{
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_HIGH);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_LOW);
		DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_LOW);
	}
	else
	{
		Emeregency_state();
	}
}
