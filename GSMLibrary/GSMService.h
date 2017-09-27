#ifndef _GSM_SERVICE_h
#define _GSM_SERVICE_h

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "SMS.h"
#include "Util.h"


/* Define API method parameter to be exclusively an input */
#define IN
/* Define API method parameter to be exclusively an output */
#define OUT
/* Define API method parameter that is used as input and output */
#define INOUT

#define GSM_SIM900_BAUD_RATE 9600
#define INTERNAL_BUFFER_SIZE 128

/* 
	Used to indicate the status of the request to the GSMService API 
*/
enum GSMRequestStatus
{
	/* The request was successful executed */
	GSM_OK = 0,
	/* Problems with the request (probably problems with communication to GSM module) */
	GSM_ERROR = -1,
	/* The GSMService API was not correctly initialized and can't process requests */
	GSM_SERVICE_NOT_INITIALIZED = -2,
	/* Null argument was provided to the GSMService API */
	GSM_REQUEST_NULL_ARGUMENT = -3
};

/*
	Used to interact with the GSM board
*/
class GSMService {

	private:

	const char *AT_OK = "\r\nOK\r\n";

	Stream* _outputStream;
	SoftwareSerial* _GSMSerial;
	char _internalBuffer[INTERNAL_BUFFER_SIZE];
	bool _isInitialized;
	bool _readyToReceiveSMS;
	bool _debug;

	public:

	/*
		Creates a GSMService
		@param txPin		- Pin used to send the serial information to GSM module
		@param rxPin		- Pin used to receive the serial information from GSM module
		@param outputStream	- Stream to output the GSM Module outputs in case of necessary debug
		@param debug		- 
	*/
	GSMService(uint8_t txPin, uint8_t rxPin, Stream *outputStream);

	/*
		Creates a GSMService
		@param txPin		- Pin used to send the serial information to GSM module
		@param rxPin		- Pin used to receive the serial information from GSM module
	*/
	GSMService(uint8_t txPin, uint8_t rxPin);

	/*
		Deallocates all the internal resources used by the GSMService
	*/
	~GSMService();

	/*  Initialize the communications with the GSM module
		@param gsmBoardBaudRate	- Baud rate of the gsm module used
		@return - GSMService API request status
	*/
	GSMRequestStatus begin(IN int gsmBoardBaudRate);

	/*
		Closes all the communications with the GSM module
		@return - GSMService API request status
	*/
	GSMRequestStatus end();

	/*
		Start listening for received SMS in the GSM module serial output
		@return	- GSMService API request status
	*/
	GSMRequestStatus beginListenForSMS();
	
	/*
		Try read a SMS from the GSM module 
		@param message	- Allocated message to be filled by the method
		@param smsRead	- Filled by the API. True if a SMS was filled. False otherwise.
		@return			- GSMService API request status
	*/
	GSMRequestStatus tryReadSMS(INOUT SMS *message, OUT bool *smsRead);

	/*
		Send a SMS through the GSM module
		@param sms	 - Filled sms to be send by the GSM module
		@return		- GSMService API request status
	*/
	GSMRequestStatus sendSMS(IN SMS *sms);

	/*
		Delete all the sms in the GSM module
		@return		- GSMService API request status
	*/
	GSMRequestStatus deleteAllSMS();

	private:
	
	void waitForGSMBoardData();

	bool lookForGSMBoardData();

	void zeroInternalBuffer();

};


#endif