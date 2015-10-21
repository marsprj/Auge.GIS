#include "FeatureImportProcessorImpl.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	FeatureImportProcessorImpl::FeatureImportProcessorImpl()
	{
		m_user = 0;
	}

	FeatureImportProcessorImpl::~FeatureImportProcessorImpl()
	{

	}

	void FeatureImportProcessorImpl::SetShapePath(const char* path)
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

	void FeatureImportProcessorImpl::SetShapeName(const char* className)
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

	void FeatureImportProcessorImpl::SetDataSourceName(const char* sourceName)
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

	void FeatureImportProcessorImpl::SetFeatureClassName(const char* typeName)
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

	const char*	FeatureImportProcessorImpl::GetShapePath()
	{
		return m_shp_path.empty() ? NULL : m_shp_path.c_str();
	}

	const char* FeatureImportProcessorImpl::GetShapeName()
	{
		return m_shp_name.empty() ? NULL : m_shp_name.c_str();
	}

	const char*	FeatureImportProcessorImpl::GetDataSourceName()
	{
		return m_db_source_name.empty() ? NULL : m_db_source_name.c_str();
	}

	const char* FeatureImportProcessorImpl::GetFeatureClassName()
	{
		return m_db_feature_class_name.empty() ? NULL : m_db_feature_class_name.c_str();
	}

	RESULTCODE FeatureImportProcessorImpl::Execute()
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
		FeatureWorkspace* pshpWorkspace = dynamic_cast<FeatureWorkspace*>(pshpEngine->CreateWorkspace());
		pshpWorkspace->SetConnectionString(constr);
		rc = pshpWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pshpWorkspace->Release();

			const char* msg = "Cannot Open Shapefile Database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		FeatureClass* pshpFeatureClass = pshpWorkspace->OpenFeatureClass(shp_name);
		if(pshpFeatureClass==NULL)
		{
			pshpWorkspace->Release();

			const char* msg = "Cannot Open Shapefile FeatureClass";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		FeatureWorkspace* pdbWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pdbWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspace(m_user, source_name));
		if(pdbWorkspace==NULL)
		{
			pshpFeatureClass->Release();
			pshpWorkspace->Release();

			const char* msg = "Cannot Connect to Database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		GFields* pFields = pshpFeatureClass->GetFields();

		FeatureClass* pdbFeatureClass = NULL;
		pdbFeatureClass = pdbWorkspace->OpenFeatureClass(type_name);
		if(pdbFeatureClass!=NULL)
		{
			pdbFeatureClass->Release();
			pshpFeatureClass->Release();
			pshpWorkspace->Release();

			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "FeatureClass [%s]ÒÑ¾­´æÔÚ", type_name);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		pdbFeatureClass = pdbWorkspace->CreateFeatureClass(type_name, pFields);
		if(pdbFeatureClass==NULL)
		{
			pshpFeatureClass->Release();
			pshpWorkspace->Release();

			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return AG_FAILURE;
		}

		Feature* pFeature = NULL;
		FeatureCursor* pshpCursor = NULL;
		pshpCursor = pshpFeatureClass->Query();

		FeatureInsertCommand* cmd = pdbFeatureClass->CreateInsertCommand();
		
		long ts = auge_get_time();

		while((pFeature=pshpCursor->NextFeature())!=NULL)
		{
			cmd->Insert(pFeature);
			pFeature->Release();
		}
		cmd->Commit();
		long te = auge_get_time();

		char msg[AUGE_MSG_MAX];
		memset(msg, 0, AUGE_MSG_MAX);
		g_sprintf(msg, "[%s] import Finished, using\t%ld\ts", shp_name, (te-ts)/1000);
		pLogger->Info(msg, __FILE__, __LINE__);

		cmd->Commit();
		cmd->Release();
		pdbFeatureClass->Refresh();
		pdbFeatureClass->Release();

		pshpCursor->Release();
		pshpFeatureClass->Release();
		pshpWorkspace->Close();
		pshpWorkspace->Release();

		

		return AG_SUCCESS;
	}

	void FeatureImportProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}