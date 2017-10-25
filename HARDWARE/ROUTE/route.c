#include <stdio.h>


#define connectnum  6 //����������
#define stepnum  5 //�ܽ���
#define pointnum 7//�ܵ���
#define directionnum 6

int nowpoint=101;

int dian[stepnum][5]= //��·ͼ
{101,0,0,0,0,
201,0,0,0,0,
301,302,0,0,0,
401,402,0,0,0,
501,0,0,0,0
};

int intersection_num[pointnum][2]=//ĳ���֧��
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


//**************************************************����·�Ĳ�ѯ
//�Ѿ����
// 0-2 Ϊ����ֵ��-1Ϊû�����·��
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


//***************************************************��֧·�����ѯ
//�Ѿ����
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
int diannum[stepnum]={0};//ÿһ�׵Ľڵ���
long zhi[connectnum]={101201,201301,201302,302401,302402,401501} ;//������ �ұ������С����
//�����

//·�߹滮ģ��ص�
//int fakelocation;
//int fakenextlocation ;
//int fakeroute[10];

//******************************************************
//����Ƿ����Ӻ���
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
//��ʼ��ÿ�׵���
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
