#pragma once
#include<string>

class Parser
{
	std::string& to_parse;
	unsigned long long indexer;
public:
	Parser() = delete;
	Parser(std::string& toParse, unsigned long long indexer = 0) : to_parse(toParse), indexer(indexer) {}
	char Consume();
	char LookAhead(unsigned long long i = 0);
	bool Finished();
};

