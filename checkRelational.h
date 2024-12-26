#ifndef CHECKRELATIONAL_H
#define CHECKRELATIONAL_H

int checkRelational(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {
        case '=':
        case '!':
            switch(sheeshLexeme[currChar++]) {
                case '=':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return 1;
                    }
            }
            currChar = 0;

        case '>':
        case '<':
            switch(sheeshLexeme[currChar++]) {
                case '\0':
                    return 1;
                
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