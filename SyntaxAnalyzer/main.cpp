// SyntaxAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SyntaxAnalyzer.h"

int main()
{
	CSyntaxAnalyzer synAnalyzer;
	inputSeq inputseq = {
		"{", "}"
	};
	if (synAnalyzer.CheckInputSequence(inputseq))
	{

	}
    return 0;
}

