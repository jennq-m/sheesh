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
        if (checkKeyword(sheeshLexeme)) {
            return newToken(sheeshLexeme, KEYWORD, sheeshLine);
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
            while ((isalnum(sheeshLine[i + 1]) || ispunct(sheeshLine[i + 1])) && !(strchr("+-*/%=!<>&|^()\"{}[];,", sheeshLine[i + 1]))) {
                temp[tempMarker++] = sheeshLine[++i];
            }
            temp[tempMarker] = '\0';

            if (checkKeyword(temp)) {
                Token token = newToken(temp, KEYWORD, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %-15s\n", token.sheeshLine, token.value, token.value);
                 
            } else if (checkReservedWord(temp)) {
                if ((strcmp(temp, "cap") != 0) && (strcmp(temp, "nocap") != 0)) {
                    Token token = newToken(temp, RESERVED_WORD, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, token.value);
                    
                } else {
                    Token token = newToken(temp, CONSTANT_LEGIT, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    
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
ASTNode* parseUnaryOp();
ASTNode* parseIdentExpr();
ASTNode* parseDataType();
ASTNode* parseVarList();

void nextToken() {
    if (currentIndex < tokenCount) {
        currentToken = allTokens[currentIndex++];
        printf("Next token: %s (%d)\n", currentToken.value, currentToken.type);
    } else {
        printf("End of token stream\n");
        currentToken.value = NULL;
        currentToken.type = INVALID;
    }
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

    // Print indentation
    for (int i = 0; i < indent; i++) {
        fprintf(file, "    "); // Write spaces to the file for indentation
    }

    // Check if the node has children
    if (node->left || node->right) {
        fprintf(file, "%s(\n", node->value); // Write the node's value with an opening parenthesis

        // Recursively write left and right children
        if (node->left) {
            printParseTree(node->left, indent + 1, file);
        }
        if (node->right) {
            printParseTree(node->right, indent + 1, file);
        }

        // Write the closing parenthesis with matching indentation
        for (int i = 0; i < indent; i++) {
            fprintf(file, "    ");
        }
        fprintf(file, ")\n");
    } else {
        // If the node has no children, just write the value
        fprintf(file, "%s\n", node->value);
    }
}



ASTNode* parseProgram() {
    printf("Entering parseProgram\n");
    if (currentToken.type == RESERVED_WORD && strcmp(currentToken.value, "toptier") == 0) {
        printf("Matched toptier\n");
        ASTNode *node = newNode("program");
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
                        printf("Syntax error: Missing closing brace\n");
                    }
                } else {
                    printf("Syntax error: Missing opening brace\n");
                }
            } else {
                printf("Syntax error: Missing closing parenthesis\n");
            }
        } else {
            printf("Syntax error: Missing opening parenthesis\n");
        }
    } else {
        printf("Syntax error: Missing toptier keyword\n");
    }
    printf("Syntax error: Invalid program structure\n");
    exit(1);
}


ASTNode* parseBody() {
    printf("Entering parseBody\n");
    ASTNode *node = newNode("body");
    node->left = parseStmts();
    return node;
}


ASTNode* parseStmts() {
    printf("Entering parseStmts\n");
    ASTNode *node = NULL;  // Root for statements
    ASTNode *current = NULL;

    while (currentToken.type != DELIM_C_BRACE && currentToken.type != INVALID) {
        ASTNode *stmt = parseExprStmt();  // Parse a single statement
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
    ASTNode *node = parseExpr(); // Parse the expression (e.g., -5)
    if (currentToken.type == DELIM_SEMCOL) {
        printf("Matched semicolon\n");
        nextToken(); // Consume the semicolon
    } else {
        printf("Syntax error: Missing semicolon\n");
        exit(1);
    }
    return node;
}


ASTNode* parseExpr() {
    printf("Checking <expr>...");
    ASTNode *node = newNode("<expr>");
    node->left = parseAndExpr();
    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "||") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseAndExpr() {
    printf("Checking <parseAndExpr>...");
    ASTNode *node = newNode("and_expr");
    node->left = parseEqualityExpr();
    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "&&") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseEqualityExpr() {
    printf("Checking <parseEqualityExpr>...");
    ASTNode *node = newNode("equality_expr");
    node->left = parseRelationalExpr();
    if (currentToken.type == RELATIONAL_OPE && (strcmp(currentToken.value, "==") == 0 || strcmp(currentToken.value, "!=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        return opNode;
    }
    return node;
}

ASTNode* parseRelationalExpr() {
    printf("Checking <parseRelationalExpr>...");
    ASTNode *node = newNode("relational_expr");
    node->left = parseAddSubExpr();
    if (currentToken.type == RELATIONAL_OPE && (strcmp(currentToken.value, "<") == 0 || strcmp(currentToken.value, "<=" ) == 0 || strcmp(currentToken.value, ">") == 0 || strcmp(currentToken.value, ">=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        return opNode;
    }
    return node;
}

ASTNode* parseAddSubExpr() {
    printf("Checking <parseAddSubExpr>...");
    ASTNode *node = newNode("addsub_expr");
    node->left = parseMultDivExpr();

    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parseMultDivExpr() {
    printf("Checking <parseMultDivExpr>...");
    ASTNode *node = newNode("<multdiv_expr>");
    node->left = parsePrimary();
    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "*") == 0 || strcmp(currentToken.value, "/") == 0 || strcmp(currentToken.value, "%") == 0) || strcmp(currentToken.value, "|") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        opNode->left = node;
        opNode->right = parseExpr();
        node = opNode;
    }
    return node;
}

ASTNode* parsePrimary() {
    printf("Entering parsePrimary. Current token: %s (Type: %d, Line: %d)\n", currentToken.value, currentToken.type, currentToken.sheeshLine);

    ASTNode *primaryNode = newNode("<primary>");

    // Handle unary '+' or '-' operators (e.g., +expr or -expr)
    if (currentToken.type == ARITHMETIC_OPE && 
        (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        
        printf("Parsing unary operator: %s\n", currentToken.value);
        ASTNode *opNode = newNode(currentToken.value); // Create a node for the unary operator
        nextToken(); // Consume the unary operator

        if (currentToken.type == DELIM_O_PAREN) {
            printf("Unary operator applied to a parenthesized expression\n");
            nextToken(); // Consume '('
            opNode->right = parseExpr(); // Parse the expression inside parentheses

            if (currentToken.type != DELIM_C_PAREN) {
                printf("Syntax error: Missing closing parenthesis\n");
                exit(1);
            }
            nextToken(); // Consume ')'
        } else {
            // If not a parenthesized expression, we treat it as a regular unary operator applied to an expression.
            opNode->right = parsePrimary();
        }

        primaryNode->left = opNode; // Set the unary operator node as the left child of the primary node
        return primaryNode;
    }

    // Handle literals or values like numbers, identifiers
    if (currentToken.type == CONSTANT_NUM || currentToken.type == CONSTANT_DRIFT || 
        currentToken.type == CONSTANT_VIBE || currentToken.type == CONSTANT_TEXT || currentToken.type == CONSTANT_LEGIT) {

        printf("Parsing literal: %s\n", currentToken.value);
        primaryNode->left = parseLiteral();
        return primaryNode;
    }

    // Handle identifiers
    else if (currentToken.type == IDENTIFIER) {
        printf("Parsing identifier: %s\n", currentToken.value);
        ASTNode *identifierNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == UNARY_OPE) {
            previousToken();
            identifierNode->left = parseUnaryVal();
        }

        if (currentToken.type == ASSIGNMENT_OPE) {
            printf("Found assignment operator: %s\n", currentToken.value);
            ASTNode *assignNode = newNode(currentToken.value); // Assignment operator node
            nextToken();
            assignNode->left = identifierNode;
            assignNode->right = parseExpr(); // Parse the expression being assigned
            primaryNode->left = assignNode;
            return primaryNode;
        }

        primaryNode->left = identifierNode;
        return primaryNode;
    }

    // Handle parenthesized expressions
    else if (currentToken.type == DELIM_O_PAREN) {
        printf("Parsing parenthesized expression\n");
        nextToken(); // Consume '('
        ASTNode *exprNode = parseExpr();

        if (currentToken.type != DELIM_C_PAREN) {
            printf("Syntax error: Missing closing parenthesis\n");
            exit(1);
        }
        nextToken(); // Consume ')'
        primaryNode->left = exprNode;
        return primaryNode;
    }

    // Handle unary operators like '!', '++', '--'
    else if (strcmp(currentToken.value, "!") == 0 || strcmp(currentToken.value, "++") == 0 || strcmp(currentToken.value, "--") == 0) {
        printf("Parsing unary operator: %s\n", currentToken.value);
        ASTNode *opNode = newNode(currentToken.value); // Create a node for the unary operator
        nextToken(); // Consume the unary operator

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
            printf("Syntax error: Expected identifier (/identifier expression) or CONSTANT_LEGIT after '%s'\n", opNode->value);
            exit(1);
        }
    }

    printf("Syntax error at Line %d: Invalid <primary_expr>. Current token: %s (Type: %d)\n", currentToken.sheeshLine, currentToken.value, currentToken.type);
    exit(1);
}




ASTNode* parseLiteral() {
    // <literal> 		::= 	<num_val> | <drift_val> | CONSTANT_VIBE | CONSTANT_TEXT | CONSTANT_LEGIT

    printf("Checking <parseLiteral>...");

    ASTNode *node = newNode("literal");

    if (currentToken.type == ARITHMETIC_OPE && 
        (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
            nextToken();

            if (currentToken.type == CONSTANT_NUM || currentToken.type == IDENTIFIER) {
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

    printf("Syntax error at Line %s: Invalid <literal>\n", currentToken.sheeshLine);
    exit(1);
}

ASTNode* parseNumVal() {
    ASTNode *signNode = parseSign(); // Parse optional sign

    if (currentToken.type == CONSTANT_NUM) {
        // Handle numeric constant
        ASTNode *numNode = newNode("num_val");
        numNode->left = newNode(currentToken.value); // Add constant
        nextToken(); // Consume constant

        if (signNode) {
            ASTNode *node = newNode("num_val");
            node->left = signNode;
            node->right = numNode;
            return node;
        }
        return numNode;

    } else if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
        // Handle identifier or parenthesized expression
        ASTNode *exprNode;
        if (currentToken.type == IDENTIFIER) {
            exprNode = parseIdentExpr(); // Parse identifier expression
        } else {
            nextToken(); // Consume '('
            exprNode = parseExpr(); // Parse expression inside parentheses

            if (currentToken.type != DELIM_C_PAREN) {
                printf("Syntax error: Missing closing parenthesis in <num_val>\n");
                exit(1);
            }
            nextToken(); // Consume ')'
        }

        if (signNode) {
            ASTNode *node = newNode("num_val");
            node->left = signNode;
            node->right = exprNode;
            return node;
        }
        return exprNode;
    }

    printf("Syntax error: Invalid <num_val>\n");
    exit(1);
}


ASTNode* parseSign() {
    // <sign>   		::= 	‘-’ | ‘+’

    if (currentToken.type == ARITHMETIC_OPE && 
       (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *node = newNode("sign"); 
        node->left = newNode(currentToken.value);
        nextToken();

        return node;
    }

    return NULL;
}

ASTNode* parseDriftVal() {
    ASTNode *signNode = parseSign();

    if (currentToken.type == CONSTANT_DRIFT) {
        ASTNode *driftNode = newNode("drift_val");
        driftNode->left = newNode(currentToken.value);
        nextToken();

        if (signNode) {
            ASTNode *node = newNode("drift_val");
            node->left = signNode;
            node->right = driftNode;
            return node;
        }
        return driftNode;

    } else if (currentToken.type == IDENTIFIER || currentToken.type == DELIM_O_PAREN) {
        // Handle identifier or parenthesized expression
        ASTNode *exprNode;
        if (currentToken.type == IDENTIFIER) {
            exprNode = parseIdentExpr(); // Parse identifier expression
        } else {
            nextToken(); // Consume '('
            exprNode = parseExpr(); // Parse expression inside parentheses

            if (currentToken.type != DELIM_C_PAREN) {
                printf("Syntax error: Missing closing parenthesis in <num_val>\n");
                exit(1);
            }
            nextToken(); // Consume ')'
        }

        if (signNode) {
            ASTNode *node = newNode("drift_val");
            node->left = signNode;
            node->right = exprNode;
            return node;
        }
        return exprNode;
    }


    printf("Syntax error: Invalid <drift_val>\n");
    exit(1);
}


ASTNode* parseUnaryVal() {
    if (currentToken.type == UNARY_OPE) {
        // Handle prefix unary operators (e.g., ++identifier or --identifier)
        ASTNode *node = newNode("<unary_val>");
        node->left = newNode(currentToken.value);
        nextToken();

        if (currentToken.type == IDENTIFIER) {
            node->right = newNode(currentToken.value);
            nextToken();
            return node;
        } else {
            printf("Syntax error: Expected identifier after unary operator\n");
            exit(1);
        }
    } else if (currentToken.type == IDENTIFIER) {
        // Handle identifier
        ASTNode *identifierNode = newNode(currentToken.value);
        nextToken();

        // Check for post-increment or post-decrement (e.g., identifier++)
        if (currentToken.type == UNARY_OPE &&
            (strcmp(currentToken.value, "++") == 0 || strcmp(currentToken.value, "--") == 0)) {
            ASTNode *node = newNode("<unary_val>");
            node->left = identifierNode;
            node->right = newNode(currentToken.value); // Add the ++ or -- token as a node
            nextToken(); // Consume the ++ or -- token
            return node;
        }

        // If no post-increment or post-decrement, return the identifier node
        return identifierNode;
    }

    // If none of the above cases matched, it's a syntax error
    printf("Syntax error: Invalid unary value\n");
    exit(1);
}

ASTNode* parseIdentExpr() {
    ASTNode *node = newNode("<ident_expr>");

    if (currentToken.type == IDENTIFIER) {
        node->left = newNode(currentToken.value); // Add identifier node
        nextToken(); // Consume identifier
        return node;
    } else if (currentToken.type == DELIM_O_PAREN) {
        nextToken(); // Consume opening parenthesis
        ASTNode *exprNode = parseExpr(); // Parse the expression inside the parenthesis

        if (currentToken.type != DELIM_C_PAREN) {
            printf("Syntax error at Line %d: Missing closing parenthesis in <ident_expr>\n", currentToken.sheeshLine);
            exit(1);
        }

        nextToken(); // Consume closing parenthesis
        return exprNode; // Return the parsed expression node
    }

    printf("Syntax error: Invalid <ident_expr>\n");
    exit(1);
}

ASTNode* parseDecStmt() {
    // <dec_stmt> ::= <data_type> ( IDENTIFIER | <initialization> | IDENTIFIER <var_list> | <initialization> <var_list>) DELIM_SEMCOL
    printf("Parse Dec Statement");
    ASTNode *stmtNode = newNode("<dec_stmt>");
    ASTNode *dtNode = parseDataType();
    stmtNode->left = dtNode;  // Set data type as the left child of the stmtNode

    if (currentToken.type == IDENTIFIER) {
        // Case: IDENTIFIER or IDENTIFIER <var_list>
        printf("Parse Dec Statement A");
        ASTNode *idNode = newNode(currentToken.value);
        nextToken();
        stmtNode->right = idNode;

        if (currentToken.type == ASSIGNMENT_OPE) {
            // Case: IDENTIFIER = <expr>
            printf("Parse Dec Statement B");
            nextToken();  // Consume '='
            ASTNode *exprNode = parseExpr();
            idNode->right = exprNode;
            return stmtNode;
        } else if (currentToken.type == DELIM_COMMA) {
            // Case: IDENTIFIER <var_list>
            ASTNode *varListNode = parseVarList();
            idNode->right = varListNode;
        }

        if (currentToken.type == DELIM_SEMCOL) {
            nextToken(); // Consume semicolon
            return stmtNode;
        } else {
            printf("Syntax error: Missing semicolon in declaration\n");
            exit(1);
        }
    }

    printf("Syntax error: Invalid declaration statement\n");
    exit(1);
}


ASTNode* parseDataType() {
    // <data_type> ::= "num" | "drift" | "vibe" | "text" | "short" | "long" | "legit"
    ASTNode* dtNode = newNode("<data_type>");

    if (strcmp(currentToken.value, "num") == 0 || strcmp(currentToken.value, "drift") == 0 || 
        strcmp(currentToken.value, "vibe") == 0 || strcmp(currentToken.value, "text") == 0 || 
        strcmp(currentToken.value, "short") == 0 || strcmp(currentToken.value, "long") == 0 || 
        strcmp(currentToken.value, "legit") == 0) {
        dtNode->left = newNode(currentToken.value);
        nextToken();  // Consume the data type
        return dtNode;
    }

    printf("Syntax error: Invalid data type\n");
    exit(1);
}


ASTNode* parseInitialization() {
    // <initialization> ::= IDENTIFIER "=" <expr>
    ASTNode* initNode = newNode("<initialization>");

    if (currentToken.type == IDENTIFIER) {
        ASTNode* idNode = newNode(currentToken.value);
        initNode->left = idNode;
        nextToken();  // Consume the identifier

        if (currentToken.type == ASSIGNMENT_OPE) {
            nextToken();  // Consume '='
            ASTNode* exprNode = parseExpr();
            idNode->right = exprNode;  // Set the expression on the right of the identifier

            return initNode;
        }
    }

    printf("Syntax error at Line %d: Invalid initialization. Expected '=' followed by an expression.\n", currentToken.sheeshLine);
    exit(1);
}


ASTNode* parseVarList() {
    // <var_list> ::= (DELIM_COMMA (IDENTIFIER | <initialization>))+ 
    ASTNode *varListNode = newNode("<var_list>");
    ASTNode *headNode = NULL;  // Track the first node in the list
    ASTNode *currentNode = NULL;  // Track the current node in the list

    if (currentToken.type == DELIM_COMMA) {
        nextToken();  // Consume the comma
    }

    while (currentToken.type == IDENTIFIER || currentToken.type == ASSIGNMENT_OPE) {
        ASTNode *node = NULL;
        
        if (currentToken.type == IDENTIFIER) {
            // Case: IDENTIFIER (not initialization)
            node = newNode(currentToken.value);
            nextToken();  // Consume identifier
        } 

        if (currentToken.type == ASSIGNMENT_OPE) {
            // Case: IDENTIFIER = <expr>
            node = parseInitialization();  // Parse initialization (identifier = expr)
        }

        // Add the node to the list
        if (headNode == NULL) {
            headNode = node;  // First node in the list
            currentNode = headNode;
        } else {
            currentNode->right = node;  // Append subsequent nodes
            currentNode = node;  // Move to the newly added node
        }

        if (currentToken.type == DELIM_COMMA) {
            nextToken();  // Consume the comma and continue parsing
        } else {
            break;  // No more variables, exit the loop
        }
    }

    varListNode->left = headNode;  // Set the head node as the left child of the var_list
    return varListNode;
}

// ASTNode* parseUnaryOp() {
//     // <unary_op>   		::= 	‘++’ | ‘--’

//     if (currentToken.type == UNARY_OPE) {
//         ASTNode *node = newNode("<unary_op>"); 
//         node->left = newNode(currentToken.value);
//         nextToken();

//         return node;
//     }

//     return NULL;
// }
// ASTNode* parseDecStmt() {
//     if (currentToken.type == IDENTIFIER) {
//         ASTNode *node = newNode("dec_stmt");
//         node->left = newNode(currentToken.value);
//         nextToken();
//         if (currentToken.type == ASSIGNMENT_OPE) {
//             nextToken();
//             node->right = parseExpr();
//         }
//         if (currentToken.type != DELIM_SEMCOL) {
//             printf("Syntax error: Missing semicolon in declaration\n");
//             exit(1);
//         }
//         nextToken();
//         return node;
//     }
//     printf("Syntax error: Invalid declaration statement\n");
//     exit(1);
// }

// ASTNode* parseAssignStmt() {
//     if (currentToken.type == IDENTIFIER) {
//         ASTNode *node = newNode("assign_stmt");
//         node->left = newNode(currentToken.value);
//         nextToken();
//         if (currentToken.type == ASSIGNMENT_OPE) {
//             nextToken();
//             node->right = parseExpr();
//         } else {
//             printf("Syntax error: Expected assignment operator\n");
//             exit(1);
//         }
//         if (currentToken.type != DELIM_SEMCOL) {
//             printf("Syntax error: Missing semicolon in assignment\n");
//             exit(1);
//         }
//         nextToken();
//         return node;
//     }
//     printf("Syntax error: Invalid assignment statement\n");
//     exit(1);
// }

// ASTNode* parseCondStmt() {
//     if (currentToken.type == KEYWORD && strcmp(currentToken.value, "if") == 0) {
//         ASTNode *node = newNode("if_stmt");
//         nextToken();
//         if (currentToken.type == DELIM_O_PAREN) {
//             nextToken();
//             node->left = parseExpr();
//             if (currentToken.type == DELIM_C_PAREN) {
//                 nextToken();
//                 if (currentToken.type == DELIM_O_BRACE) {
//                     nextToken();
//                     node->right = parseBody();
//                     if (currentToken.type == DELIM_C_BRACE) {
//                         nextToken();
//                         return node;
//                     }
//                 }
//             }
//         }
//         printf("Syntax error: Invalid if statement\n");
//         exit(1);
//     }
//     printf("Syntax error: Expected if statement\n");
//     exit(1);
// }

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

void parse(FILE* file) {
    nextToken();
    ASTNode *ast = parseProgram();
    printf("See parse_tree.txt for parse tree.");
    
    printParseTree(ast, 0, file);
    printf("\n");
}