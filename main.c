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

    FILE *outputSheesh = fopen("test.txt", "w");
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

    // printf("\nTotal Tokens Created: %d\n", tokenCount);
    // for (int i = 0; i < tokenCount; i++) {
    //     printf("Token %d: Lexeme: %-15s Token Type: %s\n", i + 1, allTokens[i].value, typeToString(allTokens[i].type));
    // }

    parse();

    fclose(file);
    fclose(outputSheesh);

    printf("See symbolTable.txt for lexical analysis.\n");
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
                } else if (ch == '\'' || ch == '"') {
                    state = 3; 
                } else if (strchr("+-*/%=!<>&|^$", ch)) { 
                    state = 4; 
                } else if (strchr(",;(){}[]", ch)) { 
                    state = 5; 
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

            case 3:
                if (ch == '\'' || ch == '"') {
                    return newToken(sheeshLexeme, CONSTANT, sheeshLine);
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
        if (checkKeyword(sheeshLexeme)) {
            return newToken(sheeshLexeme, KEYWORD, sheeshLine);
        }

        if (checkReservedWord(sheeshLexeme)) {
            if (!(strcmp(sheeshLexeme, "cap")) || !(strcmp(sheeshLexeme, "nocap"))) {
                return newToken(sheeshLexeme, CONSTANDRESERVED, sheeshLine);
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
            while ((isalnum(sheeshLine[i + 1]) || ispunct(sheeshLine[i + 1])) && !(strchr("+-*/%=!<>&|^()\"{}[];,", sheeshLine[i + 1]))) {
                temp[tempMarker++] = sheeshLine[++i];
            }
            temp[tempMarker] = '\0';

            if (checkKeyword(temp)) {
                Token token = newToken(temp, KEYWORD, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %-15s\n", token.sheeshLine, token.value, token.value);
                 
            } else if (checkReservedWord(temp)) {
                if (!(strcmp(temp, "cap")) || !(strcmp(temp, "nocap"))) {
                    Token token = newToken(temp, CONSTANDRESERVED, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, token.value);
                    
                } else {
                    Token token = newToken(temp, RESERVED_WORD, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, token.value);
                    
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
                if (token.type == CONSTANT) {
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                } else {
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                }
                  
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

void nextToken() {
    if (currentIndex < tokenCount) {
        currentToken = allTokens[currentIndex++];
    } else {
        currentToken.value = NULL;
        currentToken.type = INVALID;
    }
}

ASTNode* newNode(const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printParseTree(ASTNode *node) {
    if (!node) return;
    printf("(");
    printf("%s", node->value);
    printParseTree(node->left);
    printParseTree(node->right);
    printf(")");
}

ASTNode* parseProgram() {
    if (currentToken.type == RESERVED_WORD && strcmp(currentToken.value, "toptier") == 0) {
        ASTNode *node = newNode("program");
        nextToken();
        if (currentToken.type == DELIM_O_PAREN) {
            nextToken();
            if (currentToken.type == DELIM_C_PAREN) {
                nextToken();
                if (currentToken.type == DELIM_O_BRACE) {
                    nextToken();
                    node->left = parseBody();
                    if (currentToken.type == DELIM_C_BRACE) {
                        nextToken();
                        return node;
                    }
                }
            }
        }
    }
    printf("Syntax error: Invalid program structure\n");
    exit(1);
}

ASTNode* parseBody() {
    ASTNode *node = newNode("body");
    node->left = parseStmts();
    return node;
}

ASTNode* parseStmts() {
    ASTNode *node = newNode("stmts");
    node->left = parseExprStmt();
    while (currentToken.type == DELIM_SEMCOL) {
        nextToken();
        ASTNode *nextStmt = newNode("stmts");
        nextStmt->left = parseExprStmt();
        node->right = nextStmt;
    }
    return node;
}

ASTNode* parseExprStmt() {
    ASTNode *node = newNode("expr_stmt");
    node->left = parseExpr();
    if (currentToken.type == DELIM_SEMCOL) {
        nextToken();
        return node;
    }
    printf("Syntax error: Missing semicolon\n");
    exit(1);
}

ASTNode* parseExpr() {
    ASTNode *node = newNode("expr");
    node->left = parseAndExpr();
    while (currentToken.type == ARITHMETIC_OPE && strcmp(currentToken.value, "||") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseAndExpr() {
    ASTNode *node = newNode("and_expr");
    node->left = parseEqualityExpr();
    while (currentToken.type == ARITHMETIC_OPE && strcmp(currentToken.value, "&&") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseEqualityExpr() {
    ASTNode *node = newNode("equality_expr");
    node->left = parseRelationalExpr();
    if (currentToken.type == ARITHMETIC_OPE && (strcmp(currentToken.value, "==") == 0 || strcmp(currentToken.value, "!=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        return opNode;
    }
    return node;
}

ASTNode* parseRelationalExpr() {
    ASTNode *node = newNode("relational_expr");
    node->left = parseAddSubExpr();
    if (currentToken.type == ARITHMETIC_OPE && (strcmp(currentToken.value, "<") == 0 || strcmp(currentToken.value, "<=" ) == 0 || strcmp(currentToken.value, ">") == 0 || strcmp(currentToken.value, ">=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        return opNode;
    }
    return node;
}

ASTNode* parseAddSubExpr() {
    ASTNode *node = newNode("addsub_expr");
    node->left = parseMultDivExpr();
    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseMultDivExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseMultDivExpr() {
    ASTNode *node = newNode("primary");
    node->left = parsePrimary();
    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "*") == 0 || strcmp(currentToken.value, "/") == 0 || strcmp(currentToken.value, "%") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parsePrimary();
        node = opNode;
    }
    return node;
}

ASTNode* parsePrimary() {
    printf("Entering <primary>\n");

    if (currentToken.type == CONSTANT_NUM || 
        currentToken.type == CONSTANT_DRIFT || 
        currentToken.type == CONSTANT_VIBE || 
        currentToken.type == CONSTANT_TEXT || 
        (currentToken.type == ARITHMETIC_OPE && 
        (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0))) {
        return parseLiteral();
    } else if (currentToken.type == IDENTIFIER) {
        ASTNode *node = newNode(currentToken.value);
        nextToken();
        return node;
    } else if (currentToken.type == DELIM_O_PAREN) {
        nextToken();
        ASTNode *node = parseExpr();
        if (currentToken.type != DELIM_C_PAREN) {
            printf("Syntax error: Missing closing parenthesis\n");
            exit(1);
        }
        nextToken();
        return node;
    } else if (currentToken.type == UNARY_OPE) {
        return parseUnaryVal();
    }

    printf("Syntax error: Invalid primary expression\n");
    exit(1);
}

ASTNode* parseDecStmt() {
    if (currentToken.type == IDENTIFIER) {
        ASTNode *node = newNode("dec_stmt");
        node->left = newNode(currentToken.value);
        nextToken();
        if (currentToken.type == ASSIGNMENT_OPE) {
            nextToken();
            node->right = parseExpr();
        }
        if (currentToken.type != DELIM_SEMCOL) {
            printf("Syntax error: Missing semicolon in declaration\n");
            exit(1);
        }
        nextToken();
        return node;
    }
    printf("Syntax error: Invalid declaration statement\n");
    exit(1);
}

ASTNode* parseAssignStmt() {
    if (currentToken.type == IDENTIFIER) {
        ASTNode *node = newNode("assign_stmt");
        node->left = newNode(currentToken.value);
        nextToken();
        if (currentToken.type == ASSIGNMENT_OPE) {
            nextToken();
            node->right = parseExpr();
        } else {
            printf("Syntax error: Expected assignment operator\n");
            exit(1);
        }
        if (currentToken.type != DELIM_SEMCOL) {
            printf("Syntax error: Missing semicolon in assignment\n");
            exit(1);
        }
        nextToken();
        return node;
    }
    printf("Syntax error: Invalid assignment statement\n");
    exit(1);
}

ASTNode* parseCondStmt() {
    if (currentToken.type == KEYWORD && strcmp(currentToken.value, "if") == 0) {
        ASTNode *node = newNode("if_stmt");
        nextToken();
        if (currentToken.type == DELIM_O_PAREN) {
            nextToken();
            node->left = parseExpr();
            if (currentToken.type == DELIM_C_PAREN) {
                nextToken();
                if (currentToken.type == DELIM_O_BRACE) {
                    nextToken();
                    node->right = parseBody();
                    if (currentToken.type == DELIM_C_BRACE) {
                        nextToken();
                        return node;
                    }
                }
            }
        }
        printf("Syntax error: Invalid if statement\n");
        exit(1);
    }
    printf("Syntax error: Expected if statement\n");
    exit(1);
}

ASTNode* parseIterativeStmt() {
    if (currentToken.type == KEYWORD && strcmp(currentToken.value, "rep") == 0) {
        ASTNode *node = newNode("rep_stmt");
        nextToken();
        if (currentToken.type == DELIM_O_PAREN) {
            nextToken();
            if (currentToken.type != DELIM_C_PAREN) {
                node->left = parseExpr();
            }
            if (currentToken.type == DELIM_C_PAREN) {
                nextToken();
                if (currentToken.type == DELIM_O_BRACE) {
                    nextToken();
                    node->right = parseBody();
                    if (currentToken.type == DELIM_C_BRACE) {
                        nextToken();
                        return node;
                    }
                }
            }
        }
        printf("Syntax error: Invalid rep statement\n");
        exit(1);
    }
    printf("Syntax error: Expected iterative statement\n");
    exit(1);
}

ASTNode* parseUnaryVal() {
    if (currentToken.type == UNARY_OPE) {
        ASTNode *node = newNode(currentToken.value);
        nextToken();
        if (currentToken.type == IDENTIFIER) {
            node->left = newNode(currentToken.value);
            nextToken();
            return node;
        }
        printf("Syntax error: Expected identifier after unary operator\n");
        exit(1);
    }
    printf("Syntax error: Invalid unary value\n");
    exit(1);
}

ASTNode* parseLiteral() {
    ASTNode *node = newNode("literal");
    
    if (currentToken.type == CONSTANT_NUM || 
        (currentToken.type == ARITHMETIC_OPE && 
        (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0))) {
            node->left = parseNumVal();
            return node;
    } else if (currentToken.type == CONSTANT_DRIFT || 
               (currentToken.type == ARITHMETIC_OPE && 
               (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0))) {
            node->left = parseDriftVal();
            return node;
    } else if (currentToken.type == CONSTANT_VIBE || 
               currentToken.type == CONSTANT_TEXT) {
        ASTNode *node = newNode(currentToken.value);
        printf("Matched <literal>: %s\n", node->value);
        nextToken();
        return node;
    }

    printf("Syntax error: Invalid <literal>\n");
    exit(1);
}

ASTNode* parseNumVal() {
    ASTNode *signNode = parseSign();
    if (currentToken.type == CONSTANT_NUM) {
        ASTNode *numNode = newNode("num_val");
        numNode->left = newNode(currentToken.value);
        nextToken();

        if (signNode) {
            ASTNode *node = newNode("num_val");
            node->left = signNode;
            node->right = numNode;
            printf("Matched signed <num_val>: %s %s\n", signNode->value, numNode->value);
            return node;
        }
        printf("Matched <num_val>: %s\n", numNode->value);
        return numNode;
    }
    printf("Syntax error: Invalid <num_val>\n");
    exit(1);
}

ASTNode* parseSign() {
    if (currentToken.type == ARITHMETIC_OPE && 
       (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *node = newNode("sign"); 
        node->left = newNode(currentToken.value);
        nextToken();
        return node;
    }
    return NULL; // No sign found, which is acceptable
}

ASTNode* parseDriftVal() {
    printf("Entering <drift_val>\n");

    ASTNode *signNode = parseSign();
    if (currentToken.type == CONSTANT_DRIFT) {
        ASTNode *driftNode = newNode(currentToken.value);
        nextToken();
        if (signNode) {
            ASTNode *node = newNode("drift_val");
            node->left = signNode;
            node->right = driftNode;
            printf("Matched signed <drift_val>: %s %s\n", signNode->value, driftNode->value);
            return node;
        }
        printf("Matched <drift_val>: %s\n", driftNode->value);
        return driftNode;
    }
    printf("Syntax error: Invalid <drift_val>\n");
    exit(1);
}

void parse() {
    nextToken();
    ASTNode *ast = parseProgram();
    printf("Parse Tree: ");
    printParseTree(ast);
    printf("\n");
}
