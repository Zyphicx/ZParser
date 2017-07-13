#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h> //For testing
#include <locale.h>

#include "lexer.h"
#include "../parser/grammar.h"
#include "../utils/strutils.h"

/*int main(int argc, char *argv[]){
	Tokenizer *tokenizer = tokenizer_new(argv[1]);

	Token *token;

	clock_t startTime = clock();

	int count = 0;
	while((token = tokenizer_nexttoken(tokenizer)) != NULL){ //End of file
		//printf("%.2d - %s\n", token->type, token->value);
		free(token->value);
		free(token);
		++count;
	}

	printf("Read %d tokens!\n", count);

	tokenizer_destroy(tokenizer);

	setupGrammar();

	Node *node = parseFile(argv[1]);

	printf("%d\n", node->type);

	deleteGrammar();
	printf("Took %f seconds\n", ((double)clock() - startTime) / CLOCKS_PER_SEC);

	return 0;
}*/

int token_is_word(char *s);
int token_is_number(char *s);
int token_is_string(char *s);
TokenType token_get_wordtype(char *s);


Tokenizer *tokenizer_new(char *filepath){
	Tokenizer *tokenizer = malloc(sizeof(Tokenizer));

	tokenizer->filepath = malloc(sizeof(filepath));
	strcpy(tokenizer->filepath, filepath); //Copies filepath

	tokenizer->file = fopen(filepath, "r");

	tokenizer->buffer[0] == '\0';
	tokenizer->cur = tokenizer->buffer;
	tokenizer->line = 0;

	return tokenizer;
}

void tokenizer_setfile(Tokenizer *self, char *filepath){
	free(self->filepath);
	self->filepath = malloc(sizeof(filepath));
	strcpy(self->filepath, filepath);

	fclose(self->file);
	self->file = fopen(filepath, "r");

	self->line = 0;
}

void tokenizer_destroy(Tokenizer *self){
	free(self->filepath);
	fclose(self->file);
	free(self);
}

void tokenizer_nextline(Tokenizer *self){
	if(fgets(self->buffer, MAX_SOURCE_LINE + 1, self->file) != NULL){
		self->cur = self->buffer;

	}else{
		self->cur = NULL;
	}

	++(self->line);
}

Token *tokenizer_nexttoken(Tokenizer *self){
	Token *token = malloc(sizeof(Token));

	begin:

	if(self->cur == NULL){
		free(token);
		return NULL;
	}

	while(isspace(*(self->cur))){
		++(self->cur);
	}



	if(*(self->cur) == '\n' || *(self->cur) == '\r' || !*(self->cur) || (*(self->cur) == '/' && *(self->cur + 1) == '/')){
		tokenizer_nextline(self);

		if(self->cur == NULL){
			free(token);
			return NULL;
		}

		goto begin;
	}

	int toklength = 0;
	if(toklength = token_is_word(self->cur)) {
		token->value = malloc(toklength + 1); //+1 for NULL character
		readChars(token->value, self->cur, toklength);
		token->type = token_get_wordtype(token->value);
	}
	else if(toklength = token_is_number(self->cur)) {
		token->value = malloc(toklength + 1); //+1 for NULL character
		token->type = NUMBER;
		readChars(token->value, self->cur, toklength);
	}
	else if(toklength = token_is_string(self->cur)){
		token->value = malloc(toklength - 1); //+1 for NULL character and -2 for quotation marks
		token->type = STRING_LITERAL;
		readChars(token->value, self->cur + 1, toklength - 2); //Skips quotation marks
	}
	else if(toklength = token_is_symbol(self->cur, &token->type)){
		token->value = malloc(toklength + 1); //+1 for NULL character
		readChars(token->value, self->cur, toklength);
	}
	else{
		toklength = 1;

		token->type = UNDEFINED;
		token->value = malloc(toklength + 1);
		readChars(token->value, self->cur, toklength);

		fprintf(stderr, "Did not recognise token %s at %d:%d in file %s\n",
		token->value, self->line, TOKEN_GET_COLUMN(self), self->filepath);
	}

	self->cur += toklength;
	self->curtok = token;

	return token;
}

int token_is_word(char *s){
	int count = 1;
	if(!IS_WORD_BEGIN(*s))
		return 0;
	++s;
	while(IS_WORD_END(*s)){
		++count;
		++s;
	}
	return count;
}

int token_is_number(char *s){
	int count = 0;
	if(*s == '-' && isdigit(*(s+1))){
		++count;
		++s;
	}
	while(isdigit(*s)){
		++count;
		++s;
	}
	if(*s == '.' && isdigit(*(s+1))){
		++count;
		++s;
		while(isdigit(*s)){
			++count;
			++s;
		}
	}
	if(*s == 'E'){
		++count;
		++s;

		if(*s == '-'){
			++count;
			++s;
		}
		while(isdigit(*s)){
			++count;
			++s;
		}
	}
	return count;
}

int token_is_string(char *s){
	int count = 2;
	if(*s++ != '"')
		return 0;
	while(*s++ != '"')
		++count;
	return count;
}

TokenType token_get_wordtype(char *s){
	int low = 0;
	int max = keywordAmount - 1;
	int half = max / 2;

	while(low <= max){
		int cmp = strcmp((keywords[half]).key, s);

		if(cmp > 0)
			max = half - 1;
		else if(cmp < 0)
			low = half + 1;
		else
			return keywords[half].type;

		half = (low + max) / 2;
	}

	return IDENTIFIER;
}
