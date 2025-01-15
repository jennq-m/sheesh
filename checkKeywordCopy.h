#ifndef CHECKKEYWORDCOPY_H
#define CHECKKEYWORDCOPY_H

//Finite State Machine for Keyword
int checkKeywordCopy(const char *sheeshLexeme) {
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
                                                            return 1;
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
                                    return 1;
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
                            return 1;
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
                                                    return 1;
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
                                                    return 1;
                                            } 
                                    }
                            }
                    }
                    break;
                
                case 'x':
                    switch(sheeshLexeme[currChar++]) {
                        case '\0':
                            return 1;

                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case 'r':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'a':
                                            switch(sheeshLexeme[currChar++]) {
                                                case '\0':
                                                    return 1;
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
                                            return 1;
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
                                                            return 1;
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
                                                    return 1;
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
                            return 1;
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
                                                    return 1;
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
                                            return 1;    
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
                                                    return 1;
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
                                                            return 1;
                                                    }
                                            }
                                            break;

                                        case 'i':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'n':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case '\0':
                                                            return 1;
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
                                            return 1;
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
                                                                                    return 1;
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
                                                                            return 1;
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
                                    return 1;
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
                                            return 1;
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
                                                    return 1;

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
                                    }
                            }
                            break;

                        case 'u':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return 1;
                                    }
                            }
                    }
                    break;

                case 'u':
                    switch(sheeshLexeme[currChar++]) {
                        case 't':
                            switch(sheeshLexeme[currChar++]) {
                                case '\0':
                                    return 1;

                                case 's':
                                    switch(sheeshLexeme[currChar++]) {
                                        case 'i':
                                            switch(sheeshLexeme[currChar++]) {
                                                case 'd':
                                                    switch(sheeshLexeme[currChar++]) {
                                                        case 'e':
                                                            switch(sheeshLexeme[currChar++]) {
                                                                case '\0':
                                                                    return 1;
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
                                                            return 1;
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
                            return 1;

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
                                    return 1;

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
                                                                            return 1;
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
                                                    return 1;
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
                                                                            return 1;
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
                                            return 1;
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
                                            return 1;
                                    }
                            }
                            break;
                            
                        case 'x':
                            switch(sheeshLexeme[currChar++]) {
                                case 't':
                                    switch(sheeshLexeme[currChar++]) {
                                        case '\0':
                                            return 1;
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
                                            return 1;
                                    }
                            }
                    }
            }
    }

    return 0;
}

#endif