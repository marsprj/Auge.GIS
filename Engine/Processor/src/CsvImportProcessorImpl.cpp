#include "CsvImportProcessorImpl.h"
#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	CsvImportProcessorImpl::CsvImportProcessorImpl()
	{

	}

	CsvImportProcessorImpl::~CsvImportProcessorImpl()
	{

	}

	void CsvImportProcessorImpl::SetCvsPath(const char* path)
	{
		if(path==NULL)
		{
			m_csv_path.clear();
		}
		else
		{
			m_csv_path = path;
		}
	}

	void CsvImportProcessorImpl::SetDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	void CsvImportProcessorImpl::SetDatasetName(const char* datasetName)
	{
		if(datasetName==NULL)
		{
			m_dataset_name.clear();
		}
		else
		{
			m_dataset_name = datasetName;
		}
	}

	RESULTCODE CsvImportProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		char csvDrv[AUGE_DRV_MAX];
		char csvDir[AUGE_PATH_MAX];
		char csvName[AUGE_NAME_MAX];
		char csvPath[AUGE_PATH_MAX];
		char constr[AUGE_PATH_MAX];
		memset(csvDrv, 0, AUGE_DRV_MAX);
		memset(csvDir, 0, AUGE_PATH_MAX);
		memset(csvName, 0, AUGE_NAME_MAX);
		memset(csvPath, 0, AUGE_PATH_MAX);
		memset(constr,0,AUGE_PATH_MAX);

		auge_split_path(m_csv_path.c_str(), csvDrv, csvDir,csvName,NULL);
		auge_make_path(csvPath, csvDrv,csvDir,NULL,NULL);
		g_sprintf(constr,"DATABASE=%s",csvPath);

		DataEngine* pDataEngine = NULL;
		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		pDataEngine = pDataEngineManager->GetEngine("CsvFile");
		if(pDataEngine==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
			return AG_FAILURE;
		}
		
		RESULTCODE rc = AG_FAILURE;
		Workspace* pcsvWorkspace = pDataEngine->CreateWorkspace();
		pcsvWorkspace->SetConnectionString(constr);
		rc = pcsvWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			pcsvWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureWorksapce* pobjWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pobjWorkspace = dynamic_cast<FeatureWorksapce*>(pConnectionManager->GetWorkspace(m_source_name.c_str()));
		if(pobjWorkspace==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			pcsvWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pobjWorkspace->OpenFeatureClass(m_dataset_name.c_str());
		if(pFeatureClass==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
			pcsvWorkspace->Release();
			return AG_FAILURE;
		}

		AttributeDataSet* pcsvDataset = static_cast<AttributeDataSet*>(pcsvWorkspace->OpenDataSet(csvName));
		GFields* pcsvFields = pcsvDataset->GetFields();
		GFields* pobjFields = pFeatureClass->GetFields();
		if(!IsMatch(pcsvFields,pobjFields))
		{
			const char* msg = "Field is not matched";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			pFeatureClass->Release();
			pcsvDataset->Release();
			pcsvWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();

		Row* pRow = NULL;
		Cursor* pCursor = pcsvDataset->GetRows();
		while((pRow=pCursor->NextRow())!=NULL)
		{
			AddFeature(pRow, pFeatureClass,cmd);

			pRow->Release();
		}
		cmd->Release();
		pCursor->Release();		

		pFeatureClass->Release();
		pcsvDataset->Release();
		pcsvWorkspace->Release();

		return AG_SUCCESS;
	}

	void CsvImportProcessorImpl::Release()
	{
		delete this;
	}

	bool CsvImportProcessorImpl::IsMatch(GFields* pcsvFields, GFields* pobjFields)
	{
		g_uint csv_count = pcsvFields->Count();
		g_uint obj_count = pobjFields->Count();
		if(csv_count!=(obj_count-1))
		{
			return false;
		}

		GField* pField = NULL;
		for(g_uint i=0; i<csv_count; i++)
		{
			pField = pcsvFields->GetField(i);
			if(pobjFields->FindField(pField->GetName())<0)
			{
				return false;
			}
		}

		return true;
	}

	bool CsvImportProcessorImpl::AddFeature(Row* pRow, FeatureClass* pFeatureClass, FeatureInsertCommand* cmd)
	{
		GField*	 pField  = NULL;
		GFields* pFields = pFeatureClass->GetFields();
		Feature* pFeature = pFeatureClass->NewFeature();
		GValue* pValue = NULL;
		const char* fname = NULL;
		augeFieldType ftype = augeFieldTypeNone;
		
		const char* str = NULL;
		
		g_uint fcount = pFields->Count();
		for(g_uint i=0; i<fcount; i++)
		{
			pField = pFields->GetField(i);
			fname = pField->GetName();
			ftype = pField->GetType();

			str = pRow->GetString(fname);
			if(str==NULL)
			{
				continue;
			}
			switch(ftype)
			{
			case augeFieldTypeShort:
				pValue = new GValue((short)atoi(str));
				break;
			case augeFieldTypeInt:
				pValue = new GValue((int)atoi(str));
				break;
			case augeFieldTypeLong:
				pValue = new GValue((long)atoi(str));
				break;
			case augeFieldTypeInt64:
				pValue = new GValue((int64)atoi(str));
				break;
			case augeFieldTypeFloat:
				pValue = new GValue((float)atof(str));
				break;
			case augeFieldTypeDouble:
				pValue = new GValue((double)atof(str));
				break;
			case augeFieldTypeString:
				pValue = new GValue(str);
				break;
			}
			pFeature->SetValue(fname, pValue);
		}
		
		RESULTCODE rc = cmd->Insert(pFeature);
		pFeature->Release();

		return (rc==AG_SUCCESS);
	}
}