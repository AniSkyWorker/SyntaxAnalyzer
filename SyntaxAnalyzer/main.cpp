#include "stdafx.h"
#include "SyntaxAnalyzer.h"
#include "FlexLexer.h"
#include <fstream>
#include "JsonAgregator.h"

std::vector<std::string> GetLexerTokens(const std::string & filename)
{
	std::filebuf file;
	if(!file.open(filename, std::ios_base::in))
	{
		throw std::runtime_error("Error when open: " + filename);
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
		if (argc < 2)
		{
			std::cout << "Usage: syntaxAnalyzer.exe <filename>" << std::endl;
			return 1;
		}
		CSyntaxAnalyzer synAnalyzer;
		synAnalyzer.CheckInputSequence(GetLexerTokens(argv[1]));
	}
	catch (const std::exception & exc)
	{ 
		std::cout << exc.what();
		return 1;
	}
    return 0;
}

