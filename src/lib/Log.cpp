#include "Log.hpp"

#include <rang.hpp>

namespace LibOrichalcum {

LogLevel Log::level = LogLevel::INFO;

LogLevel Log::get_level() {
	return level;
}

void Log::set_level(LogLevel _level) {
	level = _level;
}

void Log::debug(const std::string &msg) {
	if (level <= LogLevel::DEBUG) {
		std::cout << rang::fg::blue << "DEBUG: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::verbose(const std::string &msg) {
	if (level <= LogLevel::VERBOSE) {
		std::cout << rang::fg::cyan << "VERBOSE: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::info(const std::string &msg) {
	if (level <= LogLevel::INFO) {
		std::cout << rang::fg::green << "INFO: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::warning(const std::string &msg) {
	if (level <= LogLevel::WARNING) {
		std::cerr << rang::fg::yellow << "WARNING: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::error(const std::string &msg) {
	std::cerr << rang::fg::red << "ERROR: " << rang::fg::reset << msg << std::endl;
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
