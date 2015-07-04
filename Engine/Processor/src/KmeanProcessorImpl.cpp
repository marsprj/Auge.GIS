#include "KmeanProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	KMeanProcessorImpl::KMeanProcessorImpl()
	{
		m_k = 0;
	}

	KMeanProcessorImpl::~KMeanProcessorImpl()
	{

	}

	void KMeanProcessorImpl::SetK(g_uint k)
	{
		m_k = k;
	}

	void KMeanProcessorImpl::SetInputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name.clear();
		}
		else
		{
			m_in_source_name = sourceName;
		}
	}

	void KMeanProcessorImpl::SetInputFeatureClass(const char* className)
	{
		if(className==NULL)
		{
			m_in_class_name.clear();
		}
		else
		{
			m_in_class_name = className;
		}
	}

	void KMeanProcessorImpl::SetOutputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_source_name.clear();
		}
		else
		{
			m_out_source_name = sourceName;
		}
	}

	void KMeanProcessorImpl::SetOutputFeatureClass(const char* className)
	{
		if(className==NULL)
		{
			m_out_class_name.clear();
		}
		else
		{
			m_out_class_name = className;
		}
	}

	const char* KMeanProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* KMeanProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	KMeanProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	KMeanProcessorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE KMeanProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_in = GetInputSource();
		const char* className_in  = GetInputFatureClass();

		const char* sourceName_out= GetOutputSource();
		const char* className_out = GetOutputFatureClass();

		if(sourceName_in==NULL)
		{
			return AG_FAILURE;
		}

		if(className_in==NULL)
		{
			return AG_FAILURE;
		}

		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}

		if(className_out==NULL)
		{
			return AG_FAILURE;
		}
		
		FeatureClass		*pFeatureClassIn = NULL;
		FeatureClass		*pFeatureClassOut= NULL;
		FeatureWorksapce	*pWorspaceIn = NULL;
		FeatureWorksapce	*pWorspaceOut= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();
		
		pWorspaceIn = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(sourceName_in));
		if(pWorspaceIn==NULL)
		{
			return AG_FAILURE;
		}

		pWorspaceOut = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(sourceName_out));
		if(pWorspaceOut==NULL)
		{
			return AG_FAILURE;
		}

		pFeatureClassIn = pWorspaceIn->OpenFeatureClass(className_in);
		if(pFeatureClassIn==NULL)
		{
			return AG_FAILURE;
		}
		g_uint srid = pFeatureClassIn->GetSRID();
		
		GField* pField = pFeatureClassIn->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			return AG_FAILURE;
		}

		if(pField->GetGeometryDef()->GeometryType()!=augeGTPoint)
		{
			return AG_FAILURE;
		}
		

		// input points
		pFeatureClassOut = pWorspaceOut->OpenFeatureClass(className_out);
		if(pFeatureClassOut!=NULL)
		{
			pFeatureClassOut->Release();
			return AG_FAILURE;
		}
		
		Geometry *pGeometry = NULL;
		Feature	 *pFeature = NULL;
		FeatureCursor* pCursor = NULL;		
		g_uint feature_count = pFeatureClassIn->GetCount();

		pCursor = pFeatureClassIn->Query();
		if(pCursor==NULL)
		{
			return AG_FAILURE;
		}

		 // init input points
		m_kmean.SetPoints(feature_count);
		m_kmean.SetClusters(m_k);

		g_uint index = 0;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				WKBPoint* pWKBPoint = (WKBPoint*)(pGeometry->AsBinary());
				m_kmean.SetPoint(index++, pWKBPoint->point.x, pWKBPoint->point.y);
			}

			pFeature->Release();
		}

		pCursor->Release();
		pFeatureClassIn->Release();
		
		// processing cluster 
		if(!m_kmean.Execute())
		{
			return AG_FAILURE;
		}
		
		RESULTCODE rc = SaveCentroidResult(&m_kmean, className_out, pWorspaceOut, srid);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		char cluster_name[AUGE_NAME_MAX];
		memset(cluster_name, 0, AUGE_NAME_MAX);
		g_sprintf(cluster_name, "%s_cluster", className_out);
		rc = SaveClusterResult(&m_kmean, cluster_name, pWorspaceOut, srid);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		return AG_SUCCESS;
	}

	RESULTCODE KMeanProcessorImpl::SaveCentroidResult(KMean* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid)
	{
		const char* geom_field = "shape";
		GeometryDef		*pGeometryDef  = NULL;
		GeometryDef_2	*pGeometryDef_2= NULL;

		GField		*pField = NULL;
		GField_2	*pField_2 = NULL;
		GFields		*pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(geom_field);
		pField_2->SetType(augeFieldTypeGeometry);
		pGeometryDef = pField_2->GetGeometryDef();
		pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetGeometryType(augeGTPoint);
		pGeometryDef_2->SetSRID(srid);
		pFields->Add(pField);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pWorkspace->CreateFeatureClass(className, pFields);
		pFields->Release();

		if(pFeatureClass==NULL)
		{
			return AG_FAILURE;
		}

		GPoint* pPoint = NULL;
		GeometryFactory *pGeometryFactory = augeGetGeometryFactoryInstance();
		pPoint = pGeometryFactory->CeatePoint(0,0);

		GValue* pValue = new GValue(pPoint);
		Feature	*pFeature = pFeatureClass->NewFeature();
		pFeature->SetValue(geom_field, pValue);

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();

		double x=0, y=0;
		g_uint count = kmean->GetClusterCount();
		for(g_uint i=0; i<count; i++)
		{
			kmean->GetCluster(i, x, y);
			pPoint->SetX(x);
			pPoint->SetY(y);
			cmd->Insert(pFeature);
		}
		pFeature->Release();

		cmd->Release();

		return AG_SUCCESS;
	}

	RESULTCODE KMeanProcessorImpl::SaveClusterResult(KMean* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid)
	{
		const char* cluster_field = "cluster";
		const char* geom_field = "shape";

		GeometryDef		*pGeometryDef  = NULL;
		GeometryDef_2	*pGeometryDef_2= NULL;

		GField		*pField = NULL;
		GField_2	*pField_2 = NULL;
		GFields		*pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();

		pFields = pFieldFactory->CreateFields();
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(cluster_field);
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(geom_field);
		pField_2->SetType(augeFieldTypeGeometry);
		pGeometryDef = pField_2->GetGeometryDef();
		pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetGeometryType(augeGTPoint);
		pGeometryDef_2->SetSRID(srid);
		pFields->Add(pField);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pWorkspace->CreateFeatureClass(className, pFields);
		pFields->Release();

		if(pFeatureClass==NULL)
		{
			return AG_FAILURE;
		}

		GPoint* pPoint = NULL;
		GeometryFactory *pGeometryFactory = augeGetGeometryFactoryInstance();
		pPoint = pGeometryFactory->CeatePoint(0,0);

		GValue* pValue = new GValue(pPoint);
		Feature	*pFeature = pFeatureClass->NewFeature();
		pFeature->SetValue(geom_field, pValue);

		pValue = new GValue((int)0);
		pFeature->SetValue(cluster_field, pValue);

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();

		g_uint cluster = 0;
		double x=0, y=0;
		g_uint count = kmean->GetPointCount();
		for(g_uint i=0; i<count; i++)
		{	
			kmean->GetPoint(i, x, y, cluster);
			pPoint->SetX(x);
			pPoint->SetY(y);
			pValue->SetInt(cluster);

			cmd->Insert(pFeature);
		}
		pFeature->Release();

		cmd->Release();

		return AG_SUCCESS;
	}

	void KMeanProcessorImpl::Release()
	{
		delete this;
	}

}
