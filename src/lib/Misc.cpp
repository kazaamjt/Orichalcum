#include "Misc.hpp"

#include <sstream>
#include <iomanip>

namespace LibOrichalcum::Misc {

Index::Index(int _line, int _col):
	line(_line), col(_col) { }

void Index::set(int _line, int _col) {
	line = _line;
	col = _col;
}

std::string to_hex(size_t number, int width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << number;
	return stream.str();
}

Error::Error(const COMPILER_SECTION _section, const std::string &_msg, const Index _index, const std::string &_file):
std::runtime_error(_msg), section(_section), msg(_msg), index(_index), file(_file) {
	
}

Error::Error():
std::runtime_error(""), section(COMPILER_SECTION::PARSER), index(0, 0) { }

} // LibOrichalcum
