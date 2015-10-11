#include "FeatureInsertCommandByd.h"
#include "FeatureClassByd.h"
#include "WorkspaceByd.h"
#include "SQLBuilder.h"

namespace auge
{
	FeatureInsertCommandByd::FeatureInsertCommandByd(FeatureClassByd* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pWorkspace = pFeatureClass->m_pWorkspace;
	}

	FeatureInsertCommandByd::~FeatureInsertCommandByd()
	{

	}

	RESULTCODE FeatureInsertCommandByd::Prepare(FeatureClass* pFeatureClass)
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureInsertCommandByd::Insert(Feature* pFeature)
	{
		if(pFeature==NULL)
		{
			return AG_FAILURE;
		}

		CPPIConnection* pConnection = m_pWorkspace->m_pbydConnction;

		std::string sql;
		SQLBuilder::BuildInsertFeature(sql, m_pFeatureClass, pFeature);

		return m_pWorkspace->ExecuteSQL(sql.c_str());
	}

	RESULTCODE FeatureInsertCommandByd::Commit()
	{
		m_pFeatureClass->BuildSpatialIndex();
		return AG_SUCCESS;
	}
}