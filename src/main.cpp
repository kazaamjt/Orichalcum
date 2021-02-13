#include "lib/Compiler.hpp"
#include "lib/Lexer.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
	OrichalcumLib::Compiler compiler;
	compiler.compile("test/test.or");

	OrichalcumLib::Token token = compiler.lexers[0].get_next_token();
	while (token.type != OrichalcumLib::TokenType::EOF_TOKEN) {
		std::cout << token.content << std::endl;
		token = compiler.lexers[0].get_next_token();
	}

	return 0;
}
