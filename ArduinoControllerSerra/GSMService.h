// GSMService.h

#ifndef _GSMSERVICE_h
#define _GSMSERVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include <GSM.h>
#else
	#include "WProgram.h"
#endif

namespace SerraController {

	namespace Service {

		/*
			Represents GSM service backed up by GSM board/shield
		*/
		class GSMService
		{
			private: static const int CONNECT_POOL_INTERVAL = 2000;
			private: static const int PIN_SIZE = 4;

			private: char _simPIN[PIN_SIZE + 1];

			private: GSM _gsmAccess;

			private: GSM_SMS _sms;

			public: GSMService(char[]);

			public: ~GSMService();

			public: void connect();

			public: char* getSMS();
		};

	}
}
#endif

