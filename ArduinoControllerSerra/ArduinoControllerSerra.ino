/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

#include "Valve.h"
#include "GSMLibrary.h"

#define DEBUG

#define HARDWARE_SERIAL_BAUD_RATE 19200
#define LOOP_DEBUG_DELAY 2000
#define INITIAL_DELAY 1000

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8
#define TANK_VALVE_OUT_PIN 11


GSMService gsmService(GSM_TX_PIN, GSM_RX_PIN, &Serial);

Valve tankValve(TANK_VALVE_OUT_PIN, "Tanque");

long unsigned allowedNumbers[] = { 967865329, 967992094, 962653446, 927656383 };

void setup() {
	delay(INITIAL_DELAY);
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println(F("Setup..."));
	gsmService.begin(GSM_SIM900_BAUD_RATE);
	gsmService.beginListenForSMS();
	tankValve.begin();
	Serial.println(F("Ready"));
}

void loop() {
	#ifdef DEBUG
	Serial.println(F("Loop Start"));
	#endif
	
	SMS sms;
	bool newSms = false;

	gsmService.tryReadSMS(&sms, &newSms);
	if (newSms) {

		Serial.print("Message: ");
		Serial.println(sms.getMessage());
		Serial.print("Number: ");
		Serial.println(sms.getNumber());

		if (numberAllowed(sms.getNumber())) {
			if (strcmp(sms.getMessage(), "Liga") == 0) {
				Serial.print("Liga\n");
				tankValve.open();
			}
			else if (strcmp(sms.getMessage(), "Desliga") == 0) {
				Serial.print("Desliga\n");
				tankValve.close();
			}
			else if (strcmp(sms.getMessage(), "Estado") == 0) {
				sms.setNumber(962653446);
				sms.setMessage("SMS a funcionar!");
				gsmService.sendSMS(&sms);
				gsmService.beginListenForSMS();
			}
		}
	}

	#ifdef DEBUG
	Serial.println(F("Loop End"));
	delay(LOOP_DEBUG_DELAY);
	#endif
}

bool numberAllowed(unsigned long number) {
	if(number == allowedNumbers[0] || 
	   number == allowedNumbers[1] ||
	   number == allowedNumbers[2] ||
	   number == allowedNumbers[3]) {
		return true;
	}
	else {
		return false;
	}
}