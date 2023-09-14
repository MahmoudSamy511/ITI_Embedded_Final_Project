#include"APP.h"
u8 volatile checkFlag = 0;
u8 volatile buttonFlag = 0;
void APP_Init(void){
	GI_enu_Enable();
	I2C_enu_Init();
	EEPROM_enu_Write(0,NORMAL_STATE);
	DIO_enu_SetPinDirection(DIO_U8_PORTB,DIO_U8_PIN3,DIO_U8_OUTPUT);
	UART_enu_Init();
	ADC_enu_Init();
	Timers_enu_Init(TIMER0,FAST_PWM_NON_INVERTING,CLK_8);
	EXTI_enu_setCallBack(&pressButton,EXTI0);
	EXTI_enu_Enable(EXTI0,RISING_EDGE);
	_delay_ms(150);
}
u8 getTemp(void){
	u16 digitalValue = 0;
	ADC_enu_GetDigitalValueSynchNonBlocking(ADC_U8_CHANNEL0,&digitalValue);
	return ((digitalValue*500UL)/1023);
}
void pressButton(void){
	buttonFlag = 1;
}
void abnormalDetection(void)
{
    static u8 counter = 0;
	counter++;
	Timers_enu_SetTimerValue(TIMER1,57723);
	if(getTemp()<50){
		EEPROM_enu_Write(0,NORMAL_STATE);
		Timers_enu_Stop(TIMER1);
		checkFlag = 0;
	}
	if(14==counter){
		Timers_enu_Stop(TIMER1);
		checkFlag = 0;
		EEPROM_enu_Write(0,ABNORMAL_STATE);
	}
}