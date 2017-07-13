#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "token.h"

#define TOKEN_GET_COLUMN(self) (self->cur - self->buffer + 1)

#define IS_WORD_BEGIN(c)    (c == '_' || (c >= 'a' && c <= 'z') ||\
                            (c >= 'A' && c <= 'Z'))
#define IS_WORD_END(c)      (IS_WORD_BEGIN(c) || (c >= '0' && c <= '9'))


#define MAX_SOURCE_LINE 4096

/*Defined by user*/

extern TokenName keywords[];
extern const int keywordAmount;

int token_is_symbol(char *s, TokenType *t);

/*Public functions and structs*/

typedef struct tokenizer{
    char *filepath;
    FILE *file;

    char buffer[MAX_SOURCE_LINE];
    char *cur;
    size_t line;

    Token *curtok;
} Tokenizer;

Tokenizer *tokenizer_new(char *filepath);
void tokenizer_setfile(Tokenizer *self, char *filepath);
void tokenizer_destroy(Tokenizer *self);
Token *tokenizer_nexttoken(Tokenizer *self);

#endif 