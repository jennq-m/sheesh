#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "lexer.h"


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
    if (strcmp(tokenStr, "OUT") == 0) return OUTSIDE;
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
    if (strcmp(tokenStr, "TOP") == 0) return TOPTIER;
    if (strcmp(tokenStr, "OTHER") == 0) return OTHER;
    if (strcmp(tokenStr, "OTHERWISE") == 0) return OTHER;
    return INVALID;
}

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case IDENTIFIER: return "IDENTIFIER";
        case KEYWORD: return "KEYWORD";
        case RESERVED_WORD: return "RESERVED_WORD";
        case NOISE_WORD: return "NOISE_WORD";
        case COMMENT: return "COMMENT";
        case ARITHMETIC_OPE: return "ARITHMETIC_OPE";
        case ASSIGNMENT_OPE: return "ASSIGNMENT_OPE";
        case LOGICAL_OPE: return "LOGICAL_OPE";
        case UNARY_OPE: return "UNARY_OPE";
        case RELATIONAL_OPE: return "RELATIONAL_OPE";
        case DELIM_COMMA: return "DELIM_COMMA";
        case DELIM_SEMCOL: return "DELIM_SEMCOL";
        case DELIM_QUOTATION: return "DELIM_QUOTATION";
        case DELIM_O_PAREN: return "DELIM_O_PAREN";
        case DELIM_C_PAREN: return "DELIM_C_PAREN";
        case DELIM_O_BRACE: return "DELIM_O_BRACE";
        case DELIM_C_BRACE: return "DELIM_C_BRACE";
        case DELIM_O_BRCKT: return "DELIM_O_BRCKT";
        case DELIM_C_BRCKT: return "DELIM_C_BRCKT";
        case INVALID: return "INVALID";
        case CONSTANT_NUM: return "CONSTANT_NUM";
        case CONSTANT_DRIFT: return "CONSTANT_DRIFT";
        case CONSTANT_TEXT: return "CONSTANT_TEXT";
        case CONSTANT_TXTFS: return "CONSTANT_TXTFS";
        case CONSTANT_VIBE: return "CONSTANT_VIBE";
        case CONSTANT_LEGIT: return "CONSTANT_LEGIT";
        case BOUNCE: return "BOUNCE";
        case CAR: return "CAR";
        case DO: return "DO";
        case DRIFT: return "DRIFT";
        case EMPTY: return "EMPTY";
        case EX: return "EX";
        case FLIP: return "FLIP";
        case FROZEN: return "FROZEN";
        case GROUP: return "GROUP";
        case IF: return "IF";
        case INPUT: return "INPUT";
        case JUMP: return "JUMP";
        case LEGIT: return "LEGIT";
        case LOCKED: return "LOCKED";
        case LOCKIN: return "LOCKIN";
        case LONG: return "LONG";
        case MEANWHILE: return "MEANWHILE";
        case NICKNAME: return "NICKNAME";
        case NUM: return "NUM";
        case OPEN: return "OPEN";
        case OTHER: return "OTHER";
        case OUT: return "OUT";
        case OUTSIDE: return "OUTSIDE";
        case PL: return "PL";
        case REP: return "REP";
        case SCENARIO: return "SCENARIO";
        case SHORT: return "SHORT";
        case STANDARD: return "STANDARD";
        case STOP: return "STOP";
        case TEAM: return "TEAM";
        case TEXT: return "TEXT";
        case VIBE: return "VIBE";
        case ALWAYS: return "ALWAYS";
        case CAP: return "CAP";
        case CONT: return "CONT";
        case NOCAP: return "NOCAP";
        case PLAYLIST: return "PLAYLIST";
        case REPEAT: return "REPEAT";
        case TOPTIER: return "TOPTIER";
        case TOP: return "TOP";
        case OTHERWISE: return "OTHERWISE";
        default: return "UNKNOWN";
    }
}

char* tokenTypeAndValueToString(TokenType type, const char* value) {
    char* result = malloc(256);
    snprintf(result, 256, "%s: %s", tokenTypeToString(type), value);
    return result;
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
        
        if (node->left)  {
            fprintf(file, "(\n");
            printParseTree(node->left, indent + 1, file);
            fprintf(file, ")\n");
        }
        
        if (node->right) {
            fprintf(file, "\n");
            printParseTree(node->right, indent, file);
            fprintf(file, "\n");
        }

        for (int i = 0; i < indent; i++) {
            fprintf(file, "    ");
        }
         
    } 
}


ASTNode* parseProgram() {
    if (currentToken.type != TOPTIER) {
        printf("SYNTAX ERROR LINE %d: Expected 'TOPTIER' keyword. Skipping tokens until the keyword is found.\n", currentToken.sheeshLine);
        nextToken(); 
        return NULL;
    }

    ASTNode *programNode = newNode("<program>");

    ASTNode *toptierNode = newNode(tokenTypeToString(currentToken.type));
    programNode->left = toptierNode;
    nextToken();

    if (currentToken.type == DELIM_O_PAREN) {
        ASTNode *openParenNode = newNode(tokenTypeToString(currentToken.type));
        toptierNode->right = openParenNode;
        nextToken();

        if (currentToken.type == DELIM_C_PAREN) {
            ASTNode *closeParenNode = newNode(tokenTypeToString(currentToken.type));
            openParenNode->right = closeParenNode;
            nextToken();

            if (currentToken.type == DELIM_O_BRACE) {
                ASTNode *openBraceNode = newNode(tokenTypeToString(currentToken.type));
                closeParenNode->right = openBraceNode;
                nextToken();

                ASTNode *bodyNode = parseBody();
                openBraceNode->right = bodyNode;

                if (currentToken.type == DELIM_C_BRACE) {
                    ASTNode *closeBraceNode = newNode(tokenTypeToString(currentToken.type));
                    bodyNode->right = closeBraceNode;
                    nextToken();

                    return programNode;
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected '}'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected '{'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        } else {
            printf("SYNTAX ERROR LINE %d: Expected ')'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    } else {
        printf("SYNTAX ERROR LINE %d: Expected '('. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
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
            stmt->left = parseAssignStmt();
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

ASTNode* parseExpr() {
    ASTNode* node = newNode("<expr>");
    ASTNode* andNode = parseAndExpr();  

    if (!andNode) {
        printf("SYNTAX ERROR LINE %d: Invalid expression. Expected 'and' expression. Encountered token %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    node->left = andNode;

    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "||") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();
        
        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && currentToken.type != CONSTANT_NUM 
        && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN 
        && currentToken.type != LOGICAL_OPE) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '||' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        ASTNode* newAndNode = parseAndExpr();

        if (!newAndNode) {
            printf("SYNTAX ERROR LINE %d: Invalid expression after '||' operator. Expected right operand. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node->left = opNode;
        opNode->left = andNode;
        andNode->right = newAndNode;
    }

    return node;
}

ASTNode* parseAndExpr() {
    ASTNode *node = newNode("<and_expr>");
    ASTNode *equalityNode = parseEqualityExpr();

    if (!equalityNode) {
        printf("SYNTAX ERROR LINE %d: Invalid expression in 'and' expression. Expected equality expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    node->left = equalityNode;

    while (currentToken.type == LOGICAL_OPE && strcmp(currentToken.value, "&&") == 0) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && 
            currentToken.type != CONSTANT_NUM && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && 
            currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '&&' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        ASTNode *newEqualityNode = parseEqualityExpr();

        if (!newEqualityNode) {
            printf("SYNTAX ERROR LINE %d: Expected equality expression after '&&'. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node->left = opNode;
        opNode->left = equalityNode;
        equalityNode->right = newEqualityNode;
    }
    return node;
}

ASTNode* parseEqualityExpr() {
    ASTNode *node = newNode("<equality_expr>");
    ASTNode *relNode = parseRelationalExpr();

    if (!relNode) {
        printf("SYNTAX ERROR LINE %d: Expected relational expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    node->left = relNode;

    if (currentToken.type == RELATIONAL_OPE && (strcmp(currentToken.value, "==") == 0 || strcmp(currentToken.value, "!=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT &&
            currentToken.type != CONSTANT_NUM && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS &&
            currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '&&' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        ASTNode* newRelNode = parseRelationalExpr();

        if (!newRelNode) {
            printf("SYNTAX ERROR LINE %d: Expected relational expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        
        node->left = opNode;
        opNode->left = relNode;
        relNode->right = newRelNode;
    }
    return node;
}

ASTNode* parseRelationalExpr() {
    ASTNode *node = newNode("<relational_expr>");
    ASTNode* aSNode = parseAddSubExpr();
    
    if (!aSNode) {
        printf("SYNTAX ERROR LINE %d: Expected addsub_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    node->left = aSNode;

    if (currentToken.type == RELATIONAL_OPE && 
        (strcmp(currentToken.value, "<") == 0 || strcmp(currentToken.value, "<=") == 0 || strcmp(currentToken.value, ">") == 0 || strcmp(currentToken.value, ">=") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && 
            currentToken.type != CONSTANT_NUM && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && 
            currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN) {
            printf("SYNTAX ERROR LINE %d: Invalid token after relational operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        ASTNode* newASNode = parseAddSubExpr();

        if (!newASNode) {
            printf("SYNTAX ERROR LINE %d: Expected addsub_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node->left = opNode;
        opNode->left = aSNode;
        aSNode->right = newASNode;
    }

    return node;
}

ASTNode* parseAddSubExpr() {
    ASTNode *node = newNode("<addsub_expr>");
    ASTNode *mDNode = parseMultDivExpr();

    if (!mDNode) {
        printf("SYNTAX ERROR LINE %d: Expected multdiv_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1); 
    }
    
    node->left = mDNode;

    while (currentToken.type == ARITHMETIC_OPE && (strcmp(currentToken.value, "+") == 0 || strcmp(currentToken.value, "-") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && 
            currentToken.type != CONSTANT_NUM && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && 
            currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '+' or '-' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        ASTNode *newMDNode = parseMultDivExpr();

        if (!newMDNode) {
            printf("SYNTAX ERROR LINE %d: Expected multdiv_expr. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node->left = opNode;
        opNode->left = mDNode;
        mDNode->right = newMDNode;
    }

    return node;
}

ASTNode* parseMultDivExpr() {
    ASTNode *node = newNode("<multdiv_expr>");
    ASTNode *primaryNode = parsePrimary();
    
    if (!primaryNode) {
        printf("SYNTAX ERROR LINE %d: Invalid token. Expected 'primary'. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }

    node->left = primaryNode;

    while (currentToken.type == ARITHMETIC_OPE && 
           (strcmp(currentToken.value, "*") == 0 || strcmp(currentToken.value, "/") == 0 || strcmp(currentToken.value, "%") == 0 || strcmp(currentToken.value, "|") == 0)) {
        ASTNode *opNode = newNode(currentToken.value);
        nextToken();

        if (currentToken.type != IDENTIFIER && currentToken.type != CONSTANT_DRIFT && currentToken.type != CONSTANT_LEGIT && 
            currentToken.type != CONSTANT_NUM && currentToken.type != CONSTANT_TEXT && currentToken.type != CONSTANT_TXTFS && 
            currentToken.type != CONSTANT_VIBE && currentToken.type != DELIM_O_PAREN) {
            printf("SYNTAX ERROR LINE %d: Invalid token after '*', '/', '%\" or '|' operator. Expected expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        
        ASTNode *newPrimaryNode = parsePrimary();

        if (!newPrimaryNode) {
            printf("SYNTAX ERROR LINE %d: Expected primary expression. Encountered %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        node->left = opNode;
        opNode->left = primaryNode;
        primaryNode->right = newPrimaryNode;
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
        ASTNode *identifierNode = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        if (currentToken.type == UNARY_OPE) {
            previousToken();
            identifierNode->left = parseUnaryVal();
        }

        if (currentToken.type == ASSIGNMENT_OPE) {
            printf("Found assignment operator: %s\n", currentToken.value);
            ASTNode *assignNode = newNode(currentToken.value);
            nextToken();
            primaryNode->left = assignNode;
            assignNode->left = identifierNode;
            assignNode->left->left = parseExpr();
            return primaryNode;
        }

        primaryNode->left = identifierNode;
        return primaryNode;
    }

    if (currentToken.type == DELIM_O_PAREN) {
        ASTNode *parenNode = newNode(tokenTypeToString(currentToken.type));
        nextToken();
        ASTNode *exprNode = parseExpr();
        parenNode->right = exprNode;

        if (currentToken.type != DELIM_C_PAREN) {
            printf("SYNTAX ERROR LINE %d: Expected ')'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        parenNode->right->right = newNode(tokenTypeToString(currentToken.type));
        nextToken();
        primaryNode->left = parenNode;
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
            opNode->right = newNode(tokenTypeToString(currentToken.type));
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
        node->left = newNode(tokenTypeToString(currentToken.type));
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
        numNode->left = newNode(tokenTypeToString(currentToken.type));
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
        ASTNode* driftNodeValue = newNode(tokenTypeToString(currentToken.type));
        driftNode->left = driftNodeValue;
        nextToken();

        if (signNode) {
            ASTNode *node = newNode("<drift_val>");
            node->left = signNode;
            signNode->right = driftNodeValue;
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
            node->left->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();
            return node;
        } else {
            printf("SYNTAX ERROR LINE %d: Expected identifier after unary operator Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    } else if (currentToken.type == IDENTIFIER) {
        ASTNode *identifierNode = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        if (currentToken.type == UNARY_OPE &&
            (strcmp(currentToken.value, "++") == 0 || strcmp(currentToken.value, "--") == 0)) {
            ASTNode *node = newNode("<unary_val>");
            node->left = identifierNode;
            identifierNode->right = newNode(currentToken.value);
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
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken();
        return node;
    } else if (currentToken.type == DELIM_O_PAREN) {
        ASTNode *parenNode = newNode(tokenTypeToString(currentToken.type));
        nextToken();
        ASTNode *exprNode = parseExpr();
        parenNode->right = exprNode;

        if (currentToken.type != DELIM_C_PAREN) {
            printf("SYNTAX ERROR LINE %d: Expected ')' in <ident_expr>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }

        parenNode->right->right = newNode(tokenTypeToString(currentToken.type));
        node->left = parenNode;
        nextToken();
        return node;
    }

    printf("SYNTAX ERROR LINE %d: Invalid <ident_expr>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseDecStmt() {
    // <dec_stmt> 		::= 	<data_type> ( <initialization> |  IDENTIFIER <var_list> | <initialization> <var_list>) DELIM_SEMCOL

    ASTNode* stmtNode = newNode("<dec_stmt>");
    ASTNode* dtNode = parseDataType();
    stmtNode->left = dtNode;

    if (currentToken.type == IDENTIFIER) {
        ASTNode* firstVar = parseInitialization(); 

        if (currentToken.type == DELIM_COMMA) {
            ASTNode* varListNode = parseVarList(firstVar); 
            dtNode->left = varListNode;
        }
    } else {
        printf("Syntax error: Expected an identifier or initialization after data type\n");
        exit(1);
    }

    if (currentToken.type == DELIM_SEMCOL) {
        nextToken(); 
    } else {
        printf("Syntax error: Expected ';' at the end of declaration statement\n");
        exit(1);
    }

    return stmtNode;
}

ASTNode* parseVarList(ASTNode* firstVar) {
    ASTNode* varListNode = newNode("<var_list>"); 
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
    
    varListNode->left = firstVar;
    return varListNode; 
}

ASTNode* parseDataType() {

    if (currentToken.type == NUM || currentToken.type == DRIFT || currentToken.type == VIBE || 
        currentToken.type == TEXT || currentToken.type == SHORT || 
        currentToken.type == LONG || currentToken.type == LEGIT) {
        ASTNode* dtNode = newNode("<data_type>");
        dtNode->left = newNode(tokenTypeToString(currentToken.type));
        nextToken(); 
        return dtNode;
    }

    printf("Syntax error: Invalid data type\n");
    exit(1);
}

ASTNode* parseInitialization() {

    ASTNode* initNode = newNode("<initialization>");
    
    if (currentToken.type == IDENTIFIER) {
        ASTNode* idNode = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        if (currentToken.type == ASSIGNMENT_OPE) {
            ASTNode* assignNode = newNode(currentToken.value);
            nextToken();
            ASTNode* exprNode = parseExpr();
            assignNode->left = idNode;
            assignNode->left->right = exprNode;
            idNode = assignNode;
        }

        initNode->left = idNode;
    } else {
        printf("Syntax error: Expected an identifier in initialization\n");
        exit(1);
    }

    return initNode;
}


ASTNode* parseAssignStmt() {
    //<assign_stmt> 	::=   	(IDENTIFIER ASSIGNMENT_OPE <expr> | <expr>) DELIM_SEMCOL

    ASTNode *node = newNode("<assign_stmt>");

    if (currentToken.type == IDENTIFIER) {
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        if (currentToken.type == ASSIGNMENT_OPE) {
            ASTNode *assignmentNode = newNode(currentToken.value);
            assignmentNode->left = node->left;
            nextToken();

            assignmentNode->left->right = parseExpr();
            node->left = assignmentNode;

            if (currentToken.type == DELIM_SEMCOL) {
                ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
                node->right = semicolonNode;
                nextToken();
                return node;
            }
            else {
                printf("SYNTAX ERROR LINE %d: Expected ';'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
            
        } else {
            previousToken();
            node->left = parseExpr();

            if (currentToken.type == DELIM_SEMCOL) {
                ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
                node->right = semicolonNode;
                nextToken();
                return node;
            }
            else {
                printf("SYNTAX ERROR LINE %d: Expected ';'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
        }
    }
    else {
        node->left = parseExpr();

        if (currentToken.type == DELIM_SEMCOL) {
            ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
            node->right = semicolonNode;
            nextToken();
            return node;
        }
        else {
            printf("SYNTAX ERROR LINE %d: Expected ';'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
    }

    printf("SYNTAX ERROR LINE %d: Invalid <assign_stmt>. Encountered '%s' instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);
}

ASTNode* parseIfStmt() {
    if (strcmp(currentToken.value, "if") == 0) {
        ASTNode *node = newNode("<if_stmt>");
        
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken(); 

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode *conditionNode = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            node->left->right = conditionNode;
            conditionNode->right = parseExpr();

            if (currentToken.type == DELIM_C_PAREN) {
                conditionNode->right->right = newNode(tokenTypeToString(currentToken.type));
                nextToken();

                if (currentToken.type == DELIM_O_BRACE) {
                    ASTNode *bodyNode = newNode(tokenTypeToString(currentToken.type));
                    nextToken();

                    bodyNode->right = parseBody();

                    if (currentToken.type == DELIM_C_BRACE) {
                        bodyNode->right->right = newNode(tokenTypeToString(currentToken.type));
                        nextToken();

                        conditionNode->right->right->right = bodyNode;
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

ASTNode* parseExStmt() {
    if (currentToken.type == EX) {
        ASTNode *node = newNode("<ex_stmt>");
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode *conditionNode = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            node->left->right = conditionNode;
            conditionNode->right = parseExpr();

            if (currentToken.type == DELIM_C_PAREN) {
                conditionNode->right->right = newNode(tokenTypeToString(currentToken.type));
                nextToken();

                if (currentToken.type == DELIM_O_BRACE) {
                    ASTNode *bodyNode = newNode(tokenTypeToString(currentToken.type));
                    nextToken();

                    bodyNode->right = parseBody();

                    if (currentToken.type == DELIM_C_BRACE) {
                        bodyNode->right->right = newNode(tokenTypeToString(currentToken.type));
                        nextToken();

                        conditionNode->right->right = bodyNode;
                        return node;
                    }
                    printf("SYNTAX ERROR LINE %d: Expected '}' in else-if body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
                printf("SYNTAX ERROR LINE %d: Expected '{' in else-if body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }
            printf("SYNTAX ERROR LINE %d: Expected ')' for else-if condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
            exit(1);
        }
        printf("SYNTAX ERROR LINE %d: Expected '(' for else-if condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }
    printf("SYNTAX ERROR LINE %d: Invalid <ex_stmt>\n", currentToken.sheeshLine);
    exit(1);
}

ASTNode* parseOtherStmt() {
    ASTNode *node = newNode("<other_stmt>");

    node->left = newNode(tokenTypeToString(currentToken.type));
    nextToken(); 

    if (currentToken.type == DELIM_O_BRACE) {
        ASTNode *otherBody = newNode(tokenTypeToString(currentToken.type));  
        nextToken();

        ASTNode* bodyNode = parseBody();
        otherBody->right = bodyNode;

        if (currentToken.type == DELIM_C_BRACE) {
            bodyNode->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            node->left->right = otherBody;
            return node;
        }

        printf("SYNTAX ERROR LINE %d: Expected '}' in if-other body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    } else {
        printf("SYNTAX ERROR LINE %d: Expected '{' in if-other body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
        exit(1);
    }
}

ASTNode* parseIfOtherStmt(ASTNode *ifOtherNode) {
    if (ifOtherNode->left) {
        if (currentToken.type == OTHER) {
            ASTNode *otherNode = parseOtherStmt();
            ifOtherNode->right = otherNode;

            return ifOtherNode;
        }

        printf("SYNTAX ERROR LINE %d: Invalid <other> stmt\n", currentToken.sheeshLine);
        exit(1);
    }
}

ASTNode* parseExIfStmt() {
        
    ASTNode *holderNode = newNode("");
    ASTNode *exStmtNode = parseExStmt();

    holderNode->left = exStmtNode;

    if (currentToken.type == EX) {
        holderNode->right = parseExIfStmt();
    }

    if (currentToken.type == OTHER) {
        holderNode->right = parseOtherStmt();
    }

    return holderNode;
}

ASTNode* parseCondStmt() {
    //<condi_stmt>  	::= 	<if_stmt>  |  <if_other_stmt>  | <ex_if_stmt> 
    
    ASTNode* node = newNode("<condi_stmt>");

    if (currentToken.type == IF) {
        ASTNode *ifNode = parseIfStmt();
        node->left = ifNode;

        if (currentToken.type == OTHER) {
            ASTNode *ifOtherNode = newNode("<if_other_stmt>");
            node->left = ifOtherNode;
            ifOtherNode->left = ifNode;
            return parseIfOtherStmt(ifOtherNode);
        }
        else if (currentToken.type == EX) {
            ASTNode *exIfNode = newNode("<ex_if_stmt>");
            node->left = exIfNode;
            exIfNode->left = ifNode;
            exIfNode->right =  parseExIfStmt();
            return exIfNode;
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
        ASTNode* loopNode = newNode(tokenTypeToString(currentToken.type));
        node->left = loopNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* parenNode = newNode(tokenTypeToString(currentToken.type));
            loopNode->right = parenNode;
            nextToken();

            ASTNode* initialNode = parseLoopInitial();
            parenNode->right = initialNode;

            ASTNode* exprNode = parseExpr();
            initialNode->right = exprNode;

            if (currentToken.type == DELIM_SEMCOL) {
                ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
                exprNode->right = semicolonNode;
                nextToken();

                ASTNode* updNode = parseUpdStmt();
                semicolonNode->right = updNode;

                if (currentToken.type == DELIM_C_PAREN) {
                    ASTNode* closeParenNode = newNode(tokenTypeToString(currentToken.type));
                    updNode->right = closeParenNode;
                    nextToken();

                    if (currentToken.type == DELIM_O_BRACE) {
                        ASTNode* openBraceNode = newNode(tokenTypeToString(currentToken.type));
                        closeParenNode->right = openBraceNode;
                        nextToken();

                        ASTNode* loopBody = parseBody();
                        openBraceNode->right = loopBody;

                        if (currentToken.type == DELIM_C_BRACE) {
                            ASTNode* closeBraceNode = newNode(tokenTypeToString(currentToken.type));
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
        printf("SYNTAX ERROR LINE %d: Invalid <loop_initial> statement. Expected <dec_stmt> or <assign_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
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
        node->left = newNode(tokenTypeToString(currentToken.type));

        nextToken();
        if (currentToken.type == UNARY_OPE) {
            previousToken();
            node->left->right = parseUnaryVal();
            return node;
        } 

        if (currentToken.type == ASSIGNMENT_OPE) {
            node->left->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();
            node->left->right->right = parseExpr();
            return node;
        } else {
            printf("SYNTAX ERROR LINE %d: Expected assignment operator.\n", currentToken.sheeshLine, currentToken.value);
        }
        
        
    }

    printf("SYNTAX ERROR LINE %d: Invalid <upd_stmt>. Expected <unary_val> or <expr>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
    exit(1);

    return NULL;
}

ASTNode* parseMwLoop() {
    ASTNode* node = newNode("<mw_loop>");

    if (currentToken.type == MEANWHILE) {
        ASTNode *mwNode = newNode(tokenTypeToString(currentToken.type));
        node->left = mwNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            mwNode->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            ASTNode* exprNode = parseExpr();
            mwNode->right->right = exprNode;

            if (currentToken.type != DELIM_C_PAREN) {
                printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            exprNode->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            if (currentToken.type == DELIM_O_BRACE) {
                ASTNode* braceNode = newNode(tokenTypeToString(currentToken.type));
                exprNode->right->right = braceNode;
                nextToken();

                ASTNode* bodyNode = parseBody(); 
                braceNode->right = bodyNode;

                if (currentToken.type != DELIM_C_BRACE) {
                    printf("SYNTAX ERROR LINE %d: Expected '}' to close <mw_loop> body. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }

                ASTNode* closingBraceNode = newNode(tokenTypeToString(currentToken.type));
                bodyNode->right = closingBraceNode;
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
        ASTNode* doNode = newNode(tokenTypeToString(currentToken.type));
        node->left = doNode;
        nextToken();

        if (currentToken.type == DELIM_O_BRACE) {
            ASTNode* braceNode = newNode(tokenTypeToString(currentToken.type));
            doNode->right = braceNode;
            nextToken();
            braceNode->right = parseBody();

            if (currentToken.type != DELIM_C_BRACE) {
                printf("SYNTAX ERROR LINE %d: Expected '}' to close the block. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            braceNode->right->right = newNode(tokenTypeToString(currentToken.type));
            nextToken();

            if (currentToken.type == MEANWHILE) {
                ASTNode* mwNode = newNode(tokenTypeToString(currentToken.type));
                braceNode->right->right->right = mwNode;
                nextToken();

                if (currentToken.type == DELIM_O_PAREN) {
                    mwNode->right = newNode(tokenTypeToString(currentToken.type));
                    nextToken();

                    ASTNode* exprNode = parseExpr();
                    mwNode->right->right = exprNode;

                    if (currentToken.type != DELIM_C_PAREN) {
                        printf("SYNTAX ERROR LINE %d: Expected ')' after meanwhile condition. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                        exit(1);
                    }

                    exprNode->right = newNode(tokenTypeToString(currentToken.type));
                    nextToken();

                    if (currentToken.type == DELIM_SEMCOL) {
                        exprNode->right->right = newNode(tokenTypeToString(currentToken.type));
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
        ASTNode* inputNode = newNode(tokenTypeToString(currentToken.type));
        node->left = inputNode;
        nextToken();

        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* openParen = newNode(tokenTypeToString(currentToken.type));
            inputNode->right = openParen;
            nextToken();       
            openParen->right = parseInput();

            if (currentToken.type == DELIM_C_PAREN) {
                ASTNode* closeParen = newNode(tokenTypeToString(currentToken.type));
                openParen->right->right = closeParen;
                nextToken();
                
                if (currentToken.type == DELIM_SEMCOL) {
                    ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
                    closeParen->right = semicolonNode;
                    nextToken();

                    return node;
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected ';' in <input_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
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
        ASTNode* outNode = newNode(tokenTypeToString(currentToken.type));
        node->left = outNode;
        nextToken();
        
        if (currentToken.type == DELIM_O_PAREN) {
            ASTNode* openParen = newNode(tokenTypeToString(currentToken.type));
            outNode->right = openParen;
            nextToken();         
            openParen->right = parseOutput();

            if (currentToken.type == DELIM_C_PAREN) {
                ASTNode* closeParen = newNode(tokenTypeToString(currentToken.type));
                openParen->right->right = closeParen;
                nextToken();
                
                if (currentToken.type == DELIM_SEMCOL) {
                    ASTNode* semicolonNode = newNode(tokenTypeToString(currentToken.type));
                    closeParen->right = semicolonNode;
                    nextToken();
                    return node;
                } else {
                    printf("SYNTAX ERROR LINE %d: Expected ';' in <output_stmt>. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
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
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        ASTNode* current = node;
        while (currentToken.type == DELIM_COMMA) {
            ASTNode* commaNode = newNode(tokenTypeToString(currentToken.type));
            current->left->right = commaNode;
            current = commaNode;

            nextToken();
            if (strcmp(currentToken.value, "&") == 0) {
                ASTNode* andNode = newNode("&");
                commaNode->right = andNode;
                nextToken();
                if (currentToken.type == IDENTIFIER) {
                    andNode->right = newNode(tokenTypeToString(currentToken.type));
                    nextToken();
                } else {
                printf("SYNTAX ERROR LINE %d: Expected identifier after '&'. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
                    exit(1);
                }
            } else {
                printf("SYNTAX ERROR LINE %d: Expected '&' and identifier after ','. Got %s instead.\n", currentToken.sheeshLine, currentToken.value);
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
        node->left = newNode(tokenTypeToString(currentToken.type));
        nextToken();

        ASTNode* current = node;
        while (currentToken.type == DELIM_COMMA) {
            ASTNode* commaNode = newNode(tokenTypeToString(currentToken.type));
            node->left->right = commaNode;
            nextToken();
            
            if (currentToken.type == INVALID) {
                printf("SYNTAX ERROR LINE %d: Invalid <output_stmt>. Expected expr.\n", currentToken.sheeshLine, currentToken.value);
                exit(1);
            }

            commaNode->right = parseExpr();
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

#endif 	