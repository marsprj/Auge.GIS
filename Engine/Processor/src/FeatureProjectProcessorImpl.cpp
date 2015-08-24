#include "FeatureProjectProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	FeatureProjectProcessorImpl::FeatureProjectProcessorImpl()
	{
		m_user = 0;
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

		g_uint srid_in  = pinFeatureClass->GetSRID();
		g_uint srid_out = m_srid;

		poutFeatureClass = CreateOutputFeatureClass(className_out, srid_out, poutWorkspace, pinFeatureClass);
		if(poutFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = Project(pinFeatureClass, poutFeatureClass);
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

	void FeatureProjectProcessorImpl::Release()
	{
		delete this;
	}

	FeatureClass* FeatureProjectProcessorImpl::CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorkspace* poutWorkspace, FeatureClass* pinFeatureClass)
	{
		FeatureClass* poutFatureClass = NULL;
		poutFatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFatureClass!=NULL)
		{
			return NULL;
		}

		GField * pField  = NULL;
		GFields* pFields = pinFeatureClass->GetFields();
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

		m_srsbase.Open();

		char i_pj_str[AUGE_MSG_MAX];
		memset(i_pj_str, 0, AUGE_MSG_MAX);
		m_srsbase.GetProj4Text(i_srid, i_pj_str, AUGE_MSG_MAX);

		char o_pj_str[AUGE_MSG_MAX];
		memset(o_pj_str, 0, AUGE_MSG_MAX);
		m_srsbase.GetProj4Text(o_srid, o_pj_str, AUGE_MSG_MAX);

		//const char* i_pj_str = "+proj=longlat +datum=WGS84 +no_defs";
		//const char* o_pj_str = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs";

		m_srsbase.Close();

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
					Project((WKBMultiPoint*)wkb, i_pj, o_pj);
					break;
				case augeGTLineString:
					Project((WKBLineString*)wkb, i_pj, o_pj);
					break;
				case augeGTMultiLineString:
					Project((WKBMultiLineString*)wkb, i_pj, o_pj);
					break;
				case augeGTPolygon:
					Project((WKBPolygon*)wkb, i_pj, o_pj);
					break;
				case augeGTMultiPolygon:
					Project((WKBMultiPolygon*)wkb, i_pj, o_pj);
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

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBMultiPoint* pWKBMultiPoint, projPJ i_pj, projPJ o_pj)
	{	
		int ret = 0;
		g_uint numPoints = 0;
		WKBPoint* pWKBPoint = NULL;

		numPoints = pWKBMultiPoint->numPoints;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));
		for(g_uint i=0; i<numPoints; i++, pWKBPoint++)
		{
			double *x = &(pWKBPoint->point.x);
			double *y = &(pWKBPoint->point.y);

			*x *= DEG_TO_RAD;
			*y *= DEG_TO_RAD;

			ret = pj_transform(i_pj, o_pj, 1, 1, x, y, NULL );
			if(ret!=0)
			{
				break;
			}
		}

		return ret;
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBLineString* pWKBLineString, projPJ i_pj, projPJ o_pj)
	{
		int ret = 0;

		int numPoints = pWKBLineString->numPoints;
		auge::Point *pt = (auge::Point*)(&(pWKBLineString->points[0]));

		for(int i=0; i<numPoints; i++, pt++)
		{
			double *x = &(pt->x);
			double *y = &(pt->y);

			*x *= DEG_TO_RAD;
			*y *= DEG_TO_RAD;

			ret = pj_transform(i_pj, o_pj, 1, 1, x, y, NULL );
			if(ret!=0)
			{
				break;
			}
		}
		return ret;
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBMultiLineString* pWKBMultiLineString, projPJ i_pj, projPJ o_pj)
	{
		int ret = 0;
		int numPoints = 0;
		int numLineStings = 0;
		auge::Point *pt = NULL;
		WKBLineString* pWKBLineString = NULL;

		numLineStings = pWKBMultiLineString->numLineStrings;
		pWKBLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));

		for(int i=0; i<numLineStings; i++)
		{
			numPoints = pWKBLineString->numPoints;
			pt = (auge::Point*)(&(pWKBLineString->points[0]));

			for(int j=0; j<numPoints; j++, pt++)
			{
				double *x = &(pt->x);
				double *y = &(pt->y);

				*x *= DEG_TO_RAD;
				*y *= DEG_TO_RAD;

				ret = pj_transform(i_pj, o_pj, 1, 1, x, y, NULL );
				if(ret!=0)
				{
					break;
				}
			}
			pWKBLineString = (WKBLineString*)(pt);
			if(ret!=0)
			{
				break;
			}
		}

		return ret;
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBPolygon* pWKBPolygon, projPJ i_pj, projPJ o_pj)
	{
		int ret = 0;

		int i=0, j=0, end=0;
		int numPoints = 0;
		int numRings  = pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt = NULL;

		pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt = (auge::Point*)(&(pLinearRing->points[0]));

			for(int j=0; j<numPoints; j++, pt++)
			{
				double *x = &(pt->x);
				double *y = &(pt->y);

				*x *= DEG_TO_RAD;
				*y *= DEG_TO_RAD;

				ret = pj_transform(i_pj, o_pj, 1, 1, x, y, NULL );
				if(ret!=0)
				{
					break;
				}
			}
			pLinearRing = (auge::LinearRing*)(pt);
			if(ret!=0)
			{
				break;
			}
		}

		return ret;
	}

	RESULTCODE FeatureProjectProcessorImpl::Project(WKBMultiPolygon* pWKBMultiPolygon, projPJ i_pj, projPJ o_pj)
	{
		int ret = 0;

		int i=0,j=0,k=0,end=0;
		int numPolygons = 0;
		int numRings = 0;
		int numPoints = 0;
		auge::Point *pt = NULL;
		auge::LinearRing *pLinearRing = NULL;
		auge::WKBPolygon *pWKBPolygon = NULL;

		numPolygons = pWKBMultiPolygon->numPolygons;
		pWKBPolygon = (auge::WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));

		for(i=0; i<numPolygons; i++)
		{
			numRings = pWKBPolygon->numRings;
			pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));
			for(j=0; j<numRings; j++)
			{
				numPoints = pLinearRing->numPoints;
				if(numPoints>0)
				{
					numPoints = pLinearRing->numPoints;
					end = numPoints-1;
					pt = (auge::Point*)(&(pLinearRing->points[0]));

					for(k=0; k<numPoints; k++,pt++)
					{
						double *x = &(pt->x);
						double *y = &(pt->y);

						*x *= DEG_TO_RAD;
						*y *= DEG_TO_RAD;

						ret = pj_transform(i_pj, o_pj, 1, 1, x, y, NULL );
						if(ret!=0)
						{
							break;
						}
					}					
					pLinearRing = (auge::LinearRing*)(pt);
					if(ret!=0)
					{
						break;
					}
				}
			}
			pWKBPolygon = (WKBPolygon*)(pLinearRing);
			if(ret!=0)
			{
				break;
			}
		}

		return ret;
	}

	void FeatureProjectProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
