#ifndef CHECKRESERVEDWORD_H
#define CHECKRESERVEDWORD_H

//Finite State Machine for Reserved Words
TokenType checkReservedWord(const char *sheeshLexeme) {
    int currChar = 0;

    switch(sheeshLexeme[currChar++]) {

        case 'a':
            switch(sheeshLexeme[currChar++]) {
                case 'l':
                    switch(sheeshLexeme[currChar++]) {
                        case 'w':
                            switch(sheeshLexeme[currChar++]) {
                                case 'a':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'y':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 's':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return ALWAYS;
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
        
        case 'c':
            switch(sheeshLexeme[currChar++]) {
                case 'a':
                    switch(sheeshLexeme[currChar++]) {
                        case 'p':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return CAP;
                            }
                    }
                
                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case 'n':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return CONT;
                                    }
                            }
                    }
            }

            currChar = 0;

        case 'n':
            switch(sheeshLexeme[currChar++]) {
                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case 'c':
                            switch(sheeshLexeme[currChar++]) {
                                case 'a':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'p':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return NOCAP;
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
        
        case 't':
            switch(sheeshLexeme[currChar++]) {
                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case 'p':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'i':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'e':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'r':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case '\0':
                                                                    return TOPTIER;
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
            }
    }
    
    return 0;
}

#endif