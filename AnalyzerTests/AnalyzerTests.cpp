#include "stdafx.h"
#include "../SyntaxAnalyzer/SyntaxAnalyzer.h"

struct SyntaxAnalyzer
{
	CSyntaxAnalyzer analyzer;
};

BOOST_FIXTURE_TEST_SUITE(Base_test, SyntaxAnalyzer)

BOOST_AUTO_TEST_SUITE(base_tests)

	BOOST_AUTO_TEST_CASE(start_end)
	{
	inputSeq inputseq = {
		"{", "}"
	};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_start_end)
	{
		inputSeq inputseq = {
			"{"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}


	BOOST_AUTO_TEST_CASE(incorrect_if_statement)
	{
		inputSeq inputseq = {
			"{", "if", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_if_statement2)
	{
		inputSeq inputseq = {
			"{", "if", "(", ")", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()