#ifndef CHECKNOISEWORD_H
#define CHECKNOISEWORD_H

int checkNoiseWord(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {
        case 'a':
            switch(sheeshLexeme[currChar++]) {
                case 'y':
                    switch(sheeshLexeme[currChar++]) {
                        case 'l':
                            switch(sheeshLexeme[currChar++]) {
                                case 'i':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 's':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 't':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return 1;
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;

        case 'e':
            switch(sheeshLexeme[currChar++]) {
                case 'a':
                    switch(sheeshLexeme[currChar++]) {
                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return 1;
                            }
                    }
            }

            currChar = 0;

        case 'p':
            switch(sheeshLexeme[currChar++]) {
                case 'u':
                    switch(sheeshLexeme[currChar++]) {
                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return 1;
                            }
                    }
            }
            
            currChar = 0;

        case 't':
            switch(sheeshLexeme[currChar++]) {
                case 'i':
                    switch(sheeshLexeme[currChar++]) {
                        case 'e':
                            switch(sheeshLexeme[currChar++]) {
                                case 'r':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return 1;
                                    }
                            }
                    }
                
                case 'r':
                    switch(sheeshLexeme[currChar++]) {
                        case 'a':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return 1;
                            }
                    }
            }

            currChar = 0;

        case 'w':
            switch(sheeshLexeme[currChar++]) {
                case 'i':
                    switch(sheeshLexeme[currChar++]) {
                        case 's':
                            switch(sheeshLexeme[currChar++]) {
                                case 'e':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return 1;
                                    }
                            }
                    }
            }
    }

    return 0;
}

#endif