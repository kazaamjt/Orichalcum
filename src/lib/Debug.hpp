#pragma once
#include <string>

#include "Chunk.hpp"
#include "data_structures.hpp"

namespace OrichalcumLib::Debug {

void disassemble_chunk(Chunk &chunk);
size_t disassemble_instruction(size_t index, Chunk &chunk);

void print_const(const Constant &constant);
template <typename T>
void print_stack(Stack<T> stack);

std::string op_to_string(OP_CODE op_code);

} // OrichalcumLib
