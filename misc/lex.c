#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lex.h"

int IsKeyword(char *Ident)
{
    if (!Ident)
        return 1;
    if (!strcmp(Ident, "auto") || !strcmp(Ident, "break") || !strcmp(Ident, "case") || !strcmp(Ident, "char") ||
        !strcmp(Ident, "const") || !strcmp(Ident, "continue") || !strcmp(Ident, "default") || !strcmp(Ident, "do") ||
        !strcmp(Ident, "double") || !strcmp(Ident, "else") || !strcmp(Ident, "enum") || !strcmp(Ident, "extern") ||
        !strcmp(Ident, "float") || !strcmp(Ident, "for") || !strcmp(Ident, "goto") || 
        !strcmp(Ident, "if") || !strcmp(Ident, "int") ||
        !strcmp(Ident, "long") || !strcmp(Ident, "register") || !strcmp(Ident, "return") || !strcmp(Ident, "short") ||
        !strcmp(Ident, "signed") || !strcmp(Ident, "sizeof") || !strcmp(Ident, "static") || !strcmp(Ident, "struct") ||
        !strcmp(Ident, "switch") || !strcmp(Ident, "typedef") || !strcmp(Ident, "union") || !strcmp(Ident, "unsigned") ||
        !strcmp(Ident, "void")|| !strcmp(Ident, "volatile") || !strcmp(Ident, "while"))
        return 1;
    return 0;
}

void RemoveNulls(char *String)
{
    int size = sizeof(String);
    int excess = 0;

    for (int i = 0; i < size - 1; i++)
    {
        if (!String[i])
            excess++;
    }

    String = realloc(String, size - excess);
}

int IsValidLetter(char Ident)
{
    if (isalnum(Ident) || Ident == '_' || Ident == '$')
        return 1;
    return 0;
}

int Tokenize(char *const FilePath, Token *TokenBuffer)
{
    if (!FilePath)
        return 0;
    if (!TokenBuffer)
    {
        TokenBuffer = (Token*) calloc(512 * 1024, sizeof(Token));
        if (!TokenBuffer) return 0;
    }
    
    //opening the file
    
    FILE *file = fopen(FilePath, "r");
    if (!file) return 0;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char c = '\0';
    int tokenpos = 0;
    char *copy = (char*) calloc(1, 1);
    int charpos = 0;
    if (!copy) return 0;
    while ((c = fgetc(file)) != EOF)
    {
        //tokenizing char by char

        __label__ skipDefinit;
        __label__ eof; // skipping for weird vscode error

        if (charpos == size)
            break;

        Token t = {0};
        
        // is space?
        if (isspace(c)) continue;
        
        // is digit?
        if (isdigit(c))
        {
            char *string = (char*) calloc(1, 21); // 21 is enough for 64-bit computing. 128-bit is not yet here.
            
            if (!string) return 0;
            
            int digit_index = 0;
            
            while (isdigit(c))
            {
                string[digit_index++] = c;
                c = fgetc(file);
            }

            string = (char*) realloc(string, digit_index + 1);
            unsigned long long *lval;
            if (!(lval = (unsigned long long*)
                calloc(1, 8)))
                return 0;

            *lval = atoll(string);

            t.TokenString   = string;
            t.TokenLength   = digit_index;
            t.TokenPosition = tokenpos;
            t.TokenType     = LNUMTOK;
            t.TokenValue    = lval;
            t.ValueSize     = 8;
            goto skipDefInit;
        }

        // is ident?
        if (IsValidLetter(c))
        {
            char *string = (char*) calloc(32, 1); /* 32 is enough. The program is the problem
                                                  if it uses bigger idents.*/

            if (!string) return 0;

            int index = 0;

            while (IsValidLetter(c))
            {
                string[index++] = c;
                c = fgetc(file);
            }
            char *oldstring = string;

            string = realloc(string, index + 1);

            t.TokenString   = string;
            t.TokenLength   = index;
            t.TokenPosition = tokenpos;

            /*printf("[%s]\n", string);*/

            if (IsKeyword(string))  /* the only difference is if the token 
                                       is a keyword, we change the token type.*/
                t.TokenType     = KEYWORDTOK;
            else
                t.TokenType     = IDENTTOK;
            

            t.TokenValue    = t.TokenString;
            t.ValueSize     = t.TokenLength;
            
            goto skipDefInit;
        }

        char next = fgetc(file);
        fseek(file, -1, SEEK_CUR);

        switch (c)
        {
        case '+':
        {
            if (next == '+')
            {
                t.TokenString   = "++";
                t.TokenLength   = 3;
                t.TokenType     = PLUSPLUSTOK;
            }
            else if (next == '=')
            {
                t.TokenString   = "+=";
                t.TokenLength   = 3;
                t.TokenType     = INCTOK;
            }
            else
            {
                t.TokenString   = "+";
                t.TokenLength   = 2;
                t.TokenType     = PLUSTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue = 0;
            t.ValueSize  = 0;
            goto skipDefInit;
        }
        case '-':
        {
            if (next == '-')
            {
                t.TokenString   = "--";
                t.TokenLength   = 3;
                t.TokenType     = MINUSMINUSTOK;
            }
            else if (next == '=')
            {
                t.TokenString   = "-=";
                t.TokenLength   = 3;
                t.TokenType     = DECTOK;
            }
            else
            {
                t.TokenString   = "-";
                t.TokenLength   = 2;
                t.TokenType     = MINUSTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue = 0;
            t.ValueSize  = 0;
            goto skipDefInit;
        }
        case '*':
        {
            if (next == '=')
            {
                t.TokenString   = "*=";
                t.TokenLength   = 3;
                t.TokenType     = INCMULTOK;
            }
            else
            {
                t.TokenString   = "*";
                t.TokenLength   = 2;
                t.TokenType     = STARTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '/':
        {
            if (next == '=')
            {
                t.TokenString   = "/=";
                t.TokenLength   = 3;
                t.TokenType     = DECDIVTOK;
            }
            else
            {
                t.TokenString   = "/";
                t.TokenLength   = 2;
                t.TokenType     = SLASHTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '%':
        {
            if (next == '=')
            {
                t.TokenString   = "%=";
                t.TokenLength   = 3;
                t.TokenType     = DECPERTOK;
            }
            else
            {
                t.TokenString   = "%";
                t.TokenLength   = 2;
                t.TokenType     = PERCENTAGETOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '=':
        {
            if (next == '=')
            {
                t.TokenString   = "==";
                t.TokenLength   = 3;
                t.TokenType     = EQUEQUTOK;
            }
            else
            {
                t.TokenString   = "=";
                t.TokenLength   = 2;
                t.TokenType     = EQUALTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '!':
        {
            if (next == '=')
            {
                t.TokenString   = "!=";
                t.TokenLength   = 3;
                t.TokenType     = NOTEQUTOK;
            }
            else
            {
                t.TokenString   = "*";
                t.TokenLength   = 2;
                t.TokenType     = NOTTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '&':
        {
            if (next == '&')
            {
                t.TokenString   = "&&";
                t.TokenLength   = 3;
                t.TokenType     = AMPERSANDSTOK;
            }
            else if (next == '=')
            {
                t.TokenString   = "&=";
                t.TokenLength   = 3;
                t.TokenType     = AMPERSANDEQUTOK;
            }
            else
            {
                t.TokenString   = "&";
                t.TokenLength   = 2;
                t.TokenType     = AMPERSANDTOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case '|':
        {
            if (next == '|')
            {
                t.TokenString   = "||";
                t.TokenLength   = 3;
                t.TokenType     = PIPESTOK;
            }
            else if (next == '=')
            {
                t.TokenString   = "|=";
                t.TokenLength   = 3;
                t.TokenType     = PIPEEQUTOK;
            }
            else
            {
                t.TokenString   = "|";
                t.TokenLength   = 2;
                t.TokenType     = PIPETOK;
            }
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        case ';':
        {
            t.TokenString   = ";";
            t.TokenLength   = 2;
            t.TokenType     = EOLTOK;
            t.TokenPosition = tokenpos;
            t.TokenValue    = 0;
            t.ValueSize     = 0;
            goto skipDefInit;
        }
        }

        memcpy(copy, &c, 1);
        t.TokenString   = copy;
        t.TokenLength   = 1;
        t.TokenPosition = tokenpos;
        t.TokenType     = NULL_OR_BADTOK;
        t.TokenValue    = 0;
        t.ValueSize     = 0; // if the size of the value of a token is zero, watch out.

skipDefInit:

        /*printf("[DBG]: lex-tok:\"%s\",\ttype:%i\n", t.TokenString, t.TokenType);*/
        TokenBuffer[tokenpos++] = t;

        charpos++;

        continue;
//in case we got an EOF
eof:
        break;
    }

    TokenBuffer = (Token*) realloc(TokenBuffer, sizeof(Token) * (tokenpos + 1));
    
    return 1;
}

#include "parser.h"

int main(int argc, char **argv)
{
    Token *tokenbuf = (Token*) calloc(512 * 1024, sizeof(Token));
    Tokenize(argv[1], tokenbuf);
}