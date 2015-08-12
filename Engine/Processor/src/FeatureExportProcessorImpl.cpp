#include "FeatureExportProcessorImpl.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	FeatureExportProcessorImpl::FeatureExportProcessorImpl()
	{
		m_user = 0;
	}

	FeatureExportProcessorImpl::~FeatureExportProcessorImpl()
	{

	}

	void FeatureExportProcessorImpl::SetShapePath(const char* path)
	{
		if(path==NULL)
		{
			m_shp_path.clear();
		}
		else
		{
			m_shp_path = path;
		}
	}

	void FeatureExportProcessorImpl::SetShapeName(const char* className)
	{
		if(className==NULL)
		{
			m_shp_name.clear();
		}
		else
		{
			m_shp_name = className;
		}
	}

	void FeatureExportProcessorImpl::SetDataSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_db_source_name.clear();
		}
		else
		{
			m_db_source_name = sourceName;
		}
	}

	void FeatureExportProcessorImpl::SetFeatureClassName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_db_feature_class_name.clear();
		}
		else
		{
			m_db_feature_class_name = typeName;
		}
	}

	const char*	FeatureExportProcessorImpl::GetShapePath()
	{
		return m_shp_path.empty() ? NULL : m_shp_path.c_str();
	}

	const char* FeatureExportProcessorImpl::GetShapeName()
	{
		return m_shp_name.empty() ? NULL : m_shp_name.c_str();
	}

	const char*	FeatureExportProcessorImpl::GetDataSourceName()
	{
		return m_db_source_name.empty() ? NULL : m_db_source_name.c_str();
	}

	const char* FeatureExportProcessorImpl::GetFeatureClassName()
	{
		return m_db_feature_class_name.empty() ? NULL : m_db_feature_class_name.c_str();
	}

	RESULTCODE FeatureExportProcessorImpl::Execute()
	{
		const char* shp_path = GetShapePath();
		const char* shp_name = GetShapeName();
		const char* source_name = GetDataSourceName();
		const char* type_name = GetFeatureClassName();

		GError*	 pError  = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		if(shp_path==NULL)
		{
			const char* msg = "Shapefile Database not defined";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		if(shp_name==NULL)
		{
			const char* msg = "Shapefile Name not defined";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		if(source_name==NULL)
		{
			const char* msg = "Datasource Name not defined";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		if(type_name==NULL)
		{
			const char* msg = "Type Name not defined";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		DataEngine* pshpEngine = NULL;
		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		pshpEngine = pDataEngineManager->GetEngine("Shape");
		if(pshpEngine==NULL)
		{
			const char* msg = "Cannot load shapefile data engine";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		char constr[AUGE_MSG_MAX];
		g_sprintf(constr,"DATABASE=%s", shp_path);
		
		FeatureWorkspace* pdbWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pdbWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->NewWorkspace(m_user, source_name));
		if(pdbWorkspace==NULL)
		{
			const char* msg = "Cannot Connect to Database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		FeatureClass* pdbFeatureClass = pdbWorkspace->OpenFeatureClass(type_name);
		if(pdbFeatureClass==NULL)
		{
			pdbWorkspace->Release();

			const char* msg = "Cannot Open FeatureClass";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		FeatureWorkspace* pshpWorkspace = dynamic_cast<FeatureWorkspace*>(pshpEngine->CreateWorkspace());
		pshpWorkspace->SetConnectionString(constr);
		rc = pshpWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pdbFeatureClass->Release();
			pdbWorkspace->Release();

			const char* msg = "Cannot Open Shapefile Database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		GFields* pFields = pdbFeatureClass->GetFields();
		FeatureClass* pshpFeatureClass = NULL;
		pshpFeatureClass = pshpWorkspace->CreateFeatureClass(shp_name, pFields);
		if(pdbFeatureClass==NULL)
		{
			pdbFeatureClass->Release();
			pdbWorkspace->Release();
			pshpWorkspace->Release();

			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return AG_FAILURE;
		}

		Feature* pFeature = NULL;
		FeatureCursor* pdbCursor = NULL;
		pdbCursor = pdbFeatureClass->Query();

		FeatureInsertCommand* cmd = pshpFeatureClass->CreateInsertCommand();
		
		while((pFeature=pdbCursor->NextFeature())!=NULL)
		{
			cmd->Insert(pFeature);
			pFeature->Release();
		}

		pshpFeatureClass->Refresh();
		cmd->Release();
		pshpFeatureClass->Release();
		pshpWorkspace->Close();
		pshpWorkspace->Release();

		pdbCursor->Release();
		pdbFeatureClass->Release();
		pdbWorkspace->Close();
		pdbWorkspace->Release();

		return AG_SUCCESS;
	}

	void FeatureExportProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}