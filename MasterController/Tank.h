/*
Tank.h
*/
#ifndef _TANK_h
#define _TANK_h

#include "WaterPump.h"
#include <Easyuino.h>

using Easyuino::Utilities;
using Easyuino::Device;
using Easyuino::Relay;
using Easyuino::DistanceMeter;

//#define TIMEOUT_FOR_TEN_PERCENT 600000 // 10 minutes

namespace Domain {

	class Tank : public Device {

	private:
		Relay* _valve;
		WaterPump* _waterPump;
		DistanceMeter* _distanceMeter;

		/* Distance from the Ultrasonic Sensor to the top of the tank */
		float _distanceMeterHeightCentimeters;
		/* The height of the tank */
		float _tankHeightCentimeters;

		/* Capacity threshold to automatically stop filling */
		uint8_t _maxCapacity;

	public:
		Tank(uint8_t valvePin, uint8_t waterPumpPin, uint8_t ultrasonicTriggerPin, uint8_t ultrasonicEchoPin,
			float ultraSonicHeightCentimeters, float tankHeightCentimeters, uint8_t maxCapacity, uint8_t waterFlowSensorPin);

		~Tank();

		bool begin();

		void end();

		void open();

		void close();

		void startFill();

		void stopFill();

		void fixWaterPump();

		void checkStatus();

		void updateCapacity();

		unsigned int getTankCapacity();

		uint8_t getWaterPumpState();

		uint8_t getValveState();

	};

};

#endif