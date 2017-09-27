#ifndef _SMS_h
#define _SMS_h

#include <stdlib.h>
#include <string.h>

#include "Util.h"

/* Maximum SMS chars to allow static allocation of resources */
#define MAX_SMS_SIZE 30

/*
	Represents a SMS object used to send and receive it from the GSMService
*/
class SMS {

	private: unsigned long _number;
	private: char _message[MAX_SMS_SIZE + 1];

	/*	Create a SMS object used to send/receive SMS from GSMLibrary
		@param number	- Number of sender or receiver
		@param message	- Message content
	*/
	public: SMS(int number, const char* message);

	/*	Create an empty SMS */
	public: SMS();


	/*	Get the number associated with the SMS
		@return	- Number associated with the SMS
	*/
	public: unsigned long getNumber();

	/*	Set the number of the SMS
		@param number - The new number to be set on the SMS
	*/
	public: void setNumber(unsigned long newNumber);

	/*	Get the message associated with the SMS
		@return	- Message associated with the SMS
	*/
	public: const char* getMessage();

	/*	Set the message of the SMS
		@param message - The new message to be set on the SMS
	*/
	public: void setMessage(const char* newMessage);

	/*	Resets the message to zeros and number to zero */
	public: void clear();


};

#endif
