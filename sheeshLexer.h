#ifndef SHEESHLEXER_H
#define SHEESHLEXER_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "checkIdentifier.h"
#include "checkKeyword.h"
#include "checkReservedWord.h"

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

#endif