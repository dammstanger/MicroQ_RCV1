//20141222
#include "usart.h"


void USART1_Configuration(void)                        //串口初始化函数
{
 
	USART_InitTypeDef   USART_InitStructure;//串口设置恢复默认参数
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//GPIO里面设定相应串口管脚模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//管脚9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//TX初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//管脚10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化参数设置
	USART_InitStructure.USART_BaudRate=115200;//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1位停止字节
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//打开Rx接收和Tx发送功能
	
	USART_Init(USART1,&USART_InitStructure);//初始化
	USART_Cmd(USART1,ENABLE);//启动串口
}


void USART1_RX_IntteruptEnable(USART_TypeDef* USARTx)		//接收中断使能
{
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	 //中断使能
}

/********************************************************************************
 * 函数名：USART2_Configuration
 * 描述  ：USART2 配置
 * 输入  ：-		    	
 * 返回  ：- 
 * 调用  ：外部调用
 ********************************************************************************/
void USART2_Configuration(void)                        //串口初始化函数
{
 
	USART_InitTypeDef   USART_InitStructure;//串口设置恢复默认参数
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	
	//GPIO里面设定相应串口管脚模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//管脚2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//TX初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//管脚3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化参数设置
	USART_InitStructure.USART_BaudRate=115200;//波特率115200
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1位停止字节
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//打开Rx接收和Tx发送功能
	
	USART_Init(USART2,&USART_InitStructure);//初始化
	USART_Cmd(USART2,ENABLE);//启动串口
}

#if USART2_DEBUG
#define DEBUG_USART	 USART2
#else
#define DEBUG_USART	 USART1
#endif
/*******************************************************************************
* Function Name  : int fputc(int ch, FILE *f)
* Description    : Retargets the C library printf function to the USART.printf重定向
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
* Description    : Retargets the C library printf function to the USART.fgetc重定向
* Input          : None
* Output         : None
* Return         : 读取到的字符
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



