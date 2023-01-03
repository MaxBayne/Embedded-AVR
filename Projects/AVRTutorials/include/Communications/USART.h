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

//Create Instance Object
USART uart = USART();

//Initiliaze the UART Module
uart.Initialize(USART_CHANNEL_0,COMMUNICATION_MODE_ASYNC_NORMAL,F_CPU,9600,true,true);

//Send Data OVer UART
uart.TransmitString("Hello World");

 */ 

#ifndef USART_H_
#define USART_H_

#include <Common/Common.h>
#include <DataStructures/DynamicQueue.h>

using namespace DataStructures;

#pragma region ENUMS




//USART Channel0,Channel1,Channel2
typedef enum
{
	USART_CHANNEL_0,
	USART_CHANNEL_1,
	USART_CHANNEL_2
}USART_CHANNEL;

//USART Communication Mode (Async-Sync)
typedef enum
{
	USART_COMMUNICATION_MODE_SYNC,                    //Work With Clock Generated From Master To Slave Over [XCK] Pin
	USART_COMMUNICATION_MODE_ASYNC_NORMAL,            //Work With BaudRate Sit On Master/Slave like 9600 , No Need For Clock Just Same BaudRate Between Devices
    USART_COMMUNICATION_MODE_ASYNC_DOUBLE_SPEED,
}USART_COMMUNICATION_MODE;

//USART Size of Bits For Data inside Frame (5,6,7,8,9) Bits
typedef enum
{
	USART_DATA_BITS_5,
	USART_DATA_BITS_6,
	USART_DATA_BITS_7,
    USART_DATA_BITS_8,
    USART_DATA_BITS_9
}USART_DATA_BITS;

//USART Parity Check Mode Used inside Frame (No,ODD,EVEN)
typedef enum
{
	USART_PARITY_MODE_NO,
	USART_PARITY_MODE_ODD,
	USART_PARITY_MODE_EVEN
}USART_PARITY_MODE;

//USART Stop Bits used Inside Frame (ONE,TWO) BITS
typedef enum
{
	USART_STOP_BITS_ONE,
	USART_STOP_BITS_TWO
}USART_STOP_BITS;

#pragma endregion ENUMS


class USART
{
    
    private:
    
    
    USART_CHANNEL _channel;
    USART_COMMUNICATION_MODE _mode;
    USART_DATA_BITS _dataBits;
    USART_PARITY_MODE _parity;
    USART_STOP_BITS _stopBits;
    uint32 _baudRate;
    uint64 _frequency;
    uint16 _buadRateValue;
    bool _canReceive;
    bool _canTransmit;
    bool _enableReceiveCompletedInterrupt;
    bool _enableTransmitCompletedInterrupt;
    bool _enableDataEmptyInterrupt;
    
    public:

    static bool Is_UART0_Initialized;
    static bool Is_UART1_Initialized;
    static bool Is_UART2_Initialized;
    static DynamicQueue<char*> UART0_QUEUE;
    //static DynamicQueue<char*> UART1_QUEUE;
    //static DynamicQueue<char*> UART2_QUEUE;

	public:
	
    //Constructors
    USART();

    //Initialize Module
    void Initialize(USART_CHANNEL channel,USART_COMMUNICATION_MODE mode,uint64 frequency,uint32 baudRate,bool canReceive,bool canTransmit,bool enableReceiveCompletedInterrupt,bool enableTransmitCompletedInterrupt,bool enableDataEmptyInterrupt);

    private:

    //Calculate BaudRate Depend on user BaudRate , MCU Frequency and Save it inside Registers[UBRRH+UBRRL]
    void Config_BaudRate(uint32 baudRate,uint64 frequency,USART_COMMUNICATION_MODE mode);

    //Config the Format of Frame include Data Bits , Parity Check , Stop Bits
    void Config_Frame_Format(USART_DATA_BITS dataBits,USART_PARITY_MODE parity,USART_STOP_BITS stopBits);

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
    void TransmitByte(uint8* byte);

    //Transmit Multi Bytes
    void TransmitString(uint8* text);

    // Enqueue/Store Text inside Queue
    void WriteTextInQueue(char* text);

    // Enqueue/Store Character inside Queue
    void WriteCharacterInQueue(char* character);

    //Get the BaudRate like 9600,115200 etect
    uint32 GetBaudRate();

    //Get the BaudRate Value Stored inside Register (UBRRL,UBRRH) like value 103 For rate 9600
    uint16 GetBaudRateValue();

    
    void Enable_Interrupt_Transmit_Completed();

    void Enable_Interrupt_Receive_Completed();

    void Enable_Interrupt_Data_Register_Empty();

    void Disable_Interrupt_Transmit_Completed();

    void Disable_Interrupt_Receive_Completed();

    void Disable_Interrupt_Data_Register_Empty();

};
 


#endif /* USART_H_ */