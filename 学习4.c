#include<reg52.h>
#include<intrins.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef char int8;
typedef int int16;
sbit RS=P2^6;
sbit RW=P2^5;
sbit EN=P2^7;
sbit BUSY=P0^7;
sbit DQ=P3^7; //?????
uint8 code word[]=
{
  "Temperature :"
};
uint8 num_temp[]={"0123456789"};
#define nops();  {_nop_(); _nop_(); _nop_(); _nop_();} //????? 4us

void delay(uint16 n)
{
  while (n--);
}
void delay_ms(uint16 m)
{
  uint8 n=120;
  while(m--)
    while(n--);
}
/*
 * 18B20????
*/
void DS1802_reset()
{
  uint8 flag=1;
  while(flag)
  {
    while(flag)
    {
      DQ=1;
      delay(1);
      DQ=0;
      delay(50); //550us
      DQ=1;
      delay(6); //66us
      flag=DQ;
    }
    delay(45);//??500us
    flag=~DQ;//500us?????,DQ=1;   
  }
  DQ=1;  
}
/*
 * 18B20?1?????
 * ?1-WIRE????????
*/
void write_byte(uint8 dat)
{
  uint8 i;
  for(i=0;i<8;i++)
  {
    DQ=1;
    _nop_();
    DQ=0;
    nops();
    DQ=dat&0x01;//?????
    delay(6); //66us
    dat>>=1; //????
  }
  DQ=1;
  delay(1);
}

/*
 * 18B20?1?????
 * ?1-WIRE?????????
*/
uint8 read_byte()
{
  uint8 i,byte=0;
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
    delay(6);
  }
  DQ=1;
  return byte; 
}
/*
 * ??????
*/
void start_temp_sensor()
{
  DS1802_reset();
  write_byte(0xcc);  // ?Skip ROM??
  write_byte(0x44); // ????? 
}
/*
 * ????
*/
int16 read_temp()
{
  uint8 temp_data[2]; // ??????
  uint16 temp;
  DS1802_reset();   // ??
  write_byte(0xcc);  // ?Skip ROM??
  write_byte(0xbe); // ????
  temp_data[0]=read_byte(); //???8?
  temp_data[1]=read_byte(); //???8?
  temp=temp_data[1];
  temp<<=8;
  temp|=temp_data[0];
  //temp>>=4; //????,?????16(1/16=0.0625)
  return temp;
}
/*
 * ????
*/
void wait()
{
  P0=0xff;
  do
  {
    EN=0;
    RS=0;
    RW=1;
    EN=1;
  }while(BUSY==1);
  EN=0;
}
/*
 * ???
*/
void write_cmd(uint8 cmd)
{
  wait();
  EN=0;
  P0=cmd;
  RS=0;
  RW=0;
  EN=1;
  EN=0;
}
/*
 * ???
*/
void write_dat(uint8 dat)
{
  wait();
  EN=0;
  P0=dat;
  RS=1;
  RW=0;
  EN=1;
  EN=0;
}
/*
 * ???
*/
void lcd1602_init()
{
  delay_ms(15);
  write_cmd(0x38);
  delay_ms(5);
  write_cmd(0x38);
  delay_ms(5);
  write_cmd(0x38);
  delay_ms(5);

  write_cmd(0x38);
  write_cmd(0x0c);
  write_cmd(0x06);
  write_cmd(0x01);
}
/*
 * ??????
*/
void write_lcd1602(uint8 addr,uint8 dat)
{
  write_cmd(addr);
  write_dat(dat);
}
/*
 * ????
*/
void write_string(uint8 addr,uint8 *p)
{
  write_cmd(addr);
  while(*p!='\0')
  {
    write_dat(*p++) ;
  } 
}
/*
 * ???
*/
void main()
{
  uint16 temp;
  lcd1602_init();
  while(1)
  {
    start_temp_sensor();
   delay_ms(100);
    temp=read_temp();
    if(temp&0x8000) //??????
    {
      temp=~temp+1;
      temp=(temp*0.0625)*10+0.5;
      write_lcd1602(0xc0,'-');
      write_lcd1602(0xc1,num_temp[temp/1000]);
      write_lcd1602(0xc2,num_temp[(temp%1000)/100]);
			write_lcd1602(0xc3,'.');
      write_lcd1602(0xc4,num_temp[(temp%100)/10]);
      write_lcd1602(0xc5,num_temp[temp%10]);
   
    }
    else
    {
      temp=(temp*0.0625)*100+0.5 ;
      write_lcd1602(0xc0,'+');
      write_lcd1602(0xc1,num_temp[temp/1000]);
      write_lcd1602(0xc2,num_temp[(temp%1000)/100]);
			write_lcd1602(0xc3,'.');
      write_lcd1602(0xc4,num_temp[(temp%100)/10]);
      write_lcd1602(0xc5,num_temp[temp%10]);
   
    }
    write_string(0x80,word);  //????
  } 
}