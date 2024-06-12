#include "Parser.h"

char Parser::consume()
{
    char nxt = toParse[indexer];
    indexer++;
    return nxt;
}

char Parser::lookAhead(unsigned long long i)
{
    return toParse[indexer + i];
}

bool Parser::finished()
{
    return indexer == toParse.length();
}
