#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "grammar.h"
#include "parser.h"
#include "../lexer/lexer.h"
#include "../utils/hashtable.h"

#define DEFAULT_OUTPUT_SIZE 500

ParseNode *matchGrammar(GrammarHead *head);
int matchPart(GrammarPart *part);
ParseNode *parseBuilder(char *name, size_t start, size_t top);

struct{
    ParseChild *values;

    size_t size;
    size_t cur; //Where read from/write to, if equal to top, where to place next token
    size_t top; //Where to place next token

    Tokenizer *tokenizer;
} output;

void printOutput(){
    int i;
    printf("|");
    for(i = 0; i < output.top; i++){
        if(output.values[i].type == CHILD_TOKEN)
            printf("%s|", output.values[i].val.token->value);
        else
            printf("%s|", output.values[i].val.node->name);
    }
    printf("\n");
}

void output_setup(char *filepath){
    output.values = malloc(DEFAULT_OUTPUT_SIZE * sizeof(ParseChild));

    output.size = DEFAULT_OUTPUT_SIZE;
    output.cur = 0;
    output.top = 0;

    output.tokenizer = tokenizer_new(filepath);
}

void output_reset(){
    free(output.values);

    output.cur = 0;
    output.top = 0;

    tokenizer_destroy(output.tokenizer);
}

ParseChild *output_next(){
    //printOutput();

    if(output.cur == output.top){ //Read from token stream
        while(output.top >= output.size){
            output.size *= 1.5;
            output.values = realloc(output.values, output.size);
        }

        ParseChild entry;

        entry.type = CHILD_TOKEN;

        Token *token = tokenizer_nexttoken(output.tokenizer);

        entry.val.token = token;

        output.values[output.top++] = entry;
    }

    //printf("Cur: %d - %s\n", output.cur, output.values[output.cur].val.token->value);

    return output.values + (output.cur++);
}

ParseNode *parseFile(char *filepath){
    output_setup(filepath);

    ParseNode *prog = matchGrammar(table_lookup(grammarTable, "prog"));

    output_reset();

    return prog;
}

ParseNode *matchGrammar(GrammarHead *head){
    size_t start = output.cur;

    GrammarPart *part;
    for(part = head->grammar; part != NULL; part = part->next){
        if(!matchPart(part)){
            return NULL;
        }
    }

    size_t end = output.cur - 1;

    //printf("Building %s\n", head->name);

    return parseBuilder(head->name, start, end);
}

int matchPart(GrammarPart *part){
    //printf("%d\n", part->type);

    if(part->type == OR){
        size_t start = output.cur;
        OrGrammarPart *orPart = (OrGrammarPart *)part;

        GrammarPart *p;

        int success = 1;
        for(p = orPart->op1; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(success)
            return 1;

        output.cur = start;

        success = 1;
        for(p = orPart->op2; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(success)
            return 1;

        return 0;
    }
    else if(part->type == MACRO){
        size_t start = output.cur;
        MacroGrammarPart *macroPart = (MacroGrammarPart *)part;

        ParseChild entry;

        entry.type = CHILD_NODE;
        entry.val.node = matchGrammar(table_lookup(grammarTable, macroPart->macro));

        if(entry.val.node == NULL)
            return 0;

        output.values[start] = entry;

        return 1;
    }
    else if(part->type == LOOP){

        LoopGrammarPart *loopPart = (LoopGrammarPart *)part;

        while(1){
            size_t start = output.cur;

            int success = 1;

            GrammarPart *p;
            for(p = loopPart->loop; p != NULL; p = p->next){
                if(!(success = matchPart(p)))
                    break;
            }

            if(!success){
                output.cur = start;
                break;
            }
        }

        return 1;
    }
    else if(part->type == OPTIONAL){
        OpGrammarPart *opPart= (OpGrammarPart *)part;

        size_t start = output.cur;

        int success = 1;

        GrammarPart *p;
        for(p = opPart->option; p != NULL; p = p->next){
            if(!(success = matchPart(p)))
                break;
        }

        if(!success)
            output.cur = start;

        return 1;
    }
    else{
        Token *token = output_next()->val.token;

        if(token == NULL)
            return 0;

        //printf("Comparing %d to %d!\n", part->type, token->type);

        return part->type == token->type;
    }
}

ParseNode *parseBuilder(char *name, size_t start, size_t end){
    int outputs = end - start + 1;

    //printf("Outputs: %d\n", outputs);
    //printf("Start: %d, End: %d, Cur: %d, Top: %d\n", start, end, output.cur, output.top);

    ParseNode *node = malloc(sizeof(ParseNode));

    node->name = malloc(strlen(name) * sizeof(char));
    strcpy(node->name, name);

    node->children = malloc(outputs * sizeof(ParseChild));
    node->childAmount = outputs;

    int i;
    for(i = 0; i < outputs; i++)
        node->children[i] = output.values[start + i];


    /*Shift the output stack if more than one element was reduced*/
    for(i = 1; i < output.top - end; i++){
        output.values[start + i] = output.values[end + i];
    }

    int off = output.top - output.cur; //Keep the same offset between top and cur

    output.top = start + off + 1;
    if(output.cur > start)
        output.cur = start + 1;

    //printf("Start: %d, End: %d, Cur: %d, Top: %d\n", start, end, output.cur, output.top);

    return node;
}