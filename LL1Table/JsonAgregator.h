#pragma once
#include "json.hpp"
#include "LL1TableString.h"

class CJsonAgregator
{
public:
	static void WriteLL1TableToFile(const LL1Table & table, const std::string & filename);
	static LL1Table ReadLL1TableFromFile(const std::string & filename);
};
