#include "Misc.hpp"

#include <sstream>
#include <iomanip>

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << number;
	return stream.str();
}

CompileError::CompileError():
std::runtime_error::runtime_error ("This error was not initialized properly") { }

CompileError::CompileError(
	const std::string &_msg, const std::filesystem::path &_file, COMPILER_SECTION _section):
std::runtime_error::runtime_error(_msg), msg(_msg), file(_file), section(_section) {

}

} // LibOrichalcum
