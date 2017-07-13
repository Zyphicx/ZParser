#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <unistd.h>

//Values 0-8 are user-defined, the rest are user-defined

#define UNDEFINED		0
#define OPTIONAL 		1
#define OR 				2
#define LOOP 			3
#define MACRO 			4
#define NUMBER			5
#define IDENTIFIER		6
#define STRING_LITERAL	7
#define SYMBOL			8

typedef short TokenType;

typedef struct{
	const char *key;
	const TokenType type;
} TokenName;

typedef struct token{
	TokenType type;
	char *value;
} Token;

#endif