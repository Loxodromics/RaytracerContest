#pragma once

#include <iostream>
#include <string>

class Logger {
public:
	static void setLogging(bool enable);
	static bool isLoggingEnabled();
	static void log(const std::string message);

private:
	static bool loggingEnabled;
};
