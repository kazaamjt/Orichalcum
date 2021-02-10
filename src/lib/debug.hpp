#pragma once
#include <string>

#include "chunk.hpp"

namespace OrichalcumLib {

void dissassemble_chunk(const Chunk &chunk);
std::string opcode_to_string(OpCode op_code);

} // namespace Orichalcum


