#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define unit unsigned int
uchar temp;
void delay(unit z);
void delay(unit z)
{
	unit x,y;
	for(x=100;x>0;x--)
		for(y=z;y>0;y--);
}
void main()
{
	temp=0xfe;
	while(1)
	{
		P2=temp;
		delay(200);
		temp=_crol_(temp,1);
		P2=temp;
		delay(200);
	}
	
}
