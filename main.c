#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    IDENTIFIER, KEYWORD, RESERVED_WORD, CONSTANT, NOISE_WORD, COMMENT,
    OPERATOR, DELIMITER, WHITESPACE, INVALID
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
int isKeyword(const char *word);
int isReservedWord(const char *word);
int isNoiseWord(const char *word);
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

    printf("Results saved to symbolTable.txt\n");
    return 0;
}

int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isReservedWord(const char *word) {
    for (int i = 0; reservedWords[i] != NULL; i++) {
        if (strcmp(word, reservedWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isNoiseWord(const char *word) {
    for (int i = 0; noiseWords[i] != NULL; i++) {
        if (strcmp(word, noiseWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *typeToString(TokenType type) {
     switch (type) {
        case 0: return "Identifier";
        case 1: return "Keyword";
        case 2: return "Reserved Word";
        case 3: return "Constant";
        case 4: return "Noise Word";
        case 5: return "Comment";
        case 6: return "Operator";
        case 7: return "Delimiter";
        case 8: return "Whitespace";
        case 9: return "Invalid";
        default: return "Invalid";
    }
}

int check_identifier(const char *token) {
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

    for (int i = 0; (ch = token[i]) != '\0'; i++) {
        switch (state) {
            case 0: // Initial state
                if (isalpha(ch) || ch == '_' || ch == '#') state = 1;
                else if (isdigit(ch)) state = 2;
                else if (ch == '\'' || ch == '"') state = 3;
                else if (strchr("=+-*/%&|!<>", ch)) state = 4;
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
                } else if (!isdigit(ch)) {
                    return newToken(token, INVALID, line);
                }
                break;
            case 3: // Constant (char/string)
                if (ch == '\'' || ch == '"') return newToken(token, CONSTANT, line);
                break;
            case 4: // Operator
                if (!strchr("=+-*/%&|!<>", ch)) return newToken(token, INVALID, line);
                break;
            case 5: // Delimiter
                return newToken(token, DELIMITER, line);
        }
    }

    // Final classification based on state
    if (state == 1) {
        if (isKeyword(token)) return newToken(token, KEYWORD, line);
        if (isReservedWord(token)) return newToken(token, RESERVED_WORD, line);
        if (isNoiseWord(token)) return newToken(token, NOISE_WORD, line);
        if (check_identifier(token)) return newToken(token, IDENTIFIER, line);
        return newToken(token, INVALID, line);
    } else if (state == 2 || state == 3) {
        return newToken(token, CONSTANT, line);
    } else if (state == 4) {
        return newToken(token, OPERATOR, line);
    } else if (state == 5) {
        return newToken(token, DELIMITER, line);
    }

    return newToken(token, INVALID, line);
}


void analyzeLine(FILE *outputFile, char *line, int lineNumber) {
    static int inMultiLineComment = 0; // Tracks whether inside a multi-line comment
    char *tokenStr = strtok(line, " \t\n");
    
    while (tokenStr != NULL) {
        if (inMultiLineComment) {
            // Check if the multi-line comment ends in this token
            char *endComment = strstr(tokenStr, "*/");

            // If still inside multi-line comment, skip token
            if (endComment) {
                inMultiLineComment = 0;
                Token token = newToken("*/", COMMENT, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
                free(token.value);
                tokenStr = strtok(NULL, " \t\n");
                continue;
            }

            tokenStr = strtok(NULL, " \t\n");
            continue;
        }

        if (strncmp(tokenStr, "//", 2) == 0) {
            Token token = newToken("//", COMMENT, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
            return;
        }

        if (strncmp(tokenStr, "/*", 2) == 0) {
            Token token = newToken("/*", COMMENT, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);

            if (strstr(tokenStr, "*/")) {
                tokenStr = strtok(NULL, " \t\n");
                continue;
            }

            inMultiLineComment = 1;
            tokenStr = strtok(NULL, " \t\n");
            continue;
        }

        char temp[256];
        int j = 0;
        for (int i = 0; tokenStr[i] != '\0'; i++) {
            if (strchr(",;(){}[]", tokenStr[i])) {
                if (j > 0) {
                    temp[j] = '\0';
                    Token token = fsmClassify(temp, lineNumber);
                    fprintf(outputFile, "Line %d: Lexeme: %-15s \tToken: %s\n", token.line, token.value, typeToString(token.type));
                    free(token.value);
                    j = 0;
                }

                char delim[2] = {tokenStr[i], '\0'};
                Token token = newToken(delim, DELIMITER, lineNumber);
                fprintf(outputFile, "Line %d: Lexeme: %-15s Token: Delimiter\n", token.line, token.value);
                free(token.value);
            } else {
                temp[j++] = tokenStr[i];
            }
        }

        if (j > 0) {
            temp[j] = '\0';
            Token token = fsmClassify(temp, lineNumber);
            fprintf(outputFile, "Line %d: Lexeme: %-15s Token: %s\n", token.line, token.value, typeToString(token.type));
            free(token.value);
        }

        tokenStr = strtok(NULL, " \t\n");
    }
}