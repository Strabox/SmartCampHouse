#include "DistanceMeter.h"

#define ECHO_TIMEOUT 3000000				// 3 seconds
#define MAX_UNSIGNED_LONG 4294967295
#define SOUND_SPEED_CM_PER_SEC 340000.0f	// Sound speed in air at 15 Celsius

namespace Periphericals {

	DistanceMeter* DistanceMeter::singleton;

	DistanceMeter::DistanceMeter(uint8_t triggerPin, uint8_t echoPin) : Peripherical() {
		_triggerPin = triggerPin;
		_echoPin = echoPin;
		_timeTrigger = -1;
		_distance = -1;
		_isEchoing = false;
		_echoSent = false;
		_blockingMeasure = true;
		if (DistanceMeter::singleton != NULL) {
			delete DistanceMeter::singleton;
		}
		DistanceMeter::singleton = this;
	}

	DistanceMeter::~DistanceMeter() {  
		DistanceMeter::singleton = NULL;
	}

	void DistanceMeter::InterruptCaller() {
		DistanceMeter::singleton->interruptHandler();
	}

	void DistanceMeter::begin() {
		begin(true);
	}

	void DistanceMeter::begin(bool blockingMeasure) {
		if (!_isInitialized) {
			pinMode(_triggerPin, OUTPUT);
			pinMode(_echoPin, INPUT);
			digitalWrite(_echoPin, LOW);
			_blockingMeasure = blockingMeasure;
			_isInitialized = true;
			if (!blockingMeasure) {
				attachInterrupt(digitalPinToInterrupt(_echoPin), DistanceMeter::InterruptCaller, CHANGE);
			}
		}
	}

	void DistanceMeter::end() {
		if (_isInitialized) {
			_isInitialized = false;
			_isEchoing = false;
			_echoSent = false;
			_timeTrigger = -1;
			_distance = -1;
			if (_blockingMeasure) {
				detachInterrupt(digitalPinToInterrupt(_echoPin));
			}
		}
	}

	float DistanceMeter::getDistance() const {
		if (_blockingMeasure) {
			return _distance;
		}
		else {
			if (_lastTimeEcho < _lastTimeTrigger) {		// Micro timer overflow detected
				return -1;								// return -1 meaning invalid reading
			}
			else {
				return ((_lastTimeEcho - _lastTimeTrigger) * SOUND_SPEED_CM_PER_SEC) / 2000000.0f;
			}
		}
	}

	void DistanceMeter::updateDistance() {
		if (_isInitialized) {
			triggerMeasure();
		}
	}

	char* DistanceMeter::toString() const {
		char* res = (char*)malloc(sizeof(char)*25);
		snprintf(res, 25, "Distance:%.3f", _distance);
		return res;
	}

	#pragma region Private Methods

	void DistanceMeter::triggerMeasure() {
		if (!_blockingMeasure) {
			if (!_isEchoing) {
				_isEchoing = true;
				digitalWrite(_triggerPin, LOW);
				delayMicroseconds(2);
				digitalWrite(_triggerPin, HIGH);
				delayMicroseconds(10);
				digitalWrite(_triggerPin, LOW);
				delayMicroseconds(2);
			}
			else {
				if (micros() - _timeTrigger > ECHO_TIMEOUT) {		// Timeout for the echo detected
					_echoSent = _isEchoing = false;
					triggerMeasure();
				}
			}
		}
		else {
			digitalWrite(_triggerPin, LOW);
			delayMicroseconds(2);
			digitalWrite(_triggerPin, HIGH);
			delayMicroseconds(10);
			digitalWrite(_triggerPin, LOW);
			delayMicroseconds(2);
			_distance = (pulseIn(_echoPin, HIGH, ECHO_TIMEOUT) * SOUND_SPEED_CM_PER_SEC / 2000000.0f);
		}
	}

	void DistanceMeter::interruptHandler() {
		unsigned long currentTime = micros();
		if (digitalRead(_echoPin) == HIGH && _isEchoing && !_echoSent) {
			_timeTrigger = currentTime;
			_echoSent = true;
		}
		else if (digitalRead(_echoPin) == LOW && _isEchoing && _echoSent) {
			_echoSent = _isEchoing = false;
			_lastTimeTrigger = _timeTrigger;
			_lastTimeEcho = currentTime;
		}
	}

	#pragma endregion

};
