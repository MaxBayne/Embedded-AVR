#include "Common/Logs.h"

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
        _uart.Initialize(USART_CHANNEL_0,COMMUNICATION_MODE_ASYNC_NORMAL,F_CPU,9600,true,true);
    }

}

void Logs::WriteChar(uint8 character)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitByte(character);
    }
}

void Logs::WriteText(uint8* text)
{
    if(_source==LOGS_SOURCE_UART0)
    {
        _uart.TransmitString(text);
    }
}