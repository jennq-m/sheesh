#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "checkArithmetic.h"
#include "checkAssignment.h"
#include "checkFilename.h"
#include "checkIdentifier.h"
#include "checkKeyword.h"
#include "checkLogical.h"
#include "checkNoiseWord.h"
#include "checkRelational.h"
#include "checkReservedWord.h"
#include "checkUnary.h"
#include "lex.h"

int main(int argc, char *argv[]) {
    checkFilename(argc, argv[1]);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputSheesh = fopen("symbol_table.txt", "w");
    if (!outputSheesh) {
        perror("Error opening output file");
        fclose(file);
        return 1;
    }

    char sheeshLine[256];
    int sheeshColumn = 1;

    fprintf(outputSheesh, "Lexical Analysis of %s:\n\n", argv[1]);

    while (fgets(sheeshLine, sizeof(sheeshLine), file)) {
        sheeshScanLine(outputSheesh, sheeshLine, sheeshColumn);
        sheeshColumn++;
    }

    FILE *outputParser = fopen("parse_tree.txt", "w");
    if (!outputParser) {
        perror("Error opening output file");
        fclose(file);
        return 1;
    }

    parse(outputParser);

    fclose(file);
    fclose(outputSheesh);
    fclose(outputParser);

    printf("See symbol_table.txt for lexical analysis.\n");
    return 0;
}

//Creating and returning a new token 
Token newToken(const char *value, TokenType type, int sheeshColumn) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.sheeshLine = sheeshColumn;

    allTokens[tokenCount++] = token;

    return token;
}

//LEXER
Token sheeshLexer(const char *sheeshLexeme, int sheeshLine) {
    int state = 0;
    char ch;
    int decimalCount = 0;
    int checkFloat = 0;
    char temp[256];
    int tempMarker = 0;
    

    for (int i = 0; (ch = sheeshLexeme[i]) != '\0'; i++) {
        switch (state) {
            case 0:
                if (isalpha(ch) || ch == '_' || ch == '#') {
                    state = 1;
                } else if (isdigit(ch) || ch == '.') { 
                    state = 2; 
                } else if (strchr("+-*/%=!<>&|^$", ch)) { 
                    state = 4; 
                } else if (ch == '/' && sheeshLexeme[i + 1] == '/') { 
                    return newToken(sheeshLexeme, COMMENT, sheeshLine); 
                } else if (ch == '/' && sheeshLexeme[i + 1] == '*') { 
                    return newToken(sheeshLexeme, COMMENT, sheeshLine); 
                } else { 
                    return newToken(sheeshLexeme, INVALID, sheeshLine); 
                }

                break;

            case 1:
                if (!isalnum(ch) && ch != '_' && ch != '#') {
                    return newToken(sheeshLexeme, INVALID, sheeshLine);
                }

                break;

            case 2:
                if (ch == '.') {
                    if (decimalCount > 0) {
                        return newToken(sheeshLexeme, INVALID, sheeshLine);
                    }

                    decimalCount++;
                    checkFloat = 1;
                } else if (!isdigit(ch)) {
                    return newToken(sheeshLexeme, INVALID, sheeshLine);
                }

                break;
            case 4:
                if (strchr("+-*/%=!<>&|^$", ch)) {
                    state = 4;
                }
                
                break;
        }
    }

    if (state == 1) {

        for (int i = 0; (ch = sheeshLexeme[i]) != '\0'; i++) {
        TokenType keyword = checkKeyword(sheeshLexeme);

        // Print the result
        switch (keyword) {
            case BOUNCE:
                return newToken(sheeshLexeme, BOUNCE, sheeshLine);
            case CAR:
                return newToken(sheeshLexeme, CAR, sheeshLine);
            case DO:
                return newToken(sheeshLexeme, DO, sheeshLine);
            case DRIFT:
                return newToken(sheeshLexeme, DRIFT, sheeshLine);
            case EMPTY:
                return newToken(sheeshLexeme, EMPTY, sheeshLine);
            case EX:
                return newToken(sheeshLexeme, EX, sheeshLine);
            case FLIP:
                return newToken(sheeshLexeme, FLIP, sheeshLine);
            case FROZEN:
                return newToken(sheeshLexeme, FROZEN, sheeshLine);
            case GROUP:
                return newToken(sheeshLexeme, GROUP, sheeshLine);
            case IF:
                return newToken(sheeshLexeme, IF, sheeshLine);
            case INPUT:
                return newToken(sheeshLexeme, INPUT, sheeshLine);
            case JUMP:
                return newToken(sheeshLexeme, JUMP, sheeshLine);
            case LEGIT:
                return newToken(sheeshLexeme, LEGIT, sheeshLine);
            case LOCKED:
                return newToken(sheeshLexeme, LOCKED, sheeshLine);
            case LOCKIN:
                return newToken(sheeshLexeme, LOCKIN, sheeshLine);
            case LONG:
                return newToken(sheeshLexeme, LONG, sheeshLine);
            case MEANWHILE:
                return newToken(sheeshLexeme, MEANWHILE, sheeshLine);
            case NICKNAME:
                return newToken(sheeshLexeme, NICKNAME, sheeshLine);
            case NUM:
                return newToken(sheeshLexeme, NUM, sheeshLine);
            case OPEN:
                return newToken(sheeshLexeme, OPEN, sheeshLine);
            case OTHER:
                return newToken(sheeshLexeme, OTHER, sheeshLine);
            case OUT:
                return newToken(sheeshLexeme, OUT, sheeshLine);
            case OUTSIDE:
                return newToken(sheeshLexeme, OUTSIDE, sheeshLine);
            case PL:
                return newToken(sheeshLexeme, PL, sheeshLine);
            case REP:
                return newToken(sheeshLexeme, REP, sheeshLine);
            case SCENARIO:
                return newToken(sheeshLexeme, SCENARIO, sheeshLine);
            case SHORT:
                return newToken(sheeshLexeme, SHORT, sheeshLine);
            case STANDARD:
                return newToken(sheeshLexeme, STANDARD, sheeshLine);
            case STOP:
                return newToken(sheeshLexeme, STOP, sheeshLine);
            case TEAM:
                return newToken(sheeshLexeme, TEAM, sheeshLine);
            case TEXT:
                return newToken(sheeshLexeme, TEXT, sheeshLine);
            case VIBE:
                return newToken(sheeshLexeme, VIBE, sheeshLine);
            case PLAYLIST:
                return newToken(sheeshLexeme, PLAYLIST, sheeshLine);
            case REPEAT:
                return newToken(sheeshLexeme, REPEAT, sheeshLine);
            case OUTPUT:
                return newToken(sheeshLexeme, OUTPUT, sheeshLine);
            case EXTRA:
                return newToken(sheeshLexeme, EXTRA, sheeshLine);
            case OTHERWISE:
                return newToken(sheeshLexeme, OTHERWISE, sheeshLine);

        }
    }

        if (checkReservedWord(sheeshLexeme)) {
            if (strcmp(sheeshLexeme, "cap") == 0 || strcmp(sheeshLexeme, "nocap") == 0) {
                return newToken(sheeshLexeme, CONSTANT_LEGIT, sheeshLine);
            }
            else {
                return newToken(sheeshLexeme, RESERVED_WORD, sheeshLine);
            }
        }
    
        if (checkNoiseWord(sheeshLexeme)) {
            return newToken(sheeshLexeme, NOISE_WORD, sheeshLine);
        }

        if (checkIdentifier(sheeshLexeme)) {
            return newToken(sheeshLexeme, IDENTIFIER, sheeshLine);
        }

        return newToken(sheeshLexeme, INVALID, sheeshLine);
    } else if (state == 2) {

        if (sheeshLexeme[strlen(sheeshLexeme) - 1] == '.') {
            return newToken(sheeshLexeme, INVALID, sheeshLine);
        }

        if (checkFloat) {
            return newToken(sheeshLexeme, CONSTANT_DRIFT, sheeshLine);
        } else {
            return newToken(sheeshLexeme, CONSTANT_NUM, sheeshLine);
        }
    } else if (state == 3) {
        switch(ch) {
            case '\'':
                return newToken(sheeshLexeme, CONSTANT_VIBE, sheeshLine);
            case '"':
                return newToken(sheeshLexeme, CONSTANT_TEXT, sheeshLine);    
        }
        return newToken(sheeshLexeme, CONSTANT_TEXT, sheeshLine);
    } else if (state == 4) {
        if (checkAssignment(sheeshLexeme)) {
            return newToken(sheeshLexeme, ASSIGNMENT_OPE, sheeshLine);
        }

        if (checkLogical(sheeshLexeme)) {
            return newToken(sheeshLexeme, LOGICAL_OPE, sheeshLine);
        }

        if (checkArithmetic(sheeshLexeme)) {
            return newToken(sheeshLexeme, ARITHMETIC_OPE, sheeshLine);
        } 

        if (checkUnary(sheeshLexeme)) {
            return newToken(sheeshLexeme, UNARY_OPE, sheeshLine);
        }

        if (checkRelational(sheeshLexeme)) {
            return newToken(sheeshLexeme, RELATIONAL_OPE, sheeshLine);
        }
    }

    return newToken(sheeshLexeme, INVALID, sheeshLine);
}

void sheeshScanLine(FILE *outputSheesh, char *sheeshLine, int sheeshColumn) {
    static int inMultiLineComment = 0;
    int stringLiteral = 0;
    int characterConstant = 0;
    char temp[256];
    int tempMarker = 0;
    int hasFormatSpecifier = 0;

    for (int i = 0; sheeshLine[i] != '\0'; i++) {

        if (inMultiLineComment) {
            if (sheeshLine[i] == '*' && sheeshLine[i + 1] == '/') {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s \n", token.sheeshLine, token.value, typeToString(token.type));
                i++;
            }
            continue;
        }

        if (stringLiteral) {
            temp[tempMarker++] = sheeshLine[i];
            if (sheeshLine[i] == '%' && strchr("dfsc", sheeshLine[i + 1])) {
                hasFormatSpecifier = 1;
            }

            if (sheeshLine[i] == '"') {
                temp[tempMarker] = '\0';

                if (!hasFormatSpecifier) {
                    Token token = newToken(temp, CONSTANT_TEXT, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                }
                else { 
                    Token token = newToken(temp, CONSTANT_TXTFS, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                }

                tempMarker = 0;
                stringLiteral = 0;
            }
            continue;
        }

        if (characterConstant) {
            temp[tempMarker++] = sheeshLine[i];
            if (sheeshLine[i] == '\'') {
                temp[tempMarker] = '\0';
        
                if (tempMarker == 3) {
                    Token token = newToken(temp, CONSTANT_VIBE, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    tempMarker = 0;
                    characterConstant = 0;
                } else {
                    temp[tempMarker] = '\0';
                    if (tempMarker <= 2) {
                        Token token = newToken(temp, DELIM_QUOTATION, sheeshColumn);
                        fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    }
                    else {
                        Token token = newToken(temp, INVALID, sheeshColumn);
                        fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    }

                    tempMarker = 0;
                    characterConstant = 0;
                }

                tempMarker = 0;
                characterConstant = 0;
            }
            continue;
        }

        if (sheeshLine[i] == '"') {
            stringLiteral = 1;
            temp[tempMarker++] = sheeshLine[i];
            continue;
        }

        if (sheeshLine[i] == '\'') {
            characterConstant = 1;
            temp[tempMarker++] = sheeshLine[i];
            continue;
        }

        if (sheeshLine[i] == '/' && sheeshLine[i + 1] == '/') {
            Token token = newToken("//", COMMENT, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            
            break;
        }

        if (sheeshLine[i] == '/' && sheeshLine[i + 1] == '*') {
            Token token = newToken("/*", COMMENT, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            
            inMultiLineComment = 1;
            i++;
            continue;
        }


        if (strchr("+-*/=!<>&|^%", sheeshLine[i])) {
            char lookAhead[3] = {sheeshLine[i], sheeshLine[i+1], '\0'};
            
            if ((sheeshLine[i] == '=' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '!' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '>' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '<' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '&' && sheeshLine[i+1] == '&') ||   
                (sheeshLine[i] == '|' && sheeshLine[i+1] == '|') ||   
                (sheeshLine[i] == '+' && sheeshLine[i+1] == '+') ||  
                (sheeshLine[i] == '-' && sheeshLine[i+1] == '-') ||   
                (sheeshLine[i] == '+' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '-' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '*' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '/' && sheeshLine[i+1] == '=') ||   
                (sheeshLine[i] == '%' && sheeshLine[i+1] == '=') ||
                (sheeshLine[i] == '|' && sheeshLine[i+1] == '=')) {   
                
                if (tempMarker > 0) {
                    temp[tempMarker] = '\0';
                    Token token = sheeshLexer(temp, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    
                    tempMarker = 0;
                }

                Token token = sheeshLexer(lookAhead, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, token.value);
 
                i++;  
                continue;
            }
            
            
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                
                tempMarker = 0;
            }

            char op[2] = {sheeshLine[i], '\0'};
            Token token = sheeshLexer(op, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, token.value);
            
            continue;
        }

       
        TokenType delimiter;
        switch (sheeshLine[i]) {
            case ',':
                delimiter = DELIM_COMMA;
                break;
            case ';':
                delimiter = DELIM_SEMCOL;
                break;
            case '(':
                delimiter = DELIM_O_PAREN;
                break;
            case ')':
                delimiter = DELIM_C_PAREN;
                break;
            case '[':
                delimiter = DELIM_O_BRCKT;
                break;
            case ']':
                delimiter = DELIM_C_BRCKT;
                break;
            case '{':
                delimiter = DELIM_O_BRACE;
                break;
            case '}':
                delimiter = DELIM_C_BRACE;
                break;
            default:
                delimiter = INVALID;
        }

        if (delimiter != INVALID) {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                
                tempMarker = 0;
            }

            char delim[2] = {sheeshLine[i], '\0'};
            Token token = newToken(delim, delimiter, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            
            continue;
        }
        
        if (isalpha(sheeshLine[i]) || (ispunct(sheeshLine[i]) && sheeshLine[i] != '.')) {
    temp[tempMarker++] = sheeshLine[i];
    while ((isalnum(sheeshLine[i + 1]) || ispunct(sheeshLine[i + 1])) && 
           !(strchr("+-*/%=!<>&|^()\"{}[];,", sheeshLine[i + 1]))) {
        temp[tempMarker++] = sheeshLine[++i];
    }
    temp[tempMarker] = '\0'; // Null-terminate the lexeme

    TokenType keyword = checkKeyword(temp);
    Token token; 

    TokenType reservedWord = checkReservedWord(temp);
    Token rwToken;

    if (keyword) { 
        switch (keyword) {
            case BOUNCE:    token = newToken(temp, BOUNCE, sheeshColumn); break;
            case CAR:       token = newToken(temp, CAR, sheeshColumn); break;
            case DO:        token = newToken(temp, DO, sheeshColumn); break;
            case DRIFT:     token = newToken(temp, DRIFT, sheeshColumn); break;
            case EMPTY:     token = newToken(temp, EMPTY, sheeshColumn); break;
            case EX:        token = newToken(temp, EX, sheeshColumn); break;
            case FLIP:      token = newToken(temp, FLIP, sheeshColumn); break;
            case FROZEN:    token = newToken(temp, FROZEN, sheeshColumn); break;
            case GROUP:     token = newToken(temp, GROUP, sheeshColumn); break;
            case IF:        token = newToken(temp, IF, sheeshColumn); break;
            case INPUT:     token = newToken(temp, INPUT, sheeshColumn); break;
            case JUMP:      token = newToken(temp, JUMP, sheeshColumn); break;
            case LEGIT:     token = newToken(temp, LEGIT, sheeshColumn); break;
            case LOCKED:    token = newToken(temp, LOCKED, sheeshColumn); break;
            case LOCKIN:    token = newToken(temp, LOCKIN, sheeshColumn); break;
            case LONG:      token = newToken(temp, LONG, sheeshColumn); break;
            case MEANWHILE: token = newToken(temp, MEANWHILE, sheeshColumn); break;
            case NICKNAME:  token = newToken(temp, NICKNAME, sheeshColumn); break;
            case NUM:       token = newToken(temp, NUM, sheeshColumn); break;
            case OPEN:      token = newToken(temp, OPEN, sheeshColumn); break;
            case OTHER:     token = newToken(temp, OTHER, sheeshColumn); break;
            case OUT:       token = newToken(temp, OUT, sheeshColumn); break;
            case OUTSIDE:   token = newToken(temp, OUTSIDE, sheeshColumn); break;
            case PL:        token = newToken(temp, PL, sheeshColumn); break;
            case REP:       token = newToken(temp, REP, sheeshColumn); break;
            case SCENARIO:  token = newToken(temp, SCENARIO, sheeshColumn); break;
            case SHORT:     token = newToken(temp, SHORT, sheeshColumn); break;
            case STANDARD:  token = newToken(temp, STANDARD, sheeshColumn); break;
            case STOP:      token = newToken(temp, STOP, sheeshColumn); break;
            case TEAM:      token = newToken(temp, TEAM, sheeshColumn); break;
            case TEXT:      token = newToken(temp, TEXT, sheeshColumn); break;
            case VIBE:      token = newToken(temp, VIBE, sheeshColumn); break;
            case PLAYLIST:  token = newToken(temp, PLAYLIST, sheeshColumn); break;
            case REPEAT:    token = newToken(temp, REPEAT, sheeshColumn); break;
            case OUTPUT:    token = newToken(temp, OUTPUT, sheeshColumn); break;
            case EXTRA:     token = newToken(temp, EXTRA, sheeshColumn); break;
            case OTHERWISE: token = newToken(temp, OTHERWISE, sheeshColumn); break;
            default:        token = newToken(temp, IDENTIFIER, sheeshColumn); break;
        }

                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %-15s\n", token.sheeshLine, token.value, typeToString(token.type));
    }   else if (reservedWord) {
        TokenType reservedWord = checkReservedWord(temp);
        Token rwToken; // Declare the Token structure

        if (reservedWord) { // If keyword is valid
            switch (reservedWord) {
                case ALWAYS:    rwToken = newToken(temp, ALWAYS, sheeshColumn); break;
                case CAP:       rwToken = newToken(temp, CONSTANT_LEGIT, sheeshColumn); break;
                case CONT:        rwToken = newToken(temp, CONT, sheeshColumn); break;
                case NOCAP:       rwToken = newToken(temp, CONSTANT_LEGIT, sheeshColumn); break;
                case TOPTIER:     rwToken = newToken(temp, TOPTIER, sheeshColumn); break;
                case TOP:       rwToken = newToken(temp, TOPTIER, sheeshColumn); break;
                default:        rwToken = newToken(temp, IDENTIFIER, sheeshColumn); break;
            }
            
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", rwToken.sheeshLine, rwToken.value, typeToString(rwToken.type));

            }
                } else if (checkNoiseWord(temp)) {
                    Token token = newToken(temp, NOISE_WORD, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    
                } else if (checkIdentifier(temp)) {
                    Token token = newToken(temp, IDENTIFIER, sheeshColumn);  
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    
                } else {
                    Token token = newToken(temp, INVALID, sheeshColumn);  
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    
                }
                
                tempMarker = 0;  
                continue;
            }

            if (isdigit(sheeshLine[i]) || sheeshLine[i] == '.') {
                temp[tempMarker++] = sheeshLine[i];
            } else if (isspace(sheeshLine[i])) {
                if (tempMarker > 0) {
                    temp[tempMarker] = '\0'; 
                    Token token = sheeshLexer(temp, sheeshColumn); 
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    tempMarker = 0;  
                }
            } else {
                continue;
            }
        }

        if (tempMarker > 0) {
            temp[tempMarker] = '\0';
            if (stringLiteral || characterConstant) {
                Token token = newToken(temp, DELIM_QUOTATION, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: DELIM_QUOTATION\n", token.sheeshLine, token.value);
                
            } else {
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                
            }
        }
}

typedef struct ASTNode {
    char *value;
    struct ASTNode *left, *right;
} ASTNode;

int tokenCount;
Token currentToken;
TokenType mapStringToTokenType(const char* tokenStr);
int currentIndex = 0;

ASTNode* parseProgram();
ASTNode* parseBody();
ASTNode* parseStmts();
ASTNode* parseExprStmt();
ASTNode* parseExpr();
ASTNode* parseAndExpr();
ASTNode* parseEqualityExpr();
ASTNode* parseRelationalExpr();
ASTNode* parseAddSubExpr();
ASTNode* parseMultDivExpr();
ASTNode* parsePrimary();
ASTNode* parseDecStmt();
ASTNode* parseAssignStmt();
ASTNode* parseCondStmt();
ASTNode* parseIterativeStmt();
ASTNode* parseUnaryVal();
ASTNode* parseLiteral();
ASTNode* parseNumVal();
ASTNode* parseDriftVal();
ASTNode* parseSign();
ASTNode* parseIdentExpr();
ASTNode* parseDataType();
ASTNode* parseVarList();
ASTNode* parseInitialization();
ASTNode* parseRepLoop();
ASTNode* parseLoopInitial();
ASTNode* parseUpdStmt();
ASTNode* parseMwLoop();
ASTNode* parseDmwLoop();
ASTNode* parseInputStmt();
ASTNode* parseInput();
ASTNode* parseOutputStmt();
ASTNode* parseOutput();


void initializeParser(const char* symbolTablePath) {
    FILE* symbolTable = fopen(symbolTablePath, "r");
    if (!symbolTable) {
        printf("Error opening symbol table file\n");
        exit(1);
    }

    char line[1024];
    int lineNum;
    char lexeme[256];
    char tokenTypeStr[256];
    
    while (fgets(line, sizeof(line), symbolTable)) {
        if (sscanf(line, "Line %d: Lexeme: %s Token: %s", &lineNum, lexeme, tokenTypeStr) == 3) {
            Token token;
            token.value = strdup(lexeme);
            token.type = mapStringToTokenType(tokenTypeStr);
            token.sheeshLine = lineNum;
            allTokens[tokenCount++] = token;
        }
    }

    fclose(symbolTable);
    currentIndex = 0;
    if (tokenCount > 0) {
        currentToken = allTokens[0];
    }
}

TokenType mapStringToTokenType(const char* tokenStr) {
    if (strcmp(tokenStr, "IDENTIFIER") == 0) return IDENTIFIER;
    if (strcmp(tokenStr, "KEYWORD") == 0) return KEYWORD;
    if (strcmp(tokenStr, "RESERVED_WORD") == 0) return RESERVED_WORD;
    if (strcmp(tokenStr, "NOISE_WORD") == 0) return NOISE_WORD;
    if (strcmp(tokenStr, "COMMENT") == 0) return COMMENT;
    if (strcmp(tokenStr, "ARITHMETIC_OPE") == 0) return ARITHMETIC_OPE;
    if (strcmp(tokenStr, "ASSIGNMENT_OPE") == 0) return ASSIGNMENT_OPE;
    if (strcmp(tokenStr, "LOGICAL_OPE") == 0) return LOGICAL_OPE;
    if (strcmp(tokenStr, "UNARY_OPE") == 0) return UNARY_OPE;
    if (strcmp(tokenStr, "RELATIONAL_OPE") == 0) return RELATIONAL_OPE;
    if (strcmp(tokenStr, "DELIM_COMMA") == 0) return DELIM_COMMA;
    if (strcmp(tokenStr, "DELIM_SEMCOL") == 0) return DELIM_SEMCOL;
    if (strcmp(tokenStr, "DELIM_QUOTATION") == 0) return DELIM_QUOTATION;
    if (strcmp(tokenStr, "DELIM_O_PAREN") == 0) return DELIM_O_PAREN;
    if (strcmp(tokenStr, "DELIM_C_PAREN") == 0) return DELIM_C_PAREN;
    if (strcmp(tokenStr, "DELIM_O_BRACE") == 0) return DELIM_O_BRACE;
    if (strcmp(tokenStr, "DELIM_C_BRACE") == 0) return DELIM_C_BRACE;
    if (strcmp(tokenStr, "DELIM_O_BRCKT") == 0) return DELIM_O_BRCKT;
    if (strcmp(tokenStr, "DELIM_C_BRCKT") == 0) return DELIM_C_BRCKT;
    if (strcmp(tokenStr, "INVALID") == 0) return INVALID;
    if (strcmp(tokenStr, "CONSTANT_NUM") == 0) return CONSTANT_NUM;
    if (strcmp(tokenStr, "CONSTANT_DRIFT") == 0) return CONSTANT_DRIFT;
    if (strcmp(tokenStr, "CONSTANT_TEXT") == 0) return CONSTANT_TEXT;
    if (strcmp(tokenStr, "CONSTANT_TXTFS") == 0) return CONSTANT_TXTFS;
    if (strcmp(tokenStr, "CONSTANT_VIBE") == 0) return CONSTANT_VIBE;
    if (strcmp(tokenStr, "CONSTANT_LEGIT") == 0) return CONSTANT_LEGIT;
    if (strcmp(tokenStr, "BOUNCE") == 0) return BOUNCE;
    if (strcmp(tokenStr, "CAR") == 0) return CAR;
    if (strcmp(tokenStr, "DO") == 0) return DO;
    if (strcmp(tokenStr, "DRIFT") == 0) return DRIFT;
    if (strcmp(tokenStr, "EMPTY") == 0) return EMPTY;
    if (strcmp(tokenStr, "EX") == 0) return EX;
    if (strcmp(tokenStr, "FLIP") == 0) return FLIP;
    if (strcmp(tokenStr, "FROZEN") == 0) return FROZEN;
    if (strcmp(tokenStr, "GROUP") == 0) return GROUP;
    if (strcmp(tokenStr, "IF") == 0) return IF;
    if (strcmp(tokenStr, "INPUT") == 0) return INPUT;
    if (strcmp(tokenStr, "JUMP") == 0) return JUMP;
    if (strcmp(tokenStr, "LEGIT") == 0) return LEGIT;
    if (strcmp(tokenStr, "LOCKED") == 0) return LOCKED;
    if (strcmp(tokenStr, "LOCKIN") == 0) return LOCKIN;
    if (strcmp(tokenStr, "LONG") == 0) return LONG;
    if (strcmp(tokenStr, "MEANWHILE") == 0) return MEANWHILE;
    if (strcmp(tokenStr, "NICKNAME") == 0) return NICKNAME;
    if (strcmp(tokenStr, "NUM") == 0) return NUM;
    if (strcmp(tokenStr, "OPEN") == 0) return OPEN;
    if (strcmp(tokenStr, "OTHER") == 0) return OTHER;
    if (strcmp(tokenStr, "OUT") == 0) return OUT;
    if (strcmp(tokenStr, "OUTSIDE") == 0) return OUTSIDE;
    if (strcmp(tokenStr, "PL") == 0) return PL;
    if (strcmp(tokenStr, "REP") == 0) return REP;
    if (strcmp(tokenStr, "SCENARIO") == 0) return SCENARIO;
    if (strcmp(tokenStr, "SHORT") == 0) return SHORT;
    if (strcmp(tokenStr, "STANDARD") == 0) return STANDARD;
    if (strcmp(tokenStr, "STOP") == 0) return STOP;
    if (strcmp(tokenStr, "TEAM") == 0) return TEAM;
    if (strcmp(tokenStr, "TEXT") == 0) return TEXT;
    if (strcmp(tokenStr, "VIBE") == 0) return VIBE;
    if (strcmp(tokenStr, "ALWAYS") == 0) return ALWAYS;
    if (strcmp(tokenStr, "CAP") == 0) return CAP;
    if (strcmp(tokenStr, "CONT") == 0) return CONT;
    if (strcmp(tokenStr, "NOCAP") == 0) return NOCAP;
    if (strcmp(tokenStr, "PLAYLIST") == 0) return PLAYLIST;
    if (strcmp(tokenStr, "REPEAT") == 0) return REPEAT;
    if (strcmp(tokenStr, "TOPTIER") == 0) return TOPTIER;
    if (strcmp(tokenStr, "TOP") == 0) return TOP;
    if (strcmp(tokenStr, "EXTRA") == 0) return EXTRA;
    if (strcmp(tokenStr, "OTHERWISE") == 0) return OTHERWISE;
    return INVALID;
}


ASTNode* newNode(const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void nextToken() {
    while (currentIndex < tokenCount) {
        currentToken = allTokens[currentIndex++];

        if (currentToken.type != COMMENT) {
            return;
        } else {
            printf("Skipping comment: %s\n", currentToken.value);
        }
    }

    printf("End of token stream\n");
    currentToken.value = NULL;
    currentToken.type = INVALID;
}

void previousToken() {
    if (currentIndex > 1) {
        currentIndex--;
        currentToken = allTokens[currentIndex - 1];
    } else {
        printf("Already at the start of the token stream\n");
    }
}

void printParseTree(ASTNode *node, int indent, FILE *file) {
    if (!node) return;

    for (int i = 0; i < indent; i++) {
        fprintf(file, "    ");
    }
    fprintf(file, "%s", node->value);

    if (node->left || node->right) {
        fprintf(file, "(\n");

        if (node->left) {
            printParseTree(node->left, indent + 1, file);
        }

        if (node->right) {
            printParseTree(node->right, indent, file);
        }

        for (int i = 0; i < indent; i++) {
            fprintf(file, "    ");
        }
        fprintf(file, ")\n");
    } else {
        fprintf(file, "\n");
    }
}


ASTNode* parseProgram() {
    if (currentToken.type != TOPTIER) {
        printf("SYNTAX ERROR LINE %d: Expected toptier keyword. Skipping tokens until keyword is found.\n", currentToken.sheeshLine);
        nextToken(); 
        return NULL;
    }

    if (currentToken.type == TOPTIER) {
        ASTNode *node = newNode("<program>");
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            node->right = newNode(currentToken.value);
            nextToken();

            if (currentToken.type == DELIM_C_PAREN) {
                node->left->left = newNode(currentToken.value);
                nextToken();

                if (currentToken.type == DELIM_O_BRACE) {
                    node->left->right = newNode(currentToken.value);
                    nextToken();
                    node->right->left = parseBody();

                    if (currentToken.type == DELIM_C_BRACE) {
                        node->right->right = newNode(currentToken.value);
                        nextToken();

                        return node;
                    } else {
                        printf("SYNTAX ERROR LINE %d: Expected '}'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    }
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected '{'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected ')'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '('. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        }
    } 

    printf("SYNTAX ERROR LINE %d: Invalid program structure.\n", currentToken.sheeshLine);
    exit(1);
}

ASTNode* parseBody() {
    ASTNode *node = newNode("<body>");

    node->left = parseStmts();
    return node;
}

ASTNode* parseStmts() {
    ASTNode *node = NULL; 
    ASTNode *current = NULL;

    while (currentToken.type != DELIM_C_BRACE && currentToken.type != INVALID) {
        ASTNode *stmt = NULL;

        if (currentToken.type == IF) {
            stmt = newNode("<stmts>");
            stmt->left = parseCondStmt();
        } else if (currentToken.type == NUM || currentToken.type == VIBE || currentToken.type == DRIFT || currentToken.type == TEXT ||
            currentToken.type == SHORT || currentToken.type == LONG || currentToken.type == LEGIT) {
            stmt = newNode("<stmts>");
            stmt->left = parseDecStmt();
        } else if (currentToken.type == IDENTIFIER) {
            nextToken();
            if (currentToken.type == ASSIGNMENT_OPE) {
                previousToken();
                stmt = newNode("<stmts>");
                stmt->left = parseAssignStmt();
            } else {
                previousToken();
                stmt = newNode("<stmts>");
                stmt->left = parseExprStmt();
            }
        } else if (currentToken.type == REP || currentToken.type == MEANWHILE || currentToken.type == DO) {
            stmt = newNode("<stmts>");
                stmt->left = parseIterativeStmt();
        } else if (currentToken.type == INPUT) {
            stmt = newNode("<stmts>");
                stmt->left = parseInputStmt();
        } else if (currentToken.type == OUT || currentToken.type == OUTPUT) {
            stmt = newNode("<stmts>");
            stmt->left = parseOutputStmt();
        } else {
            stmt = newNode("<stmts>");
            stmt->left = parseExprStmt();
        }

        if (currentToken.type == INVALID) {
            printf("SYNTAX ERROR LINE %d: Encountered invalid token %s.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        if (!node) {
            node = stmt;
        } else {
            current->right = stmt;
        }
        current = stmt;
    }

    return node;
}

ASTNode* parseExprStmt() {
    ASTNode *node = newNode("<expr_stmt>");
    node->left = parseExpr();

    if (currentToken.type == DELIM_SEMCOL) {
        node->right = newNode(currentToken.value);
        nextToken();
    } else {
        printf("SYNTAX ERROR LINE %d: Expected ';'. Encountered token %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }
    return node;
}

ASTNode* parseExpr() {
    ASTNode* node = newNode("<expr>");
    node->left = parseAndExpr();

    if (!node) {
        printf("SYNTAX ERROR LINE %d: Invalid expression. Expected 'and' expression. Encountered token %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "||") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        
        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && currentToken.type != CONSTANT_NUM 
        && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN 
        && currentToken.type != LOGICAL_OPE) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '||' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        opNode->left = node;
        opNode->right = parseAndExpr();
        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Invalid expression after '||' operator. Expected right operand. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        node = opNode;
    }

    return node;
}

ASTNode* parseAndExpr() {
    ASTNode *node = newNode("<and_expr>");
    node->left = parseEqualityExpr();

    if (!node->left) {
        printf("SYNTAX ERROR LINE %d: Invalid expression in 'and' expression. Expected equality expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "&&") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseEqualityExpr();

        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Expected equality expression after '&&'. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node = opNode;
    }
    return node;
}

ASTNode* parseEqualityExpr() {
    ASTNode *node = newNode("<equality_expr>");
    node->left = parseRelationalExpr();

    if (!node->left) {
        printf("SYNTAX ERROR LINE %d: Expected relational expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    if (currentToken.type == RELATIONAL_OPE && (strcmp(currentToken.value, "==") == 0 || strcmp(currentToken.value, "!=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseRelationalExpr();

        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Expected relational expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        return opNode;
    }
    return node;
}

ASTNode* parseRelationalExpr() {
    ASTNode *node = newNode("<relational_expr>");
    node->left = parseAddSubExpr();
    
    if (!node->left) {
        printf("SYNTAX ERROR LINE %d: Expected addsub_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    if (currentToken.type == RELATIONAL_OPE && 
        (strcmp(currentToken.value, "<") == 0 || strcmp(currentToken.value, "<=") == 0 || strcmp(currentToken.value, ">") == 0 || strcmp(currentToken.value, ">=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;

        opNode->right = parseAddSubExpr();
        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Expected addsub_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        return opNode;
    }

    return node;
}

ASTNode* parseAddSubExpr() {
    ASTNode *node = newNode("<addsub_expr>");
    node->left = parseMultDivExpr();

    if (!node->left) {
        printf("SYNTAX ERROR LINE %d: Expected multdiv_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1); 
    }

    while (currentToken.type == ARITHMETIC_OPE && (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;

        opNode->right = parseMultDivExpr();
        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Expected multdiv_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node = opNode;
    }

    return node;
}

ASTNode* parseMultDivExpr() {
    ASTNode *node = newNode("<multdiv_expr>");
    node->left = parsePrimary();

    if (!node->left) {
        printf("SYNTAX ERROR LINE %d: Invalid token. Expected 'primary'. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "*") == 0 || strcmp(currentToken.value, "/") == 0 || strcmp(currentToken.value, "%") == 0 || strcmp(currentToken.value, "|") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;

        opNode->right = parsePrimary();
        if (!opNode->right) {
            printf("SYNTAX ERROR LINE %d: Expected primary expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node = opNode;
    }

    return node;
}

ASTNode* parsePrimary() {
    ASTNode *primaryNode = newNode("<primary>");

    if (currentToken.type == DELIM_SEMCOL) {
            printf("SYNTAX ERROR LINE %d: Unexpected ';' while parsing primary expression.\n", currentToken.sheeshLine);
        exit(1);
    }

    if (currentToken.type == CONSTANT_NUM || currentToken.type == CONSTANT_DRIFT 
        || currentToken.type == CONSTANT_VIBE || currentToken.type == CONSTANT_TEXT 
        || currentToken.type == CONSTANT_LEGIT || (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
            
        primaryNode->left = parseLiteral();
        return primaryNode;
    }

    if (currentToken.type == IDENTIFIER) {
        ASTNode *identifierNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == UNARY_OPE) {
            previousToken();
            identifierNode->left = parseUnaryVal();
        }

        if (currentToken.type == ASSIGNMENT_OPE) {
            printf("Found assignment operator: %s\n", currentToken.value);
            ASTNode *assignNode = newNode(currentToken.value);
            nextToken();
            assignNode->left = identifierNode;
            assignNode->right = parseExpr();
            primaryNode->left = assignNode;
            return primaryNode;
        }

        primaryNode->left = identifierNode;
        return primaryNode;
    }

    if (currentToken.type == DELIM_O_PAREN) {
        nextToken();
        ASTNode *exprNode = parseExpr();

        if (currentToken.type != DELIM_C_PAREN) {
            printf("SYNTAX ERROR LINE %d: Expected ')'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        nextToken();
        primaryNode->left = exprNode;
        return primaryNode;
        
    }  
    
    if (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "!") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
            opNode->right = parseIdentExpr();
            primaryNode->left = opNode;

            return primaryNode;
        } else if (currentToken.type == CONSTANT_LEGIT) {
            opNode->right = newNode(currentToken.value);
            primaryNode->left = opNode;
            nextToken();

            return primaryNode;
        } else {
            printf("SYNTAX ERROR LINE %d: Expected identifier or CONSTANT_LEGIT. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }
    
    if (currentToken.type == UNARY_OPE) {
        primaryNode->left = parseUnaryVal();

        return primaryNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <primary_expr>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}



ASTNode* parseLiteral() {
    // <literal> 		::= 	<num_val> | <drift_val> | CONSTANT_VIBE | CONSTANT_TEXT | CONSTANT_LEGIT
    ASTNode *node = newNode("<literal>");

    if (currentToken.type == ARITHMETIC_OPE && (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
            nextToken();

            if (currentToken.type == CONSTANT_NUM || currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
                previousToken();
                node->left = parseNumVal();

                return node;
            } 

            if (currentToken.type == CONSTANT_DRIFT) {
                previousToken();
                node->left = parseDriftVal();

                return node;
            } 
        }
    
    if (currentToken.type == CONSTANT_NUM) {
        node->left = parseNumVal();

        return node;
    } else if (currentToken.type == CONSTANT_DRIFT) {
        node->left = parseDriftVal();
            
        return node;
    } else if (currentToken.type == CONSTANT_VIBE || currentToken.type == CONSTANT_TEXT || currentToken.type == CONSTANT_LEGIT) {
        node->left = newNode(currentToken.value);
        nextToken();

        return node;
    }

    if (currentToken.type == INVALID) {
        printf("SYNTAX ERROR LINE %d: Unexpected end of input. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    printf("SYNTAX ERROR LINE %d: Invalid <literal>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseNumVal() {
    ASTNode *signNode = parseSign();

    if (currentToken.type == CONSTANT_NUM) {
        ASTNode *numNode = newNode("<num_val>");
        numNode->left = newNode(currentToken.value);
        nextToken();

        if (signNode) {
            ASTNode *node = newNode("<num_val>");
            node->left = signNode;
            node->right = numNode;
            return node;
        }
        return numNode;
    } else if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
        ASTNode *exprNode;
        exprNode = parseIdentExpr();

        if (signNode) {
            ASTNode *node = newNode("<num_val>");
            node->left = signNode;
            node->right = exprNode;
            return node;
        }
        return exprNode;
    } else {
        printf("SYNTAX ERROR LINE %d: Expected a constant numeric value for <num_val>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    printf("SYNTAX ERROR LINE %d: Invalid <num_val>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}


ASTNode* parseSign() {
    // <sign>   		::= 	‘-’ | ‘+’

    if (currentToken.type == ARITHMETIC_OPE && 
       (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *node = newNode("<sign>"); 
        node->left = newNode(currentToken.value);
        nextToken();

        return node;
    }

    return NULL;
}

ASTNode* parseDriftVal() {
    ASTNode *signNode = parseSign();

    if (currentToken.type == CONSTANT_DRIFT) {
        ASTNode *driftNode = newNode("<drift_val>");
        driftNode->left = newNode(currentToken.value);
        nextToken();

        if (signNode) {
            ASTNode *node = newNode("<drift_val>");
            node->left = signNode;
            node->right = driftNode;
            return node;
        }
        return driftNode;
    } else if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
        ASTNode *exprNode;
        if (currentToken.type == IDENTIFIER) {
            exprNode = parseIdentExpr();
        } else {
            nextToken();
            exprNode = parseExpr();

            if (currentToken.type != DELIM_C_PAREN) {
                printf("Syntax error: Missing closing parenthesis in <num_val>\n");
                exit(1);
            }
            nextToken();
        }

        if (signNode) {
            ASTNode *node = newNode("<drift_val>");
            node->left = signNode;
            node->right = exprNode;
            return node;
        }
        return exprNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <drift_val>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseUnaryVal() {
    if (currentToken.type == UNARY_OPE) {
        ASTNode *node = newNode("<unary_val>");
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == IDENTIFIER) {
            node->right = newNode(currentToken.value);
            nextToken();
            return node;
        } else {
            printf("SYNTAX ERROR LINE %d: Expected identifier after unary operator Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    } else if (currentToken.type == IDENTIFIER) {
        ASTNode *identifierNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == UNARY_OPE &&
            (strcmp(currentToken.value, "++") == 0 || strcmp(currentToken.value, "--") == 0)) {
            ASTNode *node = newNode("<unary_val>");
            node->left = identifierNode;
            node->right = newNode(currentToken.value);
            nextToken();
            return node;
        }

        return identifierNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <unary_val>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseIdentExpr() {
    ASTNode *node = newNode("<ident_expr>");

    if (currentToken.type == IDENTIFIER) {
        node->left = newNode(currentToken.value);
        nextToken();
        return node;
    } else if (currentToken.type == DELIM_O_PAREN) {
        nextToken();
        ASTNode *exprNode = parseExpr();

        if (currentToken.type != DELIM_C_PAREN) {
            printf("SYNTAX ERROR LINE %d: Expected ')' in <ident_expr>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        nextToken();
        return exprNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <ident_expr>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseDecStmt() {

    // <dec_stmt> 		::= 	<data_type> ( <initialization> |  IDENTIFIER <var_list> | <initialization> <var_list>) DELIM_SEMCOL


    ASTNode* stmtNode = newNode("<dec_stmt>");
    printf("Parsing Declaration Statement\n");

    ASTNode* dtNode = parseDataType();
    stmtNode->left = dtNode;

    if (currentToken.type == IDENTIFIER) {

        ASTNode* firstVar = parseInitialization(); 
        stmtNode->right = firstVar;

        if (currentToken.type == DELIM_COMMA) {
            ASTNode* varListNode = parseVarList(firstVar); 
            stmtNode->right = varListNode;
        }
    } else {
        printf("Syntax error: Expected an identifier or initialization after data type\n");
        exit(1);
    }

    if (currentToken.type == DELIM_SEMCOL) {
        nextToken(); 
    } else {
        printf("Syntax error: Missing semicolon at the end of declaration statement\n");
        exit(1);
    }

    return stmtNode;
}

ASTNode* parseVarList(ASTNode* firstVar) {
    ASTNode* currentNode = firstVar; 

    while (currentToken.type == DELIM_COMMA) {
        nextToken(); 

        if (currentToken.type == IDENTIFIER) {
            ASTNode* nextVarNode = parseInitialization(); 

            currentNode->right = nextVarNode; 

            currentNode = nextVarNode; 
        } else {
            printf("Syntax error: Expected an identifier or initialization after ','\n");
            exit(1);
        }
    }

    return firstVar; 
}

ASTNode* parseDataType() {

    if (currentToken.type == NUM || currentToken.type == DRIFT || currentToken.type == VIBE || 
        currentToken.type == TEXT || currentToken.type == SHORT || 
        currentToken.type == LONG || currentToken.type == LEGIT) {
        ASTNode* dtNode = newNode("<data_type>");
        dtNode->left = newNode(currentToken.value);
        nextToken(); 
        return dtNode;
    }

    printf("Syntax error: Invalid data type\n");
    exit(1);
}

ASTNode* parseInitialization() {

    ASTNode* initNode = newNode("<initialization>");
    
    if (currentToken.type == IDENTIFIER) {
        ASTNode* idNode = newNode(currentToken.value);
        nextToken(); 

        if (currentToken.type == ASSIGNMENT_OPE) {
            ASTNode* assignNode = newNode(currentToken.value); 
            nextToken(); 
            ASTNode* exprNode = parseExpr(); 
            assignNode->left = idNode;
            assignNode->right = exprNode;
            initNode->left = assignNode;
        } else {
            initNode->left = idNode;
        }
    } else {
        printf("Syntax error: Expected an identifier in initialization\n");
        exit(1);
    }

    return initNode;
}


ASTNode* parseAssignStmt() {

    //<assign_stmt> 	::=   	IDENTIFIER ASSIGNMENT_OPE <expr> DELIM_SEMCOL


    if (currentToken.type == IDENTIFIER) {
        ASTNode *node = newNode("<assign_stmt>");
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == ASSIGNMENT_OPE) {
            ASTNode *operatorNode = newNode(currentToken.value);
            operatorNode->left = node->left;
            nextToken();

            operatorNode->right = parseExpr();
            node->left = operatorNode;

            if (currentToken.type == DELIM_SEMCOL) {
                ASTNode* semicolonNode = newNode(currentToken.value);
                node->right = semicolonNode;
                nextToken();
                return node;
            }
            
        } else {
        printf("SYNTAX ERROR LINE %d: Expected assignment operator. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        if (currentToken.type != DELIM_SEMCOL) {
            printf("SYNTAX ERROR LINE %d: Expected ';' in <assign_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        nextToken();

        return node;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <assign_stmt>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseIfStmt() {
    if (strcmp(currentToken.value, "if") == 0) {
        ASTNode *node = newNode("<if_stmt>");
        
        node->left = newNode(currentToken.value);
        nextToken(); 

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode *conditionNode = newNode(currentToken.value);
            nextToken();

            conditionNode->left = parseExpr();

            if (currentToken.type == DELIM_C_PAREN) {
                conditionNode->right = newNode(currentToken.value);
                nextToken();

                node->right = conditionNode;

                if (currentToken.type == DELIM_O_BRACE) {
                    ASTNode *bodyNode = newNode(currentToken.value);
                    nextToken();

                    bodyNode->left = parseBody();

                    if (currentToken.type == DELIM_C_BRACE) {
                        bodyNode->right = newNode(currentToken.value);
                        nextToken();

                        conditionNode->right->right = bodyNode;
                        return node;
                    } else {
                        printf("SYNTAX ERROR LINE %d: Expected '}' in if body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected '{' in if body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected ')' for condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '(' for condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }

    printf("SYNTAX ERROR LINE %d: Invalid <if_stmt>\n", currentToken.sheeshLine);
    exit(1);
}

ASTNode* parseIfOtherStmt(ASTNode *node) {
    if (currentToken.type == OTHER) {
        node->right = newNode("<other>");
        node->right->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == DELIM_O_BRACE) {
            node->right->left->right = newNode(currentToken.value);
            nextToken();
            ASTNode *otherBody = parseBody();

            node->right->right = otherBody;

            if (currentToken.type == DELIM_C_BRACE) {
                node->right->right->right = newNode(currentToken.value);
                nextToken();
                return node;
            }
        }
    }

    printf("SYNTAX ERROR LINE %d: Invalid <other> stmt\n", currentToken.sheeshLine);
    exit(1);
}

ASTNode* parseCondStmt() {

    //<condi_stmt>  	::= 	<if_stmt>  |  <if_other_stmt>  | <ex_if_stmt> 

    ASTNode* node = newNode("<condi_stmt>");

    if (currentToken.type == IF) {
        ASTNode *ifNode = parseIfStmt();
        node->left = ifNode;

        if (currentToken.type == OTHER) {
            ifNode = newNode("<if_other_stmt");
            return parseIfOtherStmt(node);
        }

        return node; 
    }

    printf("SYNTAX ERROR LINE %d: Expected conditional statement (if, if_other). Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseIterativeStmt() {
    ASTNode* node = newNode("<iterative_stmt>");

    if (currentToken.type == REP) {
        ASTNode *loopNode = newNode("<rep_loop>");
        node->left = parseRepLoop();
    } else if (currentToken.type == DO) {
        ASTNode *loopNode = newNode("<dmw_loop>");
        node->left = parseDmwLoop();
    } else if (currentToken.type == MEANWHILE) {
        ASTNode *loopNode = newNode("<mw_loop>");
        node->left = parseMwLoop();
    } else {
        printf("SYNTAX ERROR LINE %d: Invalid iterative statement. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    return node;
}

ASTNode* parseRepLoop() {
    ASTNode* node = newNode("<rep_loop>");

    if (currentToken.type == REP) {
        ASTNode* loopNode = newNode(currentToken.value);
        node->left = loopNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* parenNode = newNode(currentToken.value);
            loopNode->left = parenNode;
            nextToken();

            ASTNode* initialNode = parseLoopInitial();
            parenNode->left = initialNode;

            ASTNode* exprNode = parseExpr();
            parenNode->right = exprNode;

            if (currentToken.type == DELIM_SEMCOL) {
                ASTNode* semicolonNode = newNode(currentToken.value);
                exprNode->right = semicolonNode;
                nextToken();

                ASTNode* updNode = parseUpdStmt();
                semicolonNode->right = updNode;

                if (currentToken.type == DELIM_C_PAREN) {
                    ASTNode* closeParenNode = newNode(currentToken.value);
                    updNode->right = closeParenNode;
                    nextToken();

                    if (currentToken.type == DELIM_O_BRACE) {
                        ASTNode* openBraceNode = newNode(currentToken.value);
                        closeParenNode->right = openBraceNode;
                        nextToken();

                        ASTNode* loopBody = parseStmts();
                        openBraceNode->right = loopBody;

                        if (currentToken.type == DELIM_C_BRACE) {
                            ASTNode* closeBraceNode = newNode(currentToken.value);
                            loopBody->right = closeBraceNode;
                            nextToken();

                            return node;
                        } else {
                            printf("SYNTAX ERROR LINE %d: Expected '}' for loop body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                            exit(1);
                        }
                    } else {
                        printf("SYNTAX ERROR LINE %d: Expected '{' for loop body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected ')' after <upd_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected ';' after <loop_initial>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '(' after 'rep'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }

    printf("SYNTAX ERROR LINE %d: Invalid <rep_loop>\n", currentToken.sheeshLine);
    exit(1);
}


ASTNode* parseLoopInitial() {
    ASTNode* node = newNode("<loop_initial>");

    if (currentToken.type == NUM || currentToken.type == VIBE || currentToken.type == DRIFT || currentToken.type == TEXT || currentToken.type == SHORT
        || currentToken.type == LONG || currentToken.type == LEGIT) {
        node->left = parseDecStmt();
    } else if (currentToken.type == IDENTIFIER) {
        node->left = parseAssignStmt();
    } else {
        printf("SYNTAX ERROR LINE %d: Invalid <loop_initial> statement. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    return node;
}


ASTNode* parseUpdStmt() {
    ASTNode* node = newNode("<upd_stmt>");

    if (currentToken.type == UNARY_OPE) {
        node->left = parseUnaryVal();
        return node;
    } 

    if (currentToken.type == IDENTIFIER) {
        node->left = parseExpr();
        return node;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <upd_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);

    return NULL;
}

ASTNode* parseMwLoop() {
    ASTNode* node = newNode("<mw_loop>");

    if (currentToken.type == MEANWHILE) {
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            node->right = newNode(currentToken.value);
            nextToken();

            node->right->left = parseExpr(); 

            if (currentToken.type != DELIM_C_PAREN) {
                printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            node->right->right = newNode(currentToken.value);
            nextToken();

            if (currentToken.type == DELIM_O_BRACE) {
            ASTNode* braceNode = newNode(currentToken.value);
            nextToken();

            node->right->right->left = braceNode;
            braceNode->left = parseBody();

            if (currentToken.type != DELIM_C_BRACE) {
                printf("SYNTAX ERROR LINE %d: Expected '}' to close <mw_loop> body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            ASTNode* closingBraceNode = newNode(currentToken.value); 
            braceNode->right = closingBraceNode;

            node->right->right->right = braceNode;
            nextToken();
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '{' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        } else {
            printf("SYNTAX ERROR LINE %d: Expected '(' after meanwhile. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    } else {
        printf("SYNTAX ERROR LINE %d: Expected 'meanwhile'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    return node;
}

ASTNode* parseDmwLoop() {
    ASTNode* node = newNode("<dmw_loop>");

    if (currentToken.type == DO) {
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == DELIM_O_BRACE) {
            ASTNode* braceNode = newNode(currentToken.value);
            node->right = braceNode;
            nextToken();
            braceNode->left = parseBody();

            if (currentToken.type != DELIM_C_BRACE) {
                printf("SYNTAX ERROR LINE %d: Expected '}' to close the block. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            braceNode->right = newNode(currentToken.value);
            nextToken();

            if (currentToken.type == MEANWHILE) {
                braceNode->right->left = newNode(currentToken.value);
                nextToken();

                if (currentToken.type == DELIM_O_PAREN) {
                    braceNode->right->left->left = newNode(currentToken.value);
                    nextToken();

                    ASTNode* exprNode = parseExpr();
                    braceNode->right->left->right = exprNode;

                    if (currentToken.type != DELIM_C_PAREN) {
                        printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }

                    exprNode->right = newNode(currentToken.value);
                    nextToken();

                    if (currentToken.type == DELIM_SEMCOL) {
                        exprNode->right->left = newNode(currentToken.value);
                        nextToken();
                    } else {
                        printf("SYNTAX ERROR LINE %d: Expected ';' after meanwhile loop. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected '(' after meanwhile. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected meanwhile after do block. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '{' after do. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }
    return node;
}

ASTNode* parseInputStmt() {
    ASTNode* node = newNode("<input_stmt>");
    if (currentToken.type == INPUT) {
        ASTNode* inputNode = newNode(currentToken.value);
        node->left = inputNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* openParen = newNode(currentToken.value);
            inputNode->right = openParen;
            nextToken();       
            openParen->left = parseInput();

            if (currentToken.type == DELIM_C_PAREN) {
                ASTNode* closeParen = newNode(currentToken.value);
                openParen->right = closeParen;
                nextToken();
                
                if (currentToken.type == DELIM_SEMCOL) {
                    ASTNode* semicolonNode = newNode(currentToken.value);
                    closeParen->right = semicolonNode;
                    nextToken();

                    return node;
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected ')' in <input_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '(' in <input_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }

    printf("SYNTAX ERROR LINE %d: Invalid <input_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseOutputStmt() {
    // <output_stmt> ::= “out” DELIM_O_PAREN <output> DELIM_C_PAREN DELIM_SEMCOL
    ASTNode* node = newNode("<output_stmt>");
    if (currentToken.type == OUT || currentToken.type == OUTPUT) {
        ASTNode* outNode = newNode(currentToken.value);
        node->left = outNode;
        nextToken();
        
        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* openParen = newNode("("); 
            outNode->right = openParen;
            nextToken();         
            openParen->left = parseOutput();

            if (currentToken.type == DELIM_C_PAREN) {
                ASTNode* closeParen = newNode(")");
                openParen->right = closeParen;
                nextToken();
                
                if (currentToken.type == DELIM_SEMCOL) {
                    ASTNode* semicolonNode = newNode(";");
                    closeParen->right = semicolonNode;
                    nextToken();
                    return node;
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected ')' in <output_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected '(' in <output_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }
    
    printf("SYNTAX ERROR LINE %d: Invalid <output_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseInput() {
    // <input> ::= CONSTANT_TXTFS { DELIM_COMMA [ "&" <identifier> ] }
    ASTNode* node = newNode("<input>");
    if (currentToken.type == CONSTANT_TXTFS) {
        node->left = newNode(currentToken.value);
        nextToken();

        ASTNode* current = node;
        while (currentToken.type == DELIM_COMMA) {
            ASTNode* commaNode = newNode(",");
            current->right = commaNode;
            current = commaNode;

            nextToken();
            if (strcmp(currentToken.value, "&") == 0) {
                ASTNode* andNode = newNode("&");
                commaNode->right = andNode;
                nextToken();
                if (currentToken.type == IDENTIFIER) {
                    andNode->left = newNode(currentToken.value);
                    nextToken();
                } else {
                printf("SYNTAX ERROR LINE %d: Expected identifier after '&'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else if (currentToken.type == IDENTIFIER) {
                ASTNode* idNode = newNode(currentToken.value);
                commaNode->right = idNode;
                nextToken();
            } else {
                printf("SYNTAX ERROR LINE %d: Expected '&' or identifier after ','. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        }
        return node;
    }

    printf("SYNTAX ERROR LINE %d: Invalid input.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseOutput() {
    // <output> ::= CONSTANT_TEXT | CONSTANT_TXTFS { DELIM_COMMA <expr> }
    ASTNode* node = newNode("<output>");
    if (currentToken.type == CONSTANT_TEXT || currentToken.type == CONSTANT_TXTFS) {
        node->left = newNode(currentToken.value);
        nextToken();

        ASTNode* current = node;
        while (currentToken.type == DELIM_COMMA) {
            ASTNode* commaNode = newNode(",");
            current->right = commaNode;
            nextToken();
            commaNode->left = parseExpr();
            current = commaNode;
        }
        return node;
    }

    printf("SYNTAX ERROR LINE %d: Invalid output.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

void parse(FILE* file) {
    initializeParser("symbol_table.txt");
    nextToken();
    ASTNode* root = parseProgram();
    printf("See parse_tree.txt for parse tree.");
    
    printParseTree(root, 0, file);
    printf("\n");
}