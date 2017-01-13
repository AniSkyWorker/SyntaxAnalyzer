#include "stdafx.h"
#include <fstream>
#include "../lexer/FlexLexer.h"

struct LexerAnalyzer
{
	yyFlexLexer analyzer;
};

void CheckAnalyzerTokens(const std::string & name, const std::vector<std::string> & result)
{
	std::filebuf file;
	file.open(name, std::ios_base::in);
	std::istream stream(&file);

	yyFlexLexer analyz(&stream, &std::cout);
	analyz.yylex();

	auto tokens = analyz.GetTokens();
	BOOST_CHECK_EQUAL_COLLECTIONS(tokens.begin(), tokens.end(), result.begin(), result.end());
}

BOOST_FIXTURE_TEST_SUITE(Base_test, LexerAnalyzer)

BOOST_AUTO_TEST_SUITE(base_tests)

BOOST_AUTO_TEST_CASE(one_line_sequence)
{
	std::vector<std::string> result = { "TYPENAME", "ID", "=", "INT", "-", "INT", ";" };
	CheckAnalyzerTokens("test1.txt", result);
}

BOOST_AUTO_TEST_CASE(multiline_sequence)
{
	std::vector<std::string> result = { "WHILE", "(", "ID", "<", "INT", ")", "{", "ID", "=", "ID", "-", "INT", ";", "}" };
	CheckAnalyzerTokens("test2.txt", result);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()