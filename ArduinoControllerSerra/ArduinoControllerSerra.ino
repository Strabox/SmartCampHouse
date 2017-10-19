/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

#include <GSMLibrary.h>
#include "TankManager.h"

#pragma region General Constants

#define SETUP_INITIAL_DELAY 500
#define HARDWARE_SERIAL_BAUD_RATE 115200
#define MAIN_LOOP_DELAY 2000

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8
#define LAMP_PIN 11

#define LAMP_NAME "Lamp"

#pragma endregion

#pragma region Accepted SMS Commands

#define SMS_COMMAND_OPEN_VALVE "O-V"
#define SMS_COMMAND_CLOSE_VALVE "C-V"
#define SMS_COMMAND_OPEN_WATER_PUMP "O-W"
#define SMS_COMMAND_CLOSE_WATER_PUMP "C-W"

#define SMS_COMMAND_OPEN_LAMP "O-L"
#define SMS_COMMAND_CLOSE_LAMP "C-L"

#define SMS_COMMAND_STATUS_REPORT "SR"

#pragma endregion


using GSMLibrary::GSMService;
using GSMLibrary::SMS;
using Easyuino::RelayNamed;
using Domain::TankManager;

TankManager tankManager = TankManager();
GSMService gsmService = GSMService(GSM_TX_PIN, GSM_RX_PIN, Serial);
RelayNamed lamp = RelayNamed(LAMP_PIN, LAMP_NAME);

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
		if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_VALVE) == 0) {
			tankManager.open();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_VALVE) == 0) {
			tankManager.close();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_WATER_PUMP) == 0) {
			tankManager.startFill();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_WATER_PUMP) == 0) {
			tankManager.stopFill();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_LAMP) == 0) {
			lamp.open();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_LAMP) == 0) {
			lamp.close();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_STATUS_REPORT) == 0) {
			char* tankManagerString = tankManager.toString();
			char* lampToString = lamp.toString();
			char* messageToSend = NULL;
			
			size_t messageToSendSize = strlen(tankManagerString) + strlen(lampToString) + 2;
			messageToSend = (char*)malloc(sizeof(char) * messageToSendSize);
			snprintf(messageToSend, messageToSendSize, "%s\n%s", tankManagerString, lampToString);
			
			newSMS.setNumber(newSMS.getNumber());
			newSMS.setMessage(messageToSend);
			gsmService.sendSMS(newSMS);

			free(tankManagerString);
			free(lampToString);
			free(messageToSend);
		}

		gsmService.deleteAllReadSMS();	// Delete the received SMS to free space in SIM Card
	}

	Serial.println(F("Loop End"));
	delay(MAIN_LOOP_DELAY);
}
