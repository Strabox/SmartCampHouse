/*
 Name:		TestSketches.ino
 Created:	10/1/2017 6:01:24 PM
 Author:	André

 Sketch used to test some components independly from the ArduinoControllerSerra main ones

*/

#include "Relay.h"
#include "DistanceMeter.h"
#include "RainDetector.h"

#define HARDWARE_SERIAL_BAUD_RATE 115200
#define MAIN_LOOP_DELAY 2000
#define SETUP_INITIAL_DELAY 500

using Periphericals::Relay;
using Periphericals::DistanceMeter;
using Periphericals::RainDetector;

Relay relay(10, "Tanque");

DistanceMeter dm(4, 2);

void setup() {
	delay(SETUP_INITIAL_DELAY);
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println("Setup...");

	setupDistanceMeter();

	Serial.println("Ready");
}

void loop() {
	Serial.println("Loop Start");

	testDistanceMeterLoop();
	
	Serial.println("Loop End");
}

/*=================== Test Loop Functions ===================== */

void setupRelay() {
	relay.begin();
}

// Test the Relay Class
void testRelayLoop() {
	relay.open();
	Serial << relay;
	Serial.println();
	delay(2000);
	relay.close();
	Serial << relay;
	Serial.println();
	delay(2000);
}


void setupDistanceMeter() {
	pinMode(8, OUTPUT);	//Used to emulate interrupt
	dm.begin(false);
}

// Test the DistanceMeter Class
void testDistanceMeterLoop() {
	Serial.println(dm.getDistance());
	dm.updateDistance();
	delay(12);
	digitalWrite(8, HIGH);		// Emulate echo SENT
	delayMicroseconds(100);
	digitalWrite(8, LOW);		// Emulate echo RECEIVED
}
