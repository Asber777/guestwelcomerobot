#ifndef __USART4_H
#define __USART4_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口3驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define USART4_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART4_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART4_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART4_RX_BUF[USART4_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART4_TX_BUF[USART4_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART4_RX_STA;   						//接收数据状态
extern u8 usartt4;
void usart4_init(u32 bound);				//串口3初始化 

void u4_printf(char* fmt, ...);
#endif













