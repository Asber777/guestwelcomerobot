#include "math.h"
#include "pid.h"
#include "sys.h"			
#include "usart3.h"
#include "usart6.h"
#include "delay.h"
#include "route.h"
#define throttlex  800
#define throttley  750
#define arr 1000
#define xxian 210
#define yxian 300
#define PWM11 PFout(9) 
#define PWM12 PFout(10) 
#define PWM21 PAout(6) 
#define PWM22 PAout(8) 
//������������ѭ�� ʹ���ٶȽ���
int pastLSpeed,pastRSpeed;
int cutLSpeed,cutRSpeed;
u8 flagb=0;
extern u8 chao_flag;
void updateSpeed(int left,int right)
{
	pastLSpeed=left;
	pastRSpeed=right;
	cutLSpeed=left/10;
	cutRSpeed=right/10;
}

void slowdown(int left,int right)
{
	if(pastLSpeed-cutLSpeed>=left)pastLSpeed-=cutLSpeed;
	else pastLSpeed=left;
	if(pastRSpeed-cutRSpeed>=right)pastRSpeed-=cutRSpeed;
	else pastRSpeed=right;
	kongzhidong(pastLSpeed,pastRSpeed);
	delay_ms(30);
}

//****
void kongzhidong(float zuo,float you)
{
	if(zuo <= 0)
	{
		PWM11 = 0;
		PWM12 = 1;
	}
	else if(zuo >0)
	{
		PWM11 = 1;
		PWM12 = 0;
	}
	if(you <= 0)
	{
		PWM21 = 1;
		PWM22 = 0;
	}
	else if(you >0)
	{
		PWM21 = 0;
		PWM22 = 1;
	}
	zuo = fabs(zuo);
	you = fabs(you);
	TIM_SetCompare2(TIM3,you);
	TIM_SetCompare1(TIM13,zuo);
}

	extern int xpo;
	int xzhi[3];
	float pz=16.5,iz=0,dz=0;
	float pidzuo;
	int zuosudu,yousudu;
	int xaim = 60 ;
  int basespeed = 300 ;
	int maxspeed = 850 ,minspeed = -850;

void walk(void)
{
	xpo = xpo - xaim; 
//pid������ִ��
		//�����ü��׿��������������˲�����ֵ���п������˲�������û�ã�
	xzhi[2] = xzhi[1];
	xzhi[1] = xzhi[0];
	xzhi[0] = xpo;
	pidzuo = pz*xzhi[0] + iz*(xzhi[2]+xzhi[1]+xzhi[0]) + dz*(xzhi[0]-xzhi[1]);
	//x������Ļ��ʱ�� xpo<0
	zuosudu=basespeed-pidzuo;
	yousudu=basespeed+pidzuo;
	//���ű���
	if(zuosudu<minspeed)zuosudu=minspeed;else if(zuosudu>maxspeed)zuosudu=maxspeed;
	if(yousudu<minspeed)yousudu=minspeed;else if(yousudu>maxspeed)yousudu=maxspeed;
	
//	if(chao_flag==1)
//	{
//		//flagb=1;
	kongzhidong(zuosudu,yousudu);
//	}
//	if(chao_flag==0)	
//	{
//	//	flagb=2;
//		kongzhidong(0,0);
//	}
  delay_ms(50);
}


//	extern int endpoint;
	int zxaim = 60, zyaim =0;
	extern int zxpo,zypo;
	int zxzhi[3],zyzhi[3];
  float zpz=5,ziz=0,zdz=0,zpy=8,ziy=0,zdy=0;
	int slope=0;
	float slopeP=10,pidtong=0;
  
void GoToTerminus(void)
{
	// x��Ұ 0-120  xaim = 60  (xnormal 0-108 ) ~(-30,+30), y��Ұ 0-80  yaim = 10 (ynormal 10~60)  ~(0,50)
	zxpo = zxpo - zxaim; 
	zypo = zypo - zyaim;

//pid������ִ��
	zxzhi[2] = zxzhi[1];
	zxzhi[1] = zxzhi[0];
	zxzhi[0] = zxpo;
	zyzhi[2] = zyzhi[1];
	zyzhi[1] = zyzhi[0];
	zyzhi[0] = zypo;
	
	if(zypo<=5){kongzhidong(0,0);}
	else
	{
	pidzuo = zpz*zxzhi[0] + ziz*(zxzhi[2]+zxzhi[1]+zxzhi[0]) + zdz*(zxzhi[0]-zxzhi[1]);
	pidtong = zpy*zyzhi[0] + ziy*(zyzhi[2]+zyzhi[1]+zyzhi[0]) + zdy*(zyzhi[0]-zyzhi[1]);
	zuosudu=pidtong-pidzuo-slope*slopeP;
	yousudu=pidtong+pidzuo+slope*slopeP;
	if(zuosudu<minspeed)zuosudu=minspeed;else if(zuosudu>maxspeed)zuosudu=maxspeed;
	if(yousudu<minspeed)yousudu=minspeed;else if(yousudu>maxspeed)yousudu=maxspeed;
	kongzhidong(zuosudu,yousudu);
	delay_ms(60);
	updateSpeed(zuosudu,yousudu);
	}
}


//              
//
//  �������溯����ʹ��˳���� �������������������ture_num�������ˣ������0�������������1��PID���ڣ������2��choicewaystate=1,selectbegin=1���ƴ���ʹ������
//  ���ڶԵ�ǰ��ӡ������У�� ����Ҳ������� ifdetect2way��ᶯһ��������̽
//	����ҵ��� ��ʼPID����
//	PID���ڽ��� (��������)choicewaystate=0��selectbegin=1
//
	extern int ypo,zpo;//y����Ļ��ߵĽ�ӡx���꣬
	int flag=0;

//����������������ͬ�򷵻�1 ����һ��Ȼ��Ż�0
int ifdetect2way(int detect_num)
{
	if(detect_num==2)
	{
		kongzhidong(0,0);
		flag++;
		delay_ms(100);
	}
	else
	{
		flag=0;
		if(ypo!=0)//y��ֵ ˵���ұ߼�⵽�� �������û��
		{kongzhidong(-200,200);delay_ms(100);kongzhidong(0,0);}
		else if(zpo!=0)
		{kongzhidong(200,-200);delay_ms(100);kongzhidong(0,0);}
		else 
		{kongzhidong(80,80);delay_ms(100);kongzhidong(0,0);}
	}
	if(flag>20)
	{
		flag=0;
		return 1;
	}
	else return 0;
}

  float pyz=20;
  int yaim = 50,zaim =70;
	int detect_num=0;
	int waydirection =0;
  int ture_num=0;
	extern int nowpoint;
  extern int intersection_num;
	extern int selectbegin;
	extern int selectover;
//�����ж����choicewaystate��־λ���� ����֮�� selectover�ᱻ��һ
int detect_way(void)
{
	  static int i=1;
		static int flag=0;
		if(selectbegin&&i)
		{
			selectover=0;
			detect_num=0;
			if(ypo!=0)detect_num++;
			if(zpo!=0&&fabs(ypo-zpo)>10)
				detect_num++;
			if(ifdetect2way(detect_num))//�����������δ����
			{
				flag=0;
				i=0;
				u3_printf("OK");
			}
		}
		if(!i)//У����� ��ʼPID���� main����Ҫֹͣ
		{
			if(waydirection == 1)//����ת ʹ��z���� 
			{
							if(zpo)
				{
					zpo = zpo - zaim;
					pidzuo = pyz*zpo ;
					zuosudu= -pidzuo;
					yousudu= +pidzuo;
					if(zuosudu<minspeed)zuosudu=minspeed;else if(zuosudu>maxspeed)zuosudu=maxspeed;
					if(yousudu<minspeed)yousudu=minspeed;else if(yousudu>maxspeed)yousudu=maxspeed;
					kongzhidong(zuosudu,yousudu);
					delay_ms(60);
					updateSpeed(zuosudu,yousudu);
					if(fabs(zpo)<=8)flag++;
					if(flag>20)
					{
						i=1;
						selectover=1;
						kongzhidong(0,0);
						return 1;
					}
				}
			}
			else if(waydirection == -1)
			{
	
								if(ypo)
				{
					ypo = yaim - ypo  ;
					pidzuo = pyz*ypo ;
					zuosudu= pidzuo;
					yousudu= -pidzuo;
					if(zuosudu<minspeed)zuosudu=minspeed;else if(zuosudu>maxspeed)zuosudu=maxspeed;
					if(yousudu<minspeed)yousudu=minspeed;else if(yousudu>maxspeed)yousudu=maxspeed;
					kongzhidong(zuosudu,yousudu);
					delay_ms(60);
					updateSpeed(zuosudu,yousudu);
					if(fabs(ypo)<=8)flag++;
					if(flag>20)
					{
						i=1;
						selectover=1;
						kongzhidong(0,0);
						return 1;
					}
				}
			}
		}
		return 0;
}

//����һ����ĺ���
extern int terminusstate;
extern int walkstate,seeterminus;
void closetopoint(int overdistance,int detecttime,int speed)
{
	int flag=0;
	terminusstate=1;//Ѱ�����,��x��y��slope
	while(1)
	{
		slowdown(speed,speed);
//		delay_ms(300);
		if((fabs(zyzhi[0])<overdistance)&&seeterminus==1)
		{
			seeterminus=0;
			flag++;
		}
//		else flag=0;
		if(flag>detecttime)break;
	}
	terminusstate=0;
	kongzhidong(0,0);
}

//	GoToTerminus();
//Ѱ��ӡ��ʱ��ĺ���
void ontheroad(int seconds,int filter)
{
	int flag=0,i;
	walkstate=1;
	for(i=0;i<seconds*10;i++)
	{
	kongzhidong(300,300);
	delay_ms(100);
	}
//��ʱ�Ѿ�ѭ�����������
	seeterminus=0;
	walkstate=1;
	while(1)
	{
	kongzhidong(300,300);
	if(seeterminus){seeterminus=0;flag++;}
	if(flag>filter)break;
	}
	walkstate=0;
	kongzhidong(0,0);
}
//walk();
//

int chooseroad(int nowpoint,int nextpoint)
{
	int ture_num=0;
	selectover=0;
	ture_num=CheckIntersecNum(nowpoint);
	if(!ture_num)
	{
		kongzhidong(0,0);
		return 0;
	}
	else if(ture_num==1)//Ҫ���е�ĸ���Ȼ�����ȥѰ��ӡ
	{
		kongzhidong(0,0);
		return 1;
	}
	else 
	{
		waydirection=CheckDirection(nowpoint,nextpoint);
		selectbegin=1;
		while(1)
		{
			kongzhidong(0,0);
			if(selectover)break;
		}
		kongzhidong(0,0);
		selectbegin=0;
		selectover=0;
		return 2;
	}
}
//detect_way();