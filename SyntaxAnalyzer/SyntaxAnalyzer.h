#pragma once
#include <string>
#include <vector>
#include "../LL1Table/LL1Walker.h"

typedef std::vector<std::string> inputSeq;

enum State
{
	Error,
	None
};

class CSyntaxAnalyzer
{
public:
	bool CheckInputSequence(const std::vector<std::string> & inputSeq);

private:
	bool CheckProgramStruct();
	bool CheckMainStruct();

	bool CheckWhileConstruction();
	bool CheckIfConstruction();
	bool CheckAssignment();
	bool BoolExpression(size_t exprLen);

	bool CheckBracketsCondition();

	bool MakeShiftIfNeeded(const std::string & checkStr);

	LL1Walker llWalker;
	inputSeq m_inputSeq;
	size_t m_currentPos = 0;
	State m_state = None;
};

