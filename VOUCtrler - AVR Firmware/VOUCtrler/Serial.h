#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

void UsartInit();
unsigned char UsartSyncRecv();
void UsartSyncSend(unsigned char data);

void UsartSyncSendBytesSZ(unsigned char* data);
void UsartSyncSendBytes(unsigned char* data, int len);
void UsartSyncSendAT(unsigned char* data);
void UsartSyncRecvAT(unsigned char* buf);

#endif