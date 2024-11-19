#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    IDENTIFIER, KEYWORD, RESERVED_WORD, CONSTANT, NOISE_WORD, COMMENT,
    OPERATOR, DELIMITER, INVALID
} TokenType;

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
TokenType fsmClassify(const char *token);
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

    printf("Lexical analysis completed. Results saved to symbolTable.txt\n");
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

TokenType fsmClassify(const char *token) {
    int state = 0; // Initial state
    char ch;
    int decimalCount = 0; // To track the number of decimal points in the number

    for (int i = 0; (ch = token[i]) != '\0'; i++) {
        switch (state) {
            case 0: // Initial state
                if (isalpha(ch) || ch == '_' || ch == '#') state = 1; // Identifier
                else if (isdigit(ch)) state = 2;                      // Constant (number)
                else if (ch == '\'' || ch == '"') state = 3;          // Constant (char/string)
                else if (strchr("=+-*/%&|!<>", ch)) state = 4;        // Operator
                else if (strchr(",;(){}[]", ch)) state = 5;          // Delimiter
                else if (ch == '/' && token[i + 1] == '/') return COMMENT; // Single-line comment
                else if (ch == '/' && token[i + 1] == '*') return COMMENT; // Multi-line comment
                else return INVALID;
                break;
            case 1: // Identifier
                if (!isalnum(ch) && ch != '_' && ch != '#') return INVALID;
                break;
            case 2: // Constant (number)
                if (ch == '.') {
                    if (decimalCount > 0) return INVALID; // If there's already a decimal point, it's invalid
                    decimalCount++; // Increment the decimal count
                } else if (!isdigit(ch)) {
                    return INVALID; // Any non-digit that is not a decimal is invalid
                }
                break;
            case 3: // Constant (char/string)
                if (ch == '\'' || ch == '"') return CONSTANT;
                break;
            case 4: // Operator
                if (!strchr("=+-*/%&|!<>", ch)) return INVALID;
                break;
            case 5: // Delimiter
                return DELIMITER;
        }
    }

    // Final classification based on state
    if (state == 1) {
        if (isKeyword(token)) return KEYWORD;
        if (isReservedWord(token)) return RESERVED_WORD;
        if (isNoiseWord(token)) return NOISE_WORD;
        return IDENTIFIER;
    } else if (state == 2 || state == 3) {
        return CONSTANT;
    } else if (state == 4) {
        return OPERATOR;
    } else if (state == 5) {
        return DELIMITER;
    }
    return INVALID;
}


void analyzeLine(FILE *outputFile, char *line, int lineNumber) {
    char *token = strtok(line, " \t\n");
    while (token != NULL) {
        // Split the token further if it contains delimiters
        char temp[256];
        int j = 0;
        for (int i = 0; token[i] != '\0'; i++) {
            if (strchr(",;(){}[]", token[i])) {
                // Process the preceding substring as a separate token
                if (j > 0) {
                    temp[j] = '\0';
                    TokenType type = fsmClassify(temp);
                    fprintf(outputFile, "Line %d: %-15s Type: ", lineNumber, temp);
                    switch (type) {
                        case KEYWORD: fprintf(outputFile, "Keyword\n"); break;
                        case RESERVED_WORD: fprintf(outputFile, "Reserved Word\n"); break;
                        case NOISE_WORD: fprintf(outputFile, "Noise Word\n"); break;
                        case IDENTIFIER: fprintf(outputFile, "Identifier\n"); break;
                        case CONSTANT: fprintf(outputFile, "Constant\n"); break;
                        case OPERATOR: fprintf(outputFile, "Operator\n"); break;
                        case DELIMITER: fprintf(outputFile, "Delimiter\n"); break;
                        case COMMENT: fprintf(outputFile, "Comment\n"); break;
                        default: fprintf(outputFile, "Invalid\n"); break;
                    }
                    j = 0; // Reset temp buffer
                }

                // Process the delimiter itself
                char delim[2] = {token[i], '\0'};
                fprintf(outputFile, "Line %d: %-15s Type: Delimiter\n", lineNumber, delim);
            } else {
                temp[j++] = token[i];
            }
        }

        // Process any remaining characters as a token
        if (j > 0) {
            temp[j] = '\0';
            TokenType type = fsmClassify(temp);
            fprintf(outputFile, "Line %d: %-15s Type: ", lineNumber, temp);
            switch (type) {
                case KEYWORD: fprintf(outputFile, "Keyword\n"); break;
                case RESERVED_WORD: fprintf(outputFile, "Reserved Word\n"); break;
                case NOISE_WORD: fprintf(outputFile, "Noise Word\n"); break;
                case IDENTIFIER: fprintf(outputFile, "Identifier\n"); break;
                case CONSTANT: fprintf(outputFile, "Constant\n"); break;
                case OPERATOR: fprintf(outputFile, "Operator\n"); break;
                case DELIMITER: fprintf(outputFile, "Delimiter\n"); break;
                case COMMENT: fprintf(outputFile, "Comment\n"); break;
                default: fprintf(outputFile, "Invalid\n"); break;
            }
        }

        token = strtok(NULL, " \t\n");
    }
}

