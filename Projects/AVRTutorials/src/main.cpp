#include "Core/Core.h"
#include "Common/Logs.h"
#include "Timers/Common"
#include "Timers/Timer.h"
#include "avr/interrupt.h"
#include "Interrupts/TimerInterrupt.h"

Logs* _log;

//Delegates
void toggle_Pin_A0()
{
	  //GPIO::Pin_Toggle(GPIO_IO_PA0);
	  GPIO::Pin_Toggle(GPIO_IO_PA0);
    //We Can Stop Timer Here by set Clock Source = nothing

    //_log->WriteLine("1 Second Fired , and Toggle Pin A0");
}

void toggle_Pin_A1()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA1);

    //We Can Stop Timer Here by set Clock Source = nothing


    //_log->WriteLine("1 Second Fired , and Toggle Pin A1");
}

void toggle_Pin_A2()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA2);

    //We Can Stop Timer Here by set Clock Source = nothing
   //_log->WriteLine("1 Second Fired , and Toggle Pin A2");
}

int main(void)
{
  //Configuration ----------------------------------------------------
  _log = (Logs*)malloc(sizeof(Logs));
  _log->Initialize(LOGS_SOURCE_UART0,false);

  Timer timer0 = Timer(TIMER_0,F_CPU,_log);
  //Timer timer1 = Timer(TIMER_1,F_CPU,_log);
  //Timer timer2 = Timer(TIMER_2,F_CPU,_log);

  GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);
  
  timer0.Start(1,UNIT_SECOND,toggle_Pin_A0);
  //timer1.Start(1,UNIT_SECOND,toggle_Pin_A1);
  //timer2.Start(1,UNIT_SECOND,toggle_Pin_A2);


  //Loop ----------------------------------------------------
  while(1)
  {

     GPIO::Pin_Clear(GPIO_IO_PA7);

    delay_ms(1000);

    GPIO::Pin_Set(GPIO_IO_PA7);

    
    delay_ms(1000);

  }

}

