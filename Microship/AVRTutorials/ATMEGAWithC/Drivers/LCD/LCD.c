#include <avr/io.h>
#include "Common.h"
#include "GPIO.h"
#include "LCD.h"


//Private Variables -------------------------------------------
static volatile LCD_MODE _LCD_Mode;
static volatile PORT _Ctrl_Port;
static volatile PIN _Ctrl_RS_Pin;
static volatile PIN _Ctrl_RW_Pin;
static volatile PIN _Ctrl_E_Pin;
static volatile PORT _Data_Port;
static volatile PIN _Data_D0_Pin;
static volatile PIN _Data_D1_Pin;
static volatile PIN _Data_D2_Pin;
static volatile PIN _Data_D3_Pin;
static volatile PIN _Data_D4_Pin;
static volatile PIN _Data_D5_Pin;
static volatile PIN _Data_D6_Pin;
static volatile PIN _Data_D7_Pin;


//Public Methods ------------------------------------------------

//Set High and Low To Enable Pin
void LCD_Enable_FallingEdge()
{
	//Tell LCD To Read Data From Pins by Falling Edge For Enable Pin
	
	//Set Enable as High
	Pin_Write(_Ctrl_Port,_Ctrl_E_Pin,HIGH);
	
	delay_ms(2);
	
	//Set Enable as Low
	Pin_Write(_Ctrl_Port,_Ctrl_E_Pin,LOW);
	
}


//Send Command (Function Set) To LCD
void LCD_Command(uint8 command)
{
	//Set Register Select As Command [0] because we need to send Command
	Pin_Write(_Ctrl_Port,_Ctrl_RS_Pin,LOW);
	
	//Set Read/Write as Low mean We Need Write
	Pin_Write(_Ctrl_Port,_Ctrl_RW_Pin,LOW);
	
	if(_LCD_Mode==FourBIT)
	{
				
		//4 Bit Mode use Pins Data (D7:D4)
		
		//Split 8 bit Command to Two Side ( 4 Most Significant Bits) (4 Least Significant Bits) and Send the Most Significant Bits First
		//MSB(High)       LSB(LOW)
		//D7 D6 D5 D4     D3 D2 D1 D0
		//0  0	0  0      0  0  0  0
		
		//Send the 4 High Bits by anding high bits with 1(F) and Low bits with 0
		//*((volatile unsigned char *)_Data_Port)=command & 0xF0;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(command,7));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(command,6));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(command,5));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(command,4));
		
		LCD_Enable_FallingEdge();
		
		//Send the 4 Low Bits
		//*((volatile unsigned char *)_Data_Port)=command<<4;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(command,3));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(command,2));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(command,1));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(command,0));
		
		LCD_Enable_FallingEdge();
		
		
		
	}
	else if(_LCD_Mode==EightBIT)
	{
		//8 Bit Mode use All Pins Data (D7:D0)
		//*((volatile unsigned char *)_Data_Port)=command;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(command,7));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(command,6));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(command,5));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(command,4));
		Pin_Write(_Data_Port,_Data_D3_Pin,get_bit(command,3));
		Pin_Write(_Data_Port,_Data_D2_Pin,get_bit(command,2));
		Pin_Write(_Data_Port,_Data_D1_Pin,get_bit(command,1));
		Pin_Write(_Data_Port,_Data_D0_Pin,get_bit(command,0));
		
		LCD_Enable_FallingEdge();
	}
	
	_delay_ms(25);
	
}

//Send Data To LCD as One Character
void LCD_Write_Character(char character)
{
	//Set Register Select As Data [1] because we need to send Data
	Pin_Write(_Ctrl_Port,_Ctrl_RS_Pin,HIGH);
	
	//Set Read/Write as Low mean We Need Write
	Pin_Write(_Ctrl_Port,_Ctrl_RW_Pin,LOW);
	
	
	if(_LCD_Mode==FourBIT)
	{
				
		//4 Bit Mode use Pins Data (D7:D4)
		
		//Split 8 bit Character to Two Side ( 4 Most Significant Bits) (4 Least Significant Bits) and Send the Most Significant Bits First
		//MSB(High)       LSB(LOW)
		//D7 D6 D5 D4     D3 D2 D1 D0
		//0  0	0  0      0  0  0  0
		
		//Send the 4 High Bits by anding high bits with 1(F) and Low bits with 0
		//*((volatile unsigned char *)_Data_Port)=character & 0xF0;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(character,7));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(character,6));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(character,5));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(character,4));
		
		LCD_Enable_FallingEdge();
		
		//Send the 4 Low Bits
		//*((volatile unsigned char *)_Data_Port)=character<<4;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(character,3));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(character,2));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(character,1));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(character,0));
		
		LCD_Enable_FallingEdge();
		
		
		
	}
	else if(_LCD_Mode==EightBIT)
	{
		//8 Bit Mode use All Pins Data (D7:D0)
		//*((volatile unsigned char *)_Data_Port)=character;
		
		Pin_Write(_Data_Port,_Data_D7_Pin,get_bit(character,7));
		Pin_Write(_Data_Port,_Data_D6_Pin,get_bit(character,6));
		Pin_Write(_Data_Port,_Data_D5_Pin,get_bit(character,5));
		Pin_Write(_Data_Port,_Data_D4_Pin,get_bit(character,4));
		Pin_Write(_Data_Port,_Data_D3_Pin,get_bit(character,3));
		Pin_Write(_Data_Port,_Data_D2_Pin,get_bit(character,2));
		Pin_Write(_Data_Port,_Data_D1_Pin,get_bit(character,1));
		Pin_Write(_Data_Port,_Data_D0_Pin,get_bit(character,0));
		
		LCD_Enable_FallingEdge();
	}
	
	_delay_ms(2);
	
}

//Send Data To LCD as String
void LCD_Write_Text(char * text)
{
	uint8 counter=0;
	
	while (text[counter]!=0)
	{
		LCD_Write_Character(text[counter]);
		counter++;
	}
	
}

//Send Data To LCD as String with delay for every character
void LCD_Write_Text_With_Delay(char * text)
{
	uint8 counter=0;
	
	while (text[counter]!=0)
	{
		LCD_Write_Character(text[counter]);
		
		counter++;
		
		delay_ms(1000);
	}
}

//Initialize LCD
void LCD_Initialize(LCD_MODE Mode,			    //Mode 4 bit or 8 bit
					PORT Ctrl_Port,				//Control Port for (RS,RW,E) like A,B,C,D etc ..
					PIN Ctrl_RS_Pin,			//Register Select Pin where 0 for Command Register , 1 for Data Register
					PIN Ctrl_RW_Pin,			//Read/Write Operation where 0 For Write and 1 For Read
					PIN Ctrl_E_Pin,			    //Enable Pin for Latching Data Pins
					PORT Data_Port,				//Data Port for (D0:D7) like A,B,C,D etc ..
					PIN Data_D0_Pin,			//D0 Pin
					PIN Data_D1_Pin,			//D1 Pin
					PIN Data_D2_Pin,			//D2 Pin
					PIN Data_D3_Pin,			//D3 Pin
					PIN Data_D4_Pin,			//D4 Pin
					PIN Data_D5_Pin,			//D5 Pin
					PIN Data_D6_Pin,			//D6 Pin
					PIN Data_D7_Pin			    //D7 Pin
					)
{
	//Set Reused Value as private variables
	_LCD_Mode=Mode;
	_Ctrl_Port=Ctrl_Port;
	_Ctrl_RS_Pin=Ctrl_RS_Pin;
	_Ctrl_RW_Pin=Ctrl_RW_Pin;
	_Ctrl_E_Pin=Ctrl_E_Pin;
	_Data_Port=Data_Port;
	_Data_D0_Pin=Data_D0_Pin;
	_Data_D1_Pin=Data_D1_Pin;
	_Data_D2_Pin=Data_D2_Pin;
	_Data_D3_Pin=Data_D3_Pin;
	_Data_D4_Pin=Data_D4_Pin;
	_Data_D5_Pin=Data_D5_Pin;
	_Data_D6_Pin=Data_D6_Pin;
	_Data_D7_Pin=Data_D7_Pin;
	
	
	//Set Control Pins as Output Data Direction
	Pin_Direction(_Ctrl_Port,_Ctrl_RS_Pin, OUTPUT);//RS Pin as Output
	Pin_Direction(_Ctrl_Port,_Ctrl_RW_Pin,OUTPUT);//R/W Pin as Output
	Pin_Direction(_Ctrl_Port,_Ctrl_E_Pin,OUTPUT); //E Pin as Output
	
	//Set Control Initial Values
	Pin_Write(_Ctrl_Port,_Ctrl_RS_Pin,LOW);
	Pin_Write(_Ctrl_Port,_Ctrl_RW_Pin,LOW);
	Pin_Write(_Ctrl_Port,_Ctrl_E_Pin,LOW);
	
	
	//Set Data Pins as Output Data Direction
	Pin_Direction(_Data_Port,_Data_D0_Pin,OUTPUT);//D0 Pin as Output
	Pin_Direction(_Data_Port,_Data_D1_Pin,OUTPUT);//D1 Pin as Output
	Pin_Direction(_Data_Port,_Data_D2_Pin,OUTPUT);//D2 Pin as Output
	Pin_Direction(_Data_Port,_Data_D3_Pin,OUTPUT);//D3 Pin as Output
	Pin_Direction(_Data_Port,_Data_D4_Pin,OUTPUT);//D4 Pin as Output
	Pin_Direction(_Data_Port,_Data_D5_Pin,OUTPUT);//D5 Pin as Output
	Pin_Direction(_Data_Port,_Data_D6_Pin,OUTPUT);//D6 Pin as Output
	Pin_Direction(_Data_Port,_Data_D7_Pin,OUTPUT);//D7 Pin as Output
	
	//Set Data Pins as Low For Initial Value
	
	Pin_Write(_Data_Port,_Data_D0_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D1_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D2_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D3_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D4_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D5_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D6_Pin,LOW);
	Pin_Write(_Data_Port,_Data_D7_Pin,LOW);
	
	
	//Check LCD Mode 4 bit or 8 bit
	if(_LCD_Mode==FourBIT)
	{
		//4 Bit LCD
		
		LCD_Command(0x33);
		LCD_Command(0x32);
		LCD_Command(CMD_MODE_4_BIT);
		LCD_Command(0x0e);
		LCD_Command(0x01);
		
		delay_ms(1);
		
		LCD_Command(0x06);
		
		delay_ms(1);
	}
	else if(_LCD_Mode==EightBIT)
	{
		//8 Bit LCD
		
		LCD_Command(0x33);
		LCD_Command(0x32);
		LCD_Command(CMD_MODE_8_BIT);
		LCD_Command(0x0e);
		LCD_Command(0x01);
		
		delay_ms(1);
		
		LCD_Command(0x06);
		
		delay_ms(1);
	}
	
	delay_ms(15);
	
	
}



//Set Cursor on first Line
void LCD_Go_To_FirstLine()
{
	LCD_Command(CMD_CURSOR_BEING_FIRST_LINE);
}

//Set Cursor on second Line
void LCD_Go_To_SecondLine()
{
	LCD_Command(CMD_CURSOR_BEING_SECOND_LINE);
}

//Set Cursor on Position
void LCD_Go_To_Position(uint8 row,uint8 column)
{
	uint8 address;
	
	address = (row * 0x40)+column;
	address = set_bit(address,7);
	
	LCD_Command(address);
	
}