#include "Core/Core.h"
#include "Common/Logs.h"
#include "Timers/Common"
#include "Timers/Timer.h"
#include "avr/interrupt.h"


Logs _log;

//Delegates
void toggle_Pin_A0()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA0);

    //We Can Stop Timer Here by set Clock Source = nothing


    //_log.WriteLine("1 Second Fired , and Toggle Pin A0");
}

void toggle_Pin_A2()
{
	  GPIO::Pin_Toggle(GPIO_IO_PA2);

    //We Can Stop Timer Here by set Clock Source = nothing


    //_log.WriteLine("1 Second Fired , and Toggle Pin A1");
}

void delayTimer0()
{
  _log.WriteLine("Initialize Delay Timer0");

  //Calc Timer Initial Value

  //Set Timer Initial Value
  //TCNT0=241;
  TCNT0=0;

  //Set Timer To Normal Mode , No Prescaler
  TCCR0=0b00000001;

  //Enable Global Interrupt
  BITWISE_SET_BIT(STATUS_REG_SREG,7);

  //Enable Timer0 Overflow Interrupt
  BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TOIE0); //[TOIE0]


  // //Wait For Timer Overflow TOV0
  // //When Overflow happen mean time delayed finsihed
  //  while (overflowCount<65)
  //  {
  //    if((TIFR&(1<<TOV0))==1) //when Timer Overflow then Increase Counter
  //    {
  //      overflowCount++;
  //    }
  //  } 

  // TCCR0 = 0 ; //Stop Timer 0
  // TIFR = 0x01; //clear ToV0 flag
  // overflowCount = 0;
}


void delayTimer2()
{
  _log.WriteLine("Initialize Delay Timer2");

  //Calc Timer Initial Value

  //Set Timer Initial Value
  //TCNT0=241;
  TCNT2=0;

  //Set Timer To Normal Mode , No Prescaler
  TCCR2=0b00000001;

  //Enable Global Interrupt
  BITWISE_SET_BIT(STATUS_REG_SREG,7);

  //Enable Timer0 Overflow Interrupt
  BITWISE_SET_BIT(INTERRUPT_REG_TIMSK,TOIE2); //[TOIE2]


  // //Wait For Timer Overflow TOV0
  // //When Overflow happen mean time delayed finsihed
  //  while (overflowCount<65)
  //  {
  //    if((TIFR&(1<<TOV0))==1) //when Timer Overflow then Increase Counter
  //    {
  //      overflowCount++;
  //    }
  //  } 

  // TCCR0 = 0 ; //Stop Timer 0
  // TIFR = 0x01; //clear ToV0 flag
  // overflowCount = 0;
}


int main(void)
{
  //Configuration ----------------------------------------------------
  _log = Logs();
  Timer timer0 = Timer(TIMER_SELECTOR::TIMER_0,F_CPU);
  Timer timer2 = Timer(TIMER_SELECTOR::TIMER_2,F_CPU);

  _log.Initialize(LOGS_SOURCE_UART0);
  GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);



//delayTimer0();
//delayTimer2();

timer0.StartTimer(1,TIME_UNIT::SECOND,DURATION_MODE::REPEAT,toggle_Pin_A0);

//timer2.StartTimer(1,TIME_UNIT::SECOND,DURATION_MODE::REPEAT,toggle_Pin_A2);

  //Loop ----------------------------------------------------
  while(1)
  {
    // GPIO::Pin_Set(GPIO_IO_PA1);

    //Delay 
    


    //GPIO::Pin_Set(GPIO_IO_PA1);

    //Delay
    //delayTimer0();

  }

}
/*

uint32 overflowCount;
ISR(TIMER0_OVF_vect)
{
  //GPIO::Pin_Toggle(GPIO_IO_PA0);

  //Clear Timer0 Interrupt Flag to can Enter Interrupt service again when Overflow Interrupt Happen
  BITWISE_SET_BIT(INTERRUPT_REG_TIFR,TOV0); //[TOV0]

  //Re Initial Timer Value To Start Count From it
  TCNT0=0;

  //Increase Overflow Counter
  overflowCount++;

  //Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  

  if(overflowCount==65000)
  {
    //Timer Reach to overflow count needed to wait for 1 second
    overflowCount=0;
    GPIO::Pin_Toggle(GPIO_IO_PA0);

    //We Can Stop Timer Here by set Clock Source = nothing

    _log.WriteLine("1 Second Fired Timer0");
 
  }
  
}

uint32 overflowCount2;
ISR(TIMER2_OVF_vect)
{
  //GPIO::Pin_Toggle(GPIO_IO_PA0);

  //Clear Timer0 Interrupt Flag to can Enter Interrupt service again when Overflow Interrupt Happen
  BITWISE_SET_BIT(INTERRUPT_REG_TIFR,TOV2); //[TOV0]

  //Re Initial Timer Value To Start Count From it
  TCNT2=0;

  //Increase Overflow Counter
  overflowCount2++;

  //Check Overflow Counter Reach the Counts Need for Timing 1 Second (61 overflow with prescaler 1024)
  

  if(overflowCount2==62500)
  {
    //Timer Reach to overflow count needed to wait for 1 second
    overflowCount2=0;
    GPIO::Pin_Toggle(GPIO_IO_PA2);

    //We Can Stop Timer Here by set Clock Source = nothing

    _log.WriteLine("1 Second Fired Timer2");
 
  }
  
}

*/
  