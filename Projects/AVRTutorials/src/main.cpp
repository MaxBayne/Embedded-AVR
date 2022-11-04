#include "Core/Core.h"
#include "Communications/USART.h"
#include "Common/Logs.h"

//Delegates
void toggleLEDYellow()
{
	GPIO::Pin_Toggle(GPIO_IO_PA0);
}



int main(void)
{
  //Configuration ----------------------------------------------------
  Logs log = Logs();
  USART uart = USART();
  
  
  GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);

  log.Initialize(LOGS_SOURCE_UART0);
  uart.Initialize(USART_CHANNEL_0,COMMUNICATION_MODE_ASYNC_NORMAL,F_CPU,9600,true,true);
  

  log.WriteText("Ima MAx Bayne n/");


  //Loop ----------------------------------------------------
  while(1)
  {

    GPIO::Pin_Toggle(GPIO_IO_PA0);


    
    uart.TransmitString("hello world");

    
    //uart.TransmitByte(uart.ReceiveByte());

    





    delay_ms(2000);



  }

}


	