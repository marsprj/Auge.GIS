#include "KmeanProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	KMeansProcessorImpl::KMeansProcessorImpl()
	{
		m_k = 0;
		m_user = 0;
	}

	KMeansProcessorImpl::~KMeansProcessorImpl()
	{

	}

	void KMeansProcessorImpl::SetK(g_uint k)
	{
		m_k = k;
	}

	void KMeansProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void KMeansProcessorImpl::SetInputFeatureClass(const char* className)
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

	void KMeansProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void KMeansProcessorImpl::SetOutputFeatureClass(const char* className)
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

	const char* KMeansProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* KMeansProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	KMeansProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	KMeansProcessorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE KMeansProcessorImpl::Execute()
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
		
		FeatureClass		*pinFeatureClass = NULL;
		//FeatureClass		*poutFeatureClass= NULL;
		FeatureWorksapce	*pinWorkspace = NULL;
		FeatureWorksapce	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();
		
		pinWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(m_user, sourceName_in));
		if(pinWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->NewWorkspace(m_user, sourceName_out));
		if(poutWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		pinFeatureClass = pinWorkspace->OpenFeatureClass(className_in);
		if(pinFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		g_uint srid = pinFeatureClass->GetSRID();
		
		GField* pField = pinFeatureClass->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		if(pField->GetGeometryDef()->GeometryType()!=augeGTPoint)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		
		//// input points
		//poutFeatureClass = poutWorkspace->OpenFeatureClass(className_out);
		//if(poutFeatureClass!=NULL)
		//{
		//	pinFeatureClass->Release();
		//	poutWorkspace->Release();
		//	return AG_FAILURE;
		//}
		
		Geometry *pGeometry = NULL;
		Feature	 *pFeature = NULL;
		FeatureCursor* pCursor = NULL;		
		g_uint feature_count = pinFeatureClass->GetCount();

		pCursor = pinFeatureClass->Query();
		if(pCursor==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
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
		pinFeatureClass->Release();
		
		// processing cluster 
		if(!m_kmean.Execute())
		{	
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		
		RESULTCODE rc = SaveCentroidResult(&m_kmean, className_out, poutWorkspace, srid);
		if(rc!=AG_SUCCESS)
		{
			poutWorkspace->Release();
			return rc;
		}

		char cluster_name[AUGE_NAME_MAX];
		memset(cluster_name, 0, AUGE_NAME_MAX);
		g_sprintf(cluster_name, "%s_cluster", className_out);
		rc = SaveClusterResult(&m_kmean, cluster_name, poutWorkspace, srid);
		poutWorkspace->Release();

		return rc;
	}

	RESULTCODE KMeansProcessorImpl::SaveCentroidResult(KMeans* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid)
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

	RESULTCODE KMeansProcessorImpl::SaveClusterResult(KMeans* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid)
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

	void KMeansProcessorImpl::Release()
	{
		delete this;
	}

	void KMeansProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
