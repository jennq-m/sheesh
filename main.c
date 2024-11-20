#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    IDENTIFIER, KEYWORD, RESERVED_WORD, CONSTANT, NOISE_WORD, COMMENT, 
    ARITHMETIC_OPE, ASSIGNMENT_OPE, LOGICAL_OPE, UNARY_OPE, RELATIONAL_OPE, 
    DELIMITER, INVALID, INT_CONSTANT, FLOAT_CONSTANT, STRING_CONSTANT, CONSTANDRESERVED
} TokenType;

typedef struct {
    TokenType type;
    char *token_type;
    char *value;
    int line;
} Token;

const char *keywords[] = {
    "bounce", "car", "do", "drift", "empty", "ex", "flip", "frozen",
    "group", "iffy", "input", "jump", "legit", "locked", "lockin",
    "long", "meanwhile", "nickname", "num", "open", "other", "out",
    "outside", "pl", "rep", "scenario", "short", "standard", "stop",
    "team", "text", "vibe", NULL
};

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
Token newToken(const char *value, TokenType type, int line);
Token fsmClassify(const char *sheeshLexeme, int line);
void analyzeLine(FILE *outputJam, char *line, int sheeshmark);

int main(int argc, char *argv[]) {
    checkFilename(argc, argv[1]);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputJam = fopen("symbolTable.txt", "w");
    if (!outputJam) {
        perror("Error opening output file");
        fclose(file);
        return 1;
    }

    char line[256];
    int sheeshmark = 1;

    fprintf(outputJam, "Lexical Analysis of %s:\n\n", argv[1]);

    while (fgets(line, sizeof(line), file)) {
        analyzeLine(outputJam, line, sheeshmark);
        sheeshmark++;
    }

    fclose(file);
    fclose(outputJam);

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
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(sheeshLexeme, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
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
    char *arithmetic_operators[7] = {"+", "-", "*", "/", "%", "^", "$"};

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
            return "Constant, Reserved Word";
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
        case DELIMITER: 
            return "Delimiter";
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

Token newToken(const char *value, TokenType type, int sheeshmark) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.line = sheeshmark;

    return token;
}

Token fsmClassify(const char *sheeshLexeme, int line) {
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
                    return newToken(sheeshLexeme, COMMENT, line); 
                } else if (ch == '/' && sheeshLexeme[i + 1] == '*') { 
                    return newToken(sheeshLexeme, COMMENT, line); 
                } else { 
                    return newToken(sheeshLexeme, INVALID, line); 
                }

                break;

            case 1:
                if (!isalnum(ch) && ch != '_' && ch != '#') {
                    return newToken(sheeshLexeme, INVALID, line);
                }

                break;

            case 2:
                if (ch == '.') {
                    if (decimalCount > 0) {
                        return newToken(sheeshLexeme, INVALID, line);
                    }

                    decimalCount++;
                    checkFloat = 1;
                } else if (!isdigit(ch)) {
                    return newToken(sheeshLexeme, INVALID, line);
                }

                break;

            case 3:
                if (ch == '\'' || ch == '"') {
                    return newToken(sheeshLexeme, CONSTANT, line);
                }

                break;

            case 4:
                if (strchr("+-*/%=!<>&|^$", ch)) {
                    state = 4;
                }
                
                break;

            case 5:
                return newToken(sheeshLexeme, DELIMITER, line);
        }
    }

    if (state == 1) {
        if (checkKeyword(sheeshLexeme)) {
            return newToken(sheeshLexeme, KEYWORD, line);
        }

        if (checkReservedWord(sheeshLexeme)) {
            if (!(strcmp(sheeshLexeme, "cap")) || !(strcmp(sheeshLexeme, "nocap"))) {
                return newToken(sheeshLexeme, CONSTANDRESERVED, line);
            }
            else {
                return newToken(sheeshLexeme, RESERVED_WORD, line);
            }
        }
    
        if (checkNoiseWord(sheeshLexeme)) {
            return newToken(sheeshLexeme, NOISE_WORD, line);
        }

        if (checkIdentifier(sheeshLexeme)) {
            return newToken(sheeshLexeme, IDENTIFIER, line);
        }

        return newToken(sheeshLexeme, INVALID, line);
    } else if (state == 2) {
        if (checkFloat) {
            return newToken(sheeshLexeme, FLOAT_CONSTANT, line);
        } else {
            return newToken(sheeshLexeme, INT_CONSTANT, line);
        }
    } else if (state == 3) {
        return newToken(sheeshLexeme, STRING_CONSTANT, line);
    } else if (state == 4) {
        if (checkAssignment(sheeshLexeme)) {
            return newToken(sheeshLexeme, ASSIGNMENT_OPE, line);
        }

        if (checkLogical(sheeshLexeme)) {
            return newToken(sheeshLexeme, LOGICAL_OPE, line);
        }

        if (checkArithmetic(sheeshLexeme)) {
            return newToken(sheeshLexeme, ARITHMETIC_OPE, line);
        }

        if (checkUnary(sheeshLexeme)) {
            return newToken(sheeshLexeme, UNARY_OPE, line);
        }

        if (checkRelational(sheeshLexeme)) {
            return newToken(sheeshLexeme, RELATIONAL_OPE, line);
        }
    } else if (state == 5) {
        return newToken(sheeshLexeme, DELIMITER, line);
    }

    return newToken(sheeshLexeme, INVALID, line);
}


void analyzeLine(FILE *outputJam, char *line, int sheeshmark) {
    static int inMultiLineComment = 0;
    int stringLiteral = 0;
    char temp[256];
    int tempMarker = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (inMultiLineComment) {
            // Check for the end of the multi-line comment
            if (line[i] == '*' && line[i + 1] == '/') {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, sheeshmark);
                fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s \n", token.line, token.value, typeToString(token.type));
                free(token.value);
                i++; // Skip the '/'
            }
            continue; // Skip all characters in the multi-line comment
        }

        if (stringLiteral) {
            // Handle string literals
            temp[tempMarker++] = line[i];
            if (line[i] == '"') {
                // End of the string literal
                temp[tempMarker] = '\0';
                Token token = newToken(temp, CONSTANT, sheeshmark);
                fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s (Text (string in C))\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
                stringLiteral = 0;
            }
            continue;
        }

        // Start of a string literal
        if (line[i] == '"') {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0';
                Token token = fsmClassify(temp, sheeshmark);
                fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            stringLiteral = 1;
            temp[tempMarker++] = line[i];
            continue;
        }

        // Handle comments
        if (line[i] == '/' && line[i + 1] == '/') {
            Token token = newToken("//", COMMENT, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            break; // Ignore the rest of the line
        }

        if (line[i] == '/' && line[i + 1] == '*') {
            Token token = newToken("/*", COMMENT, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            inMultiLineComment = 1;
            i++; // Skip the '*'
            continue;
        }

        // Handle delimiters
        if (strchr(",;(){}[]", line[i])) {
            if (tempMarker > 0) {
                temp[tempMarker] = '\0'; // Null-terminate the current token
                Token token = fsmClassify(temp, sheeshmark);
                fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempMarker = 0;
            }

            char delim[2] = {line[i], '\0'};
            Token token = newToken(delim, DELIMITER, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: Delimiter\n", token.line, token.value);
            free(token.value);
            continue;
        }

        if (!isspace(line[i])) {
            temp[tempMarker++] = line[i];
        } else if (tempMarker > 0) {
            temp[tempMarker] = '\0';
            Token token = fsmClassify(temp, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            tempMarker = 0;
        }
    }

    if (tempMarker > 0) {
        temp[tempMarker] = '\0';
        if (stringLiteral) {
            Token token = newToken(temp, DELIMITER, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: Delimiter\n", token.line, token.value);
            free(token.value);
        } else {
            Token token = fsmClassify(temp, sheeshmark);
            fprintf(outputJam, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
        }
    }
}
