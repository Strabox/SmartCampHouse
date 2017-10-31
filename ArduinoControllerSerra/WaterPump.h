/*
WaterPump.h
*/
#ifndef _WATER_PUMP_h
#define _WATER_PUMP_h

#include <Easyuino.h>

using Easyuino::Device;
using Easyuino::RelayNamed;

namespace Domain {

	class WaterPump : public Device {

	private:
		bool _isBroken;
		RelayNamed* _waterPump;

	public:
		WaterPump(uint8_t waterPumpPin, char* waterPumpName);

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
