#include "Log.hpp"

#include <rang.hpp>

namespace LibOrichalcum {

LogLevel Log::level = LogLevel::DEBUG;
bool Log::print_output = false;
std::vector<LogLine> Log::buffer;

LogLine::LogLine(const LogLevel _level, const std::string &_msg):
level(_level), msg(_msg) { }

LogLevel Log::get_level() {
	return level;
}

void Log::set_level(LogLevel _level, bool _print_output) {
	level = _level;
	print_output = _print_output;
	if (print_output) {
		for (LogLine line: buffer) {
			switch (line.level) {
				case LogLevel::DEBUG: debug(line.msg); break;
				case LogLevel::VERBOSE: verbose(line.msg); break;
				case LogLevel::INFO: info(line.msg); break;
				case LogLevel::WARNING: warning(line.msg); break;
				case LogLevel::ERROR_: error(line.msg); break;
			}
		}
	}
}

void Log::debug(const std::string &msg) {
	if (level <= LogLevel::DEBUG) {
		if (print_output) {
			std::cout << rang::fg::blue << "DEBUG: " << rang::fg::reset << msg << std::endl;
		} else {
			buffer.push_back(LogLine(LogLevel::DEBUG, msg));
		}
	}
}

void Log::verbose(const std::string &msg) {
	if (level <= LogLevel::VERBOSE) {
		if (print_output) {
			std::cout << rang::fg::cyan << "VERBOSE: " << rang::fg::reset << msg << std::endl;
		} else {
			buffer.push_back(LogLine(LogLevel::DEBUG, msg));
		}
	}
}

void Log::info(const std::string &msg) {
	if (level <= LogLevel::INFO) {
		if (print_output) {
			std::cout << rang::fg::green << "INFO: " << rang::fg::reset << msg << std::endl;
		} else {
			buffer.push_back(LogLine(LogLevel::DEBUG, msg));
		}
	}
}

void Log::warning(const std::string &msg) {
	if (level <= LogLevel::WARNING) {
		if (print_output) {
			std::cerr << rang::fg::yellow << "WARNING: " << rang::fg::reset << msg << std::endl;
		} else {
			buffer.push_back(LogLine(LogLevel::DEBUG, msg));
		}
	}
}

void Log::error(const std::string &msg) {
	if (print_output) {
		std::cerr << rang::fg::red << "ERROR: " << rang::fg::reset << msg << std::endl;
	} else {
		buffer.push_back(LogLine(LogLevel::DEBUG, msg));
	}
}

bool Log::print_logs() {
	return print_output;
}

std::vector<LogLine> Log::get_logs() {
	return buffer;
}

std::string to_string(LogLevel level) {
	switch (level) {
		case LogLevel::DEBUG: return "DEBUG";
		case LogLevel::VERBOSE: return "VERBOSE";
		case LogLevel::INFO: return "INFO";
		case LogLevel::WARNING: return "WARNING";
		case LogLevel::ERROR_: return "ERROR";
	}
}

} // lib
