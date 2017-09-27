#include "Util.h"

void Util::zeroBuffer(void *buffer, size_t bufferSize) {
	memset(buffer, NULL, bufferSize);
}

void Util::overrideLastStringChar(char* string) {
	int stringLength = strlen(string);
	if (stringLength >= 1) {
		*(string + (sizeof(char) * (stringLength - 1))) = NULL;
	}
}

void Util::overrideLastTwoChar(char* string) {
	int stringLength = strlen(string);
	if (stringLength >= 1) {
		*(string + (sizeof(char) * (stringLength - 1))) = NULL;
	}
	if (stringLength >= 2) {
		*(string + (sizeof(char) * (stringLength - 2))) = NULL;
	}
}
