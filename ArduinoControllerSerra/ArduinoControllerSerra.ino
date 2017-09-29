/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

#include "Valve.h"
#include <GSMLibrary.h>

#define DEBUG

#define HARDWARE_SERIAL_BAUD_RATE 19200
#define LOOP_DEBUG_DELAY 2000
#define INITIAL_DELAY 1000

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8
#define TANK_VALVE_OUT_PIN 10


GSMService gsmService(GSM_TX_PIN, GSM_RX_PIN, &Serial);

Valve tankValve(TANK_VALVE_OUT_PIN, "Tanque");

void setup() {
	delay(INITIAL_DELAY);
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println(F("Setup..."));
	gsmService.begin(GSM_SIM900_BAUD_RATE);
	gsmService.beginListenForSMS();
	gsmService.addAllowedNumber(967865329);
	gsmService.addAllowedNumber(967992094);
	gsmService.addAllowedNumber(962653446);
	gsmService.addAllowedNumber(927656383);
	tankValve.begin();
	Serial.println(F("Ready"));
}

void loop() {
	#ifdef DEBUG
	Serial.println(F("Loop Start"));
	#endif
	SMS newSMS;
	bool newSms = false;

	gsmService.availableSMS(newSMS, newSms);
	if (newSms) {
		Serial.print(F("Message: "));
		Serial.println(newSMS.getMessage());
		Serial.print(F("Number: "));
		Serial.println(newSMS.getNumber());

		if (strcmp(newSMS.getMessage(), "Abre") == 0) {
			tankValve.open();
		}
		else if (strcmp(newSMS.getMessage(), "Fecha") == 0) {
			tankValve.close();
		}
		else if (strcmp(newSMS.getMessage(), "Estado") == 0) {
			newSMS.setNumber(newSMS.getNumber());
			if (tankValve.isOpen()) {
				newSMS.setMessage("Aberto");
			}
			else {
				newSMS.setMessage("Fechado");
			}
			gsmService.sendSMS(newSMS);
		}

		gsmService.deleteAllSMS();
	}

	#ifdef DEBUG
	Serial.println(F("Loop End"));
	delay(LOOP_DEBUG_DELAY);
	#endif
}
