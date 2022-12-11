#ifndef LOGS_H_
#define LOGS_H_

/*

//Declare Log instance
Logs log = Logs();

//Initialize Log Module
log.Initialize(LOGS_SOURCE_UART0);

//Write Log Anywhere
log.WriteText("Ima MAx Bayne n/");

*/


#include "Communications/USART.h"

#pragma region Enums

//Define The Source will be used to write logs like UART
typedef enum
{
	LOGS_SOURCE_UART0,
    LOGS_SOURCE_UART1,
	LOGS_SOURCE_SPI,
    LOGS_SOURCE_I2C
}LOGS_SOURCE;

#pragma endregion

class Logs
{
    private:
    LOGS_SOURCE _source;
    USART _uart;
    

    public:

    //Constructors ---------------------------
    Logs();


    public:
    //Methods --------------------------------

    //Config Module
    void Initialize(LOGS_SOURCE source);

    //Write String Text Over Log Source and type Enter on the end of line to start new line
    void WriteLine(uint8* text);

    //Write Enter Key as Hex Value to Make New Line
    void NewLine();



    //Write One Character over Log Source
    void WriteChar(char character);

    //Write boolean value True/False
    void WriteBoolean(bool value);

    //Write boolean value 1/0
    void WriteBoolean(int value);

    //Write String Text Over Log Source
    void WriteText(uint8* text);

    //Write (Bit) Value 0 or 1
    void WriteBit(uint8 value);

    //Write (Byte) Value For Register 8 Bit 0000 0000
    void WriteByte(uint8 value);

    //Write (Word) Value For Register 16 Bit (2 Byte) 0000 0000 0000 0000
    void WriteWord(uint16 value);

    //Write Integer as String (Char Array) Over Log Source
    void WriteInteger(int number);

    //Write Long Integer as String (Char Array) Over Log Source
    void WriteLong(long number);

    //Write float decimal as String (Char Array) Over Log Source
    void WriteFloat(float number,char decimalCount=4);

    //Write (Byte | Decimal | Hex) Value For Register 8 Bit
    void WriteByteInfo(uint8 value,bool withLabel=false);
    
    //Write (Byte | Decimal | Hex) Value For Register 16 Bit
    void WriteWordInfo(uint16 value,bool withLabel=false);

    //Get the Reference of USART used with logs
    USART* GetUSART();

};

#endif /* LOGS_H_ */