#include "LineToPointsProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	LineToPointsProcessorImpl::LineToPointsProcessorImpl()
	{
		m_user = 0;
	}

	LineToPointsProcessorImpl::~LineToPointsProcessorImpl()
	{

	}

	void LineToPointsProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void LineToPointsProcessorImpl::SetInputFeatureClass(const char* className)
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

	void LineToPointsProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void LineToPointsProcessorImpl::SetOutputFeatureClass(const char* className)
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

	const char* LineToPointsProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* LineToPointsProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	LineToPointsProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	LineToPointsProcessorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE LineToPointsProcessorImpl::Execute()
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
		if(type!=augeGTLineString&&type!=augeGTMultiLineString)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}


		poutFeatureClass = CreateOutputFeatureClass(className_out, poutWorkspace, pinFeatureClass);
		if(poutFeatureClass==NULL)
		{
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

	void LineToPointsProcessorImpl::Release()
	{
		delete this;
	}

	FeatureClass* LineToPointsProcessorImpl::CreateOutputFeatureClass(const char* className, FeatureWorkspace* poutWorkspace, FeatureClass* pinFeatureClass)
	{
		FeatureClass* poutFatureClass = NULL;
		poutFatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFatureClass!=NULL)
		{
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

	RESULTCODE LineToPointsProcessorImpl::Process(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass)
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
				case augeGTLineString:
					ProcessLineString(pFeature, poutFeatureClass, cmd);
					break;
				case augeGTMultiLineString:
					ProcessMultiLineString(pFeature, poutFeatureClass, cmd);
					break;
				}
			}

			pFeature->Release();
		}
		pCursor->Release();

		return AG_SUCCESS;
	}

	void LineToPointsProcessorImpl::ProcessLineString(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
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
					g_uint numPoints = NULL;					
					WKBLineString* pWKBLineString = (WKBLineString*)pGeometry->AsBinary();
					WKBPoint* pt = NULL;					

					numPoints = pWKBLineString->numPoints;
					pt = (WKBPoint*)(&(pWKBLineString->points[0]));
					for(g_uint i=0; i<numPoints; i++,pt++)
					{
						pGeoPoint = pGeometryFactory->CreateGeometryFromWKB((g_byte*)pt, true);
						pGeoValue = new GValue(pGeoPoint);
						poutFeature->SetValue(pField->GetName(), pGeoValue);

						cmd->Insert(poutFeature);
					}
				}
			}
		}
		poutFeature->Release();
	}

	void LineToPointsProcessorImpl::ProcessMultiLineString(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
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
					g_uint numPoints = NULL;
					g_uint numLineStrings = NULL;
					WKBLineString* pWKBLineString = NULL;
					WKBMultiLineString* pWKBMultiLineString = (WKBMultiLineString*)pGeometry->AsBinary();
					auge::Point* pt = NULL;
					WKBPoint wkbpt;
					wkbpt.byteOrder = coDefaultByteOrder;
					wkbpt.wkbType = wkbPoint;

					numLineStrings = pWKBMultiLineString->numLineStrings;
					pWKBLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));
					for(g_uint i=0; i<numLineStrings; i++)
					{
						pt = (auge::Point*)(&(pWKBLineString->points[0]));
						numPoints = pWKBLineString->numPoints;
						for(g_uint j=0; j<numPoints; j++,pt++)
						{
							wkbpt.point.x = pt->x;
							wkbpt.point.y = pt->y;
							pGeoPoint = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&wkbpt), true);
							pGeoValue = new GValue(pGeoPoint);
							poutFeature->SetValue(pField->GetName(), pGeoValue);

							cmd->Insert(poutFeature);
						}
						pWKBLineString = (WKBLineString*)(pt);
					}
				}
			}
		}
		poutFeature->Release();
	}


	void LineToPointsProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
