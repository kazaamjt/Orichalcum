#include "Misc.hpp"

#include <sstream>
#include <iomanip>

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << number;
	return stream.str();
}

Error::Error(
	const COMPILER_SECTION _section,
	const std::string &_msg,
	Token token
	):
	std::runtime_error(_msg),
	section(_section), msg(_msg),
	index(token.index),
	file(token.file_path),
	line_content(token.line_content) { }

Error::Error(
	const COMPILER_SECTION _section,
	const std::string &_msg,
	const Index _index,
	const std::string &_file,
	const std::string &_line_content):
	std::runtime_error(_msg), section(_section), msg(_msg), index(_index), file(_file), line_content(_line_content) { }

Error::Error():
std::runtime_error(""), section(COMPILER_SECTION::PARSER), index(0, 0) { }

} // LibOrichalcum
