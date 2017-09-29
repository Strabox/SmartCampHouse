// 
// 
// 

#include "Valve.h"

Valve::Valve(uint8_t arduinoPin, const char* name) {
	_arduinoPin = arduinoPin;
	_isOpen = false;
	strncpy(_name, name, MAX_NAME_SIZE);
}

void Valve::begin() {
	pinMode(_arduinoPin, OUTPUT);
	digitalWrite(_arduinoPin, HIGH);
}

void Valve::open() {
	if (!_isOpen) {
		digitalWrite(_arduinoPin, LOW);
		_isOpen = true;
	}
}

void Valve::close() {
	if (_isOpen) {
		digitalWrite(_arduinoPin, HIGH);
		_isOpen = false;
	}
}

bool Valve::isOpen()
{
	return _isOpen;
}

char* toString() {

}

Stream & operator<<(Stream &stream, const Valve &valve)
{
	stream.println("Valvula ");
	return stream;
}
