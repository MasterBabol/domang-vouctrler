#include "Serial.h"

void UsartInit()
{
	UCSR1A = 0x00;
	UCSR1B = 0b00011000;
	
	UBRR1L = 51; // 19200bps
	UBRR1H = 0;
}

unsigned char UsartSyncRecv()
{
	while(!(UCSR1A & (1 << RXC)));
	return UDR1;
}

void UsartSyncSend(unsigned char data)
{
	while(!(UCSR1A & (1 << UDRE)));
	UDR1 = data;
}

void UsartSyncSendBytesSZ(unsigned char* data)
{
	for(;*data;data++)
		UsartSyncSend(*data);
}

void UsartSyncSendBytes(unsigned char* data, int len)
{
	for(unsigned char i = 0; i < len; i++)
		UsartSyncSend(data[i]);
}

void UsartSyncSendAT(unsigned char* data)
{
	for(;*data;data++)
		UsartSyncSend(*data);
	UsartSyncSend(0x0D);
}

void UsartSyncRecvAT(unsigned char* buf)
{
	buf[0] = UsartSyncRecv();
	buf[1] = UsartSyncRecv();
	for (unsigned char i = 2; ; i++)
	{
		buf[i] = UsartSyncRecv();
		if (buf[i] == 0x0D)
			if ((buf[i+1] = UsartSyncRecv()) == 0x0A)
			{
				buf[i+2] = 0;
				break;
			}				
	}
}