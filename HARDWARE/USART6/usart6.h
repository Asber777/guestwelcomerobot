#ifndef __USART6_H
#define __USART6_H	 
#include "sys.h" 
#include "stdio.h"	
#include "stm32f4xx_conf.h"


#define USART6_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART6_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART6_RX_EN 			1					//0,������;1,����.


extern u8  USART6_RX_BUF[USART6_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART6_TX_BUF[USART6_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART6_RX_STA;   						//��������״̬
extern u8 usartt6;
void usart6_init(u32 bound);				//����6��ʼ�� 
void u6_printf(char* fmt, ...);
void TIM5_IRQHandler(void);
void chaoshengboqidong(void);
void chaoshengboceju(void);
#endif



