#ifndef __USART4_H
#define __USART4_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����3��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define USART4_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART4_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART4_RX_EN 			1					//0,������;1,����.

extern u8  USART4_RX_BUF[USART4_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART4_TX_BUF[USART4_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART4_RX_STA;   						//��������״̬
extern u8 usartt4;
void usart4_init(u32 bound);				//����3��ʼ�� 

void u4_printf(char* fmt, ...);
#endif













