#pragma once
#include <string>
#include <vector>

#include "Misc.hpp"

namespace LibOrichalcum {

enum class LOG_LEVEL {
	DEBUG,
	VERBOSE,
	INFO,
	WARNING,
	ERROR_,
};
std::string to_string(LOG_LEVEL level);

struct LogLine {
	LogLine(const LOG_LEVEL level, const std::string &msg);
	const LOG_LEVEL level;
	const std::string msg;
};

class Log {
public:
	static LOG_LEVEL get_level();
	static void set_level(LOG_LEVEL level, bool _print_output = false);

	static void debug(const std::string &msg);
	static void verbose(const std::string &msg);
	static void info(const std::string &msg);
	static void warning(const std::string &msg);
	static void error(const std::string &msg);
	static void error(const Misc::Error &error);

	static bool print_logs_enabled();
	static std::vector<LogLine> get_logs();

private:
	static LOG_LEVEL level;
	static bool store;
	static bool print_output;
	static std::vector<LogLine> buffer;
};

} // lib
