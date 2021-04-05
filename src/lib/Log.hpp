#pragma once
#include <string>
#include <vector>

namespace LibOrichalcum {

enum class LogLevel {
	DEBUG,
	VERBOSE,
	INFO,
	WARNING,
	ERROR_,
};
std::string to_string(LogLevel level);

struct LogLine {
	LogLine(const LogLevel level, const std::string &msg);
	const LogLevel level;
	const std::string msg;
};

class Log {
public:
	static LogLevel get_level();
	static void set_level(LogLevel level, bool _print_output = false);

	static void debug(const std::string &msg);
	static void verbose(const std::string &msg);
	static void info(const std::string &msg);
	static void warning(const std::string &msg);
	static void error(const std::string &msg);

	static bool print_logs();

private:
	static LogLevel level;
	static bool print_output;
	static std::vector<LogLine> buffer;
};

} // lib
