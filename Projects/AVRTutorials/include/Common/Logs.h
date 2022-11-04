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

    //Write One Character over Log Source
    void WriteChar(uint8 character);

    //Write String Text Over Log Source
    void WriteText(uint8* text);


};

#endif /* LOGS_H_ */