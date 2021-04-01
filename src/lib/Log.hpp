#pragma once
#include <string>

namespace LibOrichalcum {

enum class LogLevel {
	DEBUG = 0,
	VERBOSE = 1,
	INFO = 2,
	WARNING = 3,
	ERROR = 4
};

class Log {
public:
	static void set_level(LogLevel level);

	static void debug(const std::string &msg);
	static void verbose(const std::string &msg);
	static void info(const std::string &msg);
	static void warning(const std::string &msg);
	static void error(const std::string &msg);

private:
	static LogLevel level;

};

} // lib
