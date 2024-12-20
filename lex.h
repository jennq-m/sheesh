#ifndef LEX_H
#define LEX_H

#include <stdio.h>

//Enumerating different token type
typedef enum {
    IDENTIFIER, 
    KEYWORD, 
    RESERVED_WORD, 
    CONSTANT, 
    NOISE_WORD, 
    COMMENT, 
    ARITHMETIC_OPE, 
    ASSIGNMENT_OPE, 
    LOGICAL_OPE, 
    UNARY_OPE, 
    RELATIONAL_OPE, 
    DELIMITER_COMMA, 
    DELIMITER_SEMICOLON, 
    DELIMITER_QUOTATION, 
    DELIMITER_O_PARENTHESIS, 
    DELIMITER_C_PARENTHESIS, 
    DELIMITER_O_BRACE, 
    DELIMITER_C_BRACE, 
    DELIMITER_O_BRACKET, 
    DELIMITER_C_BRACKET, 
    INVALID, 
    CONSTANT_NUM, 
    CONSTANT_DRIFT, 
    CONSTANT_TEXT, 
    CONSTANDRESERVED, 
    CONSTANT_VIBE
} TokenType;

//Holding attributes of TOKEN
typedef struct {
    char *value;       
    TokenType type;    
    int sheeshLine;    
} Token;


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

char *typeToString(TokenType type) {
    switch (type) {
        case IDENTIFIER: 
            return "IDENTIFIER";
        case KEYWORD: 
            return "KEYWORD";
        case RESERVED_WORD: 
            return "RESERVED_WORD";
        case CONSTANT: 
            return "CONSTANT";
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
        case DELIMITER_COMMA:
            return "DELIMITER_COMMA";
        case DELIMITER_SEMICOLON:
            return "DELIMITER_SEMICOLON";
        case DELIMITER_O_PARENTHESIS:
            return "DELIMITER_O_PARENTHESIS";
        case DELIMITER_C_PARENTHESIS:
            return "DELIMITER_C_PARENTHESIS";
        case DELIMITER_O_BRACKET:
            return "DELIMITER_O_BRACKET";
        case DELIMITER_C_BRACKET:
            return "DELIMITER_C_BRACKET";
        case DELIMITER_O_BRACE:
            return "DELIMITER_O_BRACE";
        case DELIMITER_C_BRACE:
            return "DELIMITER_C_BRACE";
        case CONSTANT_NUM: 
            return "CONSTANT_NUM";
        case CONSTANT_DRIFT: 
            return "CONSTANT_DRIFT";
        case CONSTANT_TEXT: 
            return "CONSTANT_TEXT";
        case CONSTANT_VIBE:
            return "CONSTANT_VIBE";
        case INVALID: 
            return "INVALID";
        default: 
            return "INVALID";
    }
}

#endif