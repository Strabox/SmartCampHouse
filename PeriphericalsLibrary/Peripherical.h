/*
Peripherical.h
*/

#ifndef _PERIPHERICAL_h
#define _PERIPHERICAL_h

#include <Arduino.h>

#define MAX_NAME_SIZE 15		// Maximum size for a peripherical string name/identifier

namespace Periphericals {

	/*
	General class that provides the common API and behaviour for the majority of peripheric/sensors for Arduino
	*/
	class Peripherical {

		protected: 
		
		/* Used to know if the peripherical is initialized and ready to receive requests */
		bool _isInitialized;

		public:

		/* Constructor called by every sub-classes */
		Peripherical();

		/* Destroy all the resources associated with the peripherical (IF EXISTS) */
		~Peripherical();

		#pragma region Public Virtual Common API Methods

		/* Used to put the peripherical ready to receive requests. Normally this have some default behaviour some 
		periphericals have other API method with same name that receives other arguments to a better customization. */
		virtual void begin() = 0;

		/* Used to stop the peripherical. After this the peripherical will not process API requests. */
		virtual void end() = 0;

		/* Used to return a string representation of the peripherical content.
		IMPORTANT: It is mandatory to free the returned pointer in order to have no memory leaks. */
		virtual char* toString() const = 0;

		#pragma endregion


		#pragma region Public Common API Methods

		/*
		Verify is the periphetical is initialized and ready to use.
		@return	- True: If peripherical is initialzed. False: Otherwise.
		*/
		virtual bool isInitialized() const;

		#pragma endregion

		/* Write the object string representation to a given stream (e.g: Used in debug) */
		friend Stream& operator<<(Stream & stream, const Peripherical& peripherical);

	};

};

#endif