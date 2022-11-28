/*

################### Timer Interrupt ####################

AtMega32A Timer
---------------
- Timer0 (8 Bit Register , using as Timer , Counter)
- Timer1 (16 Bit Register , using as Timer , Counter)
- Timer2 (8 Bit Register , using as Timers Only)


Timer Interrupts Modes
---------------------

- On Normal Mode When Timer Value Register reach to max value of register then occured Overflow and interrupt fired
	Timer0 : TOV0 (Bit0 on Register TIFR)
	Timer1 : TOV1 (Bit2 on Register TIFR)
	Timer2 : TOV2 (Bit6 on Register TIFR)

- On CTC Mode (Compare Match) 
	Timer0 : OCF0 (Bit1 on Register TIFR)
	Timer1 : OCF1A (Bit4 on Register TIFR)
	Timer1 : OCF1B (Bit3 on Register TIFR)
	Timer2 : OCF2 (Bit7 on Register TIFR)

- On Input Capture Flag For Timer1 Only
	Timer1 : ICF1 (Bit5 on Register TIFR)

-------------------------
Enable Timer Interrupt
-------------------------
- To Enable Timer Interrupt we use Register [TIMSK]

- To Enable Timer Overflow Interrupt we use :
	TOIE0 : (Bit0 Timer0 Overflow Interrupt Enable)
	TOIE1 : (Bit2 Timer1 Overflow Interrupt Enable)
	TOIE2 : (Bit6 Timer2 Overflow Interrupt Enable)
	
- To Enable Timer Output Compare Interrupt we use :
	OCIE0 : (Bit1 Timer0 Output Compare Interrupt Enable)
	OCIE1B : (Bit3 Timer1 Channel B Output Compare Interrupt Enable)
	OCIE1A : (Bit4 Timer1 Channel A Output Compare Interrupt Enable)
	OCIE2 : (Bit7 Timer2 Output Compare Interrupt Enable)
	
- To Enable Timer1 Input Capture Flag Interrupt we use :
	TICIE1 : (Bit5 Timer1 Input Capture Interrupt Enable)

-------------------------
Enable Global Interrupt 
-------------------------

- by set Bit7 inside Register (SREG)(Status Register) = HIGH For Enable otherwise Set Low For Disable Global Interrupt

- when Timer Interrupt Triggered then Interrupt Flag Become 1 and Processor Jump TO Vector Table and Get the Interrupt Service Routine For That
Interrupt and Execute it and when Finished then Clear Interrupt Flag inside Register (GIFR) to Avoid ReEnter the Service Routine Again

Example: --------------------------------------

void toggleLEDYellow()
{
	GPIO::Pin_Toggle(GPIO_IO_PA0);
}

int main(void)
{
	
	GPIO::Port_Direction(GPIO_PORT_A,GPIO_DIRECTION_OUTPUT);
	GPIO::Port_Direction(GPIO_PORT_B,GPIO_DIRECTION_OUTPUT);
	
	//Config Timer 0 For Normal Mode Interrupt (overflow)
	TimerInterrupt timer0_normal(TIMER_0,MODE_NORMAL);
	
	//enable Interrupt and when Timer 0 in Normal Mode (Overflow) occured then call function (toggleLEDYellow)
	timer0_normal::Enable_Interrupt(&toggleLEDYellow);
	
	//Or -------------
	
	//Config Timer0 On Normal Mode (Overflow) and Enable Interrupt
	TimerInterrupt timer0_normal_with_enable(TIMER_0,MODE_NORMAL,&toggleLEDYellow);

	
	uint8 counter=0;
	while (1)
	{
		counter++;
		GPIO::Port_Write(GPIO_PORT_B,counter);
		delay_ms(1000);
	}
}



*/
#ifndef TIMERINTERRUPT_H_
#define TIMERINTERRUPT_H_

class TimerInterrupt
{
	public:

	//Hold Interrupt Info For Timer0
	static InterruptInfo Timer0;

	//Hold Interrupt Info For Timer1
	static InterruptInfo Timer1;

	//Hold Interrupt Info For Timer2
	static InterruptInfo Timer2;


	//Enable Timer Interrupt For Timer ,Mode  and Set Function Callback when Interrupt Fired
	static void Enable_Interrupt(TIMER_SELECTOR timer,MODE_SELECTOR mode,InterruptInfo interruptInfo,void(* functionPtr)());
	
	//Disable Timer Interrupt For Timer,Mode and Clear Interrupt Flag
	static void Disable_Interrupt(TIMER_SELECTOR timer,MODE_SELECTOR mode);
	
	//Clear Interrupt Flag For Timer , Mode when Interrupt fired we clear it inside ISR to avoid ReEnter ISR Again
	static void Clear_Interrupt_Flag(TIMER_SELECTOR timer,MODE_SELECTOR mode);

	//Read Interrupt Flag For Timer , Mode , return 1 when Flag On or 0 when flag off
	static uint8 Read_Interrupt_Flag(TIMER_SELECTOR timer,MODE_SELECTOR mode);

	//Stop Timer From External Location Like ISR Function
	static void TimerInterrupt::Stop_Timer(TIMER_SELECTOR timer);
};


#endif /* TIMERINTERRUPT_H_ */