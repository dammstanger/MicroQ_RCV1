#ifndef __USARTPROTOCOL_H__
#define __USARTPROTOCOL_H__


#include "hardware.h"
#include "string.h"


extern u8 USART_PkgR[40];
extern u8 USART_PkgT[40];
extern u8 USART_PkgR_Len;
extern u8 USART_PkgT_Len;


u8 USART_UnPkg(u8* rePkg);
u8 USART_AddPkg(u8* dat, u8 len);
void USART_Connected(void);


#define USART_SendPkg(buf, len)								\
{															\
	USART_AddPkg(buf, len);									\
	USART_Send_Buf(USART1, USART_PkgT, USART_PkgT_Len);		\
}

#endif
