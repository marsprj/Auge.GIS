#include "FeatureCursorByd.h"
#include "FeatureClassByd.h"
#include "FeatureByd.h"
#include "WorkspaceByd.h"
#include "SQLBuilder.h"
#include "AugeField.h"

namespace auge
{
	FeatureCursorByd::FeatureCursorByd()
	{
		m_pFeatureClass = NULL;
		m_pConnection = NULL;
		m_pResultSet = NULL;
		m_pStatment = NULL;
	}

	FeatureCursorByd::~FeatureCursorByd()
	{
		if(m_pFeatureClass!=NULL)
		{
			m_pFeatureClass->Release();
			m_pFeatureClass = NULL;
		}
		if (m_pResultSet != NULL)
		{
			m_pStatment->CloseResultSet(m_pResultSet);
			m_pResultSet = NULL;
		}
		if (m_pStatment != NULL)
		{
			m_pConnection->CloseStatement(m_pStatment);
			m_pStatment = NULL;
		}
		if (m_pConnection != NULL)
		{
			m_pConnection = NULL;
		}
	}

	Feature* FeatureCursorByd::NextFeature()
	{
		if(m_pResultSet==NULL)
		{
			return NULL;
		}

		if(m_pResultSet->Next()!=CS_OK)
		{
			return NULL;
		}

		FeatureByd* pFeature = new FeatureByd();
		pFeature->Create(m_pResultSet, m_pFeatureClass);

		return pFeature;
	}

	FeatureClass* FeatureCursorByd::GetFeatureClass()
	{
		return m_pFeatureClass;
	}

	bool FeatureCursorByd::Create(FeatureClassByd* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pConnection = m_pFeatureClass->m_pWorkspace->m_pbydConnction;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		std::string sql;
		SQLBuilder::BuildQuery(sql, pFeatureClass, (GFilter*)NULL);
		pLogger->Info(sql.c_str(), __FILE__, __LINE__);

		m_pStatment = m_pConnection->CreateStatement();
		if(m_pStatment==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return false;
		}
		m_pResultSet = m_pStatment->ExecuteQuery(sql.c_str());
		if(m_pResultSet==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pConnection->CloseStatement(m_pStatment);
			m_pStatment = NULL;
			return false;
		}

		return true;
	}

	bool FeatureCursorByd::Create(FeatureClassByd* pFeatureClass, GFilter* pFilter)
	{
		m_pFeatureClass = pFeatureClass;
		m_pConnection = m_pFeatureClass->m_pWorkspace->m_pbydConnction;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		std::string sql;
		SQLBuilder::BuildQuery(sql, pFeatureClass, pFilter);
		pLogger->Info(sql.c_str(), __FILE__, __LINE__);

		m_pStatment = m_pConnection->CreateStatement();
		if(m_pStatment==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return false;
		}
		m_pResultSet = m_pStatment->ExecuteQuery(sql.c_str());
		if(m_pResultSet==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pConnection->CloseStatement(m_pStatment);
			m_pStatment = NULL;
			return false;
		}

		return true;
	}

	bool FeatureCursorByd::Create(FeatureClassByd* pFeatureClass, GQuery* pQuery)
	{
		//m_pFeatureClass = pFeatureClass;
		m_pConnection = pFeatureClass->m_pWorkspace->m_pbydConnction;
		m_pFeatureClass = CreateFeatureClass(pFeatureClass, pQuery);

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GFields* pFields = pFeatureClass->GetFields();

		std::string sql;
		SQLBuilder::BuildQuery(sql, pFeatureClass, pQuery);
		pLogger->Info(sql.c_str(), __FILE__, __LINE__);

		m_pStatment = m_pConnection->CreateStatement();
		if(m_pStatment==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return false;
		}
		m_pResultSet = m_pStatment->ExecuteQuery(sql.c_str());
		if(m_pResultSet==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pConnection->CloseStatement(m_pStatment);
			m_pStatment = NULL;
			return false;
		}
		return true;
	}
	
	bool FeatureCursorByd::Create(FeatureClassByd* pFeatureClass, GEnvelope& extent)
	{
		m_pFeatureClass = pFeatureClass;
		m_pConnection = m_pFeatureClass->m_pWorkspace->m_pbydConnction;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		std::string sql;
		SQLBuilder::BuildQuery(sql, pFeatureClass, extent);
		pLogger->Info(sql.c_str(), __FILE__, __LINE__);

		m_pStatment = m_pConnection->CreateStatement();
		if(m_pStatment==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return false;
		}
		m_pResultSet = m_pStatment->ExecuteQuery(sql.c_str());
		if(m_pResultSet==NULL)
		{
			CPPIString msg = m_pConnection->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pConnection->CloseStatement(m_pStatment);
			m_pStatment = NULL;
			return false;
		}

		return true;
	}

	FeatureClassByd* FeatureCursorByd::CreateFeatureClass(FeatureClassByd* pFeatureClass, GQuery* pQuery)
	{
		FeatureClassByd* pnewFeatureClass = new FeatureClassByd();
		pnewFeatureClass->m_name = pFeatureClass->GetName();
		pnewFeatureClass->m_pWorkspace = pFeatureClass->m_pWorkspace;

		GField*  pField = NULL;
		GFields* pFields = pFeatureClass->GetFields();
		GFields* pnewFields = pnewFeatureClass->GetFields();
		const char* fname = NULL;

		g_int nField = pQuery->GetSubFieldCount();
		for(g_int i=0; i<nField; i++)
		{
			fname = pQuery->GetSubField(i);
			pField = pFields->GetField(fname);
			if(pField!=NULL)
			{
				pnewFields->Add(pField);
				pField->AddRef();
			}
		}
		if(pnewFields->FindField(fname)<0)
		{
			pField = pFields->GetField("fid");
			if(pField!=NULL)
			{
				pnewFields->Add(pField);
				pField->AddRef();
			}
		}

		return pnewFeatureClass;
	}

	FeatureClassByd* FeatureCursorByd::CreateFeatureClass(FeatureClassByd* pFeatureClass, CPPIResultSet* pResultSet)
	{
		FeatureClassByd* pnewFeatureClass = new FeatureClassByd();
		pnewFeatureClass->Create(pFeatureClass->GetName(), pFeatureClass->m_pWorkspace, m_pResultSet);
		
		GField* pgeoField = NULL;
		GField* pnewField = NULL;

		pgeoField = pFeatureClass->GetFields()->GetGeometryField();
		if(pgeoField!=NULL)
		{
			pnewField = pnewFeatureClass->GetFields()->GetGeometryField();
			if(pnewField!=NULL)
			{
				GeometryDef* pgeoGeometryDef = pgeoField->GetGeometryDef();
				GeometryDef* pnewGeometryDef = pnewField->GetGeometryDef();
				GeometryDef_2* pnewGeometryDef_2 = pnewGeometryDef->GetGeometryDef_2();
				pnewGeometryDef_2->SetDefault(true);
				pnewGeometryDef_2->SetDimension(pgeoGeometryDef->GetDimension());				
				pnewGeometryDef_2->SetGeometryType(pgeoGeometryDef->GeometryType());
				pnewGeometryDef_2->SetSRID(pgeoGeometryDef->GetSRID());
				GEnvelope extent;
				pgeoGeometryDef->GetExtent(extent);
				pnewGeometryDef_2->SetExtent(extent);
			}
		}

		return pnewFeatureClass;
	}

}