#include "Tank.h"

namespace Domain {

	Tank::Tank(uint8_t valvePin, uint8_t waterPumpPin, uint8_t ultrasonicTriggerPin, uint8_t ultrasonicEchoPin,
		float ultraSonicHeightCentimeters, float tankHeightCentimeters, uint8_t maxCapacity, uint8_t waterFlowSensorPin) : Device() {
		_valve = new Relay(valvePin);
		_waterPump = new WaterPump(waterPumpPin, waterFlowSensorPin);
		_distanceMeter = new DistanceMeter(ultrasonicTriggerPin, ultrasonicEchoPin);
		_distanceMeterHeightCentimeters = ultraSonicHeightCentimeters;
		_tankHeightCentimeters = tankHeightCentimeters;
		_maxCapacity = maxCapacity;
	}

	Tank::~Tank() {
		if (_valve != NULL) {
			delete _valve;
		}
		if (_waterPump != NULL) {
			delete _waterPump;
		}
		if (_distanceMeter != NULL) {
			delete _distanceMeter;
		}
	}

	bool Tank::begin() {
		if (!_isInitialized) {
			_valve->begin();
			_waterPump->begin();
			_distanceMeter->begin();
			_isInitialized = true;
		}
		return _isInitialized;
	}

	void Tank::end() {
		if (_isInitialized) {
			_valve->end();
			_waterPump->end();
			_distanceMeter->end();
			_isInitialized = false;
		}
	}

	void Tank::open() {
		if (_isInitialized && !_waterPump->isOn()) {
			_valve->turnOn();
		}
	}

	void Tank::close() {
		if (_isInitialized) {
			_valve->turnOff();
		}
	}

	void Tank::startFill() {
		if (_isInitialized && !_waterPump->isBroken()) {
			_waterPump->turnOn();
		}
	}

	void Tank::stopFill() {
		if (_isInitialized) {
			_waterPump->turnOff();
		}
	}

	void Tank::fixWaterPump() {
		if (_isInitialized) {
			_waterPump->fix();
		}
	}

	void Tank::checkStatus() {
		if (_isInitialized) {
			_waterPump->isBroken();	// Check if water pump is broken

			// Verify if the tank is at max capacity threshold and turn off the water pump
			if (getTankCapacity() >= _maxCapacity) {
				_waterPump->turnOff();
			}
		}
	}

	void Tank::updateCapacity() {
		if (_isInitialized) {
			_distanceMeter->updateDistance();	// TODO: Verify how non-block performs in reality
		}
	}

	unsigned int Tank::getTankCapacity() {
		float waterHeight = _tankHeightCentimeters - (_distanceMeter->getDistanceCentimeters() - _distanceMeterHeightCentimeters);
		int tempCapacity = (int)((waterHeight / _tankHeightCentimeters) * 100);
		if (tempCapacity <= 0) {
			tempCapacity = 0;
		}
		else if (tempCapacity >= 100) {
			tempCapacity = 100;
		}
		return tempCapacity;
	}

	uint8_t Tank::getWaterPumpState() {
		if (_waterPump->isBroken()) {
			return 3;
		}
		else {
			if (_waterPump->isOn()) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}

	uint8_t Tank::getValveState() {
		if (_valve->isOn()) {
			return 1;
		}
		else {
			return 0;
		}
	}

};