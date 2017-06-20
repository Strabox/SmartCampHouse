// TypesDefinitions.h

#ifndef _TYPES_DEFINITIONS_h
#define _TYPES_DEFINITIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//Type to represent an arduino I/O PIN
typedef int Pin;

#endif
