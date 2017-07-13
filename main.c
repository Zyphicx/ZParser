#include "zparser.h"
#include "zparsedefs.h"
#include <time.h>

void printTree(int indent, ParseNode *tree);

int main(int argc, char *argv[]){
	Tokenizer *tokenizer = tokenizer_new(argv[1]);

	Token *token;

	clock_t startTime = clock();

	printf("|");

	int count = 0;
	while((token = tokenizer_nexttoken(tokenizer)) != NULL){ //End of file
		//printf("%.2d - %s\n", token->type, token->value);
		printf("%s - %d|", token->value, token->type);
		free(token->value);
		free(token);
		++count;
	}

	printf("\n");

	printf("Read %d tokens!\n", count);

	tokenizer_destroy(tokenizer);

	setupGrammar("grammarFile");

	ParseNode *node = parseFile(argv[1]);

	printf("%s\n", node->name);

	deleteGrammar();
	printf("Took %f seconds\n", ((double)clock() - startTime) / CLOCKS_PER_SEC);

	printTree(1, node);

	return 0;
}

void printTree(int indent, ParseNode *tree){
	printf("%s:\n", tree->name);

	int i;
	for(i = 0; i < tree->childAmount; i++){
		ParseChild child = tree->children[i];

		int j;
		for(j = 0; j < indent; j++)
			printf("  ");

		if(child.type == CHILD_TOKEN){
			printf("%s\n", child.val.token->value);
		}
		else{
			printTree(indent + 1, child.val.node);
		}
	}	
}

int token_is_symbol(char *s, TokenType *t){
	*t = 0;
	switch(*s){
		case '+': *t = PLUS;		break;
		case '-': *t = MINUS;		break;
		case '*': *t = STAR; 		break;
		case '/': *t = SLASH;		break;
		case '%': *t = PERCENT;		break;
		case '^': *t = CARET;		break;
		case '>': *t = GREATER;		break;
		case '<': *t = LESS;		break;
		case '!': *t = EXCL;		break;
		case '?': *t = QUESTION;	break;
		case '=': *t = EQUALS;		break;
		case '&': *t = AMPER;		break;
		case '|': *t = VBAR;		break;
		case '(': *t = L_PAREN;		break;
		case ')': *t = R_PAREN;		break;
		case '{': *t = L_CURLY;		break;
		case '}': *t = R_CURLY;		break;
		case '[': *t = L_SQUARE;	break;
		case ']': *t = R_SQUARE;	break;
		case ',': *t = COMMA;		break;
		case ':': *t = COLON;		break;
		case '.': *t = DOT;			break;
		case ';': *t = SEMICOLON;	break;
		case '#': *t = HASH;		break;
		default:					return 0;
	}

	switch(*(s+1)){
		case '=':
			switch(*t){
				case PLUS: *t = PLUS_EQUALS;		return 2;
				case MINUS: *t = MINUS_EQUALS;		return 2;
				case STAR: *t = STAR_EQUALS;		return 2;
				case SLASH: *t = SLASH_EQUALS;		return 2;
				case PERCENT: *t = PERCENT_EQUALS;	return 2;
				case CARET: *t = CARET_EQUALS;		return 2;
				case GREATER: *t = GREATER_EQUALS;	return 2;
				case LESS: *t = LESS_EQUALS;		return 2;
				case EXCL: *t = EXCL_EQUALS;		return 2;
				case EQUALS: *t = EQUALS_EQUALS; 	return 2;
				default: 							return 1;
			}
		case '&':
			switch(*t){
				case AMPER: *t = AMPER_AMPER;		return 2;
				default: 							return 1;
			}
		case '|':
			switch(*t){
				case VBAR: *t = VBAR_VBAR;			return 2;
				default: 							return 1;
			}
		case '+':
			switch(*t){
				case PLUS: *t = PLUS_PLUS;			return 2;
				default:							return 1;
			}
		case '-':
			switch(*t){
				case MINUS: *t = MINUS_MINUS;		return 2;
				default:							return 1;
			}
		case '>':
			switch(*t){
				case MINUS:	*t = MINUS_GREATER;		return 2;
				default:							return 1;
			}
	}

	return *t ? 1 : 0;

}