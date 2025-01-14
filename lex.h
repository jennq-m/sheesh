#ifndef LEX_H
#define LEX_H

#include <stdio.h>

//Enumerating different token type
typedef enum {
    IDENTIFIER, 
    KEYWORD, 
    RESERVED_WORD, 
    NOISE_WORD, 
    COMMENT, 
    ARITHMETIC_OPE, 
    ASSIGNMENT_OPE, 
    LOGICAL_OPE, 
    UNARY_OPE, 
    RELATIONAL_OPE, 
    DELIM_COMMA, 
    DELIM_SEMCOL, 
    DELIM_QUOTATION, 
    DELIM_O_PAREN, 
    DELIM_C_PAREN, 
    DELIM_O_BRACE, 
    DELIM_C_BRACE, 
    DELIM_O_BRCKT, 
    DELIM_C_BRCKT, 
    INVALID, 
    CONSTANT_NUM, 
    CONSTANT_DRIFT, 
    CONSTANT_TEXT, 
    CONSTANT_TXTFS,
    CONSTANDRESERVED, 
    CONSTANT_VIBE,
    CONSTANT_LEGIT
} TokenType;

//Holding attributes of TOKEN
typedef struct {
    char *value;       
    TokenType type;    
    int sheeshLine;    
} Token;

Token allTokens[1000];
int tokenCount = 0;

Token newToken(const char *value, TokenType type, int sheeshLine);
Token sheeshLexer(const char *sheeshLexeme, int sheeshLine);
void sheeshScanLine(FILE *outputSheesh, char *sheeshLine, int sheeshLineNum);

//Declaration of Function Prototype
int checkFilename(int argc, char *argv);
int checkKeyword(const char *sheeshLexeme);
int checkReservedWord(const char *sheeshLexeme);
int checkNoiseWord(const char *sheeshLexeme);
int checkAssignment(const char *sheeshLexeme);
int checkLogical(const char *sheeshLexeme);
int checkArithmetic(const char *sheeshLexeme);
int checkUnary(const char *sheeshLexeme);
int checkRelational(const char *sheeshLexeme);
void nextToken();
void parse();


char *typeToString(TokenType type) {
    switch (type) {
        case IDENTIFIER: 
            return "IDENTIFIER";
        case KEYWORD: 
            return "KEYWORD";
        case RESERVED_WORD: 
            return "RESERVED_WORD";
        case CONSTANDRESERVED: 
            return "CONSTANDRESERVED";
        case NOISE_WORD: 
            return "NOISE_WORD";
        case COMMENT: 
            return "COMMENT";
        case ARITHMETIC_OPE: 
            return "ARITHMETIC_OPE";
        case ASSIGNMENT_OPE: 
            return "ASSIGNMENT_OPE";
        case LOGICAL_OPE: 
            return "LOGICAL_OPE";
        case UNARY_OPE: 
            return "UNARY_OPE";
        case RELATIONAL_OPE: 
            return "RELATIONAL_OPE";
        case DELIM_COMMA:
            return "DELIM_COMMA";
        case DELIM_SEMCOL:
            return "DELIM_SEMCOL";
        case DELIM_O_PAREN:
            return "DELIM_O_PAREN";
        case DELIM_C_PAREN:
            return "DELIM_C_PAREN";
        case DELIM_O_BRCKT:
            return "DELIM_O_BRCKT";
        case DELIM_C_BRCKT:
            return "DELIM_C_BRCKT";
        case DELIM_O_BRACE:
            return "DELIM_O_BRACE";
        case DELIM_C_BRACE:
            return "DELIM_C_BRACE";
        case CONSTANT_NUM: 
            return "CONSTANT_NUM";
        case CONSTANT_DRIFT: 
            return "CONSTANT_DRIFT";
        case CONSTANT_TEXT: 
            return "CONSTANT_TEXT";
        case CONSTANT_TXTFS: 
            return "CONSTANT_TXTFS";
        case CONSTANT_VIBE:
            return "CONSTANT_VIBE";
        case CONSTANT_LEGIT:
            return "CONSTANT_LEGIT";
        case INVALID: 
            return "INVALID";
        default: 
            return "INVALID";
    }
}

#endif