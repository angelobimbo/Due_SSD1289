/*
 * Demo_App.c
 *
 * Created: 4/18/2013 4:19:20 PM
 *  Author: mfolz
 */ 
#include "Demo_App.h"


// CRAPPY TEST ROUTINE !!! YAY IT SUCKS
void testLibrary(void){
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	//ReadBMP_ARRAY(40,40,BMP_IMAGE2);
	for (int y= 0; y < 4; y++){
		pointProperties->Rotation= y; 
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		
			
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		
		for (int colorFill=0; colorFill <8; colorFill++){
			LCD_Draw_FillCircle(colorFill*20, 10*colorFill*2, 20+(colorFill*10), 1, colorArray[colorFill]);
		}
		
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		
		for (int colorFill=0; colorFill <16; colorFill++){
			LCD_Fill_Rectangle(0,0,500,500, colorArray[colorFill]);
		}
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);

		for (int x=0; x < 16; x++)
		{
			for (int y=0; y < 16; y++ )
			{
				LCD_Draw_Line(0,0,x*20,y*30,2,colorArray[x]);
			}
		}
	
		LCD_Fill_Rectangle(0,0,900,900, 0x000000); 
		Delay_s(2);
		LCD_Fill_Rectangle(0,0,900,900, 0x000000); 
			
		Delay_s(2);
	}
}