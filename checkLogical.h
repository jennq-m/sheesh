#ifndef CHECKLOGICAL_H
#define CHECKLOGICAL_H

int checkLogical(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {
        case '!':
            switch(sheeshLexeme[currChar++]) {
                case '\0':
                    return 1;
            }
            currChar = 0;

        case '&':
            switch(sheeshLexeme[currChar++]) {
                case '&':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return 1;
                    }
            }
            currChar = 0;

        case '|':
            switch(sheeshLexeme[currChar++]) {
                case '|':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return 1;
                    }
            }
    }

    return 0;
}

#endif