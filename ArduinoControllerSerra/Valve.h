// Valve.h

#ifndef _VALVE_h
#define _VALVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include "TypesDefinitions.h"
#else
	#include "WProgram.h"
#endif

namespace SerraController {
	
	namespace Domain {

		/*
			Represents a physical valve
		*/
		class Valve
		{
			private: Pin _outputPin;


			public: Valve(Pin);

			public: virtual void init();

			public: virtual void open();

			public: virtual void close();
		};
	}
}
#endif

