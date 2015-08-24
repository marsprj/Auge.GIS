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

		poutFeatureClass = CreateOutputFeatureClass(className_out, poutWorkspace, pinFeatureClass);
		if(poutFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = LineToPoints(pinFeatureClass, poutFeatureClass);
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
				poutGeometryDef_2->SetGeometryType(augeGTMultiPoint);
				poutGeometryDef_2->SetDimension(pGeometryDef->GetDimension());				
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

	RESULTCODE LineToPointsProcessorImpl::LineToPoints(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass)
	{
		//g_uint i_srid = pinFeatureClass->GetSRID();
		//g_uint o_srid = poutFeatureClass->GetSRID();

		//m_srsbase.Open();

		//char i_pj_str[AUGE_MSG_MAX];
		//memset(i_pj_str, 0, AUGE_MSG_MAX);
		//m_srsbase.GetProj4Text(i_srid, i_pj_str, AUGE_MSG_MAX);

		//char o_pj_str[AUGE_MSG_MAX];
		//memset(o_pj_str, 0, AUGE_MSG_MAX);
		//m_srsbase.GetProj4Text(o_srid, o_pj_str, AUGE_MSG_MAX);

		////const char* i_pj_str = "+proj=longlat +datum=WGS84 +no_defs";
		////const char* o_pj_str = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs";

		//m_srsbase.Close();

		//projPJ i_pj = pj_init_plus(i_pj_str);
		//projPJ o_pj = pj_init_plus(o_pj_str);

		//if(i_pj==NULL)
		//{
		//	return AG_FAILURE;
		//}
		//if(o_pj==NULL)
		//{
		//	return AG_FAILURE;
		//}
		//
		//g_uchar	 *wkb = NULL;
		//Geometry *pGeometry = NULL;
		//Feature	 *pFeature = NULL;
		//FeatureCursor* pCursor = pinFeatureClass->Query();

		//FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();
		//
		//while((pFeature=pCursor->NextFeature())!=NULL)
		//{
		//	pGeometry = pFeature->GetGeometry();
		//	if( pGeometry != NULL )
		//	{
		//		wkb = pGeometry->AsBinary();

		//		switch(pGeometry->GeometryType())
		//		{
		//		case augeGTPoint:
		//			Project((WKBPoint*)wkb, i_pj, o_pj);
		//			break;
		//		case augeGTMultiPoint:
		//			Project((WKBMultiPoint*)wkb, i_pj, o_pj);
		//			break;
		//		case augeGTLineString:
		//			Project((WKBLineString*)wkb, i_pj, o_pj);
		//			break;
		//		case augeGTMultiLineString:
		//			Project((WKBMultiLineString*)wkb, i_pj, o_pj);
		//			break;
		//		case augeGTPolygon:
		//			Project((WKBPolygon*)wkb, i_pj, o_pj);
		//			break;
		//		case augeGTMultiPolygon:
		//			Project((WKBMultiPolygon*)wkb, i_pj, o_pj);
		//			break;
		//		}
		//	}

		//	cmd->Insert(pFeature);

		//	pFeature->Release();
		//}

		//pCursor->Release();

		return AG_SUCCESS;
	}

	void LineToPointsProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
