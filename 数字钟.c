#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define nops();  {_nop_(); _nop_(); _nop_(); _nop_();} 
sbit EN=P2^7;
sbit RWN=P2^5;
sbit RSN=P2^6;
sbit BA=P2^4;
sbit BUSY=P0^7;
sbit SCLK=P3^6;
sbit IO=P3^4;
sbit CEN=P3^5;
sbit K1=P3^1;
sbit K2=P3^0;
sbit K3=P3^2;
sbit K4=P3^3;
sbit BZ=P1^5;
sbit DQ=P3^7;
uchar mode=0;ok=0;ht=0;
uchar num1,num2;
uchar count;
uint temp;
bit L=0,select=0;
uchar code xie_add[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
uchar code du_add[]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
uchar time[7]={50,59,17,19,10,5,18};
uchar naozhong[3]={0,0,18};
uchar miao,fen,shi,nian,yue,ri;
uchar code liebiao[]="Set Time";
uchar code liebiao1[]="Set Naozhong";
uchar code liebiao2[]="ST:";
uchar code self_char[]={
													0x00,0x04,0x0E,0x1F,0x0E,0x04,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                       };
void delay(uchar t)
{
	do
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
	}
	while(--t);
}
void delay_ms(uchar a)
{
	uchar x,y;
	for(x=a;x>0;x--)
		for(y=120;y>0;y--);
}
void wait()
{
	P0=0xff;
	do
	{
		RSN=0;
		RWN=1;
		EN=0;
		_nop_();
		EN=1;
		_nop_();
	}
	while(BUSY==1);
	EN=0;
}
void xie_LCDcom(uchar com)
{
	wait();
	RSN=0;
	RWN=0;
	EN=0;
	P0=com;
	delay(1);
	EN=1;
	delay(2);
	EN=0;
}
void xie_LCDshuju(uchar shuju)
{
	wait();
	RSN=1;
	RWN=0;
	EN=0;
	P0=shuju;
	delay(1);
	EN=1;
	delay(2);
	EN=0;
}
void LCD_dingwei(uchar x,bit y)
{
	if(y==1)
		xie_LCDcom(x|0xc0);
	else
		xie_LCDcom(x|0x80);
}
void LCD_zf(uchar x,bit y,uchar dat)
{
	LCD_dingwei(x,y);
	xie_LCDshuju(dat);
}
void LCD_zfc(uchar x,bit y,uchar *p)
{
	LCD_dingwei(x,y);
	while((*p)!='\0')
	{
		xie_LCDshuju(*p);
		p++;
	}
}
void LCD_init()
{
	uchar i;
	xie_LCDcom(0x40);
	for(i=0;i<64;i++)
	{
		xie_LCDshuju(self_char[i]);
	}
	BA=0;
	delay_ms(15);
	xie_LCDcom(0x38);
	delay_ms(5);
	xie_LCDcom(0x38);
	delay_ms(5);
	xie_LCDcom(0x38);
	xie_LCDcom(0x38);
	xie_LCDcom(0x0c);
	xie_LCDcom(0x06);
	xie_LCDcom(0x01);
	xie_LCDcom(0x81);
	xie_LCDshuju('2');
	xie_LCDshuju('0');
	xie_LCDcom(0x85);
	xie_LCDshuju('-');
	xie_LCDcom(0x88);
	xie_LCDshuju('-');
	xie_LCDcom(0x80+0x40+3);
	xie_LCDshuju(':');
	xie_LCDcom(0x80+0x40+6);
	xie_LCDshuju(':');
}
void DS1302_xiezijie(uchar zj)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		IO=zj&0x01;
		zj>>=1;
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
}
uchar DS1302_duzj()
{
	uchar i,zj=0;
	for(i=0;i<8;i++)
	{
		zj>>=1;
		if(IO==1)
			zj|=0x80;
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
	return zj;
}
void xie_DS1302(uchar add,uchar dat)
{
	CEN=0;
	SCLK=0;
	CEN=1;
	DS1302_xiezijie(add);
	DS1302_xiezijie(dat);
	SCLK=0;
	CEN=0;
}
uchar du_DS1302(uchar add)
{
	uchar dat;
	CEN=0;
	SCLK=0;
	CEN=1;
	DS1302_xiezijie(add);
	_nop_();
	dat=DS1302_duzj();
	CEN=0;
	_nop_();
	SCLK=1;
	_nop_();
	IO=0;
	_nop_();
	IO=1;
	_nop_();
	return dat;
}
uchar Dec_BCD(uchar Dec)
{
	uchar shu,shu1;
	shu1=Dec/10;
	shu=Dec%10+shu1*16;
	return shu;
}
void DS1302init()
{
	uchar i;
	CEN=0;
	SCLK=0;
	for(i=0;i<7;i++)
		time[i]=Dec_BCD(time[i]);
	xie_DS1302(0x8e,0x00);
	for(i=0;i<7;i++)
		xie_DS1302(xie_add[i],time[i]);
	xie_DS1302(0x8e,0x80);
}
uchar BCD_Dec(uchar BCD)
{
	uchar shu,shu1;
	shu1=BCD/16;
	shu=shu1*10+(BCD&0x0f);
	return shu;
}
void du_time()
{
	uchar i;
	for(i=0;i<7;i++) 
	{
		time[i]=du_DS1302(du_add[i]);
		time[i]=BCD_Dec(time[i]);
	}
}
void delay1(uint a)
{
	while(a--);
}
void DS1802_init()
{
  uchar flag=1;
  while(flag)
  {
    while(flag)
    {
      DQ=1;
      delay1(1);
      DQ=0;
      delay1(50); 
      DQ=1;
      delay1(6); 
      flag=DQ;
    }
    delay1(45);
    flag=~DQ;
  }
  DQ=1;  
}
void xie_DS1802zijie(uchar dat)
{
  uchar i;
  for(i=0;i<8;i++)
  {
    DQ=1;
    _nop_();
    DQ=0;
    nops();
    DQ=dat&0x01;
    delay1(6); 
    dat>>=1; 
  }
  DQ=1;
  delay(1);
}
uchar du_DS1802zijie()
{
  uchar i,byte=0;
  for(i=0;i<8;i++)
  {
    byte>>=1;
    DQ=1;
    _nop_();
    DQ=0;
    nops();
    DQ=1;
    nops();
    if(DQ==1)  byte|=0x80;
    delay1(6);
  }
  DQ=1;
  return byte; 
}
void wdzh()
{
  DS1802_init();
  xie_DS1802zijie(0xcc);
  xie_DS1802zijie(0x44);
}
uint du_wendu()
{
  uchar temp_data[2];
  uint temp;
	wdzh();
	delay_ms(100);
  DS1802_init(); 
  xie_DS1802zijie(0xcc); 
  xie_DS1802zijie(0xbe);
  temp_data[0]=du_DS1802zijie(); 
  temp_data[1]=du_DS1802zijie(); 
  temp=temp_data[1];
  temp<<=8;
  temp|=temp_data[0];
  return temp;
}
void xianshi_wendu()
{
   temp=du_wendu();
    if(temp&0x8000)
    {
      temp=~temp+1;
      temp=(temp*0.0625)*10+0.5;
			xie_LCDcom(0x80+0x40+10);
      xie_LCDshuju('-');
      xie_LCDshuju(0x30+temp/1000);
      xie_LCDshuju(0x30+temp%1000/100);
			xie_LCDshuju('.');
      xie_LCDshuju(0x30+temp%100/10);
      xie_LCDshuju(0x30+temp%10);
   
    }
    else
    {
      temp=(temp*0.0625)*100+0.5 ;
			xie_LCDcom(0x80+0x40+10);
      xie_LCDshuju('+');
      xie_LCDshuju(0x30+temp/1000);
      xie_LCDshuju(0x30+temp%1000/100);
			xie_LCDshuju('.');
      xie_LCDshuju(0x30+temp%100/10);
      xie_LCDshuju(0x30+temp%10);
   
    }
}
void xianshi_sfm(uchar add,uchar dat)
{
	uchar shi,ge;
	shi=dat/10;
	ge=dat%10;
	xie_LCDcom(0x80+0x40+add);
	xie_LCDshuju(0x30+shi);
	xie_LCDshuju(0x30+ge);
}
void xianshi_nyr(uchar add,uchar dat)
{
	uchar shi,ge;
	shi=dat/10;
	ge=dat%10;
	xie_LCDcom(0x80+add);
	xie_LCDshuju(0x30+shi);
	xie_LCDshuju(0x30+ge);
}
void xianshi_riqi(uchar shu)
{
	xie_LCDcom(0x80+12);
	switch(shu)
	{
		case(1):
			xie_LCDshuju(0x44);xie_LCDshuju(0x6f);xie_LCDshuju(0x6e);break;
		case(2):
			xie_LCDshuju(0x54);xie_LCDshuju(0x75);xie_LCDshuju(0x65);break;
		case(3):
			xie_LCDshuju(0x57);xie_LCDshuju(0x65);xie_LCDshuju(0x64);break;
		case(4):
			xie_LCDshuju(0x54);xie_LCDshuju(0x68);xie_LCDshuju(0x75);break;
		case(5):
			xie_LCDshuju(0x46);xie_LCDshuju(0x72);xie_LCDshuju(0x69);break;
		case(6):
			xie_LCDshuju(0x53);xie_LCDshuju(0x61);xie_LCDshuju(0x74);break;
		case(7):
			xie_LCDshuju(0x53);xie_LCDshuju(0x75);xie_LCDshuju(0x6e);break;
	}
}
void xianshi_caidan()
{
	uchar i;
	xie_LCDcom(0x83);
	for(i=0;i<8;i++)
		xie_LCDshuju(liebiao[i]);
	xie_LCDcom(0x80+0x43);
	for(i=0;i<12;i++)
		xie_LCDshuju(liebiao1[i]);	
	if(select)
		LCD_zf(2,1,0);
	else
		LCD_zf(2,0,0);
}
void xianshi_tjsj()
{
	uchar i;
	du_time();
	miao=time[0];
	fen=time[1];
	shi=time[2];
	xie_LCDcom(0x83);
	for(i=0;i<3;i++)
	{
		xie_LCDshuju(liebiao2[i]);
	}
	xianshi_sfm(11,miao);
	xianshi_sfm(8,fen);
	xianshi_sfm(5,shi);
	xie_LCDcom(0x80+0x40+7);
	xie_LCDshuju(':');
	xie_LCDcom(0x80+0x40+10);
	xie_LCDshuju(':');
	//xie_DS1302(0x8e,0x00);
	//xie_DS1302(0x80,0x80);
	//xie_LCDcom(0x0f);
	/*if(mode==1)
	{
		if(num1!=0)
		{
			miao++;
			if(miao==60)
				miao=0;
			time[0]=miao;
			xianshi_sfm(11,miao);
			xie_LCDcom(0x80+0x40+8);
			num1=0;
		}
		if(num2!=0)
		{
			miao--;
			time[0]=miao;
			LCD_zf(11,1,miao/10);
			LCD_zf(12,1,miao%10);
			xie_LCDcom(0x80+0x40+8);
			num2=0;
		}
	}
	if(mode==2)
	{
		xie_LCDcom(0x80+0x40+8);
		if(num1==60)
			num1=0;
		time[1]=num1;
		xianshi_sfm(8,num1);
		xie_LCDcom(0x80+0x40+8);
	}
	
	if(mode==3)
	{
		xie_LCDcom(0x80+0x40+5);
		if(num1==24)
			num1=0;
		time[2]=num1;
		xianshi_sfm(5,num1);
		xie_LCDcom(0x80+0x40+8);
	}*/
}
void xianshi_naozhong()
{
	LCD_zfc(1,0,"Alarm:");
	LCD_zfc(4,1,"00:00:00");
	xie_LCDcom(0x0f);
	if(mode==1)
		xie_LCDcom(0x80+0x40+10);
	if(mode==2)
		xie_LCDcom(0x80+0x40+7);
	if(mode==3)
		xie_LCDcom(0x80+0x40+4);
	
}
void jianpan()
{
	if(K1==0)
	{
		delay_ms(5);
		if(K1==0)
		{
			while(!K1);
			mode++;
			if(mode==1&&ok==0)
				xie_LCDcom(0x01);
			if(mode==2&&ok==0)
			{
				mode=0;
				select=0;
				xie_LCDcom(0x01);
				LCD_init();
			}
			if(select==0&&ok==1)
			{
				if(mode==4)
					mode=1;
			}
			if(select==1&&ok==1)
			{
				if(mode==4)
					mode=1;
			}
		}
	}
	if(K2==0)
	{
		delay_ms(5);
		if(K2==0)
		{
			while(!K2);
			num1++;
			if(mode==1&&ok==0)
			{
				select=~select;
				xie_LCDcom(0x01);
			}
		}
	}
	if(K3==0)
	{
		delay_ms(5);
		if(K3==0)
		{
			while(!K3);
			num2++;
			if(mode==1&&select==0)
			{
				ok=1;
				xie_LCDcom(0x01);
			}
			if(mode==1&&select==1)
			{
				ok=1;
				xie_LCDcom(0x01);
			}
		}
	}
	if(K4==0)
	{
		delay_ms(5);
		if(K4==0)
		{
			while(!K4);
			if(ok==1)
			{
				num1=0;
				num2=0;
				xie_DS1302(0x80,0x00);
	      xie_DS1302(0x8e,0x80);
				xie_LCDcom(0x01);
				DS1302init();
				LCD_init();
				mode=0;
				ok=0;
			}
		}
	}
	
}
void xianshi()
{
	uchar i;
	if(mode==0)
	{
		du_time();
		if((time[0]==naozhong[0])&&(time[1]&&naozhong[1])&&(time[2]==naozhong[2]))
		{
			BZ=~BZ;
			for(i=0;i<60;i++)
			{
				delay_ms(1000);
			}
		}
		miao=time[0];
		fen=time[1];
		shi=time[2];
		ri=time[3];
		yue=time[4];
		nian=time[6];
		xianshi_sfm(7,miao);
		xianshi_sfm(4,fen);
		xianshi_sfm(1,shi);
		xianshi_nyr(9,ri);
		xianshi_nyr(6,yue);
		xianshi_nyr(3,nian);
		xianshi_riqi(time[5]);
		//xianshi_wendu();
		
	}
	if(mode==1&&ok==0)
	{
		xianshi_caidan();
	}
	if(select==0&&ok==1)
	{
		xianshi_tjsj();
	}
	if(select==1&&ok==1)
	{
		xianshi_naozhong();
	}
}
void main()
{
	LCD_init();
	DS1302init();
	while(1)
	{
		jianpan();
		xianshi();
	}
}