#ifndef ZPARSEDEFS_H
#define ZPARSEDEFS_H

#define CONST 			9
#define DOUBLE 			10
#define INT 			11
#define RETURN 			12
#define PLUS 			13
#define MINUS 			14
#define STAR 			15
#define SLASH 			16
#define PERCENT 		17
#define CARET 			18
#define GREATER 		19
#define LESS 			20
#define EXCL 			21
#define QUESTION		22
#define EQUALS 			23
#define AMPER 			24
#define VBAR 			25
#define L_PAREN 		26
#define R_PAREN 		27
#define L_CURLY 		28
#define R_CURLY 		29
#define L_SQUARE		30
#define R_SQUARE		31
#define COMMA 			32
#define COLON 			33
#define DOT				34
#define SEMICOLON		35
#define HASH			36
#define PLUS_EQUALS 	37
#define MINUS_EQUALS 	38
#define STAR_EQUALS 	39
#define SLASH_EQUALS 	40
#define PERCENT_EQUALS	41
#define CARET_EQUALS 	42
#define GREATER_EQUALS	43
#define LESS_EQUALS 	44
#define EXCL_EQUALS 	45
#define EQUALS_EQUALS 	46
#define AMPER_AMPER 	47
#define VBAR_VBAR	 	48
#define PLUS_PLUS		49
#define MINUS_MINUS 	50
#define MINUS_GREATER 	51

TokenName tokenNames[] = {
	"CONST", CONST,
	"DOUBLE", DOUBLE,
	"EQUALS", EQUALS,
	"IDENTIFIER", IDENTIFIER,
	"INT", INT,
	"NUMBER", NUMBER,
	"RETURN", RETURN,
	"PLUS", PLUS,
	"MINUS", MINUS,
	"STAR", STAR,
	"SLASH", SLASH,
	"PERCENT", PERCENT,
	"CARET", CARET,
	"L_PAREN", L_PAREN,
	"R_PAREN", R_PAREN,
	"SEMICOLON", SEMICOLON,
	"COMMA", COMMA
};

const int tokenNameAmount = sizeof(tokenNames) / sizeof(tokenNames[0]);

TokenName keywords[] = {
	"const", CONST,
	"double", DOUBLE,
	"int", INT,
	"return", RETURN,
};

const int keywordAmount = sizeof(keywords) / sizeof(keywords[0]);

#endif