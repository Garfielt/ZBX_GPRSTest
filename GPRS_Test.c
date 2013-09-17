#include <reg52.h>
#include <string.h> 

#define uchar unsigned char
#define uint unsigned int

uchar *cmd0 = "AT\n";
uchar *cmd1 = "AT+QAUDCH=0\n";
uchar *cmd2 = "AT+QAUDLOOP=1,0\n";
uchar data result[50];
sbit K1 = P0^0;
sbit KO = P0^2;
sbit LED = P0^6;
volatile uint sendflag = 0;
uint i = 0;
uint dtime = 500

void serial_init(void)
{ 
	TMOD=0x20;
	TH1=0xfd; 
	TL1=0xfd;
	TR1=1;
	SM0=0;
	SM1=1;
	ES=1;
	REN=1;
	EA=1;
}

void delaynms(uchar n)
{
    uchar t,a,b,c;
	for(t=n;t>0;t--) 
    	for(c=1;c>0;c--)
        	for(b=142;b>0;b--)
            	for(a=2;a>0;a--);
}

void send(uchar *cmd)
{
	int t;
	for(t=0;t<strlen(cmd);t++){
		SBUF=cmd[t];
		while(!TI);
		TI=0;
	}			  
}	  
 			  


void main()
{
	serial_init();
	while(1){
		KO = ~ KO;
		if(K1 == 0){
			delaynms(500);
			if(K1 == 0 && sendflag == 0){
				LED = 0;
				ES=0;
				send("\n");
				delaynms(dtime);
				send(cmd0);
				delaynms(dtime);
				send(cmd1);
				delaynms(dtime);
				send(cmd2);
				i = 0;
				sendflag = 1;
				ES=1;
			}
			while(K1 == 0);
			delaynms(500);
			LED = 0;
		}
		sendflag = 0;
	}
}

void serial() interrupt 4
{
	ES=0;
	RI=0;
	result[i++]=SBUF;
	ES=1;
 }
