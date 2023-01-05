/*
 * Atmega32A.cpp
 *
 * Created: 1/4/2023 10:33:30 PM
 * Author : maxbayne
 */ 

#include <avr/io.h>
#include <stdlib.h>

using namespace	std;

struct PrescalerInfo
{
	unsigned long  BaseFrequency;			//Base Frequency For MicroCOntroller
	float FinalFrequency;			//Final Frequency After Prescaler that used With Timer Clocks
	unsigned long TimeByMicroSecond;		//Time By MicroSecond
	float TimeOfClock;				//Time Period For One Pulse Clock Per (MicoSecond)
	unsigned long NumberOfClocks;			//Number Of Clocks Needed To Achieve Specified Time
	float OverflowCounts;			//Count of Overflow to Achieve the Specified Time
	unsigned short OverflowInteger; 		//The Integer Value of Overflow Counts
	unsigned char OverflowFractions;		//The Fractions Value of Overflow Counts
	unsigned short InitialTimerValue;		//The Initial Value For Timer To Be Set To Achieve Timer Delay
	unsigned short CurrentOverFlowCount;	//Current Count of Overflow
	
};

int main(void)
{
    /* Replace with your application code */
	
	PrescalerInfo* ptr = (PrescalerInfo*)malloc(sizeof(PrescalerInfo));
	
	ptr->BaseFrequency=100;
	
    while (1) 
    {
    }
}

