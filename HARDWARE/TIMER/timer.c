#include "timer.h"
#include "usart3.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ϴ�������ʹ�ã�ѧ���ĳ���						  
////////////////////////////////////////////////////////////////////////////////// 	 

void test()
{

}


extern u16 USART3_RX_STA;
extern u16 USART4_RX_STA;
extern u8 usartt3;
extern u8 usartt4;

extern int goal;
//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET&&USART_GetITStatus(UART4,USART_IT_RXNE) == RESET)//�Ǹ����ж�
	{	 		
    if(usartt3==1&&usartt4 ==0)
		{
		USART3_RX_STA|=1<<15;	//��ǽ������
			
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	//	lanyakongzhi();
	//	test();
			
			get_goal();
			//goal=USART3_RX_BUF[1];
		}
		if(usartt4==1&&usartt3 ==0)
		{
		USART4_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
		}
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
   
	TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}


void TIM5_Int_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	TIM_DeInit(TIM5);

	TIM_TimeBaseStructure.TIM_Period =5000;         //2800  280MS�ж�һ��
	
	TIM_TimeBaseStructure.TIM_Prescaler=8399;//84��Ƶ
	 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //or TIM_CKD_DIV2 or TIM_CKD_DIV4
	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}
//����TIM5�Ŀ����͹ر� 
//0 �ر�
//1 ����
void Ctr_Tim5(u8 sta)
{
	if(sta)
		TIM_Cmd(TIM5, ENABLE);
	else
		TIM_Cmd(TIM5, DISABLE);
}

void Init_Nvic(void)//
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	  NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;//TIM5ȫ�ֱ����жϵ�����
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}


	 
