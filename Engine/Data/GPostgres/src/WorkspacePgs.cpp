#include "WorkspacePgs.h"
#include "FeatureClassPgs.h"
#include "DataEngineImpl.h"

namespace auge
{	
	WorkspacePgs::WorkspacePgs()
	{

	}

	WorkspacePgs::~WorkspacePgs()
	{
	}

	DataEngine*	WorkspacePgs::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspacePgs::GetName()
	{
		return m_name.c_str();
	}

	void WorkspacePgs::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspacePgs::SetConnectionString(const char* conn_string)
	{
		return m_pgConnection.SetConnectionString(conn_string);
	}

	RESULTCODE WorkspacePgs::Open()
	{
		return m_pgConnection.Open();
	}

	void WorkspacePgs::Close()
	{
		m_pgConnection.Close();
	}

	bool WorkspacePgs::IsOpen()
	{
		return m_pgConnection.IsOpen();
	}

	FeatureClass* WorkspacePgs::OpenFeatureClass(const char* name)
	{
		if((name==NULL)||(!m_pgConnection.IsOpen()))
		{
			return NULL;
		}

		FeatureClassPgs* pFeatureClass = new FeatureClassPgs();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}
}
