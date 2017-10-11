/*
TankManager.h
*/
#ifndef _TANK_MANAGER_h
#define _TANK_MANAGER_h

#include "Relay.h"
#include "DistanceMeter.h"

#define ULTRASONIC_TRIGGER_PIN 1
#define ULTRASONIC_ECHO_PIN 2

#define TANK_VALVE_PIN 10
#define WATER_PUMP_PIN 12

#define ULTRASONIC_SENSOR_HEIGHT_CM 50 
#define TANK_HEIGHT_CM 200 

#define TIMEOUT_FOR_TEN_PERCENT 600000 // 10 minutes

#define VALVE_NAME "Valve"
#define WATER_PUMP_NAME "WaterPump"

namespace Domain {

	using Periphericals::Peripherical;
	using Periphericals::Relay;
	using Periphericals::DistanceMeter;

	class TankManager : public Peripherical {

		private:

		Relay valve = Relay(TANK_VALVE_PIN, VALVE_NAME);
		Relay waterPump = Relay(WATER_PUMP_PIN, WATER_PUMP_NAME);
		DistanceMeter dm = DistanceMeter(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);

		/* Last time we started checking for water pump function (Milliseconds) */
		unsigned long _lastTimeWaterPumpCheck;
		/* Capacity of the tank at lastTimeWaterPumpCheck (% Percentage) */
		unsigned int _lastMeasureWaterPumpCheck;
		/* Used to know if the water pump is ready to be activated or not */
		bool _waterPumpBroken;
		/* TODO - */
		bool _isFillingAutomatically;

		public:

		TankManager();

		~TankManager();

		void begin();

		void end();

		char* toString() const;

		void open();

		void close();

		void startFill();

		void stopFill();

		void startFillAutomatically(unsigned int minCapacity, unsigned int maxCapacity);

		void updateCapacity();

		void fixWaterPump();

		void checkWaterPumpStatus();

		unsigned int getTankCapacity() const;

	};

};

#endif