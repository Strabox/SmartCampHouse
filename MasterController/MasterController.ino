/*
Name:		MasterController.ino
Created:	11/6/2017 1:03:01 PM
Author:		André

Arduino UNO pin reservation scheme:
- Pin 2 used to read the water flow sensor (necessary due to interruptions)
- Pin 4 used to control the water pump relay
- Pin 5 used to control the lamp relay
- Pin 6 used to control the water valve relay
- Pins 7 (RX) and 8 (TX) used to Serial with the GSM board and pin 9 (Pwoer) used to turn ON/OFF the GSM board
- Pins 11 (Trigger) and 12 (Echo) used to interact with Ultrasonic Module
*/
#include <ArduinoJson.h>
#include <Easyuino.h>
#include "Tank.h"

#pragma region Configuration Constants

// General Configurations

#define DEBUG 1		// Used to control the compilation of debug instructions
#define USB_DEBUG_SERIAL_BAUD_RATE 115200
#define LOOP_DELAY 500

#define ALLOWED_PHONE_NUMER_1 99999999

// Tank Configurations

#define ULTRASONIC_SENSOR_HEIGHT_CM 0.0f 
#define TANK_HEIGHT_CM 60.0f
#define TANK_MAX_CAPACITY 90		// Tank Max Capacity we want maintain (in %)

#define ULTRASONIC_TRIGGER_PIN 11
#define ULTRASONIC_ECHO_PIN 12

#define VALVE_PIN 6

#define WATER_PUMP_PIN 4

#define WATER_FLOW_SENSOR_PIN 2

// Other Devices Configurations

#define GSM_TX_PIN 7
#define GSM_RX_PIN 8
#define GSM_POWER_PIN 9

#define LAMP_PIN 5

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
using Easyuino::GSMServiceSecure;
using Easyuino::SMS;
using Easyuino::Relay;
using Domain::Tank;

Tank tank = Tank(VALVE_PIN, WATER_PUMP_PIN, ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN,
	ULTRASONIC_SENSOR_HEIGHT_CM, TANK_HEIGHT_CM, TANK_MAX_CAPACITY, WATER_FLOW_SENSOR_PIN);

Relay lamp = Relay(LAMP_PIN);

#if (DEBUG) == 1
	GSMServiceSecure gsmService = GSMServiceSecure(GSM_TX_PIN, GSM_RX_PIN, GSM_POWER_PIN, Serial);
#else
	GSMServiceSecure gsmService = GSMServiceSecure(GSM_TX_PIN, GSM_RX_PIN, GSM_POWER_PIN);
#endif

void setup() {
#if (DEBUG) == 1
	Serial.begin(USB_DEBUG_SERIAL_BAUD_RATE);
	Serial.println(F("Setup..."));
#endif

	tank.begin();

	gsmService.begin(GSM_DEFAULT_BAUD_RATE);
	gsmService.turnOn();
	gsmService.addAllowedNumber(ALLOWED_PHONE_NUMER_1);	// WARNING: Careful when testing see if the phone number is right!
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

	// Important: This call will verify the tank invariants to see if it is needed take safety actions.
	tank.checkStatus();

#if (DEBUG) == 1
	Serial.print(F("Capacity: "));
	Serial.println(tank.getTankCapacity());
#endif

	gsmService.availableSMS(newSMS, newSms);
	if (newSms) {
		Serial.println(newSMS.getMessage());
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
			char* bufferTemp = NULL;
			const size_t bufferSize = JSON_OBJECT_SIZE(4);
			DynamicJsonBuffer jsonBuffer(bufferSize);

			JsonObject& root = jsonBuffer.createObject();
			root["C"] = tank.getTankCapacity();
			root["V"] = tank.getValveState();
			root["P"] = tank.getWaterPumpState();
			root["L"] = lamp.isOn();

			bufferTemp = (char*)Utilities::EasyMalloc(sizeof(char) * root.measureLength());

			newSMS.setMessage(bufferTemp);
			gsmService.sendSMS(newSMS);		// Reply to the SMS with the system state

			free(bufferTemp);
		}

		gsmService.deleteAllReadSMS();	//Important: Delete the received SMS to free space
	}

#if (DEBUG) == 1
	Serial.println(F("Loop End"));
#endif
	delay(LOOP_DELAY);
}
