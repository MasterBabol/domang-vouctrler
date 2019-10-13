﻿#include "Delayi.h"

void Delay_us(unsigned char time_us)
{ 
	register unsigned char i;
	for(i = 0; i < time_us; i++)   // 4 cycle +
	{ 
		asm volatile(" PUSH  R0 ");  // 2 cycle +
		asm volatile(" POP   R0 ");  // 2 cycle +
		asm volatile(" PUSH  R0 ");  // 2 cycle +
		asm volatile(" POP   R0 ");  // 2 cycle +
		asm volatile(" PUSH  R0 ");  // 2 cycle +
		asm volatile(" POP   R0 ");  // 2 cycle = 16 cycle = 1 us for 16MHz
	}
}

void Delay_ms(unsigned int time_ms)  
{
	register unsigned int i;
	for(i = 0; i < time_ms; i++)
	{ 
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
	}
}
