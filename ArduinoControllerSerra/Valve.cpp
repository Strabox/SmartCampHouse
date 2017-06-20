// 
// 
// 

#include "Valve.h"

using namespace SerraController::Domain;


Valve::Valve(Pin pin)
{
	_outputPin = pin;
}

void Valve::init() {
	pinMode(_outputPin, OUTPUT);
}

void Valve::open() {
	digitalWrite(_outputPin, HIGH);
}

void Valve::close() {
	digitalWrite(_outputPin, LOW);
}

