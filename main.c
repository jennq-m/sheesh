#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    IDENTIFIER, KEYWORD, RESERVED_WORD, CONSTANT, NOISE_WORD, COMMENT, 
    ARITHMETIC_OPE, ASSIGNMENT_OPE, LOGICAL_OPE, UNARY_OPE, RELATIONAL_OPE, 
    COMMA, SEMICOLON, QUOTATION, O_PARENTHESIS, C_PARENTHESIS, O_BRACE, 
    C_BRACE, O_BRACKET, C_BRACKET, INVALID, INT_CONSTANT, 
    FLOAT_CONSTANT, STRING_CONSTANT, CONSTANDRESERVED
} TokenType;

typedef struct {
    TokenType type;
    char *token_type;
    char *value;
    int sheeshLine;
} Token;

const char *reservedWords[] = { "always", "cap", "cont", "nocap", "toptier", NULL };

const char *noiseWords[] = { "aylist", "eat", "put", "tier", "tra", "wise", NULL };

// Function prototypes
int checkFilename(int argc, char *argv);
int checkKeyword(const char *sheeshLexeme);
int checkReservedWord(const char *sheeshLexeme);
int checkNoiseWord(const char *sheeshLexeme);
int checkAssignment(const char *sheeshLexeme);
int checkLogical(const char *sheeshLexeme);
int checkArithmetic(const char *sheeshLexeme);
int checkUnary(const char *sheeshLexeme);
int checkRelational(const char *sheeshLexeme);
Token newToken(const char *value, TokenType type, int sheeshLine);
Token sheeshLexer(const char *sheeshLexeme, int sheeshLine);
void analyzeLine(FILE *outputSheesh, char *sheeshLine, int sheeshColumn);

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
        analyzeLine(outputSheesh, sheeshLine, sheeshColumn);
        sheeshColumn++;
    }

    fclose(file);
    fclose(outputSheesh);

    printf("See symbolTable.txt for lexical analysis.\n");
    return 0;
}

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

int checkKeyword(const char *sheeshLexeme) {
    int state = 1;

    for (int i = 0; sheeshLexeme[i] != '\0'; i++) {
        char curr = sheeshLexeme[i];

        switch(state) {
            case 1: // Q1 (start state)
                if (curr == 'b') state = 25;
                if (curr == 'c') state = 31;
                if (curr == 'd') state = 34;
                if (curr == 'e') state = 40;
                if (curr == 'f') state = 46;
                if (curr == 'g') state = 55;
                if (curr == 'i') state = 60;
                if (curr == 'j') state = 66;
                if (curr == 'l') state = 70;
                if (curr == 'm') state = 83;
                if (curr == 'n') state = 92;
                if (curr == 'o') state = 102;
                if (curr == 'p') state = 116;
                if (curr == 'r') state = 118;
                if (curr == 's') state = 121;
                if (curr == 't') state = 142;
                if (curr == 'v') state = 148;

                break;

            case 25:
                if (strncmp(sheeshLexeme + i, "ounce", 5) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 31:
                if (strncmp(sheeshLexeme + i, "ar", 2) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 34:
                if (strncmp(sheeshLexeme + i, "o", 1) == 0 || strncmp(sheeshLexeme + i, "rift", 4) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 40:
                if (strncmp(sheeshLexeme + i, "mpty", 4) == 0 || strncmp(sheeshLexeme + i, "x", 1) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 46:
                if (strncmp(sheeshLexeme + i, "lip", 3) == 0 || strncmp(sheeshLexeme + i, "rozen", 5) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 55:
                if (strncmp(sheeshLexeme + i, "roup", 4) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 60:
                if (strncmp(sheeshLexeme + i, "f", 1) == 0 || strncmp(sheeshLexeme + i, "nput", 4) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 66:
                if (strncmp(sheeshLexeme + i, "ump", 3) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 70:
                if (strncmp(sheeshLexeme + i, "egit", 4) == 0 || strncmp(sheeshLexeme + i, "ockin", 5) == 0 || strncmp(sheeshLexeme + i, "ong", 3) == 0 || strncmp(sheeshLexeme + i, "ocked", 5) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 83:
                if (strncmp(sheeshLexeme + i, "eanwhile", 8) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 92:
                if (strncmp(sheeshLexeme + i, "ickname", 7) == 0 || strncmp(sheeshLexeme + i, "um", 2) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 102:
                if (strncmp(sheeshLexeme + i, "pen", 3) == 0 || strncmp(sheeshLexeme + i, "ther", 4) == 0 || strncmp(sheeshLexeme + i, "ut", 2) == 0 || strncmp(sheeshLexeme + i, "utside", 6) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 116:
                if (strncmp(sheeshLexeme + i, "l", 1) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 118:
                if (strncmp(sheeshLexeme + i, "ep", 2) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 121:
                if (strncmp(sheeshLexeme + i, "cenario", 7) == 0 || strncmp(sheeshLexeme + i, "hort", 4) == 0 || strncmp(sheeshLexeme + i, "tandard", 7) == 0 || strncmp(sheeshLexeme + i, "top", 3) == 0) {
                    return 1;
                } else {
                    return 0;
                }

                break;

            case 142:
                if (strncmp(sheeshLexeme + i, "eam", 3) == 0 || strncmp(sheeshLexeme + i, "ext", 3) == 0) {
                    return 1;
                } else {
                    return 0;
                }
                
                break;
                
            case 148:
                if (strncmp(sheeshLexeme + i, "ibe", 3) == 0) {
                    return 1;
                } else {
                    return 0;
                }
                
                break;
        }
    }
}

int checkReservedWord(const char *sheeshLexeme) {
    for (int i = 0; reservedWords[i] != NULL; i++) {
        if (strcmp(sheeshLexeme, reservedWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkNoiseWord(const char *sheeshLexeme) {
    for (int i = 0; noiseWords[i] != NULL; i++) {
        if (strcmp(sheeshLexeme, noiseWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// FSM to check if identifier is valid
int checkIdentifier(const char *sheeshLexeme) {
    int i = 0;
    char current = sheeshLexeme[i];
    int state = 1;

    while (current != '\0') {
        switch(state) {
            case 1: // Q1
                if (isalpha(current) || current == '_' || current == '#') {
                    state = 3;
                } else {
                    return 0;
                }

                break;

            case 3: // Q3
                if (isalnum(current)) {
                    state = 3;
                } else if (current == '#' || current == '_') {
                    state = 4;
                } else {
                    return 0;
                }

                break;

            case 4: // Q4
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
            return "Constant (Legit (bool in C))";
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
        case INVALID: 
            return "Invalid";
        default: 
            return "Invalid";
    }
}

Token newToken(const char *value, TokenType type, int sheeshColumn) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.sheeshLine = sheeshColumn;

    return token;
}

Token sheeshLexer(const char *sheeshLexeme, int sheeshLine) {
    int state = 0;
    char ch;
    int decimalCount = 0;
    int checkFloat = 0;

    for (int i = 0; (ch = sheeshLexeme[i]) != '\0'; i++) {
        switch (state) {
            case 0: // Q0 (start state)
                if (isalpha(ch) || ch == '_' || ch == '#') {
                    state = 1;
                } else if (isdigit(ch)) { 
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
        if (checkFloat) {
            return newToken(sheeshLexeme, FLOAT_CONSTANT, sheeshLine);
        } else {
            return newToken(sheeshLexeme, INT_CONSTANT, sheeshLine);
        }
    } else if (state == 3) {
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


void analyzeLine(FILE *outputSheesh, char *sheeshLine, int sheeshColumn) {
    static int inMultiLineComment = 0;
    int stringLiteral = 0;
    char temp[256];
    int tempMarker = 0;

    for (int i = 0; sheeshLine[i] != '\0'; i++) {
        if (inMultiLineComment) {
            // Check for the end of the multi-sheeshLine comment
            if (sheeshLine[i] == '*' && sheeshLine[i + 1] == '/') {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s \n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                i++; // Skip the '/'
            }
            continue; // Skip all characters in the multi-sheeshLine comment
        }

        if (stringLiteral) {
            // Handle string literals
            temp[tempMarker++] = sheeshLine[i];
            if (sheeshLine[i] == '"') {
                // End of the string literal
                temp[tempMarker] = '\0';
                Token token = newToken(temp, CONSTANT, sheeshColumn);
                fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s (Text (string in C))\n", token.sheeshLine, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
                stringLiteral = 0;
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
            i++; // Skip the '*'
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
        
         // Handle `++` and '--'
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

        if (!isspace(sheeshLine[i])) {
            temp[tempMarker++] = sheeshLine[i];
        } else if (tempMarker > 0) {
            temp[tempMarker] = '\0';
            Token token = sheeshLexer(temp, sheeshColumn);
            fprintf(outputSheesh, "Line %d: Lexeme: %-15s Token: %s\n", token.sheeshLine, token.value, typeToString(token.type));
            free(token.value);
            tempMarker = 0;
        }
    }

    if (tempMarker > 0) {
        temp[tempMarker] = '\0';
        if (stringLiteral) {
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
