#ifndef __USART6_H
#define __USART6_H	 
#include "sys.h" 
#include "stdio.h"	
#include "stm32f4xx_conf.h"


#define USART6_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART6_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART6_RX_EN 			1					//0,不接收;1,接收.


extern u8  USART6_RX_BUF[USART6_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART6_TX_BUF[USART6_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART6_RX_STA;   						//接收数据状态
extern u8 usartt6;
void usart6_init(u32 bound);				//串口6初始化 
void u6_printf(char* fmt, ...);
void TIM5_IRQHandler(void);
void chaoshengboqidong(void);
void chaoshengboceju(void);
#endif



