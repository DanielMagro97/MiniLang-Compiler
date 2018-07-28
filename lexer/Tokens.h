#pragma once

#include <iostream>

enum TokenIdentifier {
    TOK_RelationalOp,       // S01, S04
    TOK_Equals,             // S03
    TOK_AdditiveOp,         // S05
    TOK_MultiplicativeOp,   // S06, S07
    TOK_Comment,            // S09, S12
    TOK_StringLiteral,      // S14
    TOK_Identifier,         // S15
    TOK_Keyword,            // S15
    TOK_BooleanLiteral,     // S15
    TOK_IntegerLiteral,     // S16
    TOK_RealLiteral,        // S18
    TOK_Punctuation,        // S19
    TOK_EOF,                // S20
    TOK_ERROR
};

struct Token {
    TokenIdentifier tokenIdentifier;
    std::string lexeme;
};
