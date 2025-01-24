#ifndef NEWTOKEN_H
#define NEWTOKEN_H

#include <stdio.h>
#include <string.h>
#include "lexer.h"

Token newToken(const char *value, TokenType type, int sheeshColumn) {
    Token token;
    token.value = strdup(value);
    token.type = type;
    token.sheeshLine = sheeshColumn;

    allTokens[tokenCount++] = token;

    return token;
}

#endif