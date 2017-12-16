#include "WaterPump.h"

namespace Domain {

	WaterPump::WaterPump(uint8_t waterPumpPin, uint8_t waterFlowSensorPin) : Device() {
		_waterPump = new Relay(waterPumpPin);
		_waterFlowSensor = new WaterFlowSensor(waterFlowSensorPin);
		_isBroken = false;
		_turnOnTimestamp = 0;
	}

	WaterPump::~WaterPump() {
		if (_waterPump != NULL) {
			delete _waterPump;
			delete _waterFlowSensor;
		}
	};

	bool WaterPump::begin() {
		if (!_isInitialized) {
			_waterPump->begin();
			_waterFlowSensor->begin();
			_isInitialized = true;
		}
	}

	void WaterPump::end() {
		if (_isInitialized) {
			_waterPump->end();
			_waterFlowSensor->end();
			_isInitialized = false;
		}
	}

	void WaterPump::turnOn() {
		if (_isInitialized && !_isBroken) {
			_waterPump->turnOn();
			_turnOnTimestamp = millis();
		}
	}

	void WaterPump::turnOff() {
		if (_isInitialized) {
			_waterPump->turnOff();
		}
	}

	void WaterPump::fix() {
		if (_isInitialized) {
			_isBroken = false;
		}
	}

	bool WaterPump::isOn() {
		if (_isInitialized) {
			return _waterPump->isOn();
		}
		return false;
	}

	bool WaterPump::isBroken() {
		if (_isInitialized) {
			if (_waterPump->isOn() && 
				((millis() - _turnOnTimestamp) > DELAY_TO_START_PUMPING) && 
				!_waterFlowSensor->isFlowing()) {
				turnOff();
				_isBroken = true;
			}
			return _isBroken;
		}
		return false;
	}

};