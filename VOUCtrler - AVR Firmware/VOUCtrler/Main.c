#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "Delayi.h"
#include "Serial.h"
#include "AnalogToDigital.h"

typedef struct
{
	unsigned char x, y, z;
	unsigned char reading;
} Vector16;

static Vector16 accelVector = {0};

int main(void)
{
	char sendBuf[64];
	unsigned char c;
	
	DDRC = 0; // SW INPUTS
	
	DDRD = 1; // BT MODULE, SERIALDATA, x, FARESET
	PORTD |= 1; // disable fb155 factory reset
	
	DDRF = 0b111000; // ACC MODULE, SLEEPMODE OP2 OP1 ADC210
	PORTF |= 0b00100000; // NONE SLEEPMODE, DEFAULT 0 0
	// 0 0 1.5g 800mV/g
	// 0 1 2g 600mV/g
	// 1 0 4g 300mV/g
	// 1 1 6g 200mV/g
	
	ADCInit();
	UsartInit();
	sei();
	
	Delay_ms(1000);
	UsartSyncSendAT("ATZ");
	Delay_ms(3000);
	
	UsartSyncSendAT("AT+BTKEY=1234");
	Delay_ms(1000);
	
	UsartSyncSendAT("AT+BTNAME=VOUCtrler");
	Delay_ms(1000);
	
	ADCSR |= (1 << ADSC);
	UsartSyncSendAT("AT+BTSCAN");
	
	for(;;)
	{
		c = UsartSyncRecv();
		switch(c)
		{
		case 'S':
			{
				if (UsartSyncRecv() == 'T')
				if (UsartSyncRecv() == 'A')
				if (UsartSyncRecv() == 'T')
				{
					UsartSyncSendBytesSZ("OK\r\n");
				}
			}			
			break;
		case 'I':
			{
				if (UsartSyncRecv() == 'N')
				if (UsartSyncRecv() == 'F')
				if (UsartSyncRecv() == 'O')
				{
					sendBuf[0] = '@';
					sendBuf[1] = accelVector.x;
					sendBuf[2] = accelVector.y;
					sendBuf[3] = accelVector.z;
					sendBuf[4] = PINC;
					sendBuf[5] = (PORTF & 0b11000) >> 3;
					sendBuf[6] = -(sendBuf[0]+sendBuf[1]+sendBuf[2]+sendBuf[3]+sendBuf[4]+sendBuf[5]);
					sendBuf[7] = '#';
					UsartSyncSendBytes(sendBuf, 8);
				}
			}			
			break;
		case 'A':
			{
				unsigned char d;
				if (UsartSyncRecv() == 'C')
				if (UsartSyncRecv() == 'C')
				if (UsartSyncRecv() == 'M')
				{
					d = UsartSyncRecv();
					switch(d)
					{
					case 'A':
						PORTF &= ~0b11000;
						PORTF |= 0b00000; // Mode 0 0
						break;
					case 'B':
						PORTF &= ~0b11000;
						PORTF |= 0b01000; // Mode 0 1
						break;
					case 'C':
						PORTF &= ~0b11000;
						PORTF |= 0b10000; // Mode 1 0
						break;
					case 'D':
						PORTF &= ~0b11000;
						PORTF |= 0b11000; // Mode 1 1
						break;
					}
				}
			}
		}
	}
}

SIGNAL(SIG_ADC)
{
	switch(accelVector.reading)
	{
	case 0:
		accelVector.x = ADCH;
		ADMUX = (1 << REFS0) | (1 << ADLAR) + 1;
		break;
	case 1:
		accelVector.y = ADCH;
		ADMUX = (1 << REFS0) | (1 << ADLAR) + 2;
		break;
	case 2:
		accelVector.z = ADCH;
		ADMUX = (1 << REFS0) | (1 << ADLAR);
		break;
	}
	accelVector.reading = (accelVector.reading + 1) % 3;
	ADCSR |= (1 << ADSC);
}
