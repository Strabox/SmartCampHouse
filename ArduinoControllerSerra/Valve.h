// Valve.h

#ifndef _VALVE_h
#define _VALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define MAX_NAME_SIZE 20

class Valve
{
	private: char _name[MAX_NAME_SIZE + 1];
	private: uint8_t _arduinoPin;
	private: bool _isOpen;

	public: Valve(uint8_t arduinoPin, const char* name);

	public: void begin();

	public: void open();

	public: void close();

	public: bool isOpen();

	public: char* toString();

	public: friend Stream& operator<<(Stream &stream, const Valve &valve);

};

#endif

