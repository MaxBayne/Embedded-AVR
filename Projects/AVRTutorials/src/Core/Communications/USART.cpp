#include "Core/Core.h"
#include "Communications/USART.h"

#pragma region Static Members 
bool USART::Is_UART0_Initialized;
bool USART::Is_UART1_Initialized;
bool USART::Is_UART2_Initialized;
#pragma endregion

#pragma region Constructors

// Constructors
USART::USART()
{
    
}

#pragma endregion

#pragma region Methods

//[Completed]
//Initialize Module
void USART::Initialize(USART_CHANNEL channel, USART_COMMUNICATION_MODE mode, uint64 frequency,uint32 baudRate, bool canReceive, bool canTransmit)
{

    //Avoid Re-Initialized Module Again
    if(channel==USART_CHANNEL_0 && Is_UART0_Initialized)
    {
        return;
    }
    else if(channel==USART_CHANNEL_1 && Is_UART1_Initialized)
    {
        return;
    }
    else if(channel==USART_CHANNEL_2 && Is_UART2_Initialized)
    {
        return;
    }



    _baudRate = baudRate;
    _channel = channel;
    _mode = mode;
    _frequency = frequency;
    _canReceive = canReceive;
    _canTransmit = canTransmit;
    
    // enable/Disable Receive Data
    if (_canReceive)
    {
        EnableReceive();
    }
    else
    {
        DisableReceive();
    }

    // enable/Disable Transfer Data
    if (_canTransmit)
    {
        EnableTransmit();
    }
    else
    {
        DisableTransmit();
    }

    // Config Buad Rate For UART
    Config_BaudRate(_baudRate, _frequency, _mode);
    
    // Config Frame Width and Parity and Mode (Sync/Async)
    Config_Frame_Format(USART_DATA_BITS_8,USART_PARITY_MODE_NO,USART_STOP_BITS_ONE);

    switch (channel)
    {
        case USART_CHANNEL_0:
            Is_UART0_Initialized=true;
        break;

        case USART_CHANNEL_1:
            Is_UART1_Initialized=true;
        break;

        case USART_CHANNEL_2:
            Is_UART2_Initialized=true;
        break;
    
    }
}

//[Completed]
// Receive One Byte
uint8 USART::ReceiveByte()
{
    // wait for data to be recieved
    while (!IsReceiveComplete());

    // Read Byte From Data Register
    return USART_REG_UDR;
}

// Receive Multi Bytes
uint8 *USART::ReceiveString()
{
    // while (1)
    // {
    //     uint8* results;
    //     uint8 character = ReceiveByte();

    //     if(character=='0')
    //     {
    //         return results;
    //     }

    //     results=results + character;

    // }
    
}

//[Completed]
// Transmit one Byte
void USART::TransmitByte(uint8 byte)
{
    // Check if UART Data Register is Ready To Write Data inside it

    // Loop until UART Data Register be Empty To Can Store the byte inside Buffer that we need to send
    while (!IsTransmitBufferEmpty());
   
    // Save Byte inside Data Register
    USART_REG_UDR = byte;
}

//[Completed]
// Transmit Multi Bytes
void USART::TransmitString(uint8 *text)
{
    uint8 index = 0;

    // Loop From text Pointer to get the end of string [null terminator 0]
    while (text[index] != 0)
    {
        // Send every Character one by one
        TransmitByte(text[index]);

        index++;
    }
}

//[Completed]
// Transmit Multi Bytes
void USART::TransmitString(char* text)
{
    uint8 index = 0;

    // Loop From text Pointer to get the end of string [null terminator 0]
    while (text[index] != 0)
    {
        // Send every Character one by one
        TransmitByte(text[index]);

        index++;
    }
}


//Get the BaudRate like 9600,115200 etect
uint32 USART::GetBaudRate()
{
    return _baudRate;

}

//Get the BaudRate Value Stored inside Register (UBRRL,UBRRH) like value 103 For rate 9600
uint16 USART::GetBaudRateValue()
{
    return _buadRateValue;

}


#pragma endregion

#pragma region Helpers

//[Completed]
// Calculate BaudRate Depend on user BaudRate , MCU Frequency and Save it inside Registers[UBRRH+UBRRL]
void USART::Config_BaudRate(uint32 baudRate, uint64 frequency, USART_COMMUNICATION_MODE mode)
{
    // To Calculate the Value of Baud Rate we will use Equation
    
    if (mode == USART_COMMUNICATION_MODE_SYNC)
    {
        _buadRateValue = (frequency / 4 / baudRate - 1);
    }
    else if (mode == USART_COMMUNICATION_MODE_ASYNC_NORMAL)
    {
        _buadRateValue = (frequency / 16 / baudRate - 1);
    }
    else if (mode == USART_COMMUNICATION_MODE_ASYNC_DOUBLE_SPEED)
    {
        _buadRateValue = (frequency / 8 / baudRate - 1);
    }


    // Save baudRateValue inside Register UBRRH+UBRRL
    
    USART_REG_UBRRL = BITWISE_GET_LOW_BYTE(_buadRateValue);
    USART_REG_UBRRH = BITWISE_GET_HIGH_BYTE(_buadRateValue);
   
    
}

// Config the Format of Frame include Data Bits , Parity Check , Stop Bits
void USART::Config_Frame_Format(USART_DATA_BITS dataBits, USART_PARITY_MODE parity, USART_STOP_BITS stopBits)
{
    _dataBits = dataBits;
    _parity = parity;
    _stopBits = stopBits;

    // For Testing Only (Set Frame to 8 data bits , 1 stop bit)
    USART_REG_UCSRC = (3<<UCSZ0);
    
}

//[Completed]
// Enable Receive Data For Module by Set bit [RXEN]
void USART::EnableReceive(void)
{
    // Set Bit [RXEN] inside Register [UCSRB]
    BITWISE_SET_BIT(USART_REG_UCSRB, RXEN);
}

//[Completed]
// Disable Receive Data For Module by Clear bit [RXEN]
void USART::DisableReceive(void)
{
    // Clear Bit [RXEN] inside Register [UCSRB]
    BITWISE_CLEAR_BIT(USART_REG_UCSRB, RXEN);
}

//[Completed]
// Enable Transmit Data For Module by Set bit [TXEN]
void USART::EnableTransmit(void)
{
    // Set Bit [TXEN] inside Register [UCSRB]
    BITWISE_SET_BIT(USART_REG_UCSRB, TXEN);
}

//[Completed]
// Disable Transmit Data For Module by Clear bit [TXEN]
void USART::DisableTransmit(void)
{
    // Clear Bit [TXEN] inside Register [UCSRB]
    BITWISE_CLEAR_BIT(USART_REG_UCSRB, TXEN);
}

//[Completed]
// Check if UART Data Register is Ready To Write to it (Mean Buffer Empty)
bool USART::IsTransmitBufferEmpty()
{
    // We Check Bit [UDREn] inside Register [UCSRA]

    // if bit is 0 mean data register is empty so ready for write to it
    // if bit is 1 mean data register is not empty so cant write to it

    //while(!(UCSRA&(1<<UDRE)));

    return BITWISE_GET_BIT(USART_REG_UCSRA, UDRE);
}

//[Completed]
// Check if UART Receiving Complete [RXC] inside Register [UCSRA]
bool USART::IsReceiveComplete()
{
    // We Check Bit [RXC] inside Register [UCSRA]

    // if bit is 1 mean data receive completed so ready for reading it
    // if bit is 0 mean data receive not completed so not ready for reading it

    return BITWISE_GET_BIT(USART_REG_UCSRA, RXC);
}

//[Completed]
// Check if UART Transmit Complete [TXC] inside Register [UCSRA]
bool USART::IsTransmitComplete()
{
    // We Check Bit [TXC] inside Register [UCSRA]

    // if bit is 1 mean data transmit completed
    // if bit is 0 mean data transmit not completed

    if (BITWISE_GET_BIT(USART_REG_UCSRA, TXC))
    {
        // transmit Completed
        return true;
    }
    else
    {
        // transmit Not Completed
        return false;
    }
}

#pragma endregion