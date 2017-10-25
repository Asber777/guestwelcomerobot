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
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout6;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit6(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc6(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//ѭ������,ֱ���������   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif
 
#if USART6_RX_EN   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART6_RX_BUF[USART6_MAX_RECV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART6_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void usart6_init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //6r7t
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
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
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	 {
		 USART6_RX_BUF[j] =USART_ReceiveData(USART6);//(USART1->DR);	//��ȡ���յ�������
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

void USART6_IRQHandler(void)               	//����1�жϷ������
{
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
  #endif

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	  USART6_RX_BUF[j] =USART_ReceiveData(USART6);//(USART1->DR);	//��ȡ���յ�������
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
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
  #endif
	
} 


void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
	 TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//���TIMx���жϴ�����λ:TIM �ж�Դ 
	
	 chaoshengboqidong();
		//u3_printf("m\r\n");
	}
}

#endif	


 
