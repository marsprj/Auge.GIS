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

	FeatureClass* FeaturePgs::GetFeatureClass() const
	{
		return m_pFeatureClass;
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

	GValue*	FeaturePgs::GetValue(g_int i) const
	{
		return NULL;
	}

	bool FeaturePgs::GetBool(g_int i) const
	{
		return true;
	}

	char FeaturePgs::GetChar(g_int i) const
	{
		return 0;
	}

	short FeaturePgs::GetShort(g_int i) const
	{
		return 0;
	}

	int	FeaturePgs::GetInt(g_int i) const
	{
		return 0;
	}

	long FeaturePgs::GetLong(g_int i) const
	{
		return 0;
	}

	float FeaturePgs::GetFloat(g_int i) const
	{
		return 0;
	}

	double FeaturePgs::GetDouble(g_int i) const
	{
		return 0;
	}

	int64 FeaturePgs::GetInt64(g_int i)	const
	{
		return 0;
	}

	const char*	FeaturePgs::GetString(g_int i)	const
	{
		return NULL;
	}

	GValue*	FeaturePgs::GetValue(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		return NULL;
	}

	bool FeaturePgs::GetBool(const char* name) const
	{
		if(name==NULL)
		{
			return false;
		}
		
		return true;
	}

	char FeaturePgs::GetChar(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	short FeaturePgs::GetShort(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	int	FeaturePgs::GetInt(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	long FeaturePgs::GetLong(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	float FeaturePgs::GetFloat(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	double FeaturePgs::GetDouble(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	int64 FeaturePgs::GetInt64(const char* name)	const
	{
		if(name==NULL)
		{
			return -1;
		}
		return 0;
	}

	const char*	FeaturePgs::GetString(const char* name)	const
	{
		if(name==NULL)
		{
			return NULL;
		}
		return NULL;
	}
}