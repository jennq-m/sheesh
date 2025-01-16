#ifndef CHECKKEYWORD_H
#define CHECKKEYWORD_H
#include "lexer.h"

//Finite State Machine for Keyword
TokenType checkKeyword(const char *sheeshLexeme) {
    int currChar = 0;

    switch (sheeshLexeme[currChar++]) {

        case 'b':
            switch(sheeshLexeme[currChar++]) {
                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case 'u':
                            switch(sheeshLexeme[currChar++]) {
                                case 'n':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'c':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'e':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return BOUNCE;
                                                    } 
                                            }   
                                    }     
                            }   
                    } 
            }

            currChar = 0;
            break;

        case 'c':
            switch(sheeshLexeme[currChar++]) {
                case 'a':
                    switch(sheeshLexeme[currChar++]) {
                        case 'r':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return CAR;
                            } 
                    }
            }

            currChar = 0;
            break;

        case 'd':
            switch(sheeshLexeme[currChar++]) {
                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return DO;
                    }
                    break; 
                
                case 'r':
                    switch(sheeshLexeme[currChar++]) {
                        case 'i':
                            switch(sheeshLexeme[currChar++]) {
                                case 'f':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 't':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return DRIFT;
                                            } 
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'e':
            switch(sheeshLexeme[currChar++]) {
                case 'm':
                    switch(sheeshLexeme[currChar++]) {
                        case 'p':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'y':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return EMPTY;
                                            } 
                                    }
                            }
                    }
                    break;
                
                case 'x':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return EX;

                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case 'r':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'a':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return EXTRA;
                                            }
                                    }
                            }
                    } 
            }

            currChar = 0;
            break;

        case 'f':
            switch(sheeshLexeme[currChar++]) {
                case 'l':
                    switch(sheeshLexeme[currChar++]) {
                        case 'i':
                            switch(sheeshLexeme[currChar++]) {
                                case 'p':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return FLIP;
                                    }
                            }
                    }
                    break;
                
                case 'r':
                    switch(sheeshLexeme[currChar++]) {
                        case 'o':
                            switch(sheeshLexeme[currChar++]) {
                                case 'z':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'e':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'n':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return FROZEN;
                                                    }
                                            }
                                    }
                            }
                    }
                    break;
            }
    
            currChar = 0;
            break;
        
        case 'g':
            switch(sheeshLexeme[currChar++]) {
                case 'r':
                    switch(sheeshLexeme[currChar++]) {
                        case 'o':
                            switch(sheeshLexeme[currChar++]) {
                                case 'u':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'p':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return GROUP;
                                            }
                                    }   
                            }   
                    } 
            }

            currChar = 0;
            break;

        case 'i':
            switch(sheeshLexeme[currChar++]) {
                case 'f':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return IF;
                    }
                    break;
                
                case 'n':
                    switch(sheeshLexeme[currChar++]) {
                        case 'p':
                            switch(sheeshLexeme[currChar++]) {
                                case 'u':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 't':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return INPUT;
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'j':
            switch(sheeshLexeme[currChar++]) {
                case 'u':
                    switch(sheeshLexeme[currChar++]) {
                        case 'm':
                            switch(sheeshLexeme[currChar++]) {
                                case 'p':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return JUMP;    
                                    }
                            }   
                    } 
            }

            currChar = 0;
            break;

        case 'l':
            switch(sheeshLexeme[currChar++]) {
                case 'e':
                    switch(sheeshLexeme[currChar++]) {
                        case 'g':
                            switch(sheeshLexeme[currChar++]) {
                                case 'i':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 't':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return LEGIT;
                                            }
                                    }
                            }
                    }
                    break;

                case 'o':
                    switch(sheeshLexeme[currChar++]) {
                        case 'c':
                            switch(sheeshLexeme[currChar++]) {
                                case 'k':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'e':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'd':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return LOCKED;
                                                    }
                                            }
                                            break;

                                        case 'i':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'n':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return LOCKIN;
                                                    }
                                            }
                                    }
                            }
                            break;

                        case 'n':
                            switch(sheeshLexeme[currChar++]) {
                                case 'g':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return LONG;
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'm':
            switch(sheeshLexeme[currChar++]) {
                case 'e':
                    switch(sheeshLexeme[currChar++]) {
                        case 'a':
                            switch(sheeshLexeme[currChar++]) {
                                case 'n':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'w':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'h':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'i':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case 'l':
                                                                    switch(sheeshLexeme[currChar++]) {
                                                                        case 'e':
                                                                            switch(sheeshLexeme[currChar++]) {
                                                                                case '\0':
                                                                                    return MEANWHILE;
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'n':
            switch(sheeshLexeme[currChar++]) {
                case 'i':
                    switch(sheeshLexeme[currChar++]) {
                        case 'c':
                            switch(sheeshLexeme[currChar++]) {
                                case 'k':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'n':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'a':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'm':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case 'e':
                                                                    switch(sheeshLexeme[currChar++]) {
                                                                        case '\0':
                                                                            return NICKNAME;
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
                    break;
                    
                case 'u':
                    switch(sheeshLexeme[currChar++]) {
                        case 'm':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return NUM;
                            }
                    }
            }

            currChar = 0;
            break;

        case 'o':
            switch(sheeshLexeme[currChar++]) {
                case 'p':
                    switch(sheeshLexeme[currChar++]) {
                        case 'e':
                            switch(sheeshLexeme[currChar++]) {
                                case 'n':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return OPEN;
                                    }
                            }
                    }
                    break;

                case 't':
                    switch(sheeshLexeme[currChar++]) {
                        case 'h':
                            switch(sheeshLexeme[currChar++]) {
                                case 'e':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'r':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return OTHER;

                                                case 'w':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'i':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case 's':
                                                                    switch(sheeshLexeme[currChar++]) {
                                                                        case 'e':
                                                                            switch(sheeshLexeme[currChar++]) {
                                                                                case '\0':
                                                                                    return OTHERWISE;
                                                                            }
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                            break;

                        case 'u':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return OUT;
                                    }
                            }
                    }
                    break;

                case 'u':
                    switch(sheeshLexeme[currChar++]) {
                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return OUT;

                                case 's':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'i':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'd':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'e':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case '\0':
                                                                    return OUTSIDE;
                                                            }
                                                    }
                                            }
                                    }
                                    break;

                                case 'p':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'u':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 't':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return OUTPUT;
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'p':
            switch(sheeshLexeme[currChar++]) {
                case 'l':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return PL;

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
                                                                            return PLAYLIST;
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'r':
            switch(sheeshLexeme[currChar++]) {
                case 'e':
                    switch(sheeshLexeme[currChar++]) {
                        case 'p':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return REP;

                                case 'e':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'a':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 't':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return REPEAT;
                                                    }
                                            }
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 's':
            switch(sheeshLexeme[currChar++]) {
                case 'c':
                    switch(sheeshLexeme[currChar++]) {
                        case 'e':
                            switch(sheeshLexeme[currChar++]) {
                                case 'n':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'a':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'r':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'i':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case 'o':
                                                                    switch(sheeshLexeme[currChar++]) {
                                                                        case '\0':
                                                                            return SCENARIO;
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                    }
                    break;

                case 'h':
                    switch(sheeshLexeme[currChar++]) {
                        case 'o':
                            switch(sheeshLexeme[currChar++]) {
                                case 'r':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 't':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return SHORT;
                                            }
                                    }
                            }
                    }
                    break;

                case 't':
                    switch(sheeshLexeme[currChar++]) {
                        case 'a':
                            switch(sheeshLexeme[currChar++]) {
                                case 'n':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'd':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'a':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'r':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case 'd':
                                                                    switch(sheeshLexeme[currChar++]) {
                                                                        case '\0':
                                                                            return STANDARD;
                                                                    }
                                                            }
                                                    }
                                            }
                                    }
                            }
                            break;

                        case 'o':
                            switch(sheeshLexeme[currChar++]) {
                                case 'p':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return STOP;
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 't':
            switch(sheeshLexeme[currChar++]) {
                case 'e':
                    switch(sheeshLexeme[currChar++]) {
                        case 'a':
                            switch(sheeshLexeme[currChar++]) {
                                case 'm':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return TEAM;
                                    }
                            }
                            break;
                            
                        case 'x':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return TEXT;
                                    }
                            }
                    }
            }

            currChar = 0;
            break;

        case 'v':
            switch(sheeshLexeme[currChar++]) {
                case 'i':
                    switch(sheeshLexeme[currChar++]) {
                        case 'b':
                            switch(sheeshLexeme[currChar++]) {
                                case 'e':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return VIBE;
                                    }
                            }
                    }
            }
    }

    return 0;
}

#endif