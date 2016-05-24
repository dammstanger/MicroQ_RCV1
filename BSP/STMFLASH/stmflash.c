
/******************** (C) COPYRIGHT 2014 DammStanger  ********************
 * �ļ���  ��stmflash.c
 * ��汾  ��ST3.0.0
 * ����     ��fire  QQ:1534364663
 * �޸����ڣ�2014.2.13
**********************************************************************************/

#include "stmflash.h"


void STMFLASH_Delayus(u32 n)
{
	u32 i;
	while(n--)
		for(i=100;i>0;i--);
}

 //�õ�FLASH״̬
 u8 STMFLASH_GetStatus(void)
 {	
 	u32 res;		
 	res=FLASH->SR; 
 	if(res&(1<<0))return 1;		    //æ
 	else if(res&(1<<2))return 2;	//��̴���
 	else if(res&(1<<4))return 3;	//д��������
 	return 0;						//�������
 }
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
 u8 STMFLASH_WaitDone(u16 time)
 {
 	u8 res;
 	do
 	{
 		res=STMFLASH_GetStatus();
 		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
 		STMFLASH_Delayus(1);
 		time--;
 	 }while(time);
 	 if(time==0)res=0xff;//TIMEOUT
 	 return res;
 }

// ��FLASHָ����ַд�����
// faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
// dat:Ҫд�������
// ����ֵ:д������
 u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
 {
 	u8 res;	   	    
 	res=STMFLASH_WaitDone(0XFF);	 
 	if(res==0)//OK
 	{
 		FLASH->CR|=1<<0;//���ʹ��
 		*(vu16*)faddr=dat;//д������
 		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
 		if(res!=1)//�����ɹ�
 		{
 			FLASH->CR&=~(1<<0);//���PGλ.
 		}
 	} 
 	return res;
 } 
//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)

#if STM32_FLASH_SIZE<256		
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048//��������Ʒ
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����1K�ּ�2K�ֽ�
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();
	//STMFLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  ������������0λ���ܱ�д1���ò�����ȷ�洢  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			//STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}









