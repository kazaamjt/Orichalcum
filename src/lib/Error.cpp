#include "Error.hpp"

#include <sstream>
#include <iomanip>

namespace LibOrichalcum {

Error::Error(
	const COMPILE_RESULT _section,
	const std::string &_msg,
	Token token):
std::runtime_error(_msg),
section(_section), msg(_msg),
index(token.index),
file(token.file_path),
line_content(token.line_content) { }

Error::Error(
	const COMPILE_RESULT _section,
	const std::string &_msg,
	const Index _index,
	const std::string &_file,
	const std::string &_line_content):
std::runtime_error(_msg),
section(_section),
msg(_msg),
index(_index),
file(_file),
line_content(_line_content) { }

Error::Error():
std::runtime_error(""), section(COMPILE_RESULT::SUCCESS), index(0, 0) { }

} // LibOrichalcum
