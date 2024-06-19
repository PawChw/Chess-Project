#include "Parser.h"

char Parser::Consume()
{
    char nxt = to_parse[indexer];
    indexer++;
    return nxt;
}

char Parser::LookAhead(unsigned long long i)
{
    return to_parse[indexer + i];
}

bool Parser::Finished()
{
    return indexer == to_parse.length();
}
