#ifndef _PID_H
#define _PID_H
#include "sys.h"
void slowdown(int left,int right);
void kongzhidong(float zuo,float you);

void walk(void);
void GoToTerminus(void);
int detect_way(void);

void closetopoint(int distance,int detecttime,int speed);
void ontheroad(int seconds,int filter);
int chooseroad(int nowpoint,int nextpoint);
#endif
