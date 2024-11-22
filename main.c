#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex.h"

//Function where reading .shs file and writing to the SymbolTable.txt
int main(int argc, char *argv[]) {
    checkFilename(argc, argv[1]);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputSheesh = fopen("symbolTable.txt", "w");
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

    fclose(file);
    fclose(outputSheesh);

    printf("See symbolTable.txt for lexical analysis.\n");
    return 0;
}

//Ensure that the file is .shs
int checkFilename(int argc, char *argv) {
    if (argc < 2) {
        printf("Usage: <filename>\n");
        exit(1);
    }

    if (!strstr(argv, ".shs")) {
        printf("Error: Filetype must be '.shs'. Please try again.\n");
        exit(1);
    }
}

//Finite State Machine for Keyword
int checkKeyword(const char *sheeshLexeme) {
    int state = 1;

    for (int i = 0; sheeshLexeme[i] != '\0'; i++) {
        char curr = sheeshLexeme[i];

        switch(state) {
            case 1:
                if (curr == 'b') {
                    state = 25;
                } else if (curr == 'c') {
                    state = 31;
                } else if (curr == 'd') {
                    state = 34;
                } else if (curr == 'e') {
                    state = 40;
                } else if (curr == 'f') { 
                    state = 46;
                } else if (curr == 'g') {
                    state = 55;
                } else if (curr == 'i') {
                    state = 60;
                } else if (curr == 'j') { 
                    state = 66;
                } else if (curr == 'l') { 
                    state = 70;
                } else if (curr == 'm') {
                    state = 83;
                } else if (curr == 'n') {
                    state = 92;
                } else if (curr == 'o') {
                    state = 102;
                } else if (curr == 'p') {
                    state = 116;
                } else if (curr == 'r') {
                    state = 118;
                } else if (curr == 's') {
                    state = 121;
                } else if (curr == 't') {
                    state = 142;
                } else if (curr == 'v') {
                    state = 148;
                } else { 
                    return 0;
                }

                break;

            case 25:
                if (strcmp(sheeshLexeme + i, "ounce") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 31:
                if (strcmp(sheeshLexeme + i, "ar") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 34:
                if (strcmp(sheeshLexeme + i, "o") == 0 || strcmp(sheeshLexeme + i, "rift") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 40:
                if (strcmp(sheeshLexeme + i, "mpty") == 0 || strcmp(sheeshLexeme + i, "x") == 0 || strcmp(sheeshLexeme + i, "xtra") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 46:
                if (strcmp(sheeshLexeme + i, "lip") == 0 || strcmp(sheeshLexeme + i, "rozen") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 55:
                if (strcmp(sheeshLexeme + i, "roup") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 60:
                if (strcmp(sheeshLexeme + i, "f") == 0 || strcmp(sheeshLexeme + i, "nput") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 66:
                if (strcmp(sheeshLexeme + i, "ump") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 70:
                if (strcmp(sheeshLexeme + i, "egit") == 0 || strcmp(sheeshLexeme + i, "ockin") == 0 || strcmp(sheeshLexeme + i, "ong") == 0 || strcmp(sheeshLexeme + i, "ocked") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 83:
                if (strcmp(sheeshLexeme + i, "eanwhile") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 92:
                if (strcmp(sheeshLexeme + i, "ickname") == 0 || strcmp(sheeshLexeme + i, "um") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 102:
                if (strcmp(sheeshLexeme + i, "pen") == 0 || strcmp(sheeshLexeme + i, "ther") == 0 || strcmp(sheeshLexeme + i, "ut") == 0 || strcmp(sheeshLexeme + i, "utside") == 0 || strcmp(sheeshLexeme + i, "utput") == 0 || strcmp(sheeshLexeme + i, "therwise") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 116:
                if (strcmp(sheeshLexeme + i, "l") == 0 || strcmp(sheeshLexeme + i, "laylist") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 118:
                if (strcmp(sheeshLexeme + i, "ep") == 0 || strcmp(sheeshLexeme + i, "epeat") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 121:
                if (strcmp(sheeshLexeme + i, "cenario") == 0 || strcmp(sheeshLexeme + i, "hort") == 0 || strcmp(sheeshLexeme + i, "tandard") == 0 || strcmp(sheeshLexeme + i, "top") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 142:
                if (strcmp(sheeshLexeme + i, "eam") == 0 || strcmp(sheeshLexeme + i, "ext") == 0) {
                    return 1;
                } else {
                    return 0;
                }
                
                break;

            case 148:
                if (strcmp(sheeshLexeme + i, "ibe") == 0) {
                    return 1;
                } else {
                    return 0;
                }
                
                break;

            default:
                return 0;
        }
    }

    return 0;
}

//Finite State Machine for Reserved Words
int checkReservedWord(const char *sheeshLexeme) {
    int state = 1;

    for (int i = 0; sheeshLexeme[i] != '\0'; i++) {
        char curr = sheeshLexeme[i];

        switch(state) {
            case 1:
                if (curr == 'a') {
                    state = 2;
                } else if (curr == 'c') {
                    state = 3;
                } else if (curr == 'n') { 
                    state = 4;
                } else if (curr == 't') {
                    state = 5;
                } else {
                    return 0;
                }

                break;

            case 2:
                if (strcmp(sheeshLexeme + i, "lways") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 3:
                if (strcmp(sheeshLexeme + i, "ap") == 0 || strcmp(sheeshLexeme + i, "ont") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 4:
                if (strcmp(sheeshLexeme + i, "ocap") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 5:
                if (strcmp(sheeshLexeme + i, "op") == 0 || strcmp(sheeshLexeme + i, "optier") == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;
            
            default:
                return 0;
        }   
    }
    
    return 0;
}


int checkNoiseWord(const char *sheeshLexeme) {
    const char *noiseWords[] = { "aylist", "eat", "put", "tier", "tra", "wise", NULL };
    for (int i = 0; noiseWords[i] != NULL; i++) {
        if (strcmp(sheeshLexeme, noiseWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

//Finite State Machine for Identifier
int checkIdentifier(const char *sheeshLexeme) {
    int i = 0;
    char current = sheeshLexeme[i];
    int state = 1;

    while (current != '\0') {
        switch(state) {
            case 1:
                if (isalpha(current) || current == '_' || current == '#') {
                    state = 3;
                } else {
                    return 0;
                }

                break;

            case 3:
                if (isalnum(current)) {
                    state = 3;
                } else if (current == '#' || current == '_') {
                    state = 4;
                } else {
                    return 0;
                }

                break;

            case 4:
                if (isalnum(current)) {
                    state = 3;
                } else if (current == '#' || current == '_') {
                    state = 4;
                } else {
                    return 0;
                }

                break;

            default:
                return 0;
        }

        current = sheeshLexeme[++i];
    }

    if (state == 3) {
        return 1;
    }

    return 0;
}

int checkAssignment(const char *sheeshLexeme) {
    char *assignment_operators[6] = {"=", "+=", "-=", "*=", "/=", "%="};

    for (int i = 0; i < sizeof(assignment_operators) / sizeof(assignment_operators[0]); i++) {
        if (strcmp(sheeshLexeme, assignment_operators[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int checkLogical(const char *sheeshLexeme) {
    char *logical_operators[3] = {"&&", "||", "!"};

    for (int i = 0; i < sizeof(logical_operators) / sizeof(logical_operators[0]); i++) {
        if (strcmp(sheeshLexeme, logical_operators[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int checkArithmetic(const char *sheeshLexeme) {
    char *arithmetic_operators[7] = {"+", "-", "*", "/", "%", "^", "|"};

    for (int i = 0; i < sizeof(arithmetic_operators) / sizeof(arithmetic_operators[0]); i++) {
        if (strcmp(sheeshLexeme, arithmetic_operators[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int checkUnary(const char *sheeshLexeme) {
    char *unary_operators[2] = {"++", "--"};

    for (int i = 0; i < sizeof(unary_operators) / sizeof(unary_operators[0]); i++) {
        if (strcmp(sheeshLexeme, unary_operators[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int checkRelational(const char *sheeshLexeme) {
    char *relational_operators[6] = {"==", "!=", ">", "<", ">=", "<="};

    for (int i = 0; i < sizeof(relational_operators) / sizeof(relational_operators[0]); i++) {
        if (strcmp(sheeshLexeme, relational_operators[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

//Creating and returning a new token 
Token newToken(const char *value, TokenType type, int sheeshColumn) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.sheeshLine = sheeshColumn;

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
            return newToken(sheeshLexeme, FLOAT_CONSTANT, sheeshLine);
        } else {
            return newToken(sheeshLexeme, INT_CONSTANT, sheeshLine);
        }
    } else if (state == 3) {
        switch(ch) {
            case '\'':
                return newToken(sheeshLexeme, STRING_CONSTANT, sheeshLine);
            case '"':
                return newToken(sheeshLexeme, CHAR_CONSTANT, sheeshLine);    
        }
        return newToken(sheeshLexeme, STRING_CONSTANT, sheeshLine);
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

    for (int i = 0; sheeshLine[i] != '\0'; i++) {

        if (inMultiLineComment) {
            if (sheeshLine[i] == '*' && sheeshLine[i + 1] == '/') {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s \n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                i++;
            }
            continue;
        }

        if (stringLiteral) {
            temp[tempMarker++] = sheeshLine[i];
            if (sheeshLine[i] == '"') {
                temp[tempMarker] = '\0';
                Token token = newToken(temp, STRING_CONSTANT, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
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
                    Token token = newToken(temp, CHAR_CONSTANT, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    free(token.value);
                    tempMarker = 0;
                    characterConstant = 0;
                } else {
                    temp[tempMarker] = '\0';
                    if (tempMarker <= 2) {
                        Token token = newToken(temp, QUOTATION, sheeshColumn);
                        fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                        free(token.value);
                    }
                    else {
                        Token token = newToken(temp, INVALID, sheeshColumn);
                        fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                        free(token.value);
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
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            stringLiteral = 1;
            temp[tempMarker++] = sheeshLine[i];
            continue;
        }

        if (sheeshLine[i] == '\'') {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            characterConstant = 1;
            temp[tempMarker++] = sheeshLine[i];
            continue;
        }

        if (sheeshLine[i] == '/' && sheeshLine[i + 1] == '/') {
            Token token = newToken("//", COMMENT, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
            break;
        }

        if (sheeshLine[i] == '/' && sheeshLine[i + 1] == '*') {
            Token token = newToken("/*", COMMENT, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
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
                (sheeshLine[i] == '%' && sheeshLine[i+1] == '=')) {   
                
                if (tempMarker > 0) {
                    temp[tempMarker] = '\0';
                    Token token = sheeshLexer(temp, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    free(token.value);
                    tempMarker = 0;
                }

                Token token = sheeshLexer(lookAhead, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                
                i++;  
                continue;
            }
            
            
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            char op[2] = {sheeshLine[i], '\0'};
            Token token = sheeshLexer(op, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
            continue;
        }

       
        TokenType delimiter;
        switch (sheeshLine[i]) {
            case ',':
                delimiter = COMMA;
                break;
            case ';':
                delimiter = SEMICOLON;
                break;
            case '(':
                delimiter = O_PARENTHESIS;
                break;
            case ')':
                delimiter = C_PARENTHESIS;
                break;
            case '[':
                delimiter = O_BRACKET;
                break;
            case ']':
                delimiter = C_BRACKET;
                break;
            case '{':
                delimiter = O_BRACE;
                break;
            case '}':
                delimiter = C_BRACE;
                break;
            default:
                delimiter = INVALID;
        }

        if (delimiter != INVALID) {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            char delim[2] = {sheeshLine[i], '\0'};
            Token token = newToken(delim, delimiter, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
            continue;
        }
        
        if ((sheeshLine[i] == '+' && sheeshLine[i + 1] == '+') || 
            (sheeshLine[i] == '-' && sheeshLine[i + 1] == '-')) {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = sheeshLexer(temp, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            char *operator = "";
            if (sheeshLine[i] == '+') {
                operator = "++";
            } else if (sheeshLine[i] == '-') {
                operator = "--";
            }

            Token token = newToken(operator, UNARY_OPE, sheeshColumn);

            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
            i++;
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
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value); 
            } else if (checkReservedWord(temp)) {
                if (!(strcmp(temp, "cap")) || !(strcmp(temp, "nocap"))) {
                    Token token = newToken(temp, CONSTANDRESERVED, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    free(token.value);
                } else {
                    Token token = newToken(temp, RESERVED_WORD, sheeshColumn);
                    fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                    free(token.value);
                }
            } else if (checkNoiseWord(temp)) {
                Token token = newToken(temp, NOISE_WORD, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
            } else if (checkIdentifier(temp)) {
                Token token = newToken(temp, IDENTIFIER, sheeshColumn);  
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value); 
            } else {
                Token token = newToken(temp, INVALID, sheeshColumn);  
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value); 
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
                free(token.value);  
                tempMarker = 0;  
            }
        } else {
            continue;
        }
    }

    if (tempMarker > 0) {
        temp[tempMarker] = '\0';
        if (stringLiteral || characterConstant) {
            Token token = newToken(temp, QUOTATION, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: Delimiter (Quotation)\n", token.sheeshLine, token.value);
            free(token.value);
        } else {
            Token token = sheeshLexer(temp, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
        }
    }
}