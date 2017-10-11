#include "Relay.h"

namespace Periphericals {

	Relay::Relay(uint8_t arduinoPin, const char* name) : Peripherical() {
		_arduinoPin = arduinoPin;
		_isOpen = false;
		memset(_name, NULL, MAX_NAME_SIZE);
		strncpy(_name, name, MAX_NAME_SIZE);
	}

	Relay::~Relay() { /* Do Nothing */ }

	void Relay::begin() {
		begin(HIGH);	// Default is HIGH
	}

	void Relay::begin(uint8_t closedStatePinLevel) {
		if (!_isInitialized && (closedStatePinLevel == LOW || closedStatePinLevel == HIGH)) {
			pinMode(_arduinoPin, OUTPUT);
			digitalWrite(_arduinoPin, closedStatePinLevel);
			_isInitialized = true;
		}
	}

	void Relay::end() {
		if (_isInitialized) {
			close();
			_isInitialized = false;
		}
	}

	void Relay::open() {
		if (_isInitialized && !_isOpen) {
			if (digitalRead(_arduinoPin) == HIGH) {
				digitalWrite(_arduinoPin, LOW);
			}
			else {
				digitalWrite(_arduinoPin, HIGH);
			}
			_isOpen = true;
		}
	}

	void Relay::close() {
		if (_isInitialized && _isOpen) {
			if (digitalRead(_arduinoPin) == HIGH) {
				digitalWrite(_arduinoPin, LOW);
			}
			else {
				digitalWrite(_arduinoPin, HIGH);
			}
			_isOpen = false;
		}
	}

	bool Relay::isOpen() const {
		return _isOpen;
	}

	char* Relay::toString() const {
		const char* delimiter = ":";
		char* res = NULL;
		unsigned int resSize = 0;
		if (isOpen()) {
			resSize = strlen(_name) + strlen(delimiter) + strlen(OPEN_STRING) + 1;
			res = (char*)malloc(sizeof(char) * resSize);
			snprintf(res, resSize, "%s%s%s", _name, delimiter, OPEN_STRING);
		} else {
			resSize = strlen(_name) + strlen(delimiter) + strlen(CLOSED_STRING) + 1;
			res = (char*)malloc(sizeof(char) * resSize);
			snprintf(res, resSize, "%s%s%s", _name, delimiter, CLOSED_STRING);
		}
		return res;
	}

};