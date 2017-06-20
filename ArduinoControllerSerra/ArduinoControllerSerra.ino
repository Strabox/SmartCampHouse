/*
 Name:		ArduinoControllerSerra.ino
 Created:	6/20/2017 2:26:40 PM
 Author:	André
*/

// the setup function runs once when you press reset or power the board
#include "Valve.h"
#include "GSMService.h"

#define VALVE_PIN 7 
#define SIM_CARD_PIN "0000"

using namespace SerraController;

Service::GSMService gsmService(SIM_CARD_PIN);

void setup() {
	int test;
	Serial.begin(9600);
	Serial.println("Arduino Rocking Serra!");
	Serial.println("Start connecting to GSM network...");
	Serial.println(sizeof(test));
	//gsmService.connect();
	Serial.println("GSM board SUCCESSFUL connected!");
}

// the loop function runs over and over again until power down or reset
void loop() {
	Domain::Valve tankValve(VALVE_PIN);
	tankValve.init();
	delay(4000);
}

