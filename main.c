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
#include "lexer.h"
#include "parser.h"


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
    Token token; 

    TokenType reservedWord = checkReservedWord(temp);
    Token rwToken;

    if (keyword) { 
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



