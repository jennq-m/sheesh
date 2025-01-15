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
    Token token; // Declare the Token structure

    TokenType reservedWord = checkReservedWord(temp);
    Token rwToken;

    if (keyword) { // If keyword is valid
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


void nextToken() {
    while (currentIndex < tokenCount) {
        currentToken = allTokens[currentIndex++];
        printf("Next token: %s (%d)\n", currentToken.value, currentToken.type);

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
        printf("Previous token: %s (%d)\n", currentToken.value, currentToken.type);
    } else {
        printf("Already at the start of the token stream\n");
    }
}

ASTNode* newNode(const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printParseTree(ASTNode *node, int indent, FILE *file) {
    if (!node) return;

    for (int i = 0; i < indent; i++) {
        fprintf(file, "    ");
    }

    if (node->left || node->right) {
        fprintf(file, "%s(\n", node->value);

        if (node->left) {
            printParseTree(node->left, indent + 1, file);
        }
        if (node->right) {
            printParseTree(node->right, indent + 1, file);
        }

        for (int i = 0; i < indent; i++) {
            fprintf(file, "    ");
        }
        fprintf(file, ")\n");
    } else {
        fprintf(file, "%s\n", node->value);
    }
}

ASTNode* parseProgram() {
    printf("Entering parseProgram\n");

    if (currentToken.type != TOPTIER) {
        printf("SYNTAX ERROR LINE %d: Expected toptier keyword. Skipping tokens until keyword is found.\n", currentToken.sheeshLine);
        nextToken(); 
        return NULL;
    }

    if (currentToken.type == TOPTIER) {
        printf("Matched toptier\n");
        ASTNode *node = newNode("<program>");
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            printf("Matched opening parenthesis\n");
            nextToken();

            if (currentToken.type == DELIM_C_PAREN) {
                printf("Matched closing parenthesis\n");
                nextToken();

                if (currentToken.type == DELIM_O_BRACE) {
                    printf("Matched opening brace\n");
                    nextToken();
                    node->left = parseBody();

                    if (currentToken.type == DELIM_C_BRACE) {
                        printf("Matched closing brace\n");
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
    printf("Syntax error: Invalid program structure\n");
    exit(1);
}

ASTNode* parseBody() {
    printf("Entering parseBody\n");
    ASTNode *node = newNode("<body");

    node->left = parseStmts();
    return node;
}

ASTNode* parseStmts() {
    printf("Entering parseStmts\n");
    ASTNode *node = NULL;  // Root for statements
    ASTNode *current = NULL;

    while (currentToken.type != DELIM_C_BRACE && currentToken.type != INVALID) {
        ASTNode *stmt = NULL;

        // Check for conditional statements (if, if_other, ex_if)
        if (strcmp(currentToken.value, "if") == 0) {
            stmt = parseCondStmt();  // Parse the conditional statement (if, if_other, ex_if)

        // Check for declaration statements
        } else if (currentToken.type == NUM || currentToken.type == VIBE || currentToken.type == DRIFT || currentToken.type == TEXT ||
            currentToken.type == SHORT || currentToken.type == LONG || currentToken.type == LEGIT) {
            stmt = parseDecStmt();

        // Check for assignment statements
        } else if (currentToken.type == IDENTIFIER) {
            nextToken();
            if (currentToken.type == ASSIGNMENT_OPE) {
                previousToken();
                stmt = parseAssignStmt();
            } else {
                previousToken();
                stmt = parseExprStmt();
            }

        // Check for iterative statements
        } else if (currentToken.type == REP || currentToken.type == MEANWHILE || currentToken.type == DO) {
            stmt = parseIterativeStmt();

        // Check for input statements
        } else if (strcmp(currentToken.value, "input") == 0) {
            stmt = parseInputStmt();

        // Check for output statements
        } else if (strcmp(currentToken.value, "out") == 0) {
            stmt = parseOutputStmt();

        // Handle other statements (e.g., expression statements)
        } else {
            stmt = parseExprStmt();
        }

        // Chain the parsed statement into the statement list
        if (!node) {
            node = stmt;  // First statement becomes the root
        } else {
            current->right = stmt;  // Chain subsequent statements
        }
        current = stmt;  // Move to the latest statement
    }

    return node;
}


ASTNode* parseExprStmt() {
    printf("Entering parseExprStmt\n");
    ASTNode *node = parseExpr();
    if (currentToken.type == DELIM_SEMCOL) {
        printf("Matched semicolon\n");
        nextToken();
    } else {
        printf("SYNTAX ERROR LINE %d: Expected ';'. Encountered token %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }
    return node;
}

ASTNode* parseExpr() {
    printf("Entering parseExpr. Current token: %s (Type: %d, Line: %d)\n", currentToken.value, currentToken.type, currentToken.sheeshLine);
    ASTNode *node = NULL;

    node = parseAndExpr();
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
    printf("Checking <parseAndExpr>...\n");
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
    printf("Checking <parseEqualityExpr>...\n");
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
    printf("Checking <parseRelationalExpr>...\n");
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
    printf("Checking <parseAddSubExpr>...\n");
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
    printf("Entering parseMultDivExpr. Current token: %s (Type: %d, Line: %d)\n", currentToken.value, currentToken.type, currentToken.sheeshLine);
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
    printf("Entering parsePrimary. Current token: %s (Type: %d, Line: %d)\n", currentToken.value, currentToken.type, currentToken.sheeshLine);

    ASTNode *primaryNode = newNode("<primary>");

    if (currentToken.type == DELIM_SEMCOL) {
            printf("SYNTAX ERROR LINE %d: Unexpected ';' while parsing primary expression.\n", currentToken.sheeshLine);
        exit(1);
    }

    if (currentToken.type == CONSTANT_NUM || currentToken.type == CONSTANT_DRIFT 
        || currentToken.type == CONSTANT_VIBE || currentToken.type == CONSTANT_TEXT 
        || currentToken.type == CONSTANT_LEGIT || (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
            
        printf("Parsing literal: %s\n", currentToken.value);
        primaryNode->left = parseLiteral();
        return primaryNode;
    }

    if (currentToken.type == IDENTIFIER) {
        printf("Parsing identifier: %s\n", currentToken.value);
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
        printf("Parsing parenthesized expression\n");
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
        printf("Parsing unary operator: %s\n", currentToken.value);
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
            printf("Unary operator applied to: %s\n", currentToken.value);
            opNode->right = parseIdentExpr();
            primaryNode->left = opNode;

            return primaryNode;
        } else if (currentToken.type == CONSTANT_LEGIT) {
            printf("Unary operator applied to: %s\n", currentToken.value);
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

    printf("Entering parseLiteral. Current token: %s (Type: %d, Line: %d)\n", currentToken.value, currentToken.type, currentToken.sheeshLine);

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
    printf("Parse Dec Statement\n");
    ASTNode *stmtNode = newNode("<dec_stmt>");
    
    ASTNode *dtNode = parseDataType();  
    stmtNode->left = dtNode;

    ASTNode *varListNode = parseVarList();  
    stmtNode->right = varListNode;

    if (currentToken.type == DELIM_SEMCOL) {
        nextToken();
    } else {
        printf("SYNTAX ERROR LINE %d: Expected ';' in <dec_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    return stmtNode;
}

ASTNode* parseDataType() {
    // <data_type> ::= "num" | "drift" | "vibe" | "text" | "short" | "long" | "legit"
    ASTNode* dtNode = newNode("<data_type>");

    if (currentToken.type == NUM || currentToken.type == VIBE || currentToken.type == DRIFT || currentToken.type == TEXT || currentToken.type == SHORT
        || currentToken.type == LONG || currentToken.type == LEGIT) {
        dtNode->left = newNode(currentToken.value);
        nextToken();
        return dtNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid data type. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseInitialization() {
    printf("Enter parseInitialization\n");
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
            idNode = assignNode;
        }

        initNode->left = idNode;
        return initNode;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <initialization>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseVarList() {
    printf("Parse Variable List\n");
    ASTNode *varListNode = newNode("<var_list>");
    ASTNode *headNode = NULL;
    ASTNode *currentNode = NULL; 

    while (currentToken.type == IDENTIFIER || currentToken.type == DELIM_COMMA) {
        if (currentToken.type == DELIM_COMMA) {
            currentNode->left = newNode(currentToken.value);
            nextToken();
        }

        if (currentToken.type == IDENTIFIER) {
            ASTNode *idNode = newNode(currentToken.value);
            nextToken();

            if (currentToken.type == ASSIGNMENT_OPE && strcmp(currentToken.value, "=") == 0) {
                ASTNode *assignNode = newNode(currentToken.value);
                nextToken();
                ASTNode *exprNode = parseExpr(); 
                assignNode->left = idNode;
                assignNode->right = exprNode;
                idNode = assignNode;
            }

            if (headNode == NULL) {
                headNode = idNode;
                currentNode = headNode;
            } else {
                currentNode->right = idNode;
                currentNode = idNode;
            }
        }
    }

    if (!headNode) {
        printf("SYNTAX ERROR LINE %d: Invalid <var_list>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    varListNode->left = headNode;
    return varListNode;
}

ASTNode* parseAssignStmt() {
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
        ASTNode *node = newNode("if_stmt");

        // Debug print to check token after 'if'
        printf("Current token after 'if': %s\n", currentToken.value);
        nextToken();  // Consume 'if'

        // Debug print to check token after consuming 'if'
        printf("Current token after consuming 'if': %s\n", currentToken.value);

        // Lookahead for opening parenthesis
        if (currentToken.type == DELIM_O_PAREN) {
            printf("Found opening parenthesis '('\n");
            nextToken();  // Consume '('

            node->left = parseExpr();  // Parse the condition expression

            if (currentToken.type == DELIM_C_PAREN) {
                printf("Found closing parenthesis ')'\n");
                nextToken();  // Consume ')'

                // Look for opening brace '{'
                if (currentToken.type == DELIM_O_BRACE) {
                    nextToken();  // Consume '{'
                    node->right = parseBody();  // Parse the body of the if statement

                    // Ensure the body ends with closing brace '}'
                    if (currentToken.type == DELIM_C_BRACE) {
                        nextToken();  // Consume '}'
                        return node;
                    } else {
                        printf("Syntax error: Missing closing brace for if body\n");
                        exit(1);
                    }
                } else {
                    printf("Syntax error: Missing opening brace for if body\n");
                    exit(1);
                }
            } else {
                printf("Syntax error: Missing closing parenthesis for condition\n");
                exit(1);
            }
        } else {
            printf("Syntax error: Missing opening parenthesis for condition\n");
            exit(1);
        }
    }

    printf("Syntax error: Invalid if statement\n");
    exit(1);
}




ASTNode* parseIfOtherStmt(ASTNode *ifNode) {
    // Check for the "other" keyword
    if (strcmp(currentToken.value, "other") == 0) {
        nextToken();  // Consume 'other'

        // Parse the body of the other block
        if (currentToken.type == DELIM_O_BRACE) {
            nextToken();  // Consume '{'
            ASTNode *otherBody = parseBody();  // Parse the body of the 'other' block

            // Attach the other block as a right child of the ifNode
            ifNode->right = otherBody;

            // Check for closing brace
            if (currentToken.type == DELIM_C_BRACE) {
                nextToken();  // Consume '}'
                return ifNode;
            }
        }
    }

    // Syntax error if not found
    printf("Syntax error: Invalid other statement\n");
    exit(1);
}

ASTNode* parseCondStmt() {
    printf("Entering CondStmt\n");

    // Check for "if" statement
    if (strcmp(currentToken.value, "if") == 0) {
        // Parse the "if" statement
        ASTNode *ifNode = parseIfStmt();  // Parse the standard if statement

        // After parsing the if statement, check if 'other' follows
        if (strcmp(currentToken.value, "other") == 0) {
            return parseIfOtherStmt(ifNode);  // Parse the 'other' block and chain it to the if statement
        }

        return ifNode;  // Return the parsed if statement if no 'other' is found
    }

    // If none of the above conditions matched, print an error
    printf("Syntax error: Expected conditional statement (if, if_other, ex_if)\n");
    exit(1);
}

ASTNode* parseIterativeStmt() {
    printf("Entering iterative statements\n");
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
            nextToken();
            
            ASTNode* initialNode = parseLoopInitial();
            node->right = initialNode;
            ASTNode *exprNode = parseExpr();
            initialNode->right = exprNode;

                if (currentToken.type == DELIM_SEMCOL) {
                    nextToken();
                    ASTNode *updNode = parseUpdStmt();
                    exprNode->right = updNode;

                    if (currentToken.type == DELIM_C_PAREN) {
                        nextToken();
                        if (currentToken.type == DELIM_O_BRACE) {
                            nextToken();
                            ASTNode* loopBody = parseStmts();
                            updNode->right = loopBody;

                            if (currentToken.type == DELIM_C_BRACE) {
                                nextToken();

                                return node;
                            } else {
                                printf("SYNTAX ERROR LINE %d: Expected '}' for loop body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                                exit(1);
                            }
                        } else {
                            printf("SYNTAX ERROR LINE %d: Expected '{' for loop <body>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
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
            printf("Syntax error: Missing opening parenthesis '(' after 'rep'\n");
            exit(1);
        }
    }

    return NULL;
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
    printf("Parsing 'meanwhile' loop. Current token: %s\n", currentToken.value);

    if (currentToken.type == MEANWHILE) {
        nextToken();
        printf("Consumed 'meanwhile'. Next token: %s\n", currentToken.value);

        if (currentToken.type == DELIM_O_PAREN) {
            nextToken();
            printf("Consumed '('. Parsing condition expression.\n");

            node->left = parseExpr(); 
            printf("Parsed condition expression. Next token: %s\n", currentToken.value);

            if (currentToken.type != DELIM_C_PAREN) {
                printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            nextToken();
            printf("Consumed ')'. Next token: %s\n", currentToken.value);

            if (currentToken.type == DELIM_O_BRACE) {
                nextToken();
                printf("Consumed '{'. Parsing loop body.\n");

                node->right = parseBody();
                printf("Parsed loop body. Next token: %s\n", currentToken.value);

                if (currentToken.type != DELIM_C_BRACE) {
                    printf("SYNTAX ERROR LINE %d: Expected '}' to close <mw_loop> body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
                
                nextToken();
                printf("Consumed '}'. Next token: %s\n", currentToken.value);
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
    printf("Parsing 'do-meanwhile' loop\n");

    if (strcmp(currentToken.value, "do") == 0) {
        nextToken();

        if (currentToken.type == DELIM_O_BRACE) {
            nextToken();
            node->left = parseBody(); 

            if (currentToken.type != DELIM_C_BRACE) {
                printf("SYNTAX ERROR LINE %d: Expected '}' to close the block. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
            nextToken(); 

            if (currentToken.type == MEANWHILE) {
                nextToken();

                if (currentToken.type == DELIM_O_PAREN) {
                    nextToken();
                    node->right = parseExpr();

                    if (currentToken.type != DELIM_C_PAREN) {
                        printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }
                    nextToken();

                    if (currentToken.type == DELIM_SEMCOL) {
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
    if (strcmp(currentToken.value, "input") == 0) {
        ASTNode* inputNode = newNode("<input>");
        node->left = inputNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* openParen = newNode("(");
            inputNode->right = openParen;
            nextToken();       
            openParen->left = parseInput();

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
    if (strcmp(currentToken.value, "out") == 0) {
        ASTNode* outNode = newNode("<out>");
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
    nextToken();
    ASTNode *ast = parseProgram();
    printf("See parse_tree.txt for parse tree.");
    
    printParseTree(ast, 0, file);
    printf("\n");
}
