#include "stdafx.h"
#include "LL1Walker.h"
#include "JsonAgregator.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	LL1Table table1 = 
	{
		LL1TableString({ "prog" }, false, 1, false, true, false),
		LL1TableString({ "prog" }, true, 2, false, true, false),
		LL1TableString({ "id" }, true, 3, false, true, false),
		LL1TableString({ "var" }, false, 7, true, true, false),
		LL1TableString({ "begin" }, true, 5, false, true, false),
		LL1TableString({ "st" }, false, 18, true, true, false),
		LL1TableString({ "end" }, false, -1, false, true, true),
		LL1TableString({ "var" }, false, 8, false, true, false),
		LL1TableString({ "var" }, true, 9, false, true, false),
		LL1TableString({ "id" }, false, 10, false, true, false),
		LL1TableString({ "id" }, false, 11, false, true, false),
		LL1TableString({ "id" }, true, 12, false, true, false),
		LL1TableString({ "begin", "," }, false, 13, false, true, false),
		LL1TableString({ "," }, false, 15, false, false, false),
		LL1TableString({ "begin" }, false, 17, false, true, false),
		LL1TableString({ "," }, true, 16, false, true, false),
		LL1TableString({ "id" }, false, 10, false, true, false),
		LL1TableString({ "begin" }, false, -1, false, true, false),
		LL1TableString({ "st" }, false, 19, false, true, false),
		LL1TableString({ "st" }, true, 20, false, true, false),
		LL1TableString({ "st", "end" }, false, 21, false, true, false),
		LL1TableString({ "st" }, false, 23, false, false, false),
		LL1TableString({ "end" }, false, 24, false, true, false),
		LL1TableString({ "st", "end" }, false, 18, false, true, false),
		LL1TableString({ "end" }, false, -1, false, true, false)
	};

	std::vector<std::string> str{ "prog", "id", "var", "id", ",", "id",  ",",  "id", "begin", "st", "st", "end"};

	LL1Walker walker;
	CJsonAgregator::WriteLL1TableToFile(table1, "example.json");
	auto table = CJsonAgregator::ReadLL1TableFromFile("example.json");
	try
	{
		if (walker.CheckInputSequence(str, table))
		{
			cout << "Sequence accepted!";
		}
		
	}
	catch (runtime_error & err)
	{
		cout << err.what();
	}
	return 0;
}
