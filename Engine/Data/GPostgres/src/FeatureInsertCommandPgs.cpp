#include "FeatureInsertCommandPgs.h"
#include "FeatureClassPgs.h"
#include "WorkspacePgs.h"
#include "SQLBuilder.h"

namespace auge
{
	FeatureInsertCommandPgs::FeatureInsertCommandPgs(FeatureClassPgs* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pgConnection = &(m_pFeatureClass->m_pWorkspace->m_pgConnection);
	}

	FeatureInsertCommandPgs::~FeatureInsertCommandPgs()
	{

	}

	RESULTCODE FeatureInsertCommandPgs::Prepare(FeatureClass* pFeatureClass)
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureInsertCommandPgs::Insert(Feature* pFeature)
	{
		if(pFeature==NULL)
		{
			return AG_FAILURE;
		}

		std::string sql;
		SQLBuilder::BuildInsertFeature(sql, m_pFeatureClass->GetName(), pFeature, m_pFeatureClass->GetSRID());
		//return m_pgConnection->ExecuteSQL(sql.c_str());
		return AG_SUCCESS;
	}

	RESULTCODE FeatureInsertCommandPgs::Commit()
	{
		return AG_SUCCESS;
	}
}