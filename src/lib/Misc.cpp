#include "Misc.hpp"

#include <sstream>
#include <iomanip>

namespace OrichalcumLib::Misc {

std::string to_hex(size_t number, int width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << number;
	return stream.str();
}

} // OrichalcumLib
