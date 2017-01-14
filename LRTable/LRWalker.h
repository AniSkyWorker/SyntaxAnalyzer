#pragma once
#include "Rule.h"
#include "LRRow.h"
#include <stack>

typedef std::vector<Rule> Rules;

class LRWalker
{
public:
	static bool CheckInputSequence(std::vector<std::string> seq, const LRTable & table);
};

