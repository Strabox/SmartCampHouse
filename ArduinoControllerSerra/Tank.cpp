#include "Tank.h"

namespace Domain {

	Tank::Tank(char* valveName, uint8_t valvePin, char* waterPumpName, uint8_t waterPumpPin,
		uint8_t ultrasonicTriggerPin, uint8_t ultrasonicEchoPin, float ultraSonicHeightCM,
		float tankHeightCM, uint8_t maxCapacity) : Device() {
		_valve = new RelayNamed(valvePin, valveName);
		_waterPump = new WaterPump(waterPumpPin, waterPumpName);
		_distanceMeter = new DistanceMeter(ultrasonicTriggerPin, ultrasonicEchoPin);
		_distanceMeterHeightCM = ultraSonicHeightCM;
		_tankHeightCM = tankHeightCM;
		_maxCapacity = maxCapacity;

		_lastMeasureWaterPumpCheck = 0;
		_lastTimeWaterPumpCheck = 0;
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
			_lastTimeWaterPumpCheck = 0;
			_lastMeasureWaterPumpCheck = 0;
			_isInitialized = false;
		}
	}

	char* Tank::toString() {
		char* valveString = _valve->toString();
		char* waterPumpString = _waterPump->toString();
		char* res = NULL;
		size_t resSize = strlen(valveString) + strlen(waterPumpString) + strlen("Capacity:") + 3 + 3;
		res = (char*)Utilities::EasyMalloc(sizeof(char) * resSize);
		snprintf(res, resSize, "%s\n%s\n%s%u", valveString, waterPumpString, "Capacity:", getTankCapacity());
		free(valveString);
		free(waterPumpString);
		return res;
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
			_valve->turnOff();
		}
	}

	void Tank::stopFill() {
		if (_isInitialized) {
			_waterPump->turnOff();
			_lastTimeWaterPumpCheck = 0;
			_lastMeasureWaterPumpCheck = 0;
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
			if (getTankCapacity() > _maxCapacity) {
				_waterPump->turnOff();
			}


			/* My way of indirecting verify the water pump stopped work
			if(_waterPump->isOn()) {
				if (_lastTimeWaterPumpCheck == 0) {		// First check after the pump is opened
					_lastTimeWaterPumpCheck = millis();
					_lastMeasureWaterPumpCheck = this->getTankCapacity();
					return;
				}

				// Time to check if pump is working
				if ((millis() - _lastTimeWaterPumpCheck) >= TIMEOUT_FOR_TEN_PERCENT) {
					if ((getTankCapacity() - _lastMeasureWaterPumpCheck) <= 10) {
						_waterPumpBroken = true;
						this->stopFill();
					}
					else {
						_lastTimeWaterPumpCheck = millis();
						_lastMeasureWaterPumpCheck = this->getTankCapacity();
					}
				}
			}
			*/
		}
	}

	void Tank::updateCapacity() {
		if (_isInitialized) {
			_distanceMeter->updateDistance();	// TODO: Verify how non-block performs in reality
		}
	}

	unsigned int Tank::getTankCapacity() {
		float waterHeight = _tankHeightCM - (_distanceMeter->getDistanceCentimeters() - _distanceMeterHeightCM);
		int tempCapacity = ((int)waterHeight / (int)_tankHeightCM) * 100;
		if (tempCapacity <= 0) {
			tempCapacity = 0;
		}
		else if (tempCapacity >= 100) {
			tempCapacity = 100;
		}
		return tempCapacity;
	}

};