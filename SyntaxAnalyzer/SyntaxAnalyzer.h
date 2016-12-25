#pragma once
#include <string>
#include <vector>
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
	bool BoolExpression(size_t endOfExpression);

	bool CheckBracketsCondition();

	bool MakeShiftIfNeeded(const std::string & checkStr);
	inputSeq m_inputSeq;
	size_t m_currentPos = 0;
	State m_state = None;
};

