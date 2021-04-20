#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>

#include "Token.hpp"

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width = 5);

} // LibOrichalcum
