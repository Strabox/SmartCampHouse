#include "SMS.h"

SMS::SMS(int number, const char* message) : SMS::SMS() {
	setNumber(number);
	setMessage(message);
}

SMS::SMS() {
	Util::zeroBuffer(_message, MAX_SMS_SIZE + 1);
	setNumber(0);
}

unsigned long SMS::getNumber() {
	return _number;
}

void SMS::setNumber(unsigned long newNumber) {
	_number = newNumber;
}

const char* SMS::getMessage() {
	return _message;
}

void SMS::setMessage(const char* newMessage) {
	Util::zeroBuffer(_message, MAX_SMS_SIZE + 1);
	strncpy(_message, newMessage, MAX_SMS_SIZE);
}

void SMS::clear() {
	Util::zeroBuffer(_message, MAX_SMS_SIZE + 1);
	setNumber(0);
}