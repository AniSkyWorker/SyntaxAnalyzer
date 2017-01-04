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
	InputSequence inputseq = {
		"{", "}"
	};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_start_end)
	{
		InputSequence inputseq = {
			"{"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}


	BOOST_AUTO_TEST_CASE(incorrect_if_statement)
	{
		InputSequence inputseq = {
			"{", "if", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_if_statement2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "}"//test to delete because empty bracket
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_if_statement3)
	{
		InputSequence inputseq = {
			"{", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_primitive_bool_in_if)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	//BOOST_AUTO_TEST_CASE(incorrect_brackets_bool_in_if)
	//{
	//	inputSeq inputseq = {
	//		"{", "if", "(", ")", ")", "{", "}", "}"
	//	};
	//	BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	//}

	//BOOST_AUTO_TEST_CASE(incorrect_brackets_bool_in_if2)
	//{
	//	inputSeq inputseq = {
	//		"{", "if", "(", "(", ")", "{", "}", "}"
	//	};
	//	BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	//}

	BOOST_AUTO_TEST_CASE(correct_sequence_inside_if_state)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "if", "(", ")", "{", "}","}", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "}", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_line_ends)
	{
	InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "}"
	};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "strin", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", ";", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends3)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "string", "print", "(", "string", ")", ";", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", ")", ";", ";", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_closing_brackets_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", ")", ")", ";", "}", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()