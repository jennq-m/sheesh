#ifndef CHECKIDENTIFIER_H
#define CHECKIDENTIFIER_H

//Finite State Machine for Identifier
int checkIdentifier(const char *sheeshLexeme) {
    int i = 0;
    char current = sheeshLexeme[i];
    int state = 1;

    while (current != '\0') {
        switch(state) {
            case 1:
                switch (current) {
                    case 'A' ... 'Z':
                    case 'a' ... 'z':
                    case '_':
                    case '#':
                        state = 3;
                        break;
                        
                    default:
                        return 0;
                }

                break;

            case 3:
                switch(current) {
                    case 'A' ... 'Z':
                    case 'a' ... 'z':
                    case '0' ... '9':
                        state = 3;
                        break;

                    case '#':
                    case '_':
                        state = 4;
                        break;

                    default:
                        return 0;
                }

                break;

            case 4:
                switch(current) {
                    case 'A' ... 'Z':
                    case 'a' ... 'z':
                    case '0' ... '9':
                        state = 3;
                        break;

                    case '#':
                    case '_':
                        state = 4;
                        break;

                    default:
                        return 0;
                }

                break;
                
            default:
                return 0;
        }

        current = sheeshLexeme[++i];
    }

    if (state == 3) {
        return 1;
    }

    return 0;
}

#endif