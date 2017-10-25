#include "delay.h"
#include "usart6.h"
#include "usart3.h"
#include "usart.h"
#include "uart4.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"
#include "rs485.h"
#include "pid.h"
#include "sys.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout6;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit6(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc6(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif
 
#if USART6_RX_EN   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART6_RX_BUF[USART6_MAX_RECV_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART6_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void usart6_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //6r7t
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART6, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
	


u8 chao_flag=1;
u8 time_flag=0;
int distance;
u8 j=0;
void chaoshengboqidong(void)           
{		  
	USART_SendData(USART6, 0x55);
}

void chaoshengboceju(void)
{
	chaoshengboqidong();
	//delay_ms(50);
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	 {
		 USART6_RX_BUF[j] =USART_ReceiveData(USART6);//(USART1->DR);	//读取接收到的数据
		 USART_ClearFlag(USART6,USART_FLAG_RXNE);	
		 j++;
		 if(j==2) 
     {
       j=0;
       distance=USART6_RX_BUF[0]*256+USART6_RX_BUF[1];
			 //u3_printf("%d  %d\r\n ok1\r\n",=USART6_RX_BUF[0]);
     }
	 }//u3_printf("%d\r\n ok1\r\n",distance);		
}

/*extern int 	walkstate,
						terminusstate,
						terminusdetect,
						selectwaystate,
						selectbegin;	*/

void USART6_IRQHandler(void)               	//串口1中断服务程序
{
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
  #endif

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	  USART6_RX_BUF[j] =USART_ReceiveData(USART6);//(USART1->DR);	//读取接收到的数据
		USART_ClearFlag(USART6,USART_FLAG_RXNE);	
		j++;
		chao_flag++;
		if(j==2) 
    {
      j=0;
      distance=USART6_RX_BUF[0]*256+USART6_RX_BUF[1];
				if(distance<=300)
		{
		chao_flag=0;
			
			
		}
		else
			chao_flag=1;
//		{
//			chao_flag=1;
//		}
    }//u3_printf("%d\r\n wnm\r\n",distance);
//		if(distance<=500)
//		{
//			chao_flag=0;
//		}
		/*HMIcmd(1);
		 HMIcmd(0);*/
   } 
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
  #endif
	
} 


void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
	 TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//清除TIMx的中断待处理位:TIM 中断源 
	
	 chaoshengboqidong();
		//u3_printf("m\r\n");
	}
}

#endif	


 
