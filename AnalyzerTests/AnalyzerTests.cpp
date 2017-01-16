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
			"{", "if", "(", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"if", "(", ")", "}"
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
			"{", "if", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
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
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "}"
	};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(correct_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", "id", "int", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_some_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "strin", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends2)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "if", "(", ")", "{", "print", "(", "string", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_if_state_and_line_ends3)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", "(", "string", "print", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", ")", ";", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_sequence_of_closing_brackets_end_lines)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "print", ")", ")", ";", "}", "}"
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
			"{", "if", "(", ")", "{", "id", "=", "id", ";", "id", "=", "string", ";", "}", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test1)
	{
		InputSequence inputseq = {
			"{", "id", "=", "id", "(", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "id", "=", ";", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "(", "id", "=", "string", ";", ")", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

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
			"{", "if", "(", ")", "{", "id", "}", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(incorrect_assigment_test3)
	{
		InputSequence inputseq = {
			"{", "id", "=", "int", ";",  "id", "=", "int", ";", "id", "=", "id", "=", "int", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
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
			"{", "if", "(", ")", "{","stringtype", "id", ";", "}", "chartype", "id", "=", "char", ";", "id", "=", "id", ";", "}"
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
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
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
			"{", "while", "(", "!stream.empty()", ")", "{", "read", "(", "int", ")", ";", "}", "}"
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
			"{", "if", "(", ")", "{", "const", "stringtype", "id", ";", "}", "const", "chartype", "id", "=", "char", ";", "id", "=", "id", ";", "}"
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
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "}", "elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "if", "(", ")", "{", "}" ,"elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", ")", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", ")", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_elif)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "elf", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "if", "(", ")", "{", "}", "elif", "(", ")", "}", "}", "elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "if", "(", ")", "{", "}" ,"elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", ")", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "print", "(", "string", ")", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","elif", "(", ")", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "if", "(", ")", "{", "}" ,"else", "{", "}", "elif", "(", ")", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_else)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "if", "(", ")", "{", "}", "else", "{", "}", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "}", "if", "(", ")", "{", "}" ,"else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "else", "{", "}", "if", "(", ")", "{", "}", "elif", "(", ")", "{", "}", "elif", "(", ")", "{", "}","else", "{", "}", "}"
		};
		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_else)
	{
		InputSequence inputseq = {
			"{", "if", "(", ")", "{", "}", "els", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "if", "(", ")", "{", "}", "else", "}", "}", "else", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "if", "(", ")", "{", "}" ,"else", "{", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "print", "(", "string", ")", ";", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", "}", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);

		inputseq = {
			"{", "if", "(", ")", "{", "}", "else", "{", "print", "(", "string", ")", "}", "if", "(", ")", "{", "print", "(", "string", ")", ";", "}","else", "{", "read", "(", "string", ")", ";", "}", "print", "(", "string", ")", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), ExpectedSymbolError);
	}

	BOOST_AUTO_TEST_CASE(correct_arithmetical)
	{
		InputSequence inputseq = {
			"{", "int", "id", "=", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};

		inputseq = {
			"{", "int", "id", "=", "-", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));

		inputseq = {
			"{", "int", "id", "=", "-", "(", "int", "+", "id", "-", "id", "*", "int", "/", "id", "[", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_NO_THROW(analyzer.CheckInputSequence(inputseq));
	}

	BOOST_AUTO_TEST_CASE(incorrect_arithmetical)
	{
		InputSequence inputseq = {
			"{", "int", "id", "=", "-", "+", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "[", "int", "]", ";", "}"
		};
		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "int", "id", "=", "-", "(", "int", "+", "(", "-", "int", ")", ")", "*", "id", "int", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);

		inputseq = {
			"{", "int", "id", "=", "-", "(", "(", "int", ")", "+", "id", "-", "id", "*", "int", "/", "id", "[", "int", "]", ")", ";", "}"
		};

		BOOST_REQUIRE_THROW(analyzer.CheckInputSequence(inputseq), std::exception);
	}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()