#include "PolygonToLineProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	PolygonToLineProcessorImpl::PolygonToLineProcessorImpl()
	{
		m_user = 0;
	}

	PolygonToLineProcessorImpl::~PolygonToLineProcessorImpl()
	{

	}

	void PolygonToLineProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void PolygonToLineProcessorImpl::SetInputFeatureClass(const char* className)
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

	void PolygonToLineProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void PolygonToLineProcessorImpl::SetOutputFeatureClass(const char* className)
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

	const char* PolygonToLineProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* PolygonToLineProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	PolygonToLineProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	PolygonToLineProcessorImpl::GetOutputFatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE PolygonToLineProcessorImpl::Execute()
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

	void PolygonToLineProcessorImpl::Release()
	{
		delete this;
	}

	FeatureClass* PolygonToLineProcessorImpl::CreateOutputFeatureClass(const char* className, FeatureWorkspace* poutWorkspace, FeatureClass* pinFeatureClass)
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
				poutGeometryDef_2->SetGeometryType(augeGTLineString);
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

	RESULTCODE PolygonToLineProcessorImpl::Process(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass)
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

	void PolygonToLineProcessorImpl::ProcessPolygon(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
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

		Geometry* pGeoLine = NULL;
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
					g_uint ring_size = 0;
					g_uint line_size = 0;
					WKBPolygon* pWKBPolygon = (WKBPolygon*)pGeometry->AsBinary();
					LinearRing* pLinearRing = NULL;
					WKBLineString* pWKBLineString = NULL;
					g_byte* ptr = NULL;

					numRings = pWKBPolygon->numRings;
					pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
					for(g_uint i=0; i<numRings; i++)
					{
						numPoints = pLinearRing->numPoints;
						ring_size = sizeof(auge::Point) * numPoints + sizeof(g_int32);
						line_size = ring_size * numPoints + sizeof(g_int32) + sizeof(g_byte);
						
						pWKBLineString = (WKBLineString*)malloc(line_size);
						memset(pWKBLineString, 0, line_size);

						pWKBLineString->byteOrder = coDefaultByteOrder;
						pWKBLineString->wkbType = wkbLineString;
						pWKBLineString->numPoints = numPoints;
						memcpy(&(pWKBLineString->points[0]), &(pLinearRing->points[0]), sizeof(auge::Point) * numPoints);

						pGeoLine = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(pWKBLineString), true);
						pGeoValue = new GValue(pGeoLine);
						poutFeature->SetValue(pField->GetName(), pGeoValue);

						cmd->Insert(poutFeature);

						free(pWKBLineString);

						ptr = (g_byte*)pLinearRing;
						pLinearRing = (LinearRing*)(ptr + ring_size);
					}
				}
			}
		}
		poutFeature->Release();
	}

	void PolygonToLineProcessorImpl::ProcessMultiPolygon(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd)
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

		Geometry* pGeoLine = NULL;
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
					g_uint ring_size = 0;
					g_uint line_size = 0;
					LinearRing* pLinearRing = NULL;
					WKBPolygon* pWKBPolygon = NULL;
					WKBMultiPolygon* pWKBMultiPolygon = (WKBMultiPolygon*)pGeometry->AsBinary();
					WKBLineString* pWKBLineString = NULL;
					g_byte* ptr = NULL;

					numPolygons = pWKBMultiPolygon->numPolygons;
					pWKBPolygon = (WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));
					for(g_uint i=0; i<numPolygons; i++)
					{
						numRings = pWKBPolygon->numRings;
						pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
						for(g_uint j=0; j<numRings; j++)
						{
							numPoints = pLinearRing->numPoints;
							ring_size = sizeof(auge::Point) * numPoints + sizeof(g_int32);
							line_size = ring_size + sizeof(g_int32) + sizeof(g_byte);

							pWKBLineString = (WKBLineString*)malloc(line_size);
							memset(pWKBLineString, 0, line_size);

							pWKBLineString->byteOrder = coDefaultByteOrder;
							pWKBLineString->wkbType = wkbLineString;
							pWKBLineString->numPoints = numPoints;
							memcpy(&(pWKBLineString->points[0]), &(pLinearRing->points[0]), sizeof(auge::Point) * numPoints);

							pGeoLine = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(pWKBLineString), true);
							pGeoValue = new GValue(pGeoLine);
							poutFeature->SetValue(pField->GetName(), pGeoValue);

							cmd->Insert(poutFeature);

							free(pWKBLineString);

							ptr = (g_byte*)pLinearRing;
							pLinearRing = (LinearRing*)(ptr + ring_size);
						}
						pWKBPolygon = (WKBPolygon*)pLinearRing;
					}
				}
			}
		}
		poutFeature->Release();
	}


	void PolygonToLineProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
