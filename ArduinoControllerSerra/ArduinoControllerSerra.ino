/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

#include <GSMLibrary.h>
#include "TankManager.h"

#define SETUP_INITIAL_DELAY 500
#define HARDWARE_SERIAL_BAUD_RATE 19200
#define MAIN_LOOP_DELAY 2000

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8
#define LAMP_PIN 11

using Periphericals::Relay;
using Domain::TankManager;

TankManager tankManager = TankManager();
GSMService gsmService(GSM_TX_PIN, GSM_RX_PIN, &Serial);
Relay lamp(LAMP_PIN, "Lampada");

void setup() {
	delay(SETUP_INITIAL_DELAY);
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println(F("Setup..."));
	gsmService.begin(GSM_SIM900_BAUD_RATE);
	gsmService.beginListenForSMS();
	tankManager.begin();
	Serial.println(F("Ready"));
}

void loop() {
	Serial.println(F("Loop Start"));

	SMS newSMS;
	bool newSms = false;

	gsmService.availableSMS(newSMS, newSms);
	if (newSms) {
		if (strcmp(newSMS.getMessage(), "Abre") == 0) {
			tankManager.open();
		}
		else if (strcmp(newSMS.getMessage(), "Fecha") == 0) {
			tankManager.close();
		}
		else if (strcmp(newSMS.getMessage(), "Estado") == 0) {
			char* tempToString = tankManager.toString();
			newSMS.setNumber(newSMS.getNumber());
			newSMS.setMessage(tempToString);
			gsmService.sendSMS(newSMS);
			free(tempToString);
		}

		gsmService.deleteAllSMS();
	}

	Serial.println(F("Loop End"));
	delay(MAIN_LOOP_DELAY);
}
