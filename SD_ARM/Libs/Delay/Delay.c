/*
 * Delay.c
 *
 * Created: 7/31/2013 5:12:33 PM
 *  Author: mfolz
 */ 
#include "Delay.h"

void Delay_s(int delays){
	while (delays != 0){
		Delay_ms(1000);
		delays--;
	}
}

void Delay_ms(int delayms){
	while (delayms != 0){
		Delay_us(1000);
		delayms--;
	}
}
void Delay_us(int delayus){
	while (delayus != 0){
		Delay();
		delayus--;
	}

}

void Delay(void){
	float usdelay = 0.0;
	usdelay =(1000/ ( (1.0 / 28000000) * 1000000000 ) );
	
	while (usdelay != 0) {
		asm("NOP");
		usdelay--;
	}
}

