/******************************************************************
**********************  Author: Ehab Magdy       ******************
**********************	File  : timer2.h         ******************
**********************	Date  : 09 Sep, 2023     ******************
*******************************************************************/

#ifndef TIMER2_H
#define TIMER2_H

#include"../../5-LIB/TYPEDEF.h"

void Timer2_voidInit(void (*Copy_PFvoidName)(void));
void Timer2_voidStartCounting(void);
void Timer2_voidDisableCounting(void);

#endif /* TIMER2_H */
