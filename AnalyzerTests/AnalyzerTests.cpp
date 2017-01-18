#include "stdafx.h"
#include "../SyntaxAnalyzer/SyntaxAnalyzer.h"
#include "../SyntaxAnalyzer/SyntaxExceptions.h"

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
	BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_start_end)
	{
		InputSequence inputseq = {
			"{"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}


	BOOST_AUTO_TEST_CASE(incorrect_if_statement)
	{
		InputSequence inputseq = {
			"{", "if", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_if_statement2)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"if", "(", "bool", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_if_statement3)
	{
		InputSequence inputseq = {
			"{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), UnexpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_primitive_bool_in_if)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_inside_if_state)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}","}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "}"
	};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", "(", "strin", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends2)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "print", "(", "string", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends3)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", "(", "string", "print", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", ")", ";", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_closing_brackets_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "print", ")", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	} 

	BOOST_AUTO_TEST_CASE(correct_assigment_test1)
	{
		InputSequence inputseq = {
			"{", "id", "=", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "string", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "char", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_assigment_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "id", "=", "id", ";", "id", "=", "string", ";", "}", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test1)
	{
		InputSequence inputseq = {
			"{", "id", "=", "id", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "id", "=", ";", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "(", "id", "=", "string", ";", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "id", "(", "=", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "id", "(", "=", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "id", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "id", "}", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test3)
	{
		InputSequence inputseq = {
			"{", "id", "=", "int", ";",  "id", "=", "int", ";", "id", "=", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
	}

	BOOST_AUTO_TEST_CASE(correct_declare_test)
	{
		InputSequence inputseq = {
			"{", "inttype", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "inttype", "id", ";", "booltype", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "stringtype", "id", "=", "string", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "chartype", "id", "=", "char", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "id", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_declare_test2)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{","stringtype", "id", ";", "}", "chartype", "id", "=", "char", ";", "id", "=", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_declare_test)
	{
		InputSequence inputseq = {
			"{", "inttype", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "inttype", "id", "=", ";", "booltype", "id", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "stringtype", "id", "=", "char", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "chartype", "id", "=", "string", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "booltype", "=", "id", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "booltype", "bool", "id", "=", "id", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "booltype", "id", ";", "=", "id", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "string", "id", ";", "=", "id", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "stringtype", "id", ";", "=", "stringtype", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_index_test)
	{
		InputSequence inputseq = {
			"{", "inttype", "id","[", "id", "]", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "inttype", "id","[","id","]","[", "int", "]","[", "int", "]", ";", "booltype", "id","[", "id", "]", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "id", "[", "int", "]","[", "id", "]", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "id","[", "id", "]","[", "int", "]", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "=", "id","[", "id", "]","[", "int", "]", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "read", "(", "id", "[", "id", "]","[", "id", "]", ")", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "[", "id", "]","[", "id", "]", "=", "int", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "[", "id", "]","[", "id", "]", "=", "float", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "[", "id", "]","[", "id", "]", "=", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "[", "id", "]", "=", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_index_test)
	{
		InputSequence inputseq = {
			"{", "inttype", "id","[", "id", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "inttype", "id","[",
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
		
		inputseq = {
			"{", "inttype", "id","[","id","[","]","[", "int", "]","[", "int", "]", ";", "booltype", "id","[", "id", "]", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "booltype", "id", "=", "id", "[", "]", "[", "id", "]", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "booltype", "id", "=", "id","[", "id", "]","[", "6", "]", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "id", "=", "id","[", "]","[", "int", "]", ";", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{","[", "id", "]","[", "id", "]", "=", "int", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "id", "[", "id","[", "id", "]", "=", "float", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "id", "[", "id", "]","[", "id", "]", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
		inputseq = {
			"{", "id", "[", "id", "]","[", "id", "]", "id", ";","}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
	}

	BOOST_AUTO_TEST_CASE(correct_read_test)
	{
		InputSequence inputseq = {
			"{", "read", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "read", "(", "char", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "read", "(", "int", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "while", "(", "not", "bool", ")", "{", "read", "(", "int", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "print", "(", "float", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_read_test)
	{
		InputSequence inputseq = {
			"{", "read", "int", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "read", "(", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "read", "(", "int", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "read", "(", "int", ")", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "read", "(", "int", ")", ";", "read", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_print_test)
	{
		InputSequence inputseq = {
			"{", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "print", "(", "char", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "print", "(", "int", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "while", "(", "!stream.empty()", ")", "{", "print", "(", "int", ")", ";", "}", "}"
		};

		inputseq = {
			"{", "print", "(", "float", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_print_test)
	{
		InputSequence inputseq = {
			"{", "print", "float", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "print", "(", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "print", "(", "int", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "print", "(", "int", ")", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "print", "(", "int", ")", ";", "print", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_const_declare_test)
	{
		InputSequence inputseq = {
			"{", "const", "inttype", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "const", "inttype", "id", ";", "const", "booltype", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "const", "stringtype", "id", "=", "string", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "const", "booltype", "id", "=", "id", ";", "const", "chartype", "id", ";","}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "const", "stringtype", "id", ";", "}", "const", "chartype", "id", "=", "char", ";", "id", "=", "id", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_const_declare_test)
	{
		InputSequence inputseq = {
			"{", "cnst", "inttype", "id", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "const", "id", ";", "const", "booltype", "id", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "const", "stringtype", "string", "id", "=", "string", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_elif)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "}", "elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "if", "(", "bool", ")", "{", "}" ,"elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", "bool", ")", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", "bool", ")", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_elif)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elf", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "}", "}", "elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}" ,"elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", "bool", ")", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "print", "(", "string", ")", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", "bool", ")", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}" ,"else", "{", "}", "elif", "(", "bool", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_else)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}", "else", "{", "}", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "}", "if", "(", "bool", ")", "{", "}" ,"else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "else", "{", "}", "if", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}", "elif", "(", "bool", ")", "{", "}","else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_else)
	{
		InputSequence inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "els", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "if", "(", "bool", ")", "{", "}", "else", "}", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "if", "(", "bool", ")", "{", "}" ,"else", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", "bool", ")", "{", "}", "else", "{", "print", "(", "string", ")", "}", "if", "(", "bool", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_arithmetical)
	{
		InputSequence inputseq = {
			"{", "inttype", "id", "=", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "inttype", "id", "=", "-", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "inttype", "id", "=", "-", "(", "int", "+", "id", "-", "id", "*", "int", "/", "id", "[", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "inttype", "id", "=", "int", "+", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "+", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_arithmetical)
	{
		InputSequence inputseq = {
			"{", "inttype", "id", "=", "-", "+", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "inttype", "id", "=", "-", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "int", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{",  "inttype", "id", "=", "(", "(", "int", ")", "+", "id", "-", "id", "*", "int", "/", "id", "[", "-", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "(", "(", "int", ")", "+", "id", "-", "id", "*", "int", "/", "id", "[", "-", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "int", "+", "id", "[", "int", "]", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
	}

	BOOST_AUTO_TEST_CASE(correct_bool)
	{
		InputSequence inputseq = {
			"{", "booltype", "id", "=", "(", "id", "equal", "(", "not", "bool", ")", ")", "or", "id",";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "not", "(", "id", "equal", "(", "not", "bool", ")", ")", "or", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "not", "(", "id", "equal", "id", "or", "bool", "or", "bool", "equal", "(","not", "id", "[", "int", "]", ")", ")", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "booltype", "id", "=", "id", "equal", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "id", "equal", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{",  "booltype", "id", "=", "(", "(", "id", ")", "more", "id", "less", "id", "more", "bool", "equal", "id", "[", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_bool)
	{
		InputSequence inputseq = {
			"{", "booltype", "id", "=", "not", "or", "(", "bool", "less", "(", "not", "id", ")", ")", "more", "id", "[", "int", "]", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "booltype", "id", "=", "not", "(", "bool", "less", "(", "not", "id", ")", ")", "more", "id", "bool", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{",  "booltype", "id", "=", "(", "(", "id", ")", "more", "id", "less", "id", "more", "bool", "equal", "id", "[", "not", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "id", "equal", "id", "[", "int", "]", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
	}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()