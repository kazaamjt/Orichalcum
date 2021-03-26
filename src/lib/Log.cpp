#include "Log.hpp"

#include <rang.hpp>

namespace OrichalcumLib {

LogLevel Log::level = LogLevel::INFO;

void Log::set_level(LogLevel _level) {
	level = _level;
}

void Log::debug(const std::string &msg) {
	if (level >= LogLevel::DEBUG) {
		std::cout << rang::fg::blue << "DEBUG: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::verbose(const std::string &msg) {
	if (level >= LogLevel::VERBOSE) {
		std::cout << rang::fg::cyan << "VERBOSE: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::info(const std::string &msg) {
	if (level >= LogLevel::INFO) {
		std::cout << rang::fg::green << "INFO: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::warning(const std::string &msg) {
	if (level >= LogLevel::WARNING) {
		std::cerr << rang::fg::yellow << "WARNING: " << rang::fg::reset << msg << std::endl;
	}
}

void Log::error(const std::string &msg) {
	std::cerr << rang::fg::red << "ERROR: " << rang::fg::reset << msg << std::endl;
}

} // lib
