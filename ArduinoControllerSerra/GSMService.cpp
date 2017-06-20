// 
// 
// 

#include "GSMService.h"

using namespace SerraController::Service;


GSMService::GSMService(char simPin[]) {
	size_t i;
	for (i = 0; i < PIN_SIZE; i++)
	{
		_simPIN[i] = simPin[i];
		Serial.println(_simPIN[i]);
	}
	_simPIN[i] = '\0';
}

GSMService::~GSMService() { }

void GSMService::connect() {
	bool notConnected = true;
	while (notConnected) {
		if (_gsmAccess.begin(_simPIN) == GSM_READY) {
			notConnected = false;
		}
		else {
			delay(CONNECT_POOL_INTERVAL);
		}
	}
}

char* GSMService::getSMS() {
	if (_sms.available()) {

	}
}

