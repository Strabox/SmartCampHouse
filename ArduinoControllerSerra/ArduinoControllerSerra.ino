/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

#include <Easyuino.h>
#include "Tank.h"

#pragma region Configuration Constants

// General Configurations

#define DEBUG 1	//Used to control the compilation of debug instructions 0 = FALSE and 1 = TRUE
#define HARDWARE_SERIAL_BAUD_RATE 115200
#define LOOP_DELAY 500

// Tank Configurations

#define ULTRASONIC_SENSOR_HEIGHT_CM 0.0f 
#define TANK_HEIGHT_CM 60.0f
#define TANK_MAX_CAPACITY 90

#define VALVE_NAME "Valve"			// TIGHTLY COUPLED with Android Application
#define VALVE_PIN 4

#define WATER_PUMP_NAME "WaterPump"	// TIGHTLY COUPLED with Android Application
#define WATER_PUMP_PIN 5

#define ULTRASONIC_ECHO_PIN 13
#define ULTRASONIC_TRIGGER_PIN 12

// Other Devices Configurations

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8

#define LAMP_NAME "Lamp"			// TIGHTLY COUPLED with Android Application
#define LAMP_PIN 11

#pragma endregion

#pragma region Accepted SMS Commands
/* TIGHTLY COUPLED with Android Application */

#define SMS_COMMAND_OPEN_VALVE "O-V"
#define SMS_COMMAND_CLOSE_VALVE "C-V"

#define SMS_COMMAND_OPEN_WATER_PUMP "O-W"
#define SMS_COMMAND_CLOSE_WATER_PUMP "C-W"

#define SMS_COMMAND_OPEN_LAMP "O-L"
#define SMS_COMMAND_CLOSE_LAMP "C-L"

#define SMS_COMMAND_STATUS_REPORT "SR"

#pragma endregion

using Easyuino::Utilities;
using Easyuino::GSMService;
using Easyuino::SMS;
using Easyuino::RelayNamed;
using Domain::Tank;

Tank tank = Tank(VALVE_NAME, VALVE_PIN, WATER_PUMP_NAME, WATER_PUMP_PIN,
	ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_SENSOR_HEIGHT_CM, TANK_HEIGHT_CM, TANK_MAX_CAPACITY);
RelayNamed lamp = RelayNamed(LAMP_PIN, LAMP_NAME);
#if (DEBUG) == 1
	GSMService gsmService = GSMService(GSM_TX_PIN, GSM_RX_PIN, Serial);
#else
	GSMService gsmService = GSMService(GSM_TX_PIN, GSM_RX_PIN);
#endif

void setup() {
#if (DEBUG) == 1
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println(F("Setup..."));
#endif

	tank.begin();
	gsmService.begin(GSM_DEFAULT_BAUD_RATE);
	gsmService.beginListenForSMS();

#if (DEBUG) == 1
	Serial.println(F("Ready"));
#endif
}

void loop() {
#if (DEBUG) == 1
	Serial.println(F("Loop Start"));
#endif

	SMS newSMS;
	bool newSms = false;

	// Important: Calculate the new capacity of the tank
	tank.updateCapacity();	

#if (DEBUG) == 1
	Serial.print(F("Capacity: "));
	Serial.println(tank.getTankCapacity());
#endif

	gsmService.availableSMS(newSMS, newSms);
	if (newSms) {
		if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_VALVE) == 0) {
			tank.open();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_VALVE) == 0) {
			tank.close();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_WATER_PUMP) == 0) {
			tank.startFill();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_WATER_PUMP) == 0) {
			tank.stopFill();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_OPEN_LAMP) == 0) {
			lamp.turnOn();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_CLOSE_LAMP) == 0) {
			lamp.turnOff();
		}
		else if (strcmp(newSMS.getMessage(), SMS_COMMAND_STATUS_REPORT) == 0) {
			char* tankManagerString = tank.toString();
			char* lampToString = lamp.toString();
			char* messageToSend = NULL;
			
			size_t messageToSendSize = strlen(tankManagerString) + strlen(lampToString) + 2;
			messageToSend = (char*)Utilities::EasyMalloc(sizeof(char) * messageToSendSize);
			snprintf(messageToSend, messageToSendSize, "%s\n%s", tankManagerString, lampToString);

			newSMS.setNumber(newSMS.getNumber());
			newSMS.setMessage(messageToSend);
			gsmService.sendSMS(newSMS);

			free(tankManagerString);
			free(lampToString);
			free(messageToSend);
		}

		gsmService.deleteAllReadSMS();	//Important: Delete the received SMS to free space
	}

	// Important: This call will verify the tank invariant to see if it is needed take actions
	tank.checkStatus();

#if (DEBUG) == 1
	Serial.println(F("Loop End"));
#endif
	delay(LOOP_DELAY);
}
