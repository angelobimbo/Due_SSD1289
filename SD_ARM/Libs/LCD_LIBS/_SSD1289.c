/*
 * _SSD1289.c
 *
 * Created: 4/18/2013 3:56:29 PM
 *  Author: mfolz
 */ 


#include "_SSD1289.h"
#include "../Delay/Delay.h"
#include "sam.h"

#define Delay			Delay_us(1) 
#define RS_LOW			REG_PIOC_CODR |= (RSPIN)
#define RS_HIGH			REG_PIOC_SODR |= (RSPIN)
#define WR_CS_LOW		REG_PIOC_CODR |= (CSPIN | WRPIN)
#define WR_CS_HIGH		REG_PIOC_SODR |= (CSPIN | WRPIN)

void LCD_Writ_Bus(char VH,char LowerByte);

void Send_Data_Fast(char UpperByte, char LowerByte);
// PAge 71 using 65k for the color
//0x60B0
// //these pins .... are addressed individually so it doest fucking matter
// RS		PIN38		-C6
// WR		PIN39		-C7
// CS		PIN40		-C8
// RESET	PIN41		-C9 

// send the SPI commands to
void SSD1289_SendCommand(int command_address, int command_data){
	/// Send Command Address
	RS_LOW;
	SSD1289_SendPacket(command_address >> 8, command_address);
	//// Send Command Data
	RS_HIGH; 
	SSD1289_SendPacket(command_data >> 8, command_data);
	RS_LOW; 
}

void SSD1289_SendPacket(char UpperByte, char LowerByte){
	Clear_Pins();		
	WR_CS_LOW; // pull WR low then write data	
// 	CLEARBIT(SSD_1289_RCW_PORT, SSD_1289_CS);
// 	CLEARBIT(SSD_1289_RCW_PORT, SSD_1289_WR); // pull WR low to send write data	
	//Send_Data((UpperByte <<8 )| LowerByte);
	Send_Data_Fast(UpperByte, LowerByte);
	//Send_Data((UpperByte << 8) | LowerByte);
// 	SETBIT(SSD_1289_RCW_PORT, SSD_1289_WR);
// 	SETBIT(SSD_1289_RCW_PORT, SSD_1289_CS);
	WR_CS_HIGH; // pull high after data has been sent 
	
}

void SSD1289_Initalize(){
	struct LCD_Properties *pointProperties = &Properties;
	//SSD_1289_LB_DDR = 0xFF;  /// Set Low Bit output 
	//SSD_1289_HB_DDR = 0xFF; // Set high bit output
	
	pointProperties->colorBytes = 16;
	pointProperties->xResolution = 240;
	pointProperties->yResolution = 320;

	//SSD_1289_RCW_DDR = (1 << SSD_1289_CS) | (1 << SSD_1289_WR) | (1 << SSD_1289_RESET);
	//SSD_1289_DA_DDR = (1 << SSD_1289_RS);
	REG_PIOC_SODR |= RESETPIN; 
	//SETBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	Delay_us(250);
	//CLEARBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	REG_PIOC_CODR |= RESETPIN; 
	Delay_us(250);
	//SETBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	REG_PIOC_SODR |= RESETPIN; 
	Delay_us(250);
	
	WR_CS_HIGH; // pull high after data has been sent
	

	SSD1289_SendCommand(Display_Control, 0x0021);
	SSD1289_SendCommand(Oscillator, 0x0001);
	SSD1289_SendCommand(Display_Control, 0x2B3F);
	SSD1289_SendCommand(Sleep_Mode, 0x0000);
	SSD1289_SendCommand(Display_Control, 0x0033);
	SSD1289_SendCommand(LCD_Waveform_Control, 0x0600);
	SSD1289_SendCommand(Driver_Output_Control ,0x2B3F); // check this
	SSD1289_SendCommand(Entry_Mode, 0x40B0);   //65k color  - type B color input // type c 8-R 8-G 8-B 8-blank


	SSD1289_SendCommand(Power_Control_1, 0x6664);
	SSD1289_SendCommand(Power_Control_2, 0x0000);
	SSD1289_SendCommand(Power_Control_3, 0x080C);
	SSD1289_SendCommand(Power_Control_4, 0x2B00);
	SSD1289_SendCommand(Power_Control_5, 0x0029);
	SSD1289_SendCommand(Compare_Register, 0x0000);
	SSD1289_SendCommand(Compare_Register_2, 0x0000);
	SSD1289_SendCommand(Horizontal_Porch, 0xEF1C);
	SSD1289_SendCommand(Frame_Frequency_Control, 0x6000);
	SSD1289_SendCommand(Vertical_Porch, 0x0003); // 0x0017 vertical porch
	SSD1289_SendCommand(Display_Control, 0x0233);  // can turn on and off the display; keeps image in gddr
	//SSD1289_SendCommand(0x000B, 0x0000);  // FRAMe cycle control

	SSD1289_SendCommand(Gate_Scan_Position, 0x0000); /// Starting position of the gate driver G0
	SSD1289_SendCommand(Gamma_Control_1,0x0007);
	SSD1289_SendCommand(Gamma_Control_2,0x0004);
	SSD1289_SendCommand(Gamma_Control_3,0x0004);
	SSD1289_SendCommand(Gamma_Control_4,0x0002);
	SSD1289_SendCommand(Gamma_Control_5,0x0007);
	SSD1289_SendCommand(Gamma_Control_6,0x0004);
	SSD1289_SendCommand(Gamma_Control_7,0x0004);
	SSD1289_SendCommand(Gamma_Control_8,0x0202);
	SSD1289_SendCommand(Gamma_Control_9,0x0002);
	SSD1289_SendCommand(Gamma_Control_10,0x0002);

	/// Scroll control and  screen driving Position
	/*
	SSD1289_SendCommand(Vertical_Scroll_Control, 0x0000);
	SSD1289_SendCommand(Vertical_Scroll_Control_2, 0x0000);
	SSD1289_SendCommand(Screen_driving_position, 0x0000);
	SSD1289_SendCommand(Screen_driving_position_2, 0x013F);
	SSD1289_SendCommand(Second_screen_driving_position, 0x0000);
	SSD1289_SendCommand(Second_screen_driving_position_2, 0x013F);
	*/
}


void SSD1289_Address_set()
{
	struct LCD_Geometry *pointCurrent = &Current;  /* To declare p as a pointer of type struct point */
	struct LCD_Properties *pointProperties = &Properties;  /* To declare p as a pointer of type struct point */
	
	SSD1289_SendCommand(Horizontal_Ram_Address_Pos,(pointCurrent->mem_x1Position<<8)+pointCurrent->mem_xPosition); // Horizontal ram address position
	SSD1289_SendCommand(Vertical_Ram_Address_Pos,pointCurrent->mem_yPosition);  // vertical ram address position
	SSD1289_SendCommand(Vertical_Ram_Address_Pos_2,pointCurrent->mem_y1Position);
	SSD1289_SendCommand(Horizontal_Ram_Address_X_Initial,pointCurrent->mem_xPosition); // initial settings for GDDRAM x
	SSD1289_SendCommand(Vertical_Ram_Address_Y_Initial,pointCurrent->mem_yPosition); // initial settings for GDDRAM y
	
	// Set Write to the GDDR All Packets sent afterwards will be taken as input color
	RS_LOW; // pull RS low to send command
	SSD1289_SendPacket(0x00, 0x22);	  /// write to gddr
	RS_HIGH;	// Pull back to high assume all information after will be RGB data
	
}




void SSD1289_SendPixel(int x, int y, long int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	int countx, county;
	int red, green, blue;
	pointCurrent->mem_xPosition= x; pointCurrent->mem_x1Position = (x+1);
	pointCurrent->mem_yPosition= y; pointCurrent->mem_y1Position = (y+1);
	
	red = (color >> 16);
	green = (color >> 8);
	blue = (color);
		// if xposition is greater than resolution return
	if (pointCurrent->mem_x1Position > pointProperties->xResolution || pointCurrent->mem_y1Position > pointProperties->yResolution){return;}

	SSD1289_Address_set();
	SSD1289_SendPacket(red, green);
	SSD1289_SendPacket(blue, 0x00);
	RS_LOW; // Pull RS back down to low default state
}



void SSD1289_Fill_Rectangle(int x, int y, int width, int height, long int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	int red, green, blue;
	int drawrow, drawcolumn;
	
	// Left side Portriait 
	if (pointProperties->Rotation == 1)
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->xResolution || y > pointProperties->yResolution || y< 0 || x < 0)return;

		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width > pointProperties->xResolution) width= pointProperties->xResolution - (x+1);
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y+1);

		pointCurrent->mem_xPosition= (pointProperties->xResolution - (x+width));
		pointCurrent->mem_x1Position = (pointProperties->xResolution - x)-1;
		
		pointCurrent->mem_yPosition= y;
		pointCurrent->mem_y1Position = ((y+height)-1);
	}
	
	// Right side Portriait 
	else if (pointProperties->Rotation == 2)
	{
	
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->xResolution || y > pointProperties->yResolution || y< 0 || x < 0)return;

		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width > pointProperties->xResolution) width= pointProperties->xResolution - (x+1);
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y+1);

		pointCurrent->mem_xPosition=x; 
		pointCurrent->mem_x1Position =(x+width)-1;
			
		pointCurrent->mem_yPosition= (pointProperties->yResolution - (y+height));
		pointCurrent->mem_y1Position = (pointProperties->yResolution - y)-1;
	}
	
	// Upside down  Landscape
	else if (pointProperties->Rotation == 3)
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->yResolution || y > pointProperties->xResolution || y< 0 || x < 0)return;
		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width  > pointProperties->yResolution) width = pointProperties->yResolution - (x+1);
		if(height+y > pointProperties->xResolution) height= pointProperties->xResolution -(y+1);
				
		// Put display in landscape mode swap the height and width values
		pointCurrent->mem_xPosition= (pointProperties->xResolution - (y+height)); 
		pointCurrent->mem_x1Position = (pointProperties->xResolution - y)-1;
		pointCurrent->mem_yPosition= (pointProperties->yResolution - ((x+width)));
		pointCurrent->mem_y1Position = (pointProperties->yResolution -x)-1;	
	}
	
	
	// default rotation LandScape
	else 
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->yResolution || y > pointProperties->xResolution || y< 0 || x < 0)return;
		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width  > pointProperties->yResolution) width = pointProperties->yResolution - (x+1);
		if(height+y > pointProperties->xResolution) height= pointProperties->xResolution -(y+1);
		
		// Put display in landscape mode swap the height and width values
		pointCurrent->mem_xPosition= y; 
		pointCurrent->mem_x1Position = ((y+height)-1);
		pointCurrent->mem_yPosition= x; 
		pointCurrent->mem_y1Position = ((x+width)-1);

	}

	red = (color >> 16);
	green = (color >> 8);
	blue = (color);
	// Set the lcd address

	SSD1289_Address_set();
	
	for(drawcolumn=0; drawcolumn<=height; drawcolumn++){
		for (drawrow=0; drawrow<=width; drawrow++)
		{
			SSD1289_SendPacket(red, green);
			SSD1289_SendPacket(blue, 0x00);
		}
	}
	RS_LOW; // Pull RS back down to low default state

}


// SO FUCKING UGLY
void Send_Data(int data){
	// 8 bytes
	Clear_Pins(); 
	char count=0;

	for (count =0 ; count <16;  count++){

		if (data & 0x01){
			switch(count){
				
				case 0:
				REG_PIOC_SODR |= DB0 ;
				//printf("DB0: 0x%x, ", DB0);
				break;
				
				case 1:
				REG_PIOC_SODR |= DB1;
				//printf("DB1: 0x%x, ", DB1);
				break;
				
				case 2:
				REG_PIOC_SODR |= DB2;
				//printf("DB2: 0x%x, ", DB2);
				break;
				
				case 3:
				REG_PIOC_SODR |= DB3;
				//printf("DB3: 0x%x, ", DB3);
				break;
				
				case 4:
				REG_PIOC_SODR |= DB4;
				//printf("DB4: 0x%x, ", DB4);
				break;
				
				case 5:
				REG_PIOD_SODR |= DB5;
				//printf("DB5: 0x%x, ", DB5);
				break;
				
				case 6:
				REG_PIOA_SODR |= DB6;
				//printf("DB6: 0x%x, ", DB6);
				break;
				
				case 7:
				REG_PIOD_SODR |= DB7;
				//printf("DB7: 0x%x, ", DB7);
				break;
				
				case 8:
				REG_PIOB_SODR |= DB8 ;
				//printf("DB8: 0x%x, ", DB8);
				break;
				
				case 9:
				REG_PIOA_SODR |= DB9;
				//printf("DB9: 0x%x, ", DB9);
				break;
				
				case 10:
				REG_PIOA_SODR |= DB10;
				//printf("DB10: 0x%x, ", DB10);
				break;
				
				case 11:
				REG_PIOD_SODR |= DB11;
				//printf("DB11: 0x%x, ", DB11);
				break;
				
				case 12:
				REG_PIOD_SODR |= DB12;
				//printf("DB12: 0x%x, ", DB12);
				break;
				
				case 13:
				REG_PIOD_SODR |= DB13;
				//printf("DB13: 0x%x, ", DB13);
				break;
				
				case 14:
				REG_PIOD_SODR |= DB14;
				//printf("DB14: 0x%x, ", DB14);
				break;
				
				case 15:
				REG_PIOD_SODR |= DB15;
				//printf("DB15: 0x%x, ", DB15);
				break;
			}
		}
		data = (data >> 1);
	}
}


void Clear_Pins(void){
	REG_PIOA_CODR |= (DB6 | DB9 | DB10);
	REG_PIOB_CODR |= (DB8);
	REG_PIOC_CODR |= (DB0 | DB1 | DB2 | DB3 | DB4);
	REG_PIOD_CODR |= (DB5 | DB7 | DB11 | DB12 | DB13| DB14 | DB15);
}


void Send_Data_Fast(char UpperByte, char LowerByte){
	//DB00 on PIN37 -> PIO_PC5
	REG_PIOC_SODR=(LowerByte<<5) & 0x20;
	//DB01 on PIN36 -> PIO_PC4
	REG_PIOC_SODR=(LowerByte<<3) & 0x10;
	//DB02 on PIN35 -> PIO_PC3
	REG_PIOC_SODR=(LowerByte<<1) & 0x08;
	//DB03 on PIN34 -> PIO_PC2
	REG_PIOC_SODR=(LowerByte>>1) & 0x04;
	//DB04 on PIN33 -> PIO_PC1
	REG_PIOC_SODR=(LowerByte>>3) & 0x02;
	//DB05 on PIN32 -> PIO_PD10
	REG_PIOD_SODR=(LowerByte<<5) & 0x400;
	//DB06 on PIN31 -> PIO_PA7
	REG_PIOA_SODR=(LowerByte<<1) & 0x80;
	//DB07 on PIN30 -> PIO_PD9
	REG_PIOD_SODR=(LowerByte<<2) & 0x200;
	//DB08 on PIN22 -> PIO_PB26
	REG_PIOB_SODR=(UpperByte<<26) & 0x4000000;
	//DB09 on PIN23 -> PIO_PA14
	//DB10 on PIN24 -> PIO_PA15
	REG_PIOA_SODR=(UpperByte<<13) & 0xC000;
	//DB11 on PIN25 -> PIO_PD0
	REG_PIOD_SODR=(UpperByte>>3) & 0x0F;
	//DB15 on PIN29 -> PIO_PD6
	REG_PIOD_SODR=(UpperByte>>1) & 0x40; 
}