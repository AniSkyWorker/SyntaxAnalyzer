// SyntaxAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include "FlexLexer.h"
#include <fstream>

std::vector<std::string> GetLexerTokens(const std::string & filename)
{
	std::filebuf file;
	file.open(filename, std::ios_base::in);
	std::istream stream(&file);

	yyFlexLexer analyz(&stream, &std::cout);
	analyz.yylex();

	return analyz.GetTokens();
}

int main()
{
	CSyntaxAnalyzer synAnalyzer;
	InputSequence inputseq = {
		"{", "int", "id","[", "int", "]", ";", "}"
	};
	if (synAnalyzer.CheckInputSequence(inputseq))
	{
		std::cout << "Run sucessful!";
	}
	else
	{
		std::cerr << "Error while parsing!";
	}
    return 0;
}

