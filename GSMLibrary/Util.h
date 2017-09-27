#ifndef _UTIL_h
#define _UTIL_h

#include <stdlib.h>
#include <string.h>

/*
	Contains some util funtions used across the library 
*/
class Util {

	public: static void zeroBuffer(void *buffer, size_t bufferSize);

	public: static void overrideLastStringChar(char* string);

	public: static void overrideLastTwoChar(char* string);

};


#endif