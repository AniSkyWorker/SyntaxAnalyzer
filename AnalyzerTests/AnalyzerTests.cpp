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

	BOOST_AUTO_TEST_CASE(correct_assigment_test1)
	{
		InputSequence inputseq = {
			"{", "id", "=", "id", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "int", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "string", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "char", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_assigment_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "id", "=", "id", ";", "id", "=", "string", ";", "}", "id", "=", "int", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test1)
	{
		InputSequence inputseq = {
			"{", "id", "=", "id", "(", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", ";", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "(", "id", "=", "string", ";", ")", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "(", "=", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "(", "=", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "id", "}", "id", "=", "int", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test3)
	{
		InputSequence inputseq = {
			"{", "id", "=", "int", ";",  "id", "=", "int", ";", "id", "=", "id", "=", "int", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_declare_test)
	{
		InputSequence inputseq = {
			"{", "int", "id", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "id", ";", "bool", "id", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "string", "id", "=", "string", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "char", "id", "=", "char", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_declare_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{","string", "id", ";", "}", "char", "id", "=", "char", ";", "id", "=", "id", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_declare_test)
	{
		InputSequence inputseq = {
			"{", "int", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "id", "=", ";", "bool", "id", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "string", "id", "=", "char", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "char", "id", "=", "string", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "=", "id", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "bool", "id", "=", "id", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", ";", "=", "id", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_index_test)
	{
		InputSequence inputseq = {
			"{", "int", "id","[", "id", "]", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "id","[","id","]","[", "int", "]","[", "int", "]", ";", "bool", "id","[", "id", "]", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id", "[", "int", "]","[", "id", "]", ";", "}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id","[", "id", "]","[", "int", "]", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "id","[", "id", "]","[", "int", "]", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_index_test)
	{
		InputSequence inputseq = {
			"{", "int", "id","[", "id", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "id","[",
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
		
		inputseq = {
			"{", "int", "id","[","id","[","]","[", "int", "]","[", "int", "]", ";", "bool", "id","[", "id", "]", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id", "[", "]", "int", "]","[", "id", "]", ";", "}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "bool", "id", "=", "id","[", "id", "]","[", "6", "]", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "id","[", "]","[", "int", "]", ";", "char", "id", ";","}"
		};
		BOOST_CHECK(!analyzer.CheckInputSequence(inputseq));
	}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()