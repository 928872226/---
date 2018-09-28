#include<reg52.h>   //用定时器精准定时，实现了LED灯每秒闪烁，LED1闪烁10次灭，LED2闪烁10次灭，LED3闪烁10次灭,重新进入循环，重点掌握flag标志位的应用
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