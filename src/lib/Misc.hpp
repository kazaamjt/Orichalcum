#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>

#include "Token.hpp"

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width = 5);
int count_whitespace(const std::string &string);
std::string remove_white_space(const std::string &string);

} // LibOrichalcum
