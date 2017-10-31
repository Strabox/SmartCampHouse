#include "WaterPump.h"

namespace Domain {

	WaterPump::WaterPump(uint8_t waterPumpPin, char* waterPumpName) : Device() {
		_waterPump = new RelayNamed(waterPumpPin, waterPumpName);
		_isBroken = false;
	}

	WaterPump::~WaterPump() {
		if (_waterPump != NULL) {
			delete _waterPump;
		}
	};

	bool WaterPump::begin() {
		if (!_isInitialized) {
			_waterPump->begin();
			_isInitialized = true;
		}
	}

	void WaterPump::end() {
		if (_isInitialized) {
			_isInitialized = false;
		}
	}

	void WaterPump::turnOn() {
		if (_isInitialized && !_waterPump->isOn() && !_isBroken) {
			_waterPump->turnOn();
		}
	}

	void WaterPump::turnOff() {
		if (_isInitialized && _waterPump->isOn()) {
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
			if (_waterPump->isOn() && false) {	//TODO: Replace false by water flow sensor check
				_isBroken = true;
			}
			return _isBroken;
		}
		return false;
	}

	char* WaterPump::toString() {
		return _waterPump->toString();
	}

};