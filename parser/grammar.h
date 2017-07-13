#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../lexer/token.h"
#include "../utils/hashtable.h"
#include "parser.h"

#define MAX_GRAMMAR_LINE 4096

typedef struct grammar_head GrammarHead;
typedef struct grammar_part GrammarPart;

typedef struct or_grammar_part OrGrammarPart;
typedef struct option_grammar_part OpGrammarPart;
typedef struct loop_grammar_part LoopGrammarPart;
typedef struct macro_grammar_part MacroGrammarPart;

struct grammar_part{
	TokenType type;
	struct grammar_part *next;
};

struct or_grammar_part{
	struct grammar_part super;

	struct grammar_part *op1;
	struct grammar_part *op2;
};

struct option_grammar_part{
	struct grammar_part super;

	struct grammar_part *option;
};

struct loop_grammar_part{
	struct grammar_part super;

	struct grammar_part *loop;
};

struct macro_grammar_part{
	struct grammar_part super;

	char *macro;
};

struct grammar_head{
	char *name;
	struct grammar_head *next;
	struct grammar_part *grammar;
};

void setupGrammar(char *grammarFile);
void deleteGrammar();

extern HashTable *grammarTable;
extern GrammarHead *grammar;

extern TokenName tokenNames[];
extern const int tokenNameAmount;

#endif