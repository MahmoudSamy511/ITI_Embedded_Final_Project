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
void follow_abnormal_5Sec(void);

u16 digitalValue = 0;

u8 volatile Flag = 0;
void checkStringAvalabile(void){Flag = 1;}

volatile u8 Timer2_Falg;
void Timer2_InterruptHandler(void)
{
	Timer2_Falg = 1;
}

int main()
{
	application_init();
	GI_enu_Enable();
	DC_Motor_enu_Rotate(MOTOR1,CW);

	u8 str[4];

	while(1)
	{
		u8 speed = 0;
		ADC_enu_GetDigitalValueSynchNonBlocking(ADC_U8_CHANNEL0,&digitalValue);
		speed =(digitalValue * 255UL)/1023;
		Timers_enu_Timer0SetCompareValue(speed);

		UART_enu_receiveStringWithInterrrupt(str,&checkStringAvalabile,3);

		if(Flag == 1){
			LCD_enu_Init();
			LCD_enu_SendString(str);
			if(!strcmp(str,"EM\r"))
			{
				DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_HIGH);
				DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_HIGH);
				DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
				DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_LOW);
			}
			else if(!strcmp(str,"SM\r"))
			{
				/*  stop the machine */
				DC_Motor_enu_Stop(MOTOR1);
			}
			else if(!strcmp(str,"AB\r"))
			{
				follow_abnormal_5Sec();
			}
			else
			{
				u8 i = 0;
				u8 temp = 0;
				while(str[i] != '\r'){
					temp = (temp*10) + (str[i] - '0');
					i++;
				}
				LCD_enu_Init();
				LCD_enu_SendNum(temp);
				if(temp < 20)
				{
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_HIGH);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_LOW);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_LOW);
				}
				else if((temp >= 20)&&(temp <= 40))
				{
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
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN0,DIO_U8_HIGH);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN3,DIO_U8_HIGH);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN1,DIO_U8_LOW);
					DIO_enu_SetPinValue(DIO_U8_PORTC,DIO_U8_PIN2,DIO_U8_LOW);
				}
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
}

void follow_abnormal_5Sec(void)
{
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
}
