/*
WaterPump.h
*/
#ifndef _WATER_PUMP_h
#define _WATER_PUMP_h

#include <Easyuino.h>

#define DELAY_TO_START_PUMPING 10000	// 10 Seconds

using Easyuino::Device;
using Easyuino::Relay;
using Easyuino::WaterFlowSensor;

namespace Domain {

	class WaterPump : public Device {

	private:
		Relay* _waterPump;
		WaterFlowSensor * _waterFlowSensor;
		unsigned long _turnOnTimestamp;
		bool _isBroken;

	public:
		WaterPump(uint8_t waterPumpPin, uint8_t waterFlowSensorPin);

		~WaterPump();

		bool begin();

		void end();

		void turnOn();

		void turnOff();

		void fix();

		bool isOn();

		bool isBroken();

	};

};

#endif
