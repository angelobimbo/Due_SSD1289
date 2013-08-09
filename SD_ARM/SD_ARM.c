/*
 * SSD1289_ARM.c
 *
 * Created: 7/31/2013 9:03:14 PM
 *  Author: Michael Folz
 */ 

#include "sam.h"
#include <stdio.h>
#include "Libs/Delay/Delay.h"
#include "Libs/LCD_LIBS/LCD_Config.h"
#include "Libs/LCD_LIBS/_SSD1289.h"
#include "Libs/LCD_LIBS/LCD_Draw.h"


/*
 SSD	ARDUINO		SAM3X8E
 // LOW BYTE
 DB0-   PIN37		-C5
 DB1-   PIN36		-C4
 DB2-   PIN35		-C3
 DB3-   PIN34		-C2
 DB4-   PIN33		-C1
 DB5-   PIN32		-D10
 DB6-   PIN31		-A7
 DB7-   PIN30		-D9
 // HIGH BYTE
  DB8-	PIN22		-B26
 DB9-	PIN23		-A14
 DB10-	PIN24		-A15
 DB11-	PIN25		-D0
 DB12-	PIN26		-D1
 DB13-	PIN27		-D2
 DB14-	PIN28		-D3
 DB15-	PIN29		-D6
 
 // For Both Upper and lower bits i will need to use ugly conditional statements 
 To properly address the pins. I think the best method will be 
 
 Likely just break it out into a few groups ... what a pain D: could be worse though
 its not that bad its only 6 pins, i am more worried about the micro SD considering ..uggg
 i will need to make my own software spi T.T

//these pins .... are addressed individually so it doest fucking matter
 RS		PIN38		-C6  // DHigh Data, Low Command 
 WR		PIN39		-C7  // Write signal Pin active low pull low b4 command pull high after 
 CS		PIN40		-C8 //  Chip Select Active Low
 RESET	PIN41		-C9 //  Reset Signal Active low 
 
 ////////////////////////////////////
 SD PINS 
 ////////////////////////////////////
 MISO	PIN50		-C13
 MOSI	PIN51		-C12
 SCK	PIN52		-B21
 SS		PIN53		-B14
 
 */
int main(void)
{
    /* Initialize the SAM system */
	SystemInit();
	REG_PIOA_OER = 0xFFFFFFF;
	REG_PIOB_OER = 0xFFFFFFF;
	REG_PIOC_OER = 0xFFFFFFF;
	REG_PIOD_OER = 0xFFFFFFF;
	SSD1289_Initalize();
 	Delay_us(350);
	
	while(1){
		testLibrary(); 
	}
}





