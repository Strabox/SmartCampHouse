#include "GSMService.h"

#define DELAY_WAIT_FOR_DATA 250

GSMService::GSMService(uint8_t txPin, uint8_t rxPin, Stream *outputStream) 
	: GSMService(txPin, rxPin) {
	if (outputStream != nullptr) {
		_outputStream = outputStream;
		_debug = true;
	}
}

GSMService::GSMService(uint8_t txPin, uint8_t rxPin) {
	_GSMSerial = new SoftwareSerial(txPin, rxPin);
	_outputStream = nullptr;
	_isInitialized = false;
	_readyToReceiveSMS = false;
	_debug = false;
	zeroInternalBuffer();
}

GSMService::~GSMService() {
	end();
	delete _GSMSerial;
	_GSMSerial = nullptr;
	_outputStream = nullptr;
}

GSMRequestStatus GSMService::begin(IN int gsmBoardBaudRate) {
	if (!_isInitialized) {
		_GSMSerial->begin(gsmBoardBaudRate);
		_GSMSerial->print(F("ATE0\r"));
		waitForGSMBoardData();
		if (strcmp(_internalBuffer, AT_OK) == 0) {
			_isInitialized = true;
		}
		else {
			_GSMSerial->end();
			_isInitialized = false;
			return GSMRequestStatus::GSM_ERROR;
		}
	}
	return GSMRequestStatus::GSM_OK;
}

GSMRequestStatus GSMService::end() {
	if (_isInitialized) {
		_GSMSerial->end();
		_isInitialized = false;
	}
	return GSMRequestStatus::GSM_OK;
}

GSMRequestStatus GSMService::beginListenForSMS() {
	if (_isInitialized) {

		_GSMSerial->print(F("AT+CMGF=1\r"));
		waitForGSMBoardData();

		if (strcmp(_internalBuffer, AT_OK) == 0) {
			// Blurt out contents of new SMS upon receipt to the GSM shield’s serial out
			_GSMSerial->print(F("AT+CNMI=2,2,0,0,0\r"));
			waitForGSMBoardData();

			if (strcmp(_internalBuffer, AT_OK) == 0) {
				_readyToReceiveSMS = true;
				return GSMRequestStatus::GSM_OK;
			}
		}

		_readyToReceiveSMS = false;
		return GSMRequestStatus::GSM_ERROR;
	}
	else {
		return GSMRequestStatus::GSM_SERVICE_NOT_INITIALIZED;
	}
}

GSMRequestStatus GSMService::tryReadSMS(INOUT SMS* message, OUT bool *smsRead)
{
	const char delim[] = "\n";
	char *token = NULL, *next_token = NULL;
	int num_tokens = 0;

	*smsRead = false;

	if (!_isInitialized) {
		return GSMRequestStatus::GSM_SERVICE_NOT_INITIALIZED;
	}
	if (message == NULL || smsRead == NULL) {
		return GSMRequestStatus::GSM_REQUEST_NULL_ARGUMENT;
	}

	if (!_readyToReceiveSMS || !lookForGSMBoardData()) {
		return GSMRequestStatus::GSM_OK;
	}

	//Parse the SMS text in the buffer
	token = strtok_r(_internalBuffer, delim, &next_token);
	while (token != NULL) {
		if (num_tokens == 0) {
			if (strcmp(token, "\r") != 0) {
				message->clear();
				return GSMRequestStatus::GSM_OK;
			}
		}
		else if (num_tokens == 1) {
			if (strlen(token) > 20) {
				char number[10];
				memset(number, NULL, 10);
				for (int i = 11; i < 20; i++) {
					number[i - 11] = token[i];
				}
				message->setNumber(atol(number));
			}
			else {
				message->clear();
				return GSMRequestStatus::GSM_OK;
			}
		}
		else if (num_tokens == 2) {
			Util::overrideLastStringChar(token);
			message->setMessage(token);
		}
		else {
			message->clear();
			return GSMRequestStatus::GSM_OK;
		}

		num_tokens++;
		token = strtok_r(NULL, delim, &next_token);
	}

	if (num_tokens == 3) {
		*smsRead = true;
	}
	else {
		message->clear();
	}
	return GSMRequestStatus::GSM_OK;
}

GSMRequestStatus GSMService::sendSMS(IN SMS *sms)
{
	if (!_isInitialized) {
		return GSMRequestStatus::GSM_SERVICE_NOT_INITIALIZED;
	}
	if (sms == NULL) {
		return GSMRequestStatus::GSM_REQUEST_NULL_ARGUMENT;
	}

	// sending SMS in text mode
	_GSMSerial->print(F("AT+CMGF=1\r"));
	waitForGSMBoardData();

	zeroInternalBuffer();
	snprintf(_internalBuffer, INTERNAL_BUFFER_SIZE - 1, "AT+CMGS=\"+351%ld\"\r%s\r",
				sms->getNumber(), sms->getMessage());
	_GSMSerial->print(_internalBuffer);
	waitForGSMBoardData();

	zeroInternalBuffer();
	_GSMSerial->write(0x1A);
	waitForGSMBoardData();

	return GSMRequestStatus::GSM_OK;
}

GSMRequestStatus GSMService::deleteAllSMS() {
	if (!_isInitialized) {
		return GSMRequestStatus::GSM_SERVICE_NOT_INITIALIZED;
	}

	_GSMSerial->print(F("AT+CMGDA=\"DEL ALL\""));
	waitForGSMBoardData();

	if (strcmp(_internalBuffer, AT_OK)) {
		return GSMRequestStatus::GSM_OK;
	}
	else {
		return GSMRequestStatus::GSM_ERROR;
	}
}

void GSMService::waitForGSMBoardData() {

	while (_GSMSerial->available() == 0) { 
		/* ACTIVELY wait for data */
	}
	lookForGSMBoardData();
}

bool GSMService::lookForGSMBoardData() {
	int count = 0;

	if (_GSMSerial->available() > 0) {
		zeroInternalBuffer();
		delay(DELAY_WAIT_FOR_DATA);

		while (_GSMSerial->available() > 0) {
			_internalBuffer[count++] = _GSMSerial->read();
			if (count == (INTERNAL_BUFFER_SIZE - 1)) {
				break;
			}
		}

		if (_debug) {	//Output the GSM module output if user want
			_outputStream->write(_internalBuffer, count);
		}

		return true;
	}
	else {
		return false;
	}
}

void GSMService::zeroInternalBuffer() {
	Util::zeroBuffer(_internalBuffer, INTERNAL_BUFFER_SIZE);
}
