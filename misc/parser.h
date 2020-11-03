#ifndef PARSER_H__
#define PARSER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "lex.h"

typedef enum ASTType_prototype
{

    UNARYAST,
    BINARYAST,
    VALUEAST,

} ASTType;

typedef struct AST_prototype
{

    struct AST_prototype *Right;
    Token         *Token; // The token representing the node. Can be null.
    struct AST_prototype *Left;

} ParseTree;

/* Parses the Token input into an AST. */
int Parse(Token *TokenBuffer, ParseTree *Tree);

#ifdef __cplusplus
}
#endif
#endif