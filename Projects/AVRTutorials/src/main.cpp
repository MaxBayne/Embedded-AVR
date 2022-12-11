#include "Core/Core.h"
#include "Common/Logs.h"
#include "Timers/Common"
#include "Timers/Timer.h"
#include "avr/interrupt.h"
#include "Interrupts/TimerInterrupt.h"

Logs _log;


//Delegates
void toggle_Pin_A0()
{
	  //GPIO::Pin_Toggle(GPIO_IO_PA0);
	  GPIO::Pin_Toggle(GPIO_IO_PA0);
    //We Can Stop Timer Here by set Clock Source = nothing


    _log.WriteLine("1 Second Fired , and Toggle Pin A0 \n");
}

void toggle_Pin_A1()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA1);

    //We Can Stop Timer Here by set Clock Source = nothing


    //_log.WriteLine("1 Second Fired , and Toggle Pin A1");
}

void toggle_Pin_A2()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA2);

    //We Can Stop Timer Here by set Clock Source = nothing


   // _log.WriteLine("1 Second Fired , and Toggle Pin A2");
}

int main(void)
{
  //Configuration ----------------------------------------------------
  _log = Logs();
  _log.Initialize(LOGS_SOURCE_UART0);

  Timer timer0 = Timer(TIMER_0,F_CPU);
  //Timer timer1 = Timer(TIMER_1,F_CPU);
  //Timer timer2 = Timer(TIMER_2,F_CPU);

  GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);
  
timer0.Start(1,UNIT_SECOND,toggle_Pin_A0);
//timer1.Start(1,UNIT_SECOND,toggle_Pin_A1);
//timer2.Start(1,UNIT_SECOND,toggle_Pin_A2);


  //Set Timer Initial Value = 0
  //TCNT0=0;
  //TCNT2=0;

  //Set Timer Mode To Normal and Prescaler 1024
  //TCCR0=0b00000101;
  //TCCR2=0b00000101;

  //while ((TIFR&(1<<TOV0))==0);

  //Enable Global Interrupt using register (SREG)
  //BITWISE_SET_BIT(SREG,SREG_I);
  //sei();
  
  //Enable Timer0 Interrupt using register (TIMSK)
  //BITWISE_SET_BIT(TIMSK,TOIE0);
  //BITWISE_SET_BIT(TIMSK,TOIE2);

  




  //Loop ----------------------------------------------------
  while(1)
  {
     GPIO::Pin_Clear(GPIO_IO_PA1);

    delay_ms(1000);

    GPIO::Pin_Set(GPIO_IO_PA1);

	//Timer* timer2 = TimerInterrupt::Timer2;

    delay_ms(1000);

	
	//_log.WriteLine("Timer2 GetOverflowInteger = \n");
	//_log.WriteFloat(timer2->GetOverflowInteger());


  }

}


/*
int timer2Counter;
ISR(TIMER2_OVF_vect)
{
	timer2Counter++;

	if(timer2Counter==61)
	{
		GPIO::Pin_Toggle(GPIO_IO_PA2);

		_log.WriteLine("Timer2 Tick Second \n");

		timer2Counter=0;
	}
	
}
*/