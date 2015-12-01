#include "PolygonToPointsProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	PolygonToPointsProcessorImpl::PolygonToPointsProcessorImpl()
	{
		m_user = 0;
	}

	PolygonToPointsProcessorImpl::~PolygonToPointsProcessorImpl()
	{

	}

	void PolygonToPointsProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void PolygonToPointsProcessorImpl::SetInputFeatureClass(const char* className)
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

	void PolygonToPointsProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void PolygonToPointsProcessorImpl::SetOutputFeatureClass(const char* className)
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

	const char* PolygonToPointsProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* PolygonToPointsProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	PolygonToPointsProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	PolygonToPointsProcessorImpl::GetOutputFatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE PolygonToPointsProcessorImpl::Execute()
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
		augeGeometryType type = pField->GetGeometryDef()->GeometryType();
		if(type!=augeGTPolygon&&type!=augeGTMultiPolygon)
		{
			const char* msg = "输入数据集几何类型必须是[面]类型";
			pError->SetError(msg);
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
		rc = Process(pinFeatureClass, poutFeatureClass);
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

	void PolygonToPointsProcessorImpl::Release()
	{
		delete this;
	}

	FeatureClass* PolygonToPointsProcessorImpl::CreateOutputFeatureClass(const char* className, FeatureWorkspace* poutWorkspace, FeatureClass* pinFeatureClass)
	{
		FeatureClass* poutFatureClass = NULL;
		poutFatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFatureClass!=NULL)
		{
			GError* pError = augeGetErrorInstance();
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_sprintf(msg, "outputType [%s] already exists", className);
			pError->SetError(msg);
			return NULL;
		}

		GField * pField  = NULL;
		GFields* pinFields = pinFeatureClass->GetFields();
		GeometryDef		*pGeometryDef = NULL;

		pField = pinFields->GetGeometryField();
		if(pField==NULL)
		{
			return NULL;
		}

		GField	 *poutField = NULL;
		GField_2 *poutField_2 = NULL;
		GeometryDef		*poutGeometryDef = NULL;
		GeometryDef_2	*poutGeometryDef_2=NULL;
		GEnvelope		extent;

		GFields* poutFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		poutFields = pFieldFactory->CreateFields();

		g_uint count = pinFields->Count();
		for(g_uint i=0; i<count; i++)
		{
			pField = pinFields->GetField(i);
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				pGeometryDef = pField->GetGeometryDef();

				poutField = pFieldFactory->CreateField();
				poutField_2 = poutField->Field_2();
				poutField_2->SetName(pField->GetName());
				poutField_2->SetType(augeFieldTypeGeometry);

				poutGeometryDef = poutField->GetGeometryDef();
				poutGeometryDef_2 = poutGeometryDef->GetGeometryDef_2();
				poutGeometryDef_2->SetGeometryType(augeGTPoint);
				poutGeometryDef_2->SetDimension(pGeometryDef->GetDimension());	
				poutGeometryDef_2->SetSRID(pGeometryDef->GetSRID());
				pGeometryDef->GetExtent(extent);
				poutGeometryDef_2->SetExtent(extent);

				poutFields->Add(poutField);
			}
			else
			{
				poutFields->Add(pField);
				pField->AddRef();
			}
		}

		FeatureClass* poutFeatureClass = poutWorkspace->CreateFeatureClass(className, poutFields);
		poutFields->Release();

		return poutFeatureClass;
	}

	RESULTCODE PolygonToPointsProcessorImpl::Process(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass)
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
				switch(pGeometry->GeometryType())
				{
				case augeGTPolygon:
					ProcessPolygon(pFeature, poutFeatureClass, cmd);
					break;
				case augeGTMultiPolygon:
					ProcessMultiPolygon(pFeature, poutFeatureClass, cmd);
					break;
				}
			}

			pFeature->Release();
		}
		pCursor->Release();

		cmd->Commit();
		cmd->Release();

		return AG_SUCCESS;
	}

	void PolygonToPointsProcessorImpl::ProcessPolygon(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
	{
		Geometry* pGeometry = NULL;
		pGeometry = pinFeature->GetGeometry();
		if(pGeometry==NULL)
		{
			return;
		}

		const char* fname = NULL;
		GField*  pField = NULL;
		GFields* pFields = pinFeature->GetFeatureClass()->GetFields();
		g_uint count = pFields->Count();

		GValue* pValue = NULL;
		GValue* pGeoValue = NULL;		
		Feature* poutFeature = poutFeatureClass->NewFeature();

		augeFieldType type = augeFieldTypeNone;
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			type = pField->GetType();
			if(type==augeFieldTypeGeometry)
			{
				continue;
			}

			fname = pField->GetName();
			pValue = pinFeature->GetValue(i);			
			poutFeature->SetValue(fname, pValue);
		}

		Geometry* pGeoPoint = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		pField = pinFeature->GetFeatureClass()->GetFields()->GetGeometryField();
		if(pField!=NULL)
		{
			pValue = pinFeature->GetValue(pField->GetName());
			if(pValue!=NULL)
			{
				pGeometry = pValue->GetGeometry();
				if(pGeometry!=NULL)
				{
					g_uint numPoints = 0;
					g_uint numRings = 0;
					WKBPolygon* pWKBPolygon = (WKBPolygon*)pGeometry->AsBinary();
					LinearRing* pLinearRing = NULL;
					auge::Point* pt = NULL;
					WKBPoint wkbpt;
					wkbpt.byteOrder = coDefaultByteOrder;
					wkbpt.wkbType = wkbPoint;

					numRings = pWKBPolygon->numRings;
					pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
					for(g_uint i=0; i<numRings; i++)
					{
						numPoints = pLinearRing->numPoints;
						pt = (auge::Point*)(&(pLinearRing->points[0]));
						for(g_uint j=0; j<numPoints; j++,pt++)
						{
							wkbpt.point.x = pt->x;
							wkbpt.point.y = pt->y;
							pGeoPoint = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&wkbpt), true);
							pGeoValue = new GValue(pGeoPoint);
							poutFeature->SetValue(pField->GetName(), pGeoValue);

							cmd->Insert(poutFeature);
						}
						pLinearRing = (LinearRing*)pt;
					}
					pWKBPolygon = (WKBPolygon*)pt;
				}
			}
		}
		poutFeature->Release();
	}

	void PolygonToPointsProcessorImpl::ProcessMultiPolygon(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
	{
		Geometry* pGeometry = NULL;
		pGeometry = pinFeature->GetGeometry();
		if(pGeometry==NULL)
		{
			return;
		}

		const char* fname = NULL;
		GField*  pField = NULL;
		GFields* pFields = pinFeature->GetFeatureClass()->GetFields();
		g_uint count = pFields->Count();

		GValue* pValue = NULL;
		GValue* pGeoValue = NULL;		
		Feature* poutFeature = poutFeatureClass->NewFeature();

		augeFieldType type = augeFieldTypeNone;
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			type = pField->GetType();
			if(type==augeFieldTypeGeometry)
			{
				continue;
			}

			fname = pField->GetName();
			pValue = pinFeature->GetValue(i);			
			poutFeature->SetValue(fname, pValue);
		}

		Geometry* pGeoPoint = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		pField = pinFeature->GetFeatureClass()->GetFields()->GetGeometryField();
		if(pField!=NULL)
		{
			pValue = pinFeature->GetValue(pField->GetName());
			if(pValue!=NULL)
			{
				pGeometry = pValue->GetGeometry();
				if(pGeometry!=NULL)
				{
					g_uint numPoints = 0;
					g_uint numRings = 0;
					g_uint numPolygons = 0;
					LinearRing* pRing = NULL;
					WKBPolygon* pWKBPolygon = NULL;
					WKBMultiPolygon* pWKBMultiPolygon = (WKBMultiPolygon*)pGeometry->AsBinary();
					auge::Point* pt = NULL;
					WKBPoint wkbpt;
					wkbpt.byteOrder = coDefaultByteOrder;
					wkbpt.wkbType = wkbPoint;

					numPolygons = pWKBMultiPolygon->numPolygons;
					pWKBPolygon = (WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));
					for(g_uint i=0; i<numPolygons; i++)
					{
						numRings = pWKBPolygon->numRings;
						pRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
						for(g_uint j=0; j<numRings; j++)
						{
							pt = (auge::Point*)(&(pRing->points[0]));
							numPoints = pRing->numPoints;
							for(g_uint j=0; j<numPoints; j++,pt++)
							{
								wkbpt.point.x = pt->x;
								wkbpt.point.y = pt->y;
								pGeoPoint = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&wkbpt), true);
								pGeoValue = new GValue(pGeoPoint);
								poutFeature->SetValue(pField->GetName(), pGeoValue);

								cmd->Insert(poutFeature);
							}
							pRing = (LinearRing*)(pt);
						}
						pWKBPolygon = (WKBPolygon*)pt;
					}
				}
			}
		}
		poutFeature->Release();
	}


	void PolygonToPointsProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
