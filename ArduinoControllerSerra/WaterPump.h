/*
WaterPump.h
*/
#ifndef _WATER_PUMP_h
#define _WATER_PUMP_h

#include <Easyuino.h>

#define DELAY_TO_START_PUMPING 10000	// 10 Seconds

using Easyuino::Device;
using Easyuino::RelayNamed;

namespace Domain {

	class WaterPump : public Device {

	private:
		unsigned long _turnOnTimestamp;
		bool _isBroken;
		RelayNamed* _waterPump;

	public:
		WaterPump(uint8_t waterPumpPin, const char* waterPumpName);

		~WaterPump();

		bool begin();

		void end();

		void turnOn();

		void turnOff();

		void fix();

		bool isOn();

		bool isBroken();

		char* toString();

	};

};

#endif
