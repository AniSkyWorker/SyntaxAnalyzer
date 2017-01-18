#include "stdafx.h"
#include "TableStorage.h"
#include "JsonAgregator.h"

CTableStorage::CTableStorage()
{
	LoadTables();
}

const LL1Table & CTableStorage::GetLL1Table(TableType type)
{
	return m_LL1Tables.at(type);
}

const LRTable & CTableStorage::GetLRTable(TableType type)
{
	return m_LRTables.at(type);
}


void CTableStorage::LoadTables()
{
	m_LL1Tables.emplace(TableType::arithmetic, CJsonAgregator::ReadLL1TableFromFile("arithmetic1.json"));
	m_LL1Tables.emplace(TableType::boolean, CJsonAgregator::ReadLL1TableFromFile("bool.json"));
}
