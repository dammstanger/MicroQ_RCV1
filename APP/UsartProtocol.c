#include "UsartProtocol.h"


u8 USART_PkgR[40];
u8 USART_PkgT[40];
u8 USART_PkgR_Len = 0;
u8 USART_PkgT_Len = 0;


u8 USART_UnPkg(u8* rePkg)
{
	u8* head_P=0;
	u8* end_P=0;
	u8 i=0,len=0;
	
	if (USART_Rx_P<2)	return 0;												//确定缓冲区长度
	head_P = memchr(USART_Rx_Buf, 0xff, USART_Rx_P);							//获取首字位置
	while (head_P!=0)
	{
		memmove(USART_Rx_Buf, head_P, USART_Rx_P-(head_P-USART_Rx_Buf));		//清除首字前其他数据
		USART_Rx_P -= (head_P-USART_Rx_Buf);
		head_P = memchr(USART_Rx_Buf+1, 0xff, USART_Rx_P);						//判断是否有第二个首字
		if (head_P==0)															//只有一个首字，继续解包
		{
			end_P = memchr(USART_Rx_Buf, 0xfe, USART_Rx_P);						//获取包尾位置
			if (end_P!=0)
			{
				memmove(rePkg, USART_Rx_Buf, (end_P-USART_Rx_Buf+1));			//获取数据包，从缓冲区中移除
				memmove(USART_Rx_Buf, end_P+1, USART_Rx_P-(end_P-USART_Rx_Buf+1));
				USART_Rx_P -= (end_P-USART_Rx_Buf+1);
				len = (end_P-USART_Rx_Buf+1);
				while (*(rePkg+i)!=0xfe)										//去掉转义字符
				{
					if (*(rePkg+i)==0xf7)
					{
						switch (*(rePkg+i+1))
						{
							case 0x7f:
								*(rePkg+i) = 0xff;
								memmove(rePkg+i+1, rePkg+i+2, len-i);
								len--;
								break;
							case 0x7e:
								*(rePkg+i) = 0xfe;
								memmove(rePkg+i+1, rePkg+i+2, len-i);
								len--;
								break;
							case 0xf7:
								*(rePkg+i) = 0xf7;
								memmove(rePkg+i+1, rePkg+i+2, len-i);
								len--;
								break;
							default:
								break;
						}
					}
					i++;
				}
				USART_PkgR_Len = len;
				memmove(rePkg, rePkg+1, (USART_PkgR_Len-2));					//去除包头包尾
				USART_PkgR_Len -= 2;
				return 1;
			}
			else																//未找到包尾，停止拆包，不清除数据
			{
				return 0;
			}
		}
//		else																	
//		{
//			memmove(USART_Rx_Buf, head_P, USART_Rx_P-(head_P-USART_Rx_Buf));	//发现第二个首字，清除第二个首字前的数据，重新判断数据包
//			USART_Rx_P -= (head_P-USART_Rx_Buf);
//			head_P = USART_Rx_Buf;
//		}
	}
	return 0;
}


u8 USART_AddPkg(u8* dat, u8 len)
{
	u8 i;
	
	if (len>32||len==0)
	{
		USART_PkgT_Len = 0;
		return 0;
	}
	*USART_PkgT = 0xff;
	memmove(USART_PkgT+1, dat, len);
	*(USART_PkgT+len+1) = 0xfe;
	USART_PkgT_Len = len + 2;
	for (i=1;i<USART_PkgT_Len-1;i++)
	{
		switch(*(USART_PkgT+i))
		{
			case 0xff:
				memmove(USART_PkgT+i+2, USART_PkgT+i+1, USART_PkgT_Len-(i+1));
				*(USART_PkgT+i) = 0xf7;	*(USART_PkgT+i+1) = 0x7f;
				USART_PkgT_Len += 1;	i++;
				break;
			case 0xfe:
				memmove(USART_PkgT+i+2, USART_PkgT+i+1, USART_PkgT_Len-(i+1));
				*(USART_PkgT+i) = 0xf7;	*(USART_PkgT+i+1) = 0x7e;
				USART_PkgT_Len += 1;	i++;
				break;
			case 0xf7:
				memmove(USART_PkgT+i+2, USART_PkgT+i+1, USART_PkgT_Len-(i+1));
				*(USART_PkgT+i) = 0xf7;	*(USART_PkgT+i+1) = 0xf7;
				USART_PkgT_Len += 1;	i++;
				break;
			default:
				break;
		}
	}
	return 1;
}


void USART_Connected(void)
{
	const u8 ConStr[10] = {'a','b','c','d','e','f','g','h'};
	
	while(1)
	{
		if (USART_Rx_Flag == SET)
		{
			while (USART_UnPkg(USART_PkgR))
			{
				if (memcmp(USART_PkgR,ConStr,8)==0)
					return;
			}
			USART_Rx_Flag = RESET;
		}
	}
}
