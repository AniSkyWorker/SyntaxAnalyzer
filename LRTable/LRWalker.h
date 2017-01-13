#pragma once
#include "Rule.h"
#include "LRRow.h"
#include <vector>
#include <stack>

typedef std::vector<Rule> Rules;
typedef std::vector<std::vector<RowElement>> LRTable;

class LRWalker
{
public:
	LRWalker(const std::vector<std::string> & seq, const LRTable & table);
	bool CheckInputSequence();

private:
	std::vector<std::string> m_inputSeq;
	LRTable m_table;
	std::stack<std::string> m_elements;
	std::stack<unsigned> m_transitions;
};

