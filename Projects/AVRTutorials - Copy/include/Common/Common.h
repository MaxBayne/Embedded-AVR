#ifndef COMMON_H_
#define COMMON_H_


#include <util/delay.h>

////////////////////////////////////////////////
//Custom Data Types 
////////////////////////////////////////////////

#pragma region Data Types

//unsigned(+) integer 8 bit (unsigned char)
typedef unsigned char uint8;

//unsigned(+) integer 16 bit (unsigned short)
typedef unsigned short uint16;

//unsigned(+) integer 32 bit (unsigned int)
typedef unsigned int uint32;

//unsigned(+) integer 64 bit (unsigned long)
typedef unsigned long uint64;

//signed(+-) integer 8 bit (signed char)
typedef signed char sint8;

//signed(+-) integer 16 bit (signed short)
typedef signed short sint16;

//signed(+-) integer 32 bit (signed int)
typedef signed int sint32;

//signed(+-) integer 64 bit (signed long)
typedef signed long sint64;



#pragma endregion Data Types


////////////////////////////////////////////////
//Times Operations
////////////////////////////////////////////////

#pragma region Times Operations



//Delay Timer For Mills Seconds (ms)
#define delay_ms(millis) _delay_ms(millis)

//Delay Timer For Micro Seconds (us)
#define delay_us(micros) _delay_us(micros)

#pragma endregion Times Operations


#endif /* COMMON_H_ */