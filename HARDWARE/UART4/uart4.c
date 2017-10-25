#include "delay.h"
#include "uart4.h"
#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4������
//����3��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/8/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//���ڷ��ͻ����� 	
__align(8) u8 USART4_TX_BUF[USART4_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
#ifdef USART4_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART4_RX_BUF[USART4_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART4_RX_STA=0;  
u8 usartt4 = 0;
void UART4_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET&&USART_GetITStatus(USART3, USART_IT_RXNE) == RESET)//���յ�����
	{	 
  usartt4 = 1;
	res =USART_ReceiveData(UART4);		
	if((USART4_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
	{ 
		if(USART4_RX_STA<USART4_MAX_RECV_LEN)		//�����Խ�������
		{
			TIM_SetCounter(TIM7,0);//���������        				 
			if(USART4_RX_STA==0)		
				TIM_Cmd(TIM7, ENABLE);  //ʹ�ܶ�ʱ��7 
			USART4_RX_BUF[USART4_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART4_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}
     if(USART3_RX_BUF[3] == '1')//����ͨ������3�������ݾͿɼ�����Ӧ����
		{
			printf("@TextToSpeech#�����ҽ�С�����ܸ�����ʶ��$");
		
		}
	  	else if(USART3_RX_BUF[3] == '2')//�����Ҫ����4,3�������Ļ��͸ĸ�  USART4_RX_BUF[2] == 'X'&&USART3_RX_BUF[3] == '1'
		{                                  //Ҳ���������޸� ɧ�� �ҿ�������
			printf("@TextToSpeech#�������ֱ����ڴ��ռ䣬�������ֱ��ǰ칫��$");
		
		}
		  else if(USART3_RX_BUF[3] == '3')
		{
			printf("@TextToSpeech#����һ�¿�����$");
		
		}
			else if(USART3_RX_BUF[3] == '4')
		{
			printf("@TextToSpeech#��������ߵ��׾Ϳ��Կ�����$");
		
		}	 
		  else if(USART3_RX_BUF[3] == '5')
		{
			printf("@TextToSpeech#�����������ǹ�����Ա���ܰ�$");
		
		}	 
		  else if(USART3_RX_BUF[3] == '6')
		{
			printf("@TextToSpeech#лл�佱$");
		
		}	
 }										 
}  
#endif	
//��ʼ��IO ����4
//bound:������	  
void usart4_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(UART4);  //��λ����4
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART3ʱ��
	
 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOB10����ΪUSART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�  
		
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	
	USART4_RX_STA =0;				//���� 
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u4_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART4_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(UART4,(uint8_t)USART4_TX_BUF[j]); 	 //�������ݵ�����3 
	}
	
}
 
 
 











