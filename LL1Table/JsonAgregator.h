#pragma once
#include "json.hpp"
//TODO выпести ll1table
#include "LL1Walker.h"

class CJsonAgregator
{
public:
	static void WriteLL1TableToFile(const LL1Table & table, const std::string & filename);
	static LL1Table ReadLL1TableFromFile(const std::string & filename);
};

