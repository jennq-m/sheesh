#ifndef CHECKARITHMETIC_H
#define CHECKARITHMETIC_H

int checkArithmetic(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
        case '|':
            switch(sheeshLexeme[currChar++]) {
                case '\0':
                    return 1;
            }
    }

    return 0;
}

#endif