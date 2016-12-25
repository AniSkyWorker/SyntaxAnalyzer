#include "stdafx.h"
#include "LL1TableString.h"

LL1TableString::LL1TableString(const std::vector<std::string> &input, bool shift, int transition, bool stack, bool error, bool end)
	: transition(transition)
	, error(error)
	, shift(shift)
	, end(end)
	, input(input)
	, stack(stack)
{
}