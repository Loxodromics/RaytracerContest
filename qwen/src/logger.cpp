#include "logger.h"

/// Static variable definition
inline bool Logger::loggingEnabled = false;

void Logger::log(const std::string message) {
	if (loggingEnabled)
		std::cout << message << std::endl;
}

void Logger::setLogging(bool enable) {
	loggingEnabled = enable;
}

bool Logger::isLoggingEnabled() {
	return loggingEnabled;
}