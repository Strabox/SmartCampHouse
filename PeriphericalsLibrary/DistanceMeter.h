/*
DistanceMeter.h
*/
#ifndef _DISTANCE_METER_h
#define _DISTANCE_METER_h

#include "Peripherical.h"

namespace Periphericals {

	/*
	DistanceMeter offers an API to interact with a typical Ultrasonic sensor to measure distance to objects
	*/
	class DistanceMeter : public Peripherical {

		private:

		uint8_t _triggerPin;
		uint8_t _echoPin;
		bool _blockingMeasure;

		volatile unsigned long _timeTrigger;
		volatile bool _isEchoing;
		volatile bool _echoSent;
		volatile float _distance;

		volatile unsigned long _lastTimeTrigger;
		volatile unsigned long _lastTimeEcho;

		static DistanceMeter* singleton;

		public:

		DistanceMeter(uint8_t triggerPin, uint8_t echoPin);

		~DistanceMeter();

		void begin();

		void begin(bool blockingMeasure);

		void end();

		void updateDistance();

		float getDistance() const;

		char* toString() const;


		private:

		void triggerMeasure();

		void interruptHandler();

		static void InterruptCaller();


	};

};


#endif