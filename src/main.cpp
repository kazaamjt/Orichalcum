#include "lib/Compiler.hpp"
#include "lib/Lexer.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
	OrichalcumLib::Compiler compiler("test/test.or");

	OrichalcumLib::Token token = compiler.lexer->get_next_token();
	while (token.type != OrichalcumLib::TokenType::EOF_TOKEN) {
		std::cout << "[" << token.index.line << "," << token.index.col << "] ";
		std::cout << token.content << std::endl;
		token = compiler.lexer->get_next_token();
	}

	return 0;
}
