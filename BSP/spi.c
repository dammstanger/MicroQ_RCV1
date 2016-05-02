//20141222
#include "spi.h"

//SPI ���� 

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
	{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1, ENABLE );

	//����ֻ���SPI�ڳ�ʼ��
	//PA5.6.7���� 	    
    //PA5.6.7����
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
		
	/* SPI1 configuration */	
	#if 1	/*  configuration for 24L01 and etc*/												
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//���ݲ����ڵ�һ��ʱ����	(��24L01Ҫ���� CPHA=0;CPOL=0;)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	#endif
	#if 0
	/*  configuration for ADNS3080 and etc*/	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//����ģʽ��SCKΪ1 CPOL=1//��ADNS3080Ҫ���� CPHA=1;CPOL=1;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	#endif
	
	SPI_Init(SPI1, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI1_SetSpeed(SPI_SPEED_32);
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	}  
 
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_4   4��Ƶ   (SPI 18M@sys 72M)
//SPI_SPEED_8  	8��Ƶ  	(SPI 9M@sys 72M)
//SPI_SPEED_16 	16��Ƶ 	(SPI 4.5@sys 72M)
//SPI_SPEED_32  32��Ƶ  (SPI 2.25M@sys 72M)
//SPI_SPEED_64  64��Ƶ  (SPI 1.125M@sys 72M)
//SPI_SPEED_128 128��Ƶ (SPI 562.5K@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPI1_SetSpeed(u8 SpeedSet)
	{
	SPI_Cmd(SPI1, DISABLE); //�ȹر�SPI���裬���޸ġ����ڹ���ʱ�ǸĲ��˵�  

	SPI1->CR1&=0XFFC7;		
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
		{
		SPI1->CR1|=0<<3;//FPCLK=PCLK/2=36Mhz
		}
	else if(SpeedSet==SPI_SPEED_4)//�˷�Ƶ 
		{
		SPI1->CR1|=1<<3;//FPCLK=PCLK/8=9Mhz	
		}
	else if(SpeedSet==SPI_SPEED_8)//ʮ����Ƶ
		{
		SPI1->CR1|=2<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_16)//�˷�Ƶ 
		{
		SPI1->CR1|=3<<3;//FPCLK=PCLK/8=9Mhz	
		}
	else if(SpeedSet==SPI_SPEED_32)//ʮ����Ƶ
		{
		SPI1->CR1|=4<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_64)//ʮ����Ƶ
		{
		SPI1->CR1|=5<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_128)//ʮ����Ƶ
		{
		SPI1->CR1|=6<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else			 	 //256��Ƶ
		{
		SPI1->CR1|=7<<3; //FPCLK=PCLK/256=281.25Khz ����ģʽ
		}
	//SPI1->CR1|=1<<6; //SPI�豸ʹ��	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����  
	} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
	{		
	u8 retry=0;				 
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����				    
	}

/**************************************************************SPI2*******************************************************************/


//SPI2 ���� 

//������SPI2ģ��ĳ�ʼ�����룬���ó�����ģʽ��						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
	{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2, ENABLE );		//FPCLK2 =72Mhz

	//����ֻ���SPI�ڳ�ʼ��
	//PA5.6.7���� 	    
    //PA5.6.7����
	/* Configure SPI2 pins: SCK 13, MISO 14and MOSI 15*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);

	/* SPI2 configuration */
	#if 0	/*  configuration for 24L01 and etc*/												
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//���ݲ����ڵ�һ��ʱ����	(��24L01Ҫ���� CPHA=0;CPOL=0;)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	#endif
	#if 1
	/*  configuration for ADNS3080 and etc*/	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//����ģʽ��SCKΪ1 CPOL=1//��ADNS3080Ҫ���� CPHA=1;CPOL=1;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	#endif
	
	SPI_Init(SPI2, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

	}  
 
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_4   4��Ƶ   (SPI 18M@sys 72M)
//SPI_SPEED_8  	8��Ƶ  	(SPI 9M@sys 72M)
//SPI_SPEED_16 	16��Ƶ 	(SPI 4.5@sys 72M)
//SPI_SPEED_32  32��Ƶ  (SPI 2.25M@sys 72M)
//SPI_SPEED_64  64��Ƶ  (SPI 1.125M@sys 72M)
//SPI_SPEED_128 128��Ƶ (SPI 562.5K@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPI2_SetSpeed(u8 SpeedSet)
	{
	SPI_Cmd(SPI2, DISABLE); //�ȹر�SPI���裬���޸ġ����ڹ���ʱ�ǸĲ��˵�  

	SPI2->CR1&=0XFFC7;		
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
		{
		SPI2->CR1|=0<<3;//FPCLK=PCLK/2=36Mhz	
		}
	else if(SpeedSet==SPI_SPEED_4)//�˷�Ƶ 
		{
		SPI2->CR1|=1<<3;//FPCLK=PCLK/8=9Mhz	
		}
	else if(SpeedSet==SPI_SPEED_8)//ʮ����Ƶ
		{
		SPI2->CR1|=2<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_16)//�˷�Ƶ 
		{
		SPI2->CR1|=3<<3;//FPCLK=PCLK/8=9Mhz	
		}
	else if(SpeedSet==SPI_SPEED_32)//ʮ����Ƶ
		{
		SPI2->CR1|=4<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_64)//ʮ����Ƶ
		{
		SPI2->CR1|=5<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else if(SpeedSet==SPI_SPEED_128)//ʮ����Ƶ
		{
		SPI2->CR1|=6<<3;//FPCLK=PCLK/16=4.5Mhz
		}
	else			 	 //256��Ƶ
		{
		SPI2->CR1|=7<<3; //FPCLK=PCLK/256=281.25Khz ����ģʽ
		}
	//SPI2->CR1|=1<<6; //SPI�豸ʹ��	
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����  
	} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
	{		
	u8 retry=0;				 
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPI2������յ�����				    
	}


	
