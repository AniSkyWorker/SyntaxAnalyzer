#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../LL1Table/LL1Walker.h"

typedef std::vector<std::string> InputSequence;

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

	bool CheckAssignment(const InputSequence &);
	bool BoolExpression(const InputSequence &);
	bool CheckArithmeticExpression(const InputSequence &);

	bool CheckPrint();
	bool CheckRead();
	bool CheckData(const InputSequence &);

	bool CheckBracketsExpr(const std::function<bool(const InputSequence &)> & insideBracketsExpr);

	bool ChekDeclare(const InputSequence &);

	bool MakeShiftIfNeeded(const std::string & checkStr);
	bool CheckOneTokenExpr(const std::string & token, const InputSequence & seq);
	
	size_t GetNextExpressionLength();

	LL1Walker llWalker;
	InputSequence m_inputSeq;
	size_t m_currentPos = 0;
	State m_state = None;
};

