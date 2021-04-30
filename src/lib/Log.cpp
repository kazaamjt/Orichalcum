#include "Log.hpp"

#include <algorithm>

#include <rang.hpp>

#include "Misc.hpp"

namespace LibOrichalcum {

LOG_LEVEL Log::level = LOG_LEVEL::DEBUG;
bool Log::print_output = false;
bool Log::store = true;
std::vector<LogLine> Log::buffer;

LogLine::LogLine(const LOG_LEVEL _level, const std::string &_msg):
level(_level), msg(_msg) { }

LOG_LEVEL Log::get_level() {
	return level;
}

void Log::set_level(LOG_LEVEL _level, bool _print_output) {
	level = _level;
	print_output = _print_output;
	if (print_output) {
		store = false;
		for (LogLine line: buffer) {
			if (level <= line.level) {
				switch (line.level) {
					case LOG_LEVEL::DEBUG: debug(line.msg); break;
					case LOG_LEVEL::VERBOSE: verbose(line.msg); break;
					case LOG_LEVEL::INFO: info(line.msg); break;
					case LOG_LEVEL::WARNING: warning(line.msg); break;
					case LOG_LEVEL::ERROR_: error(line.msg); break;
				}
			}
		}
		store = true;
	}
}

void Log::debug(const std::string &msg) {
	if (level <= LOG_LEVEL::DEBUG) {
		if (store) {
			buffer.push_back(LogLine(LOG_LEVEL::DEBUG, msg));
		}
		if (print_output) {
			std::cout << rang::fg::blue << "DEBUG: " << rang::fg::reset << msg << std::endl;
		}
	}
}

void Log::verbose(const std::string &msg) {
	if (level <= LOG_LEVEL::VERBOSE) {
		if (store) {
			buffer.push_back(LogLine(LOG_LEVEL::DEBUG, msg));
		}
		if (print_output) {
			std::cout << rang::fg::cyan << "VERBOSE: " << rang::fg::reset << msg << std::endl;
		}
	}
}

void Log::info(const std::string &msg) {
	if (level <= LOG_LEVEL::INFO) {
		buffer.push_back(LogLine(LOG_LEVEL::DEBUG, msg));
		if (print_output) {
			std::cout << rang::fg::green << "INFO: " << rang::fg::reset << msg << std::endl;
		}
	}
}

void Log::warning(const std::string &msg) {
	if (level <= LOG_LEVEL::WARNING) {
		if (store) {
			buffer.push_back(LogLine(LOG_LEVEL::DEBUG, msg));
		}
		if (print_output) {
			std::cerr << rang::fg::yellow << "WARNING: " << rang::fg::reset << msg << std::endl;
		}
	}
}

void Log::error(const std::string &msg) {
	if (store) {
		buffer.push_back(LogLine(LOG_LEVEL::ERROR_, msg));
	}
	if (print_output) {
		std::cerr << rang::fg::red << "ERROR: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::error(const Error &error) {
	std::string line = Misc::remove_white_space(error.line_content);
	int count = Misc::count_whitespace(error.line_content);
	std::string output = error.msg + "\n"
			"    File " + error.file + "(" + std::to_string(error.index.line + 1)
			+ "," + std::to_string(error.index.col + 1) + "):\n"
			+ std::string(8, ' ') + line + "\n"
			+ std::string(static_cast<size_t>(error.index.col + 8 - count), ' ') + "^";
	Log::error(output);
}

bool Log::print_logs_enabled() {
	return print_output;
}

std::vector<LogLine> Log::get_logs() {
	return buffer;
}

std::string to_string(LOG_LEVEL level) {
	switch (level) {
		case LOG_LEVEL::DEBUG: return "DEBUG";
		case LOG_LEVEL::VERBOSE: return "VERBOSE";
		case LOG_LEVEL::INFO: return "INFO";
		case LOG_LEVEL::WARNING: return "WARNING";
		case LOG_LEVEL::ERROR_: return "ERROR";
	}
}

} // lib
