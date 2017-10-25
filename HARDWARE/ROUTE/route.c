#include <stdio.h>


#define connectnum  6 //总连接线数
#define stepnum  5 //总阶数
#define pointnum 7//总点数
#define directionnum 6

int nowpoint=101;

int dian[stepnum][5]= //线路图
{101,0,0,0,0,
201,0,0,0,0,
301,302,0,0,0,
401,402,0,0,0,
501,0,0,0,0
};

int intersection_num[pointnum][2]=//某点分支数
{
101,1,
201,2,
301,0,
302,2,
401,1,
402,0,
501,0
};
#define left 1
#define right -1
#define oneway 0
int direction[directionnum][3]=
{
101,201,oneway,
201,301,right,
201,302,left,
302,401,right,
302,402,left,
401,501,oneway
};


//**************************************************对线路的查询
//已经完成
// 0-2 为正常值，-1为没有这个路口
int CheckIntersecNum(int point)
{
	int i=0;
	for(;i<pointnum;i++)
	{
		if(intersection_num[i][0]==point)
		return intersection_num[i][1];
	}
	return -1;
}


//***************************************************对支路方向查询
//已经完成
int CheckDirection(int nowpoint,int nextpoint)
{
	int i=0;
	for(i=0;i<directionnum;i++)
	{
		if(direction[i][0]==nowpoint&&direction[i][1]==nextpoint)
		return direction[i][2];
	}
	return 0;
}

//***************************************************
int diannum[stepnum]={0};//每一阶的节点数
long zhi[connectnum]={101201,201301,201302,302401,302402,401501} ;//连接线 右边往左从小到大
//已完成

//路线规划模拟地点
//int fakelocation;
//int fakenextlocation ;
//int fakeroute[10];

//******************************************************
//检测是否连接函数
int checkconnect(int nowlocation,int nextlocation)
{
	int a,b,i;
	for(i=0;i<connectnum;i++)
	{
		a = zhi[i]/1000;
		b = zhi[i]%1000;
		if((a==nowlocation && b==nextlocation)||(b==nowlocation && a==nextlocation))return 1;
	}
	return 0;
}

//******************************************************
//初始化每阶点数
void routenuminit(void)
{
	int i,j;
	for(i=0;i<stepnum;i++)
	{
		for(j=0;j<5;j++)
		{
		if(dian[i][j])diannum[i]++;
		}
	}
}
