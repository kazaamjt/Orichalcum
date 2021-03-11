#pragma once
#include <string>

namespace OrichalcumLib {

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

	static void debug(std::string &msg);
	static void verbose(std::string &msg);
	static void info(std::string &msg);
	static void warning(std::string &msg);
	static void error(std::string &msg);

private:
	static LogLevel level;

};

} // lib
