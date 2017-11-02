/*
Tank.h
*/
#ifndef _TANK_h
#define _TANK_h

#include "WaterPump.h"
#include <Easyuino.h>

using Easyuino::Utilities;
using Easyuino::Device;
using Easyuino::RelayNamed;
using Easyuino::DistanceMeter;

//#define TIMEOUT_FOR_TEN_PERCENT 600000 // 10 minutes

namespace Domain {

	class Tank : public Device {

	private:
		RelayNamed* _valve;
		WaterPump* _waterPump;
		DistanceMeter* _distanceMeter;

		/* Distance from the Ultrasonic Sensor to the top of the tank */
		float _distanceMeterHeightCM;
		/* The height of the tank */
		float _tankHeightCM;

		/* Capacity threshold to automatically stop filling */
		uint8_t _maxCapacity;

		/* Last time we started checking for water pump function (Milliseconds) */
		unsigned long _lastTimeWaterPumpCheck;
		/* Capacity of the tank at lastTimeWaterPumpCheck (% Percentage) */
		unsigned int _lastMeasureWaterPumpCheck;

	public:
		Tank(const char* valveName, uint8_t valvePin, const char* waterPumpName, uint8_t waterPumpPin,
			uint8_t ultrasonicTriggerPin, uint8_t ultrasonicEchoPin, float ultraSonicHeightCM,
			float tankHeightCM, uint8_t maxCapacity);

		~Tank();

		bool begin();

		void end();

		char* toString();

		void open();

		void close();

		void startFill();

		void stopFill();

		void fixWaterPump();

		void checkStatus();

		void updateCapacity();

		unsigned int getTankCapacity();

	};

};

#endif