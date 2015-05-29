#include "FeatureImportProcessorImpl.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	FeatureImportProcessorImpl::FeatureImportProcessorImpl()
	{

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

	void FeatureImportProcessorImpl::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_db_type_name.clear();
		}
		else
		{
			m_db_type_name = typeName;
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

	const char* FeatureImportProcessorImpl::GetTypeName()
	{
		return m_db_type_name.empty() ? NULL : m_db_type_name.c_str();
	}

	RESULTCODE FeatureImportProcessorImpl::Execute()
	{
		const char* shp_path = GetShapePath();
		const char* shp_name = GetShapeName();
		const char* source_name = GetDataSourceName();
		const char* type_name = GetTypeName();

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
		FeatureWorksapce* pshpWorkspace = dynamic_cast<FeatureWorksapce*>(pshpEngine->CreateWorkspace());
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

		FeatureWorksapce* pdbWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pdbWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(source_name));
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
		
		while((pFeature=pshpCursor->NextFeature())!=NULL)
		{
			cmd->Insert(pFeature);
			pFeature->Release();
		}

		cmd->Release();
		pdbFeatureClass->Release();

		pshpCursor->Release();
		pshpFeatureClass->Release();
		pshpWorkspace->Close();
		pshpWorkspace->Release();

		return AG_SUCCESS;
	}
}