#include "stdafx.h"
#include "TableStorage.h"

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

}
