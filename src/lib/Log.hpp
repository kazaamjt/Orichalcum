#pragma once
#include <string>

namespace LibOrichalcum {

enum class LogLevel {
	DEBUG,
	VERBOSE,
	INFO,
	WARNING,
	ERROR_,
};
std::string to_string(LogLevel level);

class Log {
public:
	static LogLevel get_level();
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
