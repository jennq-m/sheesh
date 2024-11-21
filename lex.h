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
    COMMA, 
    SEMICOLON, 
    QUOTATION, 
    O_PARENTHESIS, 
    C_PARENTHESIS, 
    O_BRACE, 
    C_BRACE, 
    O_BRACKET, 
    C_BRACKET, 
    INVALID, 
    INT_CONSTANT, 
    FLOAT_CONSTANT, 
    STRING_CONSTANT, 
    CONSTANDRESERVED, 
    CHAR_CONSTANT
} TokenType;

//Holding attributes of TOKEN
typedef struct {
    char *value;       
    TokenType type;    
    int sheeshLine;    
} Token;


Token newToken(const char *value, TokenType type, int sheeshLine);
Token sheeshLexer(const char *sheeshLexeme, int sheeshLine);
void analyzeLine(FILE *outputSheesh, char *sheeshLine, int sheeshLineNum);

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
            return "Identifier";
        case KEYWORD: 
            return "Keyword";
        case RESERVED_WORD: 
            return "Reserved Word";
        case CONSTANT: 
            return "Constant";
        case CONSTANDRESERVED: 
            return "Constant (Legit (bool in C)), Reserved Word";
        case NOISE_WORD: 
            return "Noise Word";
        case COMMENT: 
            return "Comment";
        case ARITHMETIC_OPE: 
            return "Arithmetic Operator";
        case ASSIGNMENT_OPE: 
            return "Assignment Operator";
        case LOGICAL_OPE: 
            return "Logical Operator (Boolean)";
        case UNARY_OPE: 
            return "Unary Operator";
        case RELATIONAL_OPE: 
            return "Relational Operator (Boolean)";
        case COMMA:
            return "Delimiter (Comma)";
        case SEMICOLON:
            return "Delimiter (Semicolon)";
        case O_PARENTHESIS:
            return "Delimiter (Open (Left) Parenthesis)";
        case C_PARENTHESIS:
            return "Delimiter (Closing (Right) Parenthesis)";
        case O_BRACKET:
            return "Bracket (Open (Left) Bracket)";
        case C_BRACKET:
            return "Bracket (Closing (Right) Bracket)";
        case O_BRACE:
            return "Bracket (Open (Left) Brace)";
        case C_BRACE:
            return "Bracket (Closing (Right) Brace)";
        case INT_CONSTANT: 
            return "Constant (Num (int in C))";
        case FLOAT_CONSTANT: 
            return "Constant (Drift (float in C))";
        case STRING_CONSTANT: 
            return "Constant (Text (string in C))";
        case CHAR_CONSTANT:
            return "Constant (Vibe (char in C))";
        case INVALID: 
            return "Invalid";
        default: 
            return "Invalid";
    }
}

#endif