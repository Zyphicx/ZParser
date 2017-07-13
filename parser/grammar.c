#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "grammar.h"
#include "../lexer/lexer.h"
#include "../utils/strutils.h"

void createGrammars(FILE *file);
void deleteGrammar();
void freeGrammarPart(GrammarPart *part);
GrammarPart *createGrammar(char **line);
GrammarPart *getNextPart(char **line);
int getNamePart(char *line);
int getGrammarPart(char *line);
TokenType getTokenType(char *s);

GrammarHead *grammar;
HashTable *grammarTable;

void setupGrammar(char *grammarFile){
	FILE *file = fopen(grammarFile, "r");

	grammarTable = table_new(20);
	//addToTable(file);

	//rewind(file); //Makes file ready for another read

	createGrammars(file);

	fclose(file);
}

void deleteGrammar(){

	GrammarHead *h = grammar;
	while(h != NULL){
		GrammarHead *nextHead = h->next;
		GrammarPart *g = h->grammar;

		freeGrammarPart(h->grammar);

		free(h->name);
		free(h);

		h = nextHead;
	}

	table_free(grammarTable);
}

void freeGrammarPart(GrammarPart *part){
	GrammarPart *p = part;
	while(p != NULL){
		GrammarPart *next = p->next;

		switch(p->type){
			case OPTIONAL:
				freeGrammarPart(((OpGrammarPart *)p)->option);
				break;
			case OR:
				freeGrammarPart(((OrGrammarPart *)p)->op1);
				freeGrammarPart(((OrGrammarPart *)p)->op2);
				break;
			case LOOP:
				freeGrammarPart(((LoopGrammarPart *)p)->loop);
				break;
			case MACRO:
				free(((MacroGrammarPart *)p)->macro);
				break;
		}

		free(p);
		p = next;
	}
}

void createGrammars(FILE *file){
	char line[MAX_GRAMMAR_LINE];
	GrammarHead *grammars = NULL;
	GrammarHead *curHead = NULL;

	while(fgets(line, MAX_GRAMMAR_LINE + 1, file) != NULL){
		if(*line == '\n' || *line == '\r' || *line == '#')
			continue;

		char *line_ptr = line;

		GrammarHead *head = malloc(sizeof(GrammarHead));

		int nameLength = getNamePart(line_ptr);
		head->name = malloc(nameLength + 1);
		readChars(head->name, line_ptr, nameLength);

		head->next = NULL;

		line_ptr += nameLength + 1; //Skips the colon

		if(grammars == NULL){
			grammars = head;
			curHead = head;
		}
		else{
			curHead->next = head;
			curHead = head;
		}

		head->grammar = createGrammar(&line_ptr);

		table_insert(grammarTable, head->name, head);
	}

	grammar = grammars; //Sets global grammar to local grammar
}

GrammarPart *createGrammar(char **line){
	GrammarPart *g = NULL;
	GrammarPart *curPart = NULL;

	while(**line && **line != '\n' && **line != '\r' && **line != ')'){
		GrammarPart *part = getNextPart(line);

		if(g == NULL){
			g = part;
			curPart = part;
		}
		else{
			GrammarPart *p;
			for(p = curPart; p->next != NULL; p = p->next) 	//Walks list to find end
				;											//To then attach part

			p->next = part;
			curPart = part;
		}
	}

	return g;
}

GrammarPart *getNextPart(char **line){
	char token[MAX_GRAMMAR_LINE + 1];
	char *ptr = token;

	GrammarPart *part;
	GrammarPart *tokenPart;

	while(isspace(**line))
		++(*line);

	if(isupper(**line)){ //If character is upper, then it's a token, read token
		while(isupper(**line) || **line == '_'){

			*ptr++ = *(*line)++;

		}
		*ptr = '\0';

		tokenPart = malloc(sizeof(GrammarPart));

		tokenPart->type = getTokenType(token);
		tokenPart->next = NULL;

		//printf("Still running! %d\n", tokenPart->type); PRINTING OVER HEREE

	}else if(islower(**line)){ //If character is lower, then it's a macro
		int chars = 1; //Add 1 for the NULL character at the end
		while(islower(**line) || **line == '_'){
			*ptr++ = *(*line)++;
			++chars;
		}
		*ptr = '\0';

		tokenPart = malloc(sizeof(MacroGrammarPart));

		tokenPart->type = MACRO;
		tokenPart->next = NULL;

		((MacroGrammarPart *)tokenPart)->macro = malloc(chars * sizeof(char));
		strcpy(((MacroGrammarPart *)tokenPart)->macro, token);

	}else if(**line == '('){ //If character is parenthesis, then it's a group, read group
		++(*line); //Skips the opening parenthesis
		tokenPart = createGrammar(line);
		++(*line); //Skips the ending parenthesis
	}else{
		return NULL;
	}

	while(isspace(**line)) //Skips spaces between tokens and operators
		++(*line);

	if(**line == '-'){
		++(*line);

		part = malloc(sizeof(OpGrammarPart));
		part->type = OPTIONAL;
		part->next = NULL;

		((OpGrammarPart *)part)->option = tokenPart;

	}else if(**line == '|'){
		++(*line);

		part = malloc(sizeof(OrGrammarPart));
		part->type = OR;
		part->next = NULL;

		((OrGrammarPart *)part)->op1 = tokenPart;
		((OrGrammarPart *)part)->op2 = getNextPart(line);

	}else if(**line == '*'){
		++(*line);

		part = malloc(sizeof(LoopGrammarPart));
		part->type = LOOP;
		part->next = NULL;

		((LoopGrammarPart *)part)->loop = tokenPart;

	}else{
		part = tokenPart;
	}

	while(isspace(**line))
		++(*line);

	return part;
}

int getNamePart(char *line){
	int length = 0;

	while(*line++ != ':')
		++length;

	return length;
}

int getGrammarPart(char *line){
	int length = 0;

	while(isspace(*line)){
		++length;
		++line;
	}

	while(*line != '\n' && *line != '\r' && *line){
		++length;
		++line;
	}

	return length;
}

TokenType getTokenType(char *s){
	TokenName *kp;
	for(kp = tokenNames; kp < (tokenNames + tokenNameAmount); kp++){
		if(!strcmp(s, kp->key)){
			return kp->type;
		}
	}
	return UNDEFINED;
}