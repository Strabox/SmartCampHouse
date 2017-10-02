#include "Peripherical.h"

namespace Periphericals {

	Peripherical::Peripherical() {
		_isInitialized = false;
	}

	Peripherical::~Peripherical() { /* Do Nothing */ }

	bool Peripherical::isInitialized() const {
		return _isInitialized;
	}

	Stream & operator<<(Stream & stream, const Peripherical & peripherical) {
		char *tempRes = peripherical.toString();
		stream.print(tempRes);
		free(tempRes);
		return stream;
	}

};