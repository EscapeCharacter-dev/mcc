#ifndef LEX_H__
#define LEX_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum TokenType_prototype {
    NULL_OR_BADTOK,
    LNUMTOK,
    LSTRTOK,
    IDENTTOK,
    KEYWORDTOK,
    PLUSPLUSTOK,
    MINUSMINUSTOK,
    NOTTOK,

    STARTOK,
    SLASHTOK,
    PERCENTAGETOK,
    PLUSTOK,
    MINUSTOK,


    EQUEQUTOK,
    NOTEQUTOK,
    AMPERSANDTOK,
    PIPETOK,

    AMPERSANDSTOK,
    PIPESTOK,


    INCTOK,
    DECTOK,
    INCMULTOK,
    DECDIVTOK,
    DECPERTOK,
    EQUALTOK,
    AMPERSANDEQUTOK,
    PIPEEQUTOK,

    EOLTOK,
} TokenType;

typedef struct Token_prototype {
    char       *TokenString;
    int   	    TokenLength;
    int   	    TokenPosition;
    void       *TokenValue;
    int   	    ValueSize;
    TokenType 	TokenType;
} Token;

/* Tokenizes a whole file. */
int Tokenize(char *const FilePath, Token *TokenBuffer);

/* Creates a new instance of a token. Allocates on heap. */
Token *NewToken(char *String, int Length, int Position, 
                void *Value, int ValueSize, TokenType Type);

#ifdef __cplusplus
}
#endif
#endif
