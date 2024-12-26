#ifndef CHECKFILENAME_H
#define CHECKFILENAME_H

#include <stdio.h>

//Ensure that the file is .shs
int checkFilename(int argc, char *argv) {
    if (argc < 2) {
        printf("Usage: <filename>\n");
        exit(1);
    }

    if (!strstr(argv, ".shs")) {
        printf("Error: Filetype must be '.shs'. Please try again.\n");
        exit(1);
    }
}

#endif