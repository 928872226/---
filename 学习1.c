#include<reg52.h>   //�ö�ʱ����׼��ʱ��ʵ����LED��ÿ����˸��LED1��˸10����LED2��˸10����LED3��˸10����,���½���ѭ�����ص�����flag��־λ��Ӧ��
#define uint unsigned int
#define uchar unsigned char
uchar a,i,flag,flag1;
sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;
void init()
{
	i=0;
	a=0;
	TMOD=0x10;
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	ET1=1;
	EA=1;
	TR1=1;
}
void dingshi() interrupt 3
{
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	a++;
}
void main()
{
	init();
	while(1)
	{
		while(!flag)
		{
			if(a==20)
			{
				a=0;
				LED1=~LED1;
				i++;
				if(i==20)
				{
					i=0;
					flag=1;
					LED1=1;
				}
			}
	  }
		while(!flag1)
		{
			if(a==20)
			{
				a=0;
				LED2=~LED2;
				i++;
				if(i==20)
				{
					i=0;
					flag1=1;
					LED2=1;
				}
			}
	  }
		if(a==20)
		{
			a=0;
			LED3=~LED3;
			i++;
			if(i==20)
			{
				flag=0;
				flag1=0;
			}
		}
	}
}