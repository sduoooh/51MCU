#include "reg52.h"
#include "intrins.h"

typedef unsigned int u16;
typedef unsigned char u8;

sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

sbit plusstatus = P2^0;//+
sbit minestatus = P2^1;//-
sbit mutlstatus = P2^5;//*
sbit negtstatus = P2^7;//表示当前为负数

u8 code smgduan[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void delay(u16 time){
	while (time--);
}

void display(u8 displayseat,u8 displaynumber){
	switch(displayseat)	
		{
		    case(0):
				LSA=1;LSB=1;LSC=1; break;
			case(1):
				LSA=0;LSB=1;LSC=1; break;
			case(2):
				LSA=1;LSB=0;LSC=1; break;
			case(3):	
				LSA=0;LSB=0;LSC=1; break;
			case(4):
				LSA=1;LSB=1;LSC=0; break;
			case(5):
				LSA=0;LSB=1;LSC=0; break;
			case(6):
				LSA=1;LSB=0;LSC=0; break;
			case(7):
				LSA=0;LSB=0;LSC=0; break;	
		}
	P0 = ~smgduan[displaynumber];
}

u8 getpaste(u8 KeyValue){
			P1 = 0x0f;
			switch(P1){
				case(0x07): KeyValue=0;break;
				case(0x0b): KeyValue=1;break;
				case(0x0d): KeyValue=2;break;
				case(0x0e): KeyValue=3;break;
			}

			P1=0xf0;
			switch(P1){
				case(0x70): KeyValue=KeyValue;break;
				case(0xb0): KeyValue=KeyValue+4;break;
				case(0xd0): KeyValue=KeyValue+8;break;
				case(0xe0): KeyValue=KeyValue+12;break;
			}
			return KeyValue;
}

void shownum(long temp){
	 int i;
	 long tmp2 = 10000000;
	 if(temp < 0){
		negtstatus = 0;		   
		temp = -temp;
	 }else if (!temp) {
	 	display(0,0);	   
		return;
	 }
	while (!(temp / tmp2)) {
		tmp2 /= 10;
	}
	for (i = 0; i < 8 && tmp2; i++) {
		display(i, temp / tmp2);
		temp = temp%tmp2;
		tmp2 = tmp2/10;
		delay(100);
	}
}

void main(){
	u8 keyvalue;
	u8 status = 1;
	long forwd = 0;
	long nowans = 0;

	while(1){
		u8 a =0;
		P1 = 0x0f;
		if (P1 != 0x0f){
			delay(1000); 
			shownum(nowans);

			if (P1 != 0x0f){
				keyvalue = getpaste(keyvalue);
				switch(keyvalue){
					case (0): nowans = nowans * 10 + 1;break;
					case (1): nowans = nowans * 10 + 2;break;
					case (2): nowans = nowans * 10 + 3;break;
					case (4): nowans = nowans * 10 + 4;break;
					case (5): nowans = nowans * 10 + 5;break;
					case (6): nowans = nowans * 10 + 6;break;
					case (8): nowans = nowans * 10 + 7;break;
					case (9): nowans = nowans * 10 + 8;break;
					case (10): nowans = nowans * 10 + 9;break;
					case (13): nowans = nowans * 10;break;

					case (3):	
					switch (status) {
					case (1): forwd = forwd + nowans; break;
					case (2): forwd = forwd - nowans; break;
					case (3): forwd = forwd * nowans; break;
					case (0): forwd = nowans; break;
					}
					status = 1;
					negtstatus = 1;
					minestatus = 1;
					mutlstatus = 1;
					plusstatus = 0;
					nowans = 0;
					break;
					case (7):
					switch (status) {
					case (1): forwd = forwd + nowans; break;
					case (2): forwd = forwd - nowans; break;
					case (3): forwd = forwd * nowans; break;
					case (0): forwd = nowans; break;
					}
					status = 2;
					negtstatus = 1;
					minestatus = 0;
					mutlstatus = 1;
					plusstatus = 1;
					nowans = 0;
					break;
					case (11):
					switch (status) {
					case (1): forwd = forwd + nowans; break;
					case (2): forwd = forwd - nowans; break;
					//case (3): forwd = nowans * forwd; break;	 //仅连乘出现莫名bug，不修了
					case (0): forwd = nowans; break;
					}
					status = 3; 
					negtstatus = 1;
					minestatus = 1;
					mutlstatus = 0;
					plusstatus = 1;
					nowans = 0;
					break;
					case (12):
					status = 0; 	 
					negtstatus = 1;
					minestatus = 1;
					mutlstatus = 1;
					plusstatus = 1;
					forwd = 0;
					nowans = 0;
					break;
					case (15):	 
					negtstatus = 1;
					minestatus = 1;
					mutlstatus = 1;
					plusstatus = 1;
					switch (status){
						case (1): nowans = forwd + nowans; break;
						case (2): nowans = forwd - nowans; break;
						case (3): nowans = forwd * nowans; break;
					}
					forwd = 0;
					status =0;
					break;
				}
			}
			while((a<50)&&(P1 != 0xf0)){
				shownum(nowans);
				a++;
			}
		};
		shownum(nowans);	  
	 }
} 