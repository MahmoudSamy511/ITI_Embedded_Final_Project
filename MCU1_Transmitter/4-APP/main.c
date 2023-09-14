#include"APP.h"
u8 lastTemp = 0;
u8 state = 0;
int main(void)
{
	APP_Init();
	while(1){
		EEPROM_enu_Read(0,&state);
		if(buttonFlag){
			if((getTemp() >= 40)&&(getTemp() <= 50)){
				UART_enu_sendString("SM");
			}
			buttonFlag = 0;
		}
		switch(state){
		case NORMAL_STATE:{
			u8 temp2  = 0;
			temp2 = getTemp();
			if(lastTemp - temp2){
			UART_enu_sendNUmber(temp2);
			if(temp2 < 20){
				Timers_enu_Stop(TIMER0);
				DIO_enu_SetPinValue(DIO_U8_PORTB,DIO_U8_PIN3,DIO_U8_LOW);
			}else if((temp2 >= 20)&&(temp2 <= 40)){
				Timers_enu_Init(TIMER0,FAST_PWM_NON_INVERTING,CLK_8);
				u8 fanSpeed = ((temp2 *127)/10)-254;
				Timers_enu_TimerSetCompareValue(TIMER0,fanSpeed);
			}else if((temp2 >= 40)&&(temp2 <= 50)){
				Timers_enu_Init(TIMER0,FAST_PWM_NON_INVERTING,CLK_8);
				Timers_enu_TimerSetCompareValue(TIMER0,255);
			}else if(temp2 > 50){
				EEPROM_enu_Write(0,EMERGENCY_STATE);
			}
			lastTemp = temp2;
		}
			break;
		}
		case EMERGENCY_STATE:
			if(!checkFlag)
			{
				Timers_enu_SetTimerValue(TIMER1,57723);
				Timers_enu_SetCallBack(TIMER1,&abnormalDetection,NORMAL_MODE);
				Timers_enu_Init(TIMER1,NORMAL_MODE,CLK_1024);
				checkFlag = 1;
			}
			Timers_enu_TimerSetCompareValue(TIMER0,255);
			break;
		case ABNORMAL_STATE:
			WDT_enu_Enable(TIME_16_3_MS);
			UART_enu_sendString("AB");
			break;
		}
	}
	return 0;
}