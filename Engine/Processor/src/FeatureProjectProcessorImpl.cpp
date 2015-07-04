#include "FeatureProjectProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	FeatureProjectProcessorImpl::FeatureProjectProcessorImpl()
	{

	}

	FeatureProjectProcessorImpl::~FeatureProjectProcessorImpl()
	{

	}

	void FeatureProjectProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void FeatureProjectProcessorImpl::SetInputFeatureClass(const char* className)
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

	void FeatureProjectProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void FeatureProjectProcessorImpl::SetOutputSRID(g_uint srid)
	{
		m_srid = srid;
	}

	void FeatureProjectProcessorImpl::SetOutputFeatureClass(const char* className)
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

	const char* FeatureProjectProcessorImpl::GetInputSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* FeatureProjectProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	FeatureProjectProcessorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	FeatureProjectProcessorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}

	RESULTCODE FeatureProjectProcessorImpl::Execute()
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
		FeatureWorksapce	*pinWorkspace = NULL;
		FeatureWorksapce	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();
		
		pinWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(sourceName_in));
		if(pinWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(sourceName_out));
		if(poutWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		pinFeatureClass = pinWorkspace->OpenFeatureClass(className_in);
		if(pinFeatureClass==NULL)
		{
			return AG_FAILURE;
		}

		g_uint srid_in  = pinFeatureClass->GetSRID();
		g_uint srid_out = m_srid;

		poutFeatureClass = CreateOutputFeatureClass(className_out, srid_out, poutWorkspace, pinFeatureClass);
		if(poutFeatureClass==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = Project(poutFeatureClass, poutFeatureClass);
		
		

		pinFeatureClass->Release();
		poutFeatureClass->Release();
		
		return AG_SUCCESS;
	}

	void FeatureProjectProcessorImpl::Release()
	{
		delete this;
	}

	FeatureClass* FeatureProjectProcessorImpl::CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorksapce* poutWorkspace, FeatureClass* pinFeatureClass)
	{
		FeatureClass* poutFatureClass = NULL;
		poutFatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFatureClass!=NULL)
		{
			return NULL;
		}

		GField * pField  = NULL;
		GFields* pFields = pFields = pinFeatureClass->GetFields();
		pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			return NULL;
		}
		GeometryDef		*pGeometryDef = pField->GetGeometryDef();
		GeometryDef_2	*pGeometryDef_2=pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetSRID(srid);

		return poutWorkspace->CreateFeatureClass(className, pFields);
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass)
	{
		g_uint i_srid = pinFeatureClass->GetSRID();
		g_uint o_srid = poutFeatureClass->GetSRID();
		
		Geometry *pGeometry = NULL;
		Feature	 *pFeature = NULL;
		FeatureCursor* pCursor = pinFeatureClass->Query();
		
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if( pGeometry != NULL )
			{
				switch(pGeometry->GeometryType())
				{
				case augeGTPoint:					
					break;
				case augeGTMultiPoint:					
					break;
				case augeGTLineString:
					break;
				case augeGTMultiLineString:
					break;
				case augeGTPolygon:
					break;
				case augeGTMultiPolygon:
					break;
				}
			}

			pFeature->Release();
		}

		pCursor->Release();

		return AG_SUCCESS;
	}
}
