#include"APP.h"
int main()
{
	APP_Init();
	while(1)
	{
		Set_Machine_speed();
		UART_enu_receiveStringWithInterrrupt(received_msg,&StringInterruptHandler,3);
		if(strFlag)
		{
			if(!(strCmp(received_msg,"SM\r")))		{Stop_Machine();}
			else if(!(strCmp(received_msg,"AB\r")))	{Abnormal_state_5Sec();}
			else									{Normal_state();}
			strFlag = 0;
		}
	}
	return 0;
}
