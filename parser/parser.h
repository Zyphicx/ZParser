#ifndef PARSER_H
#define PARSER_H

#define CHILD_TOKEN 0
#define CHILD_NODE 1

struct parsenode;
struct parsechild;

typedef struct parsenode ParseNode;
typedef struct parsechild ParseChild;

struct parsenode{
    char *name;
    ParseChild *children;
    int childAmount;
};

struct parsechild{
    unsigned int type : 1;

    union{
        Token *token;
        ParseNode *node;
    } val;
};

ParseNode *parseFile(char *filepath);

#endif