#include "FeaturePgs.h"
#include "FeatureClassPgs.h"

namespace auge
{
	FeaturePgs::FeaturePgs():
	m_pFeatureClass(NULL),
	m_pGeometry(NULL),
	m_pgResult(NULL),
	m_fid(-1),
	m_geom_findex(-1),
	m_index(-1)
	{

	}

	FeaturePgs::~FeaturePgs()
	{
		AUGE_SAFE_RELEASE(m_pFeatureClass);
		AUGE_SAFE_RELEASE(m_pGeometry);
	}

	g_int FeaturePgs::GetFID()
	{
		return m_fid;
	}

	Geometry* FeaturePgs::GetGeometry()
	{
		if(m_pGeometry==NULL)
		{
			m_pGeometry = CreateGeometry();
		}
		return m_pGeometry;
	}

	void FeaturePgs::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool FeaturePgs::Create(g_uint index, g_int geom_findex, PGresult* pgResult, FeatureClassPgs* pFeatureClass)
	{
		m_fid = -1;
		m_index = index;
		m_geom_findex = geom_findex;
		m_pgResult = pgResult;
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();

		return true;
	}

	Geometry* FeaturePgs::CreateGeometry()
	{
		if(m_geom_findex<0)
		{
			return NULL;
		}
		Geometry* pGeometry = NULL;
		g_char* value = PQgetvalue(m_pgResult, m_index, m_geom_findex);
		if(value!=NULL)
		{
			size_t numBytes = 0;
			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
			g_uchar* pWKB = PQunescapeBytea((const g_uchar*)value, &numBytes);
			if(pWKB!=NULL)
			{
				g_uchar* wkb = (g_uchar*)auge_malloc(sizeof(g_uchar), numBytes);
				memcpy(wkb, pWKB, numBytes);
				pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_uchar*)wkb, true);
				PQfreemem(pWKB);
			}
		}

		
		return pGeometry;
	}
}