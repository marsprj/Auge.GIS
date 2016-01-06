#include "XYImportProcessorImpl.h"
#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	XYImportProcessorImpl::XYImportProcessorImpl()
	{
		m_user = 0;
	}

	XYImportProcessorImpl::~XYImportProcessorImpl()
	{

	}

	void XYImportProcessorImpl::SetXYPath(const char* path)
	{
		if(path==NULL)
		{
			m_xy_path.clear();
		}
		else
		{
			m_xy_path = path;
		}
	}

	void XYImportProcessorImpl::SetDataSource(const char* sourceName)
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

	void XYImportProcessorImpl::SetDatasetName(const char* datasetName)
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

	void XYImportProcessorImpl::SetX(const char* fname)
	{
		if(fname==NULL)
		{
			m_x_field.clear();
		}
		else
		{
			m_x_field = fname;
		}
	}

	void XYImportProcessorImpl::SetY(const char* fname)
	{
		if(fname==NULL)
		{
			m_y_field.clear();
		}
		else
		{
			m_y_field = fname;
		}
	}

	RESULTCODE XYImportProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		char xyDrv[AUGE_DRV_MAX];
		char xyDir[AUGE_PATH_MAX];
		char xyName[AUGE_NAME_MAX];
		char xyPath[AUGE_PATH_MAX];
		char constr[AUGE_PATH_MAX];
		memset(xyDrv, 0, AUGE_DRV_MAX);
		memset(xyDir, 0, AUGE_PATH_MAX);
		memset(xyName, 0, AUGE_NAME_MAX);
		memset(xyPath, 0, AUGE_PATH_MAX);
		memset(constr,0,AUGE_PATH_MAX);

		auge_split_path(m_xy_path.c_str(), xyDrv, xyDir,xyName,NULL);
		auge_make_path(xyPath, xyDrv,xyDir,NULL,NULL);
		g_sprintf(constr,"DATABASE=%s",xyPath);

		DataEngine* pDataEngine = NULL;
		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		pDataEngine = pDataEngineManager->GetEngine("CsvFile");
		if(pDataEngine==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
			return AG_FAILURE;
		}
		
		RESULTCODE rc = AG_FAILURE;
		Workspace* pxyWorkspace = pDataEngine->CreateWorkspace();
		pxyWorkspace->SetConnectionString(constr);
		rc = pxyWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			pxyWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureWorkspace* pobjWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pobjWorkspace = dynamic_cast<FeatureWorkspace*>(pConnectionManager->GetWorkspace(m_user, m_source_name.c_str()));
		if(pobjWorkspace==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			pxyWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pobjWorkspace->OpenFeatureClass(m_dataset_name.c_str());
		if(pFeatureClass==NULL)
		{
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
			pxyWorkspace->Release();
			return AG_FAILURE;
		}

		AttributeDataSet* pxyDataset = static_cast<AttributeDataSet*>(pxyWorkspace->OpenDataSet(xyName));
		GFields* pxyFields = pxyDataset->GetFields();
		GFields* pobjFields = pFeatureClass->GetFields();
		if(!IsMatch(pxyFields,pobjFields))
		{
			const char* msg = "Field is not matched";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			pFeatureClass->Release();
			pxyDataset->Release();
			pxyWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();

		Row* pRow = NULL;
		Cursor* pCursor = pxyDataset->GetRows();
		while((pRow=pCursor->NextRow())!=NULL)
		{
			AddFeature(pRow, pFeatureClass,cmd);

			pRow->Release();
		}
		cmd->Commit();
		cmd->Release();
		pCursor->Release();		

		pFeatureClass->Refresh();
		pFeatureClass->Release();
		pxyDataset->Release();
		pxyWorkspace->Release();

		return AG_SUCCESS;
	}

	void XYImportProcessorImpl::Release()
	{
		delete this;
	}

	bool XYImportProcessorImpl::IsMatch(GFields* pxyFields, GFields* pobjFields)
	{
		g_uint xy_count = pxyFields->Count();
		g_uint obj_count = pobjFields->Count();
		//if(xy_count!=(obj_count-1))
		//{
		//	return false;
		//}

		GField* pField = NULL;
		for(g_uint i=0; i<xy_count; i++)
		{
			pField = pxyFields->GetField(i);
			if(pobjFields->FindField(pField->GetName())<0)
			{
				return false;
			}
		}

		return true;
	}

	bool XYImportProcessorImpl::AddFeature(Row* pRow, FeatureClass* pFeatureClass, FeatureInsertCommand* cmd)
	{
		GField*	 pField  = NULL;
		GFields* pFields = pFeatureClass->GetFields();
		Feature* pFeature = pFeatureClass->NewFeature();
		GValue* pValue = NULL;
		const char* fname = NULL;
		augeFieldType ftype = augeFieldTypeNone;
		double x = 0.0f;
		double y = 0.0f;
		bool hasx=false, hasy=false;
		
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
			if(g_stricmp(fname, m_x_field.c_str())==0)
			{
				x = (double)atof(str);
				hasx = true;
			}
			else if(g_stricmp(fname, m_y_field.c_str())==0)
			{
				y = (double)atof(str);
				hasy = true;
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

		if(hasx&&hasy)
		{
			pField = pFields->GetGeometryField();
			if(pField!=NULL)
			{
				unsigned char wkb[AUGE_NAME_MAX];
				memset(wkb, 0, AUGE_NAME_MAX);
				WKBPoint*pWKBPoint = (WKBPoint*)wkb;
				pWKBPoint->byteOrder = coDefaultByteOrder;
				pWKBPoint->wkbType = wkbPoint;
				pWKBPoint->point.x = x;
				pWKBPoint->point.y = y;
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				Geometry*pGeometry = pGeometryFactory->CreateGeometryFromWKB(wkb, true);
				if(pGeometry!=NULL)
				{
					GValue* pValue = new GValue(pGeometry);
					pFeature->SetValue(pField->GetName(), pValue);
				}
			}
		}
		
		RESULTCODE rc = cmd->Insert(pFeature);
		pFeature->Release();

		return (rc==AG_SUCCESS);
	}

	void XYImportProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}