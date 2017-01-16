// SyntaxAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include "FlexLexer.h"
#include <fstream>

std::vector<std::string> GetLexerTokens(const std::string & filename)
{
	std::filebuf file;
	if(!file.open(filename, std::ios_base::in))
	{
		throw std::runtime_error("Error when open " + filename);
	}
	std::istream stream(&file);
	yyFlexLexer analyz(&stream, &std::cout);
	analyz.yylex();
	return analyz.GetTokens();
}

int main(int argc, char * argv[])
{
	try
	{
		CSyntaxAnalyzer synAnalyzer;
		synAnalyzer.CheckInputSequence(GetLexerTokens(argv[1]));
	}
	catch (const std::exception & exc)
	{
		std::cout << exc.what();
	}
    return 0;
}

