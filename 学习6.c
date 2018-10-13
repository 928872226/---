#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit CEN=P3^5;
sbit IO=P3^4;
sbit SCLK=P3^6;
sbit EN=P2^7;
sbit RWN=P2^5;
sbit RSN=P2^6;
uchar code xie_add[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
uchar code du_add[]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
uchar time[7]={50,59,17,13,10,6,18};
uchar miao,fen,shi,nian,yue,ri;
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=100;y>0;y--);
}
void xie_zijie(uchar zijie)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		IO=zijie&0x01;
		zijie>>=1;
		SCLK=1;
	}
}
void xie_ds(uchar add,uchar shuju)
{
	CEN=0;
	_nop_();
	SCLK=0;
	_nop_();
	CEN=1;
	_nop_();
	xie_zijie(add);
	xie_zijie(shuju);
	CEN=0;
}
uchar du_ds(uchar add)
{
	uchar i,shuju=0x00;
	CEN=0;
	_nop_();
	_nop_();
	SCLK=0;
	_nop_();
	_nop_();
	CEN=1;
	_nop_();
	_nop_();
	xie_zijie(add);
	for(i=0;i<8;i++)
	{
		if(IO==1)
			shuju|=0x80;
		SCLK=0;
		shuju>>=1;
		_nop_();
		_nop_();
		_nop_();
		SCLK=1;
	}
	CEN=0;
	_nop_();
	_nop_();
	CEN=0;
	SCLK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCLK=1;
	_nop_();
	_nop_();
	IO=0;
	_nop_();
	_nop_();
	IO=1;
	_nop_();
	_nop_();
	return shuju;
}
void du_time()
{
	uchar i;
	for(i=0;i<7;i++)
	{
		time[i]=du_ds(du_add[i]);
	}
}
uchar Dec_BCD(uchar Dec)
{
	uchar shu,shu1;
	shu1=Dec/10;
	shu=Dec%10+shu1*16;
	return shu;
}
uchar BCD_Dec(uchar BCD)
{
	uchar shu,shu1;
	shu1=BCD/16;
	shu=shu1*10+(BCD&0x0f);
	return shu;
}
void ds_init()
{
	uchar i;
	CEN=0;
	SCLK=0;
	for(i=0;i<7;i++)
	{
		time[i]=Dec_BCD(time[i]);
	}
	xie_ds(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		xie_ds(xie_add[i],time[i]);
	}	
	xie_ds(0x8e,0x80);
}
void xie_LCDcom(uchar com)
{
	RSN=0;
	RWN=0;
	EN=0;
	P0=com;
	delay(5);
	EN=1;
	delay(10);
	EN=0;
}
void xie_LCDshuju(uchar shuju)
{
	RSN=1;
	RWN=0;
	EN=0;
	P0=shuju;
	delay(5);
	EN=1;
	delay(10);
	EN=0;
}
void LCD_init()
{
	EN=0;
	xie_LCDcom(0x38);
	xie_LCDcom(0x0c);
	xie_LCDcom(0x06);
	xie_LCDcom(0x01);
	xie_LCDcom(0x80+0x46);
	xie_LCDshuju(':');
	xie_LCDcom(0x80+0x49);
	xie_LCDshuju(':');
	xie_LCDcom(0x81);
	xie_LCDshuju('2');
	xie_LCDshuju('0');
	xie_LCDcom(0x85);
	xie_LCDshuju('-');
	xie_LCDcom(0x88);
	xie_LCDshuju('-');
	
}
void xianshi_sfm(uchar add,uchar shuju)
{
	uchar shi,ge;
	shi=shuju/10;
	ge=shuju%10;
	xie_LCDcom(0x80+0x40+add);
	xie_LCDshuju(0x30+shi);
	xie_LCDshuju(0x30+ge);
}
void xianshi_nyr(uchar add,uchar shuju)
{
	uchar shi,ge;
	shi=shuju/10;
	ge=shuju%10;
	xie_LCDcom(0x80+add);
	xie_LCDshuju(0x30+shi);
	xie_LCDshuju(0x30+ge);
}
void xianshi_riqi(uchar shuju)
{
	xie_LCDcom(0x80+12);
	switch(shuju)
	{
		case(1):
			xie_LCDshuju(0x4d);xie_LCDshuju(0x6f);xie_LCDshuju(0x6e);break;
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
void main()
{
	uchar i;
	ds_init();
	LCD_init();
	while(1)
	{
		du_time();
		for(i=0;i<7;i++)
		{
			time[i]=BCD_Dec(time[i]);
		}
		miao=time[0];
		xianshi_sfm(10,miao);
		fen=time[1];
		xianshi_sfm(7,fen);
		shi=time[2];
		xianshi_sfm(4,shi);
		nian=time[6];
		xianshi_nyr(3,nian);
		yue=time[4];
		xianshi_nyr(6,yue);
		ri=time[3];
		xianshi_nyr(9,ri);
		xianshi_riqi(time[5]);
	}
}