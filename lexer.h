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
    CONSTANT_VIBE,
    CONSTANT_LEGIT,
    BOUNCE,
    CAR,
    DO,
    DRIFT,
    EMPTY,
    EX,
    FLIP,
    FROZEN,
    GROUP,
    IF,
    INPUT,
    JUMP,
    LEGIT,
    LOCKED,
    LOCKIN,
    LONG,
    MEANWHILE,
    NICKNAME,
    NUM,
    OPEN,
    OTHER,
    OUT,
    OUTSIDE,
    PL,
    REP,
    SCENARIO,
    SHORT,
    STANDARD,
    STOP,
    TEAM,
    TEXT,
    VIBE,
    ALWAYS,
    CAP,
    CONT,
    NOCAP,
    PLAYLIST,
    REPEAT,
    TOP,
    TOPTIER,
    OUTPUT,
    EXTRA,
    OTHERWISE
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
        case BOUNCE:    
            return "BOUNCE";
        case CAR:       
            return "CAR";
        case DO:        
            return "DO";
        case DRIFT:     
            return "DRIFT";
        case EMPTY:     
            return "EMPTY";
        case EX:        
            return "EXTRA";
        case FLIP:      
            return "FLIP";
        case FROZEN:    
            return "FROZEN";
        case GROUP:     
            return "GROUP";
        case IF:        
            return "IF";
        case INPUT:     
            return "INPUT";
        case JUMP:      
            return "JUMP";
        case LEGIT:     
            return "LEGIT";
        case LOCKED:    
            return "LOCKED";
        case LOCKIN:    
            return "LOCKIN";
        case LONG:      
            return "LONG";
        case MEANWHILE: 
            return "MEANWHILE";
        case NICKNAME:  
            return "NICKNAME";
        case NUM:       
            return "NUM";
        case OPEN:      
            return "OPEN";
        case OTHER:   
            return "OTHER";  
        case OUT:      
            return "OUTPUT"; 
        case OUTSIDE:
            return "OUTSIDE";   
        case PL:        
            return "PL";
        case REP:   
            return "REP";    
        case SCENARIO:
            return "SCENARIO";  
        case SHORT:     
            return "SHORT";
        case STANDARD: 
            return "STANDARD"; 
        case STOP:      
            return "STOP";
        case TEAM:    
            return "TEAM";  
        case TEXT:    
            return "TEXT";  
        case VIBE:    
            return "VIBE";  
        case PLAYLIST:
            return "PLAYLIST";  
        case REPEAT:    
            return "REPEAT";
        case OUTPUT:    
            return "OUTPUT";
        case EXTRA:     
            return "EXTRA";
        case OTHERWISE:
            return "OTHER"; 
        case ALWAYS:    
            return "ALWAYS";
        case CONT:
            return "CONT";
        case TOPTIER:
            return "TOPTIER";
        case TOP: 
            return "TOPTIER";
        case INVALID: 
            return "INVALID";
        default: 
            return "INVALID";
    }
}

#endif