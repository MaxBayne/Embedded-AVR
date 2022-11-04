#ifndef LCD_H_
#define LCD_H_

/*
LCD_Initialize(EightBIT,PortC,Pin0,Pin1,Pin2,PortD,Pin0,Pin1,Pin2,Pin3,Pin4,Pin5,Pin6,Pin7);

LCD_Write_Text_With_Delay("Hello World");

*/

//DECLARE LCD ENUMS -------------------------------------------------------------

typedef enum 
{
	FourBIT,									//4 bit Mode use Data Pins (D7:D4)
	EightBIT									//8 bit Mode use All Data Pins (D7:D0)
} LCD_MODE;

//DECLARE LCD COMMANDS -------------------------------------------------------------

#define CMD_MODE_4_BIT 0x28						//2 line and 5 x 7 (D4:D7) 4 bit
#define CMD_MODE_8_BIT 0x38						//2 line and 5 x 7 (D0:D7) 8 bit

#define CMD_DISPLAY_CLEAR 0x01
#define CMD_DISPLAY_RETURN 0x02

#define CMD_DISPLAY_OFF_CURSOR_OFF 0x08
#define CMD_DISPLAY_OFF_CURSOR_ON 0x0A
#define CMD_DISPLAY_ON_CURSOR_OFF 0x0C
#define CMD_DISPLAY_ON_CURSOR_BLINK 0x0E
#define CMD_DISPLAY_OFF_CURSOR_BLINK 0x0F
#define CMD_DISPLAY_SHIFT_RIGHT 0x05
#define CMD_DISPLAY_SHIFT_LEFT 0x07
#define CMD_DISPLAY_ENTIRE_SHIFT_LEFT 0x18
#define CMD_DISPLAY_ENTIRE_SHIFT_RIGHT 0x1C

#define CMD_CURSOR_DECREMENT 0x04
#define CMD_CURSOR_INCREMENT 0x06
#define CMD_CURSOR_SHIFT_LEFT 0x10
#define CMD_CURSOR_SHIFT_RIGHT 0x14
#define CMD_CURSOR_BEING_FIRST_LINE 0x80
#define CMD_CURSOR_BEING_SECOND_LINE 0xC0



//DECLARE LCD Methods -------------------------------------------------------------

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
					);

//Send Command (Function Set) To LCD
void LCD_Command(uint8 command);

//Send Data To LCD as One Character
void LCD_Write_Character(char character);

//Send Data To LCD as String
void LCD_Write_Text(char * text);

//Send Data To LCD as String with delay for every character
void LCD_Write_Text_With_Delay(char * text);

//Set High and Low To Enable Pin
void LCD_Enable_FallingEdge();

//Set Cursor on first Line
void LCD_Go_To_FirstLine();

//Set Cursor on second Line
void LCD_Go_To_SecondLine();

//Set Cursor on Position
void LCD_Go_To_Position(uint8 row,uint8 column);

#endif /* LCD_H_ */