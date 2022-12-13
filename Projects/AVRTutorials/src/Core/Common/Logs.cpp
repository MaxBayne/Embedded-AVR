#include "GPIO/GPIO.h"
#include "Common/Logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//Constructors
Logs::Logs()
{
    //Constructors
}

void Logs::Initialize(LOGS_SOURCE source)
{
    _source=source;
    
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.Initialize(USART_CHANNEL_0,USART_COMMUNICATION_MODE_ASYNC_NORMAL,F_CPU,9600,true,true);
    }

}

//Write String Text Over Log Source and type Enter on the end of line to start new line
void Logs::WriteLine(uint8* text)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitString(text);

        //Out Enter Key on the End of String line
        _uart.TransmitByte(0x0D);
        _uart.TransmitString("\n");

    }


    
}
//Write Enter Key as Hex Value to Make New Line
void Logs::NewLine()
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitByte(0x0D);
        _uart.TransmitString("\n");
    }

}


void Logs::WriteChar(char character)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitByte(character);
    }
}

void Logs::WriteBoolean(bool value)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        if(value==0)
        {
            _uart.TransmitString("False");
        }
        else
        {
            _uart.TransmitString("True");
        }
        
    }
}
void Logs::WriteBoolean(int value)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        if(value==0)
        {
            _uart.TransmitByte('0');
        }
        else
        {
            _uart.TransmitByte('1');
        }
        
    }
}

void Logs::WriteText(uint8* text)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitString(text);
    }
}

//Write (Bit) Value 0 or 1
void Logs::WriteBit(uint8 value)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        if(value==0)
        {
            _uart.TransmitByte('0');
        }
        else
        {
            _uart.TransmitByte('1');
        }
        
    }
}

//Write (Byte) Value For Register 8 Bit 0000 0000
void Logs::WriteByte(uint8 value)
{
    for (sint8 i = 7; i >= 0; i--)
    {
        if(BITWISE_GET_BIT(value,i)==0)
        {
            WriteChar('0');
        }
        else
        {
            WriteChar('1');
        }
        
        if(i==4)
        {
            WriteChar(' ');
        }
    }

    //Enter Key
    //WriteChar(0x0D);
}

//Write (Word) Value For Register 16 Bit (2 Byte) 0000 0000 0000 0000
void Logs::WriteWord(uint16 value)
{
    //Print High Byte
    uint8 highByte = BITWISE_GET_HIGH_BYTE(value);
    WriteText("HIGH: ");
    WriteByte(highByte);


    //Print LOW Byte
    uint8 lowByte = BITWISE_GET_LOW_BYTE(value);
    WriteText("LOW : ");
    WriteByte(lowByte);
}

//Write Integer as String (Char Array) Over Log Source
void Logs::WriteInteger(int number)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        //Convert integer to ascii
        char ascii[10];
        itoa(number,ascii,10);

        _uart.TransmitString(ascii);
    }

}

//Write Long Integer as String (Char Array) Over Log Source
void Logs::WriteLong(long number)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        //Convert integer to ascii
        char ascii[15];
        ltoa(number,ascii,10);

        _uart.TransmitString(ascii);
    }
}

//Write float decimal as String (Char Array) Over Log Source
void Logs::WriteFloat(float number,char decimalCount=4)
{

    if (_source == LOGS_SOURCE_UART0)
    {

        // create an empty string to store number
        char output[20] = {0};
        

        switch (decimalCount)
        {
            case 0:
                sprintf(output, "%.0f", number);
            break;

            case 1:
                sprintf(output, "%.1f", number);
            break;

            case 2:
                sprintf(output, "%.2f", number);
            break;

            case 3:
                sprintf(output, "%.3f", number);
            break;

            case 4:
                sprintf(output, "%.4f", number);
            break;

            case 5:
                sprintf(output, "%.5f", number);
            break;

            case 6:
                sprintf(output, "%.6f", number);
            break;

            case 7:
                sprintf(output, "%.7f", number);
            break;

            default:
                sprintf(output, "%f", number);
            break;

        }


        _uart.TransmitString(output);
    }
}

//Write (Byte | Decimal | Hex) Value For Register 8 Bit
void Logs::WriteByteInfo(uint8 value,bool withLabel=false)
{
    // create an empty string to store number
    char output[50] = {0};

    if(withLabel)
    {
        WriteText("BIN-> ");
        WriteByte(value);
        sprintf(output, " | DEC-> %d | HEX-> %x \n\r", value,value,value);

        _uart.TransmitString(output);
    }
    else
    {
        WriteByte(value);
        sprintf(output, " / %d / %x \n\r", value,value,value);

        _uart.TransmitString(output);
    }
}

//Write (Byte | Decimal | Hex) Value For Register 16 Bit
void Logs::WriteWordInfo(uint16 value,bool withLabel=false)
{
    //Print High Byte
    uint8 highByte = BITWISE_GET_HIGH_BYTE(value);
    WriteText("HIGH: ");
    WriteByteInfo(highByte,withLabel);


    //Print LOW Byte
    uint8 lowByte = BITWISE_GET_LOW_BYTE(value);
    WriteText("LOW : ");
    WriteByteInfo(lowByte,withLabel);
}

//Get the Reference of USART used with logs
USART* Logs::GetUSART()
{
    return &_uart;
}