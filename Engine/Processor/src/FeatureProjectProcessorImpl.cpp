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
		rc = Project(pinFeatureClass, poutFeatureClass);
		
		

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

		const char* i_pj_str = "+proj=longlat +datum=WGS84 +no_defs";
		const char* o_pj_str = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs";

		projPJ i_pj = pj_init_plus(i_pj_str);
		projPJ o_pj = pj_init_plus(o_pj_str);

		if(i_pj==NULL)
		{
			return AG_FAILURE;
		}
		if(o_pj==NULL)
		{
			return AG_FAILURE;
		}
		
		g_uchar	 *wkb = NULL;
		Geometry *pGeometry = NULL;
		Feature	 *pFeature = NULL;
		FeatureCursor* pCursor = pinFeatureClass->Query();

		FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();
		
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if( pGeometry != NULL )
			{
				wkb = pGeometry->AsBinary();

				switch(pGeometry->GeometryType())
				{
				case augeGTPoint:
					Project((WKBPoint*)wkb, i_pj, o_pj);
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

			cmd->Insert(pFeature);

			pFeature->Release();
		}

		pCursor->Release();

		return AG_SUCCESS;
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBPoint* pWKBPoint, projPJ i_pj, projPJ o_pj)
	{
		double x = pWKBPoint->point.x;
		double y = pWKBPoint->point.y;

		x *= DEG_TO_RAD;
		y *= DEG_TO_RAD;
		
		int ret = pj_transform(i_pj, o_pj, 1, 1, &x, &y, NULL );
		
		pWKBPoint->point.x = x;
		pWKBPoint->point.y = y;

		return ret;
	}
}
