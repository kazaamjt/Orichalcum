#pragma once
#include <string>

#include "Chunk.hpp"

namespace OrichalcumLib::Debug {

void disassemble_chunk(Chunk &chunk);
size_t disassemble_instruction(size_t index, Chunk &chunk);

std::string op_to_string(OP_CODE op_code);

} // OrichalcumLib
