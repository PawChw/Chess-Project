#pragma once
#include<string>

class Parser
{
	std::string& toParse;
	unsigned long long indexer;
public:
	Parser() = delete;
	Parser(std::string& toParse, unsigned long long indexer = 0) : toParse(toParse), indexer(indexer) {}
	char consume();
	char lookAhead(unsigned long long i = 0);
	bool finished();
};

