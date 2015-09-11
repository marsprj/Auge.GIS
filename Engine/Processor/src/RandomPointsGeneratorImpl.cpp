#include "RandomPointsGeneratorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	RandomPointsGeneratorImpl::RandomPointsGeneratorImpl()
	{
		m_user = 0;
		m_extent.Set(-180.0,-90.0,180.0,90.0);
		m_srid = AUGE_DEFAULT_SRID;
		m_count = 10;
		m_geom_field = "shape";
	}

	RandomPointsGeneratorImpl::~RandomPointsGeneratorImpl()
	{

	}
	
	void RandomPointsGeneratorImpl::SetOutputDataSource(const char* sourceName)
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

	void RandomPointsGeneratorImpl::SetOutputFeatureClass(const char* className)
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

	const char*	RandomPointsGeneratorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RandomPointsGeneratorImpl::GetOutputFatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}

	void RandomPointsGeneratorImpl::SetSRID(g_uint srid)
	{
		m_srid = srid;
	}

	g_uint RandomPointsGeneratorImpl::GetSRID()
	{
		return m_srid;
	}

	void RandomPointsGeneratorImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	GEnvelope& RandomPointsGeneratorImpl::GetExtent()
	{
		return m_extent;
	}

	void RandomPointsGeneratorImpl::SetCount(g_uint count)
	{
		m_count = count;
	}

	g_uint RandomPointsGeneratorImpl::GetCount()
	{
		return m_count;
	}

	RESULTCODE RandomPointsGeneratorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_out= GetOutputSource();
		const char* className_out = GetOutputFatureClass();
				
		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}

		if(className_out==NULL)
		{
			return AG_FAILURE;
		}
		if(!m_extent.IsValid())
		{
			return AG_FAILURE;
		}
		
		FeatureClass		*pinFeatureClass = NULL;
		FeatureClass		*poutFeatureClass= NULL;
		FeatureWorkspace	*pinWorkspace = NULL;
		FeatureWorkspace	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();
		
		poutWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->NewWorkspace(m_user, sourceName_out));
		if(poutWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutFeatureClass = CreateOutputFeatureClass(className_out, poutWorkspace);
		if(poutFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = Process(poutFeatureClass);
		poutFeatureClass->Refresh();
		poutFeatureClass->Release();

		if(rc!=AG_SUCCESS)
		{
			poutWorkspace->RemoveFeatureClass(className_out);
		}

		poutWorkspace->Release();
		
		return AG_SUCCESS;
	}

	void RandomPointsGeneratorImpl::Release()
	{
		delete this;
	}

	FeatureClass* RandomPointsGeneratorImpl::CreateOutputFeatureClass(const char* className, FeatureWorkspace* poutWorkspace)
	{
		FeatureClass* poutFatureClass = NULL;
		poutFatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFatureClass!=NULL)
		{
			return NULL;
		}

		GField	 *poutField = NULL;
		GField_2 *poutField_2 = NULL;
		GeometryDef		*poutGeometryDef = NULL;
		GeometryDef_2	*poutGeometryDef_2=NULL;

		GFields* poutFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		poutFields = pFieldFactory->CreateFields();

		poutField = pFieldFactory->CreateField();
		poutField_2 = poutField->Field_2();
		poutField_2->SetName(m_geom_field.c_str());
		poutField_2->SetType(augeFieldTypeGeometry);

		poutGeometryDef = poutField->GetGeometryDef();
		poutGeometryDef_2 = poutGeometryDef->GetGeometryDef_2();
		poutGeometryDef_2->SetGeometryType(augeGTPoint);
		poutGeometryDef_2->SetDimension(2);	
		poutGeometryDef_2->SetSRID(m_srid);		
		poutGeometryDef_2->SetExtent(m_extent);

		poutFields->Add(poutField);

		FeatureClass* poutFeatureClass = poutWorkspace->CreateFeatureClass(className, poutFields);
		poutFields->Release();

		return poutFeatureClass;
	}

	RESULTCODE RandomPointsGeneratorImpl::Process(FeatureClass* poutFeatureClass)
	{
		WKBPoint wkbpt;
		wkbpt.byteOrder = coDefaultByteOrder;
		wkbpt.wkbType = wkbPoint;

		Geometry *pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		GValue	*pValue = NULL;
		Feature	*pFeature = NULL;
		FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();
		pFeature = poutFeatureClass->NewFeature();

		double width  = m_extent.GetWidth();
		double height = m_extent.GetHeight();
		double xmin = m_extent.m_xmin;
		double ymin = m_extent.m_ymin;

		for(g_uint i=0; i<m_count; i++)
		{
			wkbpt.point.x = xmin + auge_get_random_value(width);
			wkbpt.point.y = ymin + auge_get_random_value(height);

			pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&wkbpt), true);
			pValue = new GValue(pGeometry);

			pFeature->SetValue(m_geom_field.c_str(), pValue);

			cmd->Insert(pFeature);
		}
		pFeature->Release();

		return AG_SUCCESS;
	}

	void RandomPointsGeneratorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
