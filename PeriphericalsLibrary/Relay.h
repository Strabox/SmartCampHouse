/*
Relay.h
*/
#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Peripherical.h"
#else
#include "WProgram.h"
#endif

#define OPEN_STRING "Aberto"
#define CLOSED_STRING "Fechado"

namespace Periphericals {

	/*
	Represents a simple relay and provides a simple API to interact with the physical relay
	*/
	class Relay : public Peripherical {

		private:
		
		/* Arduino pin connected to relay */
		uint8_t _arduinoPin;

		/* Name of the device that relay activates (Used as Identifier in toString method) */
		char _name[MAX_NAME_SIZE + 1];
		/* Used to know the logical position of the relay */
		bool _isOpen;


		public:

		/*
		Creates a Relay object to interact with the physical relay
		@param arduionPin	- Arduino pin that is connected with the relay (normal in relay is IN pins) 
		@param name			- Name (identifier) to the device that the relay activates
		*/
		Relay(uint8_t arduinoPin, const char* name);

		/*
		Used to free all the resources associated with the object Relay
		*/
		~Relay();


		#pragma region Public Peripherical API Methods

		void begin();

		void end();

		char* toString() const;

		#pragma endregion


		#pragma region Public Relay API Methods 

		/*
		Used to initialize the relay API with a custom closed pin level depending on the wanted semantic.
		@param closedStatePinLevel	- Define the start relay pin level of the Closed state
		*/
		void begin(uint8_t closedStatePinLevel);

		/*
		If the relay is in closed state opens it
		*/
		void open();

		/*
		If the relay is in open state closes it
		*/
		void close();

		/*
		@return	- True: If it is in open state. False: Otherwise.
		*/
		bool isOpen() const;

		#pragma endregion

	};

};

#endif
