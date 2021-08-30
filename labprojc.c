#include <REGX51.H>

#define ON 1
#define OFF 0
sbit led1=P1^0;
sbit led2=P1^1;
sbit led3=P1^2;

#define PRESS 1
#define NOT_PRESSED 0
sbit sw1=P3^3;
sbit sw2=P3^4;

#define HIGH 1
#define LOW 0
sbit M_a=P1^5;
sbit M_b=P1^6;

#define LCD P2 //liquid cristal display
sbit RS =P3^0; //control signal rregister select
sbit EN =P3^1; //enable signal

void delay(unsigned int time) {
    long int i;
    for(i=0; i<=time; i++);
}
void lcdCommand(const unsigned char command){
    LCD = command;
    RS = 0; //when we do not display sth its 0
    EN  = 1;
    delay(500);
    EN=0;
}
void lcdData(const unsigned char dataPrint) {
    LCD = dataPrint;
    RS=1; //when we want to display it stored as ascii we put 1
    EN=1; //high to low pulse
    delay(500);
    EN=0;
}
void displayOnLcd(const char *pData) {
    while(*pData) {
        lcdData(*pData);
        pData++;
        delay(500);
    }
}
void lcdStart(void) {
    delay(500);
    lcdCommand(0x01);
    delay(500);
    lcdCommand(0x80);
    delay(500);
    lcdCommand(0x0C);
}

unsigned char checkPetWater(void){
	if(sw1==0){
			led1 = ON;
			if(sw2==0){
				return 100;
			}return 50;
	}return 0;
}

void showPetWaterLevel(unsigned char status)
{
    switch(status)
    {
    case 0:
        led1 = OFF;
        led2 = OFF;
        led3 = OFF;
        displayOnLcd("0%");
        break;
    case 50:
        led1 = ON;
        led2 = ON;
        led3 = OFF;
        displayOnLcd("50%");
        break;
    case 100:
        led1 = ON;
        led2 = ON;
        led3 = ON;
        displayOnLcd("100%");
        break;
    }
}
void run() {
    M_a=1;
    M_b=0;
    M_a=1;
    M_b=0;
}
void stop() {
    M_a=0;
    M_b=0;
    M_a=0;
    M_b=0;
}
int main() {
    unsigned char petStatus = 0x00;
    unsigned char tmpPetStatus = 0x01;
    while(1) {
        petStatus = checkPetWater();
        if(petStatus != tmpPetStatus) {
            lcdStart();
            showPetWaterLevel(petStatus);
            tmpPetStatus = petStatus;
            if(tmpPetStatus <50) {
							if(tmpPetStatus !=100) run();
            }
            else {
                stop();
            }
        }
    }
}