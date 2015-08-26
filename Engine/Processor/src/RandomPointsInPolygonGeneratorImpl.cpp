#include "RandomPointsInPolygonGeneratorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	RandomPointsInPolygonGeneratorImpl::RandomPointsInPolygonGeneratorImpl()
	{
		m_user = 0;
		m_extent.Set(-180.0,-90.0,180.0,90.0);
		m_srid = AUGE_DEFAULT_SRID;
		m_count = 10;
		m_geom_field = "shape";
	}

	RandomPointsInPolygonGeneratorImpl::~RandomPointsInPolygonGeneratorImpl()
	{

	}

	void RandomPointsInPolygonGeneratorImpl::SetInputDataSource(const char* sourceName)
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

	void RandomPointsInPolygonGeneratorImpl::SetInputFeatureClass(const char* className)
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
	
	void RandomPointsInPolygonGeneratorImpl::SetOutputDataSource(const char* sourceName)
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

	void RandomPointsInPolygonGeneratorImpl::SetOutputFeatureClass(const char* className)
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

	const char* RandomPointsInPolygonGeneratorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* RandomPointsInPolygonGeneratorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	RandomPointsInPolygonGeneratorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RandomPointsInPolygonGeneratorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}
	
	void RandomPointsInPolygonGeneratorImpl::SetCount(g_uint count)
	{
		m_count = count;
	}

	g_uint RandomPointsInPolygonGeneratorImpl::GetCount()
	{
		return m_count;
	}

	RESULTCODE RandomPointsInPolygonGeneratorImpl::Execute()
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
		if(!m_extent.IsValid())
		{
			return AG_FAILURE;
		}
		if(m_count==0)
		{
			return AG_FAILURE;
		}
		
		FeatureClass		*pinFeatureClass = NULL;
		FeatureClass		*poutFeatureClass= NULL;
		FeatureWorkspace	*pinWorkspace = NULL;
		FeatureWorkspace	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();

		pinWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspace(m_user, sourceName_in));
		if(pinWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->NewWorkspace(m_user, sourceName_out));
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

		GField* pField = pinFeatureClass->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		augeGeometryType geoType = pField->GetGeometryDef()->GeometryType();
		if(geoType!=augeGTMultiPolygon && geoType!=augeGTPolygon)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		poutFeatureClass = CreateOutputFeatureClass(className_out, poutWorkspace, pinFeatureClass);
		if(poutFeatureClass==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = Process(poutFeatureClass, pinFeatureClass);
		poutFeatureClass->Refresh();
		poutFeatureClass->Release();

		if(rc!=AG_SUCCESS)
		{
			poutWorkspace->RemoveFeatureClass(className_out);
		}

		pinFeatureClass->Release();
		poutWorkspace->Release();
		
		return AG_SUCCESS;
	}

	void RandomPointsInPolygonGeneratorImpl::Release()
	{
		delete this;
	}

	FeatureClass* RandomPointsInPolygonGeneratorImpl::CreateOutputFeatureClass(const char* className, FeatureWorkspace* poutWorkspace, FeatureClass* pinFeatureClass)
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
		poutField_2->SetName("id");
		poutField_2->SetType(augeFieldTypeInt);
		poutFields->Add(poutField);

		poutField = pFieldFactory->CreateField();
		poutField_2 = poutField->Field_2();
		poutField_2->SetName(m_geom_field.c_str());
		poutField_2->SetType(augeFieldTypeGeometry);

		poutGeometryDef = poutField->GetGeometryDef();
		poutGeometryDef_2 = poutGeometryDef->GetGeometryDef_2();
		poutGeometryDef_2->SetGeometryType(augeGTMultiPoint);
		poutGeometryDef_2->SetDimension(2);	
		poutGeometryDef_2->SetSRID(m_srid);		
		poutGeometryDef_2->SetExtent(m_extent);

		poutFields->Add(poutField);

		FeatureClass* poutFeatureClass = poutWorkspace->CreateFeatureClass(className, poutFields);
		poutFields->Release();

		return poutFeatureClass;
	}

	RESULTCODE RandomPointsInPolygonGeneratorImpl::Process(FeatureClass* poutFeatureClass, FeatureClass* pinFeatureClass)
	{
		Geometry *pGeometry = NULL;
		Feature	 *pFeature = NULL;
		FeatureCursor* pCursor = pinFeatureClass->Query();

		FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();

		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if( pGeometry != NULL )
			{
				GenerateRandomPoints(m_count, pGeometry, poutFeatureClass, cmd);
			}

			pFeature->Release();
		}

		pCursor->Release();

		return AG_SUCCESS;
	}

	void RandomPointsInPolygonGeneratorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	void RandomPointsInPolygonGeneratorImpl::GenerateRandomPoints(g_uint count, Geometry* pGeometry, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
	{
		GEnvelope* extent = (GEnvelope*)pGeometry->Envelope();
		double x, y;
		double xmin, ymin;
		double width,height;
		augeGeometryType type = pGeometry->GeometryType();
		xmin = extent->m_xmin;
		ymin = extent->m_ymin;
		width = extent->GetWidth();
		height= extent->GetHeight();

		WKBPoint* pWKBPoint = NULL;
		WKBMultiPoint* pWKBMultiPoint = NULL;
		size_t wkb_size = sizeof(WKBMultiPoint) + (count-1) * sizeof(WKBPoint);
		pWKBMultiPoint = (WKBMultiPoint*)malloc(wkb_size);
		pWKBMultiPoint->byteOrder = coDefaultByteOrder;
		pWKBMultiPoint->wkbType = wkbMultiPoint;
		pWKBMultiPoint->numPoints = count;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));

		for(g_uint i=0; i<count; )
		{
			x = xmin + auge_get_random_value(width);
			y = ymin + auge_get_random_value(height);
			switch(type)
			{
			case augeGTPolygon:
				{
					GPolygon* pPolygon = (GPolygon*)pGeometry;
					if(pPolygon->Contain(x, y))
					{
						pWKBPoint->byteOrder = coDefaultByteOrder;
						pWKBPoint->wkbType = wkbPoint;
						pWKBPoint->point.x = x;
						pWKBPoint->point.y = y;
						i++, pWKBPoint++;
					}
				}
				break;
			case augeGTMultiPolygon:
				{
					GMultiPolygon* pPolygon = (GMultiPolygon*)pGeometry;
					if(pPolygon->Contain(x, y))
					{
						pWKBPoint->byteOrder = coDefaultByteOrder;
						pWKBPoint->wkbType = wkbPoint;
						pWKBPoint->point.x = x;
						pWKBPoint->point.y = y;

						i++, pWKBPoint++;
					}
				}
				break;
			}
		}

		Geometry* pPoints = Execute(pGeometry, m_count);
		if(pPoints!=NULL)
		{
			GValue* pValue = NULL;
			Feature* pFeature = poutFeatureClass->NewFeature();
			pValue = new GValue(pFeature->GetFID());
			pFeature->SetValue("id", pValue);
			pValue = new GValue(pPoints);
			pFeature->SetValue(m_geom_field.c_str(), pValue);
			cmd->Insert(pFeature);

			pFeature->Release();
		}

		free(pWKBMultiPoint);
	}

	GMultiPoint* RandomPointsInPolygonGeneratorImpl::Execute(Geometry* pGeometry, g_uint count)
	{
		GEnvelope* extent = (GEnvelope*)pGeometry->Envelope();
		double x, y;
		double xmin, ymin;
		double width,height;
		augeGeometryType type = pGeometry->GeometryType();
		xmin = extent->m_xmin;
		ymin = extent->m_ymin;
		width = extent->GetWidth();
		height= extent->GetHeight();

		WKBPoint* pWKBPoint = NULL;
		WKBMultiPoint* pWKBMultiPoint = NULL;
		size_t wkb_size = sizeof(WKBMultiPoint) + (count-1) * sizeof(WKBPoint);
		pWKBMultiPoint = (WKBMultiPoint*)malloc(wkb_size);
		pWKBMultiPoint->byteOrder = coDefaultByteOrder;
		pWKBMultiPoint->wkbType = wkbMultiPoint;
		pWKBMultiPoint->numPoints = count;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));

		for(g_uint i=0; i<count; )
		{
			x = xmin + auge_get_random_value(width);
			y = ymin + auge_get_random_value(height);
			switch(type)
			{
			case augeGTPolygon:
				{
					GPolygon* pPolygon = (GPolygon*)pGeometry;
					if(pPolygon->Contain(x, y))
					{
						pWKBPoint->byteOrder = coDefaultByteOrder;
						pWKBPoint->wkbType = wkbPoint;
						pWKBPoint->point.x = x;
						pWKBPoint->point.y = y;
						i++, pWKBPoint++;
					}
				}
				break;
			case augeGTMultiPolygon:
				{
					GMultiPolygon* pPolygon = (GMultiPolygon*)pGeometry;
					if(pPolygon->Contain(x, y))
					{
						pWKBPoint->byteOrder = coDefaultByteOrder;
						pWKBPoint->wkbType = wkbPoint;
						pWKBPoint->point.x = x;
						pWKBPoint->point.y = y;

						i++, pWKBPoint++;
					}
				}
				break;
			}
		}

		GMultiPoint* pPoints = (GMultiPoint*)(augeGetGeometryFactoryInstance()->CreateGeometryFromWKB((g_uchar*)pWKBMultiPoint, false));
		free(pWKBMultiPoint);

		return pPoints;
	}
}
