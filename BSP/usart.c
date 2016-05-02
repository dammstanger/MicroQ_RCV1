//20141222
#include "usart.h"


void USART1_Configuration(void)                        //���ڳ�ʼ������
{
 
	USART_InitTypeDef   USART_InitStructure;//�������ûָ�Ĭ�ϲ���
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//GPIO�����趨��Ӧ���ڹܽ�ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//�ܽ�9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//TX��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//�ܽ�10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ����������
	USART_InitStructure.USART_BaudRate=115200;//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ֳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1λֹͣ�ֽ�
	USART_InitStructure.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//��Rx���պ�Tx���͹���
	
	USART_Init(USART1,&USART_InitStructure);//��ʼ��
	USART_Cmd(USART1,ENABLE);//��������
}


void USART1_RX_IntteruptEnable(USART_TypeDef* USARTx)		//�����ж�ʹ��
{
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	 //�ж�ʹ��
}

/********************************************************************************
 * ��������USART2_Configuration
 * ����  ��USART2 ����
 * ����  ��-		    	
 * ����  ��- 
 * ����  ���ⲿ����
 ********************************************************************************/
void USART2_Configuration(void)                        //���ڳ�ʼ������
{
 
	USART_InitTypeDef   USART_InitStructure;//�������ûָ�Ĭ�ϲ���
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
	//GPIO�����趨��Ӧ���ڹܽ�ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//�ܽ�2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//TX��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//�ܽ�3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ����������
	USART_InitStructure.USART_BaudRate=115200;//������115200
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ֳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1λֹͣ�ֽ�
	USART_InitStructure.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//��Rx���պ�Tx���͹���
	
	USART_Init(USART2,&USART_InitStructure);//��ʼ��
	USART_Cmd(USART2,ENABLE);//��������
}

#if USART2_DEBUG
#define DEBUG_USART	 USART2
#else
#define DEBUG_USART	 USART1
#endif
/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.printf�ض���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USART,(uint8_t)ch);
	/*Loopuntiltheendoftransmission*/
	while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET)
	{	
	}
	return ch;
}

/*******************************************************************************
* Function Name  : int fgetc(FILE *f)
* Description    : Retargets the C library printf function to the USART.fgetc�ض���
* Input          : None
* Output         : None
* Return         : ��ȡ�����ַ�
*******************************************************************************/
int fgetc(FILE *f)
{
	/* Loop until received a char */
	while(!(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == SET))
	{
	}
	
	/* Read a character from the USART and RETURN */
	return (USART_ReceiveData(DEBUG_USART));
}



