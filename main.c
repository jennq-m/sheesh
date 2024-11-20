#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
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

// Keywords, Reserved Words, and Noise Words
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
int checkKeyword(const char *word);
int checkReservedWord(const char *word);
int checkNoiseWord(const char *word);
int checkAssignment(const char *token);
int checkLogical(const char *token);
int checkArithmetic(const char *token);
int checkUnary(const char *token);
int checkRelational(const char *token);
Token newToken(const char *value, TokenType type, int line);
Token fsmClassify(const char *token, int line);
void analyzeLine(FILE *outputFile, char *line, int lineNumber);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <test.shs>\n", argv[0]);
        return 1;
    }

    // Validate file extension
    if (!strstr(argv[1], ".shs")) {
        printf("Error: Only .shs files are allowed.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputFile = fopen("symbolTable.txt", "w");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(file);
        return 1;
    }

    char line[256];
    int lineNumber = 1;

    fprintf(outputFile, "Lexical Analysis of %s:\n\n", argv[1]);

    while (fgets(line, sizeof(line), file)) {
        analyzeLine(outputFile, line, lineNumber);
        lineNumber++;
    }

    fclose(file);
    fclose(outputFile);

    printf("See symbolTable.txt for lexical analysis.\n");
    return 0;
}

int checkKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkReservedWord(const char *word) {
    for (int i = 0; reservedWords[i] != NULL; i++) {
        if (strcmp(word, reservedWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkNoiseWord(const char *word) {
    for (int i = 0; noiseWords[i] != NULL; i++) {
        if (strcmp(word, noiseWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkIdentifier(const char *token) {
    int i = 0;
    char current = token[i];
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

        current = token[++i];
    }

    return state == 3;
}

int checkAssignment(const char *token) {
    int assignment = strcmp(token, "=") == 0 || strcmp(token, "+=") == 0 || strcmp(token, "-=") == 0 || strcmp(token, "*=") == 0 || strcmp(token, "/=") == 0 || strcmp(token, "%=") == 0;
    
    return assignment;
}

int checkLogical(const char *token) {
    int logical = strcmp(token, "&&") == 0 || strcmp(token, "||") == 0 || strcmp(token, "!") == 0;

    return logical;
}

int checkArithmetic(const char *token) {
    int arithmetic = strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0 || strcmp(token, "%") == 0 || strcmp(token, "^") == 0 || strcmp(token, "$") == 0;
    
    return arithmetic;
}

int checkUnary(const char *token) {
    int unary = strcmp(token, "++") == 0 || strcmp(token, "--") == 0;

    return unary;
}

int checkRelational(const char *token) {
    char *relational_operators[6] = {"==", "!=", ">", "<", ">=", "<="};

    for (int i = 0; i < sizeof(relational_operators); i++) {
        if (strcmp(token, relational_operators[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

char *typeToString(TokenType type) {
    switch (type) {
        case IDENTIFIER: return "Identifier";
        case KEYWORD: return "Keyword";
        case RESERVED_WORD: return "Reserved Word";
        case CONSTANT: return "Constant";
        case CONSTANDRESERVED: return "Constant, Reserved Word";
        case NOISE_WORD: return "Noise Word";
        case COMMENT: return "Comment";
        case ARITHMETIC_OPE: return "Arithmetic Operator";
        case ASSIGNMENT_OPE: return "Assignment Operator";
        case LOGICAL_OPE: return "Logical Operator (Boolean)";
        case UNARY_OPE: return "Unary Operator";
        case RELATIONAL_OPE: return "Relational Operator (Boolean)";
        case DELIMITER: return "Delimiter";
        case INT_CONSTANT: return "Constant (Num (int in C))";
        case FLOAT_CONSTANT: return "Constant (Drift (float in C))";
        case STRING_CONSTANT: return "Constant (Text (string in C))";
        case INVALID: return "Invalid";
        default: return "Invalid";
    }

}

Token newToken(const char *value, TokenType type, int line) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.line = line;

    return token;
}
Token fsmClassify(const char *token, int line) {
    int state = 0; // Initial state
    char ch;
    int decimalCount = 0; // To track the number of decimal points in the number
    int isInteger = 1; // To track if the constant is an integer
    int isFloat = 0;   // To track if the constant is a floating-point number

    for (int i = 0; (ch = token[i]) != '\0'; i++) {
        switch (state) {
            case 0: // Initial state
                if (isalpha(ch) || ch == '_' || ch == '#') state = 1;
                else if (isdigit(ch)) state = 2;
                else if (ch == '\'' || ch == '"') state = 3;
                else if (strchr("+-*/%=!<>&|^$", ch)) state = 4;
                else if (strchr(",;(){}[]", ch)) state = 5;
                else if (ch == '/' && token[i + 1] == '/') return newToken(token, COMMENT, line);
                else if (ch == '/' && token[i + 1] == '*') return newToken(token, COMMENT, line);
                else return newToken(token, INVALID, line);
                break;
            case 1: // Identifier
                if (!isalnum(ch) && ch != '_' && ch != '#') return newToken(token, INVALID, line);
                break;
            case 2: // Constant (number)
                if (ch == '.') {
                    if (decimalCount > 0) return newToken(token, INVALID, line);
                    decimalCount++;
                    isFloat = 1; // Set flag if decimal point is found
                } else if (!isdigit(ch)) {
                    return newToken(token, INVALID, line);
                }
                break;
            case 3: // Constant (char/string)
                if (ch == '\'' || ch == '"') return newToken(token, CONSTANT, line);
                break;
            case 4: // Operator
                if (strchr("+-*/%=!<>&|^$", ch)) state = 4;
                break;
            case 5: // Delimiter
                return newToken(token, DELIMITER, line);
        }
    }

    // Final classification based on state
    if (state == 1) {
        if (checkKeyword(token)) return newToken(token, KEYWORD, line);
        if (checkReservedWord(token)) {
            if (strcmp(token, "cap") || strcmp(token, "nocap")) {
                return newToken(token, CONSTANDRESERVED, line);
            }
            else {
                return newToken(token, RESERVED_WORD, line);
            }
        }
    
        if (checkNoiseWord(token)) return newToken(token, NOISE_WORD, line);
        if (checkIdentifier(token)) return newToken(token, IDENTIFIER, line);
        return newToken(token, INVALID, line);
    } else if (state == 2) {
        if (isFloat) {
            return newToken(token, FLOAT_CONSTANT, line); // Float constant
        } else {
            return newToken(token, INT_CONSTANT, line); // Integer constant
        }
    } else if (state == 3) {
        return newToken(token, STRING_CONSTANT, line); // String constant
    } else if (state == 4) {
        if (checkAssignment(token)) return newToken(token, ASSIGNMENT_OPE, line);
        if (checkLogical(token)) return newToken(token, LOGICAL_OPE, line);
        if (checkArithmetic(token)) return newToken(token, ARITHMETIC_OPE, line);
        if (checkUnary(token)) return newToken(token, UNARY_OPE, line);
        if (checkRelational(token)) return newToken(token, RELATIONAL_OPE, line);
    } else if (state == 5) {
        return newToken(token, DELIMITER, line);
    }

    return newToken(token, INVALID, line);
}


void analyzeLine(FILE *outputFile, char *line, int lineNumber) {
    static int inMultiLineComment = 0; // Tracks whether inside a multi-line comment
    int stringLiteral = 0;             // Tracks whether inside a string literal
    char temp[256];                   // Buffer for building tokens
    int tempIndex = 0;                // Index for buffer

    for (int i = 0; line[i] != '\0'; i++) {
        if (inMultiLineComment) {
            // Check for the end of the multi-line comment
            if (line[i] == '*' && line[i + 1] == '/') {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s \n", token.line, token.value, typeToString(token.type));
                free(token.value);
                i++; // Skip the '/'
            }
            continue; // Skip all characters in the multi-line comment
        }

        if (stringLiteral) {
            // Handle string literals
            temp[tempIndex++] = line[i];
            if (line[i] == '"') {
                // End of the string literal
                temp[tempIndex] = '\0';
                Token token = newToken(temp, CONSTANT, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s (Text (string in C))\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempIndex = 0;
                stringLiteral = 0;
            }
            continue;
        }

        // Start of a string literal
        if (line[i] == '"') {
            if (tempIndex > 0) {
                temp[tempIndex] = '\0';
                Token token = fsmClassify(temp, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempIndex = 0;
            }

            stringLiteral = 1;
            temp[tempIndex++] = line[i];
            continue;
        }

        // Handle comments
        if (line[i] == '/' && line[i + 1] == '/') {
            Token token = newToken("//", COMMENT, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            break; // Ignore the rest of the line
        }
        if (line[i] == '/' && line[i + 1] == '*') {
            Token token = newToken("/*", COMMENT, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            inMultiLineComment = 1;
            i++; // Skip the '*'
            continue;
        }

        // Handle delimiters
        if (strchr(",;(){}[]", line[i])) {
            if (tempIndex > 0) {
                temp[tempIndex] = '\0'; // Null-terminate the current token
                Token token = fsmClassify(temp, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tempIndex = 0;
            }

            char delim[2] = {line[i], '\0'};
            Token token = newToken(delim, DELIMITER, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: Delimiter\n", token.line, token.value);
            free(token.value);
            continue;
        }

        if (!isspace(line[i])) {
            temp[tempIndex++] = line[i];
        } else if (tempIndex > 0) {
            temp[tempIndex] = '\0';
            Token token = fsmClassify(temp, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            tempIndex = 0;
        }
    }

    if (tempIndex > 0) {
        temp[tempIndex] = '\0';
        if (stringLiteral) {
            Token token = newToken(temp, DELIMITER, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: Delimiter\n", token.line, token.value);
            free(token.value);
        } else {
            Token token = fsmClassify(temp, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
        }
    }
}
