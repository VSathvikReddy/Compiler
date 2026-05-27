#include "lexer.hpp"
#include "parser.hpp"
#include "token_code_gen.hpp"
#include "lexer_code_gen.hpp"

#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }



    Lexer lexer(argv[1]);
    lexer.print_tokens();
    Parser parser(lexer.getTokens());
    parser.debug_print();

    TokenGenerator gen(parser.getlLexicalNodes(),parser.grtParserNodes());
    gen.output("token");

    LexerGenerator lgen(gen);
    lgen.output("lexer");


    return 0;
}