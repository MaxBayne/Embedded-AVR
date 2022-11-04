/*
#####################
Serial Communications
#####################

USART (Universal Sync Async Receive Transmit)

####
Pins
####

PD0 (RXD) : Receive Data
PD1 (TXD) : Transmit Data

 */ 

#ifndef USART_H_
#define USART_H_

#include <Common/Common.h>


#pragma region ENUMS




//Define The USART
typedef enum
{
	USART_CHANNEL_0,
	USART_CHANNEL_1,
	USART_CHANNEL_2
}USART_CHANNEL;

//Define The USART Communication Mode (Async-Sync)
typedef enum
{
	COMMUNICATION_MODE_SYNC,                    //Work With Clock Generated From Master To Slave Over []XCK] Pin
	COMMUNICATION_MODE_ASYNC_NORMAL,            //Work With BaudRate Sit On Master/Slave like 9600 , No Need For Clock Just Same BaudRate Between Devices
    COMMUNICATION_MODE_ASYNC_DOUBLE_SPEED,
}COMMUNICATION_MODE;

//Define The Size of Bits For Data inside Frame
typedef enum
{
	DATA_BITS_5,
	DATA_BITS_6,
	DATA_BITS_7,
    DATA_BITS_8,
    DATA_BITS_9
}DATA_BITS;

//Define The Parity Check Mode Used inside Frame
typedef enum
{
	PARITY_MODE_NO,
	PARITY_MODE_ODD,
	PARITY_MODE_EVEN
}PARITY_MODE;

//Define The Stop Bits used Inside Frame
typedef enum
{
	STOP_BITS_ONE,
	STOP_BITS_TWO
}STOP_BITS;

#pragma endregion ENUMS


class USART
{
    
    private:
    
    USART_CHANNEL _channel;
    COMMUNICATION_MODE _mode;
    DATA_BITS _dataBits;
    PARITY_MODE _parity;
    STOP_BITS _stopBits;
    uint32 _baudRate;
    uint64 _frequency;
    bool _canReceive;
    bool _canTransmit;
    
    public:

    static bool Is_UART0_Initialized;
    static bool Is_UART1_Initialized;
    static bool Is_UART2_Initialized;

	public:
	
    //Constructors
    USART();

    //Initialize Module
    void Initialize(USART_CHANNEL channel,COMMUNICATION_MODE mode,uint64 frequency,uint32 baudRate,bool canReceive,bool canTransmit);

    private:

    //Calculate BaudRate Depend on user BaudRate , MCU Frequency and Save it inside Registers[UBRRH+UBRRL]
    void Config_BaudRate(uint32 baudRate,uint64 frequency,COMMUNICATION_MODE mode);

    //Config the Format of Frame include Data Bits , Parity Check , Stop Bits
    void Config_Frame_Format(DATA_BITS dataBits,PARITY_MODE parity,STOP_BITS stopBits);

    //Enable Receive Data For Module by Set bit [RXEN]
    void EnableReceive(void);
    //Disable Receive Data For Module by Clear bit [RXEN]
    void DisableReceive(void);

    //Enable Transmit Data For Module by Set bit [TXEN]
    void EnableTransmit(void);
    //Disable Transmit Data For Module by Clear bit [TXEN]
    void DisableTransmit(void);
    
    //Check if UART Data Register is Ready To Write to it (Mean Buffer Empty)
    bool IsTransmitBufferEmpty();

    //Check if UART Receiving Complete [RXC] inside Register [UCSRA]
    bool IsReceiveComplete();

    //Check if UART Transmit Complete [TXC] inside Register [UCSRA]
    bool IsTransmitComplete();


    public:

    //Receive One Byte
    uint8 ReceiveByte();

    //Receive Multi Bytes
    uint8* ReceiveString();

    //Transmit one Byte
    void TransmitByte(uint8 byte);

    //Transmit Multi Bytes
    void TransmitString(uint8* text);

};



#endif /* USART_H_ */