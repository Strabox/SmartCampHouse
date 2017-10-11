#include "TankManager.h"

namespace Domain {

	TankManager::TankManager() : Peripherical() {
		_lastMeasureWaterPumpCheck = 0;
		_lastTimeWaterPumpCheck = 0;
		_waterPumpBroken = false;
		_isFillingAutomatically = false;
	}

	TankManager::~TankManager() { /* Do Nothing */ }

	void TankManager::begin() {
		if (!_isInitialized) {
			valve.begin();
			waterPump.begin();
			dm.begin();
			_isInitialized = true;
		}
	}

	void TankManager::end() {
		if (_isInitialized) {
			valve.end();
			waterPump.end();
			dm.end();
			_lastTimeWaterPumpCheck = 0;
			_lastMeasureWaterPumpCheck = 0;
			_isFillingAutomatically = false;
			_isInitialized = false;
		}
	}

	char* TankManager::toString() const {
		char* valveString = valve.toString();
		char* waterPumpString = waterPump.toString();
		char* res = NULL;
		size_t resSize = strlen(valveString) + strlen(waterPumpString) + 2;
		res = (char*)malloc(sizeof(char) * resSize);
		snprintf(res, resSize, "%s\n%s\n%s%u", valveString, waterPumpString, "Capacity:", getTankCapacity());
		free(valveString);
		free(waterPumpString);
		return res;
	}

	void TankManager::open() {
		if (_isInitialized && !waterPump.isOpen()) {
			valve.open();
		}
	}

	void TankManager::close() {
		if (_isInitialized) {
			valve.close();
		}
	}

	void TankManager::startFill() {
		if (_isInitialized && !_waterPumpBroken) {
			waterPump.open();
			valve.close();
		}
	}

	void TankManager::stopFill() {
		if (_isInitialized) {
			waterPump.close();
			_lastTimeWaterPumpCheck = 0;
			_lastMeasureWaterPumpCheck = 0;
		}
	}

	void TankManager::startFillAutomatically(unsigned int minCapacity, unsigned int maxCapacity) {
		// TODO - Implement after verification of the current simple implementation of TankManager 
	}

	void TankManager::updateCapacity() {
		if (_isInitialized) {
			dm.updateDistance();
		}
	}

	void TankManager::fixWaterPump() {
		_waterPumpBroken = false;
	}

	void TankManager::checkWaterPumpStatus() {
		if (_isInitialized && waterPump.isOpen()) {
			if (_lastTimeWaterPumpCheck == 0) {		// First check after the pump is opened
				_lastTimeWaterPumpCheck = millis();
				_lastMeasureWaterPumpCheck = this->getTankCapacity();
				return;
			}

			if ((millis() - _lastTimeWaterPumpCheck) >= TIMEOUT_FOR_TEN_PERCENT) {	// Time to check if pump is working
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
	}

	unsigned int TankManager::getTankCapacity() const {
		float distanceToTop = TANK_HEIGHT_CM - (dm.getDistance() - ULTRASONIC_SENSOR_HEIGHT_CM);
		int tempCapacity = ((TANK_HEIGHT_CM - distanceToTop) / TANK_HEIGHT_CM) * 100;
		if (tempCapacity <= 0) {
			tempCapacity = 0;
		}
		else if (tempCapacity >= 100) {
			tempCapacity = 100;
		}
		return tempCapacity;
	}

};