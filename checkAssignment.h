#ifndef CHECKASSIGNMENT_H
#define CHECKASSIGNMENT_H

int checkAssignment(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {
        case '=':
            switch(sheeshLexeme[currChar++]) {
                case '\0':
                    return 1;
            }
            currChar = 0;

        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '|':
            switch(sheeshLexeme[currChar++]) {
                case '=':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return 1;
                    }
            }
    }

    return 0;
}

#endif