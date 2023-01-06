#ifndef GLOBALINTERRUPT_H_
#define GLOBALINTERRUPT_H_

#include "Common/Logs.h"

class GlobalInterrupt
{
	private:
	Logs* _log;

	public:

	//Constructors
	GlobalInterrupt();
	GlobalInterrupt(Logs* log);
	
	//Enable Global Interrupt
	void Enable_Global_Interrupt();
	
	//Disable Global Interrupt
	void Disable_Global_Interrupt();
};


#endif /* GLOBALINTERRUPT_H_ */