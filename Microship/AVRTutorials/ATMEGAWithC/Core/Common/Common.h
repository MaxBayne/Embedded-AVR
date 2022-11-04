#ifndef COMMON_H_
#define COMMON_H_

#include <util/delay.h>

//Custom Data Types -------------------------------------------------------------

//unsigned(+) integer 8 bit
typedef unsigned char uint8;

//unsigned(+) integer 16 bit
typedef unsigned short uint16;

//unsigned(+) integer 32 bit
typedef unsigned int uint32;

//unsigned(+) integer 64 bit
typedef unsigned long uint64;

//signed(+-) integer 8 bit
typedef signed char sint8;

//signed(+-) integer 16 bit
typedef signed short sint16;

//signed(+-) integer 32 bit
typedef signed int sint32;

//signed(+-) integer 64 bit
typedef signed long sint64;


//Times Operations --------------------------------------------------------------

//Delay Timer For Mills Seconds (ms)
#define delay_ms(millis) _delay_ms(millis)

//Delay Timer For Micro Seconds (us)
#define delay_us(micros) _delay_us(micros)

#endif /* COMMON_H_ */