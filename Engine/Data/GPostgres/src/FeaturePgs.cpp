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
	m_index(-1),
	m_pFields(NULL)
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
		if(m_fid<0)
		{
			m_fid = GetInt("gid");
		}
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
		m_nfield = PQnfields(pgResult);
		m_geom_findex = geom_findex;
		m_pgResult = pgResult;
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();
		m_pFields = m_pFeatureClass->GetFields();
		
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
				//g_uchar* wkb = (g_uchar*)auge_malloc(sizeof(g_uchar), numBytes);
				//memcpy(wkb, pWKB, numBytes);
				pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_uchar*)pWKB, false);
				PQfreemem(pWKB);
			}
		}

		
		return pGeometry;
	}

	GValue*	FeaturePgs::GetValue(g_uint i) const
	{
		const char* value = NULL;
		if(PQgetisnull(m_pgResult, m_index, i))
		{
			value = PQgetvalue(m_pgResult, m_index, i);
		}
		else
		{
			value = PQgetvalue(m_pgResult, m_index, i);
		}

		GValue* pValue = NULL;
		GField* pField = m_pFields->GetField(i);		
		switch(pField->GetType())
		{
		case augeFieldTypeShort:
			{
				pValue = new GValue((short)atoi(value));
			}
			break;
		case augeFieldTypeInt:
			{
				pValue = new GValue((int)atoi(value));
			}
			break;
		case augeFieldTypeLong:
			{
				pValue = new GValue((long)atoi(value));
			}
			break;
		case augeFieldTypeInt64:
			{
				pValue = new GValue((g_long)atoi(value));
			}
			break;
		case augeFieldTypeFloat:
			{
				pValue = new GValue((float)atof(value));
			}
			break;
		case augeFieldTypeDouble:
			{
				pValue = new GValue((double)atof(value));
			}
			break;
		case augeFieldTypeChar:			 
			{
				pValue = new GValue(value[0]);
			}
			break;
		case augeFieldTypeString:
			{
				pValue = new GValue(value);
			}
			break;
		case augeFieldTypeTime:	
			{
				TIME_STRU tim;
				memset(&tim,0, sizeof(TIME_STRU));
				sscanf(value,"%d-%2d-%2d %2d:%2d:%2d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond));
				pValue = new GValue(&tim,true);
			}
			break;
		case augeFieldTypeBool:			 
			{

			}
			break;
		case augeFieldTypeBLOB:			 
			{

			}
			break;
		case augeFieldTypeGeometry:
			{
				size_t numBytes = 0;
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				g_uchar* pWKB = PQunescapeBytea((const g_uchar*)value, &numBytes);
				if(pWKB!=NULL)
				{
					//g_uchar* wkb = (g_uchar*)auge_malloc(sizeof(g_uchar), numBytes);
					//memcpy(wkb, pWKB, numBytes);
					Geometry* pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_uchar*)pWKB, false);
					if(pGeometry!=NULL)
					{
						pValue = new GValue(pGeometry);
					}
					PQfreemem(pWKB);
				}
			}
			break;
		}//switch

		return pValue;
		
		return NULL;
	}

	bool FeaturePgs::GetBool(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeBool)
		{
			return false;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);

		return true;
	}

	char FeaturePgs::GetChar(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeChar)
		{
			return false;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return 0;
	}

	short FeaturePgs::GetShort(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeShort)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atoi(value);
	}

	int	FeaturePgs::GetInt(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeInt)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atoi(value);
	}

	long FeaturePgs::GetLong(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeLong)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atoi(value);
	}

	float FeaturePgs::GetFloat(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeFloat)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atof(value);
	}

	double FeaturePgs::GetDouble(g_uint i) const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeDouble)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atof(value);
	}

	int64 FeaturePgs::GetInt64(g_uint i)	const
	{
		if(i>=m_nfield)
		{
			return false;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeInt64)
		{
			return 0;
		}
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		return atof(value);
	}

	const char*	FeaturePgs::GetString(g_uint i)	const
	{
		if(i>=m_nfield)
		{
			return NULL;
		}
		GField* pField = m_pFields->GetField(i);
		if(pField->GetType()!=augeFieldTypeString)
		{
			return NULL;
		}
		return PQgetvalue(m_pgResult, m_index, i);
	}

	TIME_STRU* FeaturePgs::GetTime(g_uint i)		const
	{
		const char* value = PQgetvalue(m_pgResult, m_index, i);
		TIME_STRU *tim = (TIME_STRU*)malloc(sizeof(TIME_STRU));
		memset(tim,0, sizeof(TIME_STRU));
		sscanf(value,"%d-%2d-%2d %2d:%2d:%2d",&(tim->usYear),&(tim->usMonth),&(tim->usDay),&(tim->usHour),&(tim->usMinute),&(tim->usSecond));
		return tim;
	}

	GValue*	FeaturePgs::GetValue(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}

		GField* pField = m_pFields->GetField(name);
		if(pField==NULL)
		{
			return NULL;
		}

		GValue* pValue = NULL;
		int findex = PQfnumber(m_pgResult, name);
		const char* value = PQgetvalue(m_pgResult, m_index, findex);
		switch(pField->GetType())
		{
		case augeFieldTypeShort:
			{
				pValue = new GValue((short)atoi(value));
			}
			break;
		case augeFieldTypeInt:
			{
				pValue = new GValue((int)atoi(value));
			}
			break;
		case augeFieldTypeLong:
			{
				pValue = new GValue((long)atoi(value));
			}
			break;
		case augeFieldTypeInt64:
			{
				pValue = new GValue((g_long)atoi(value));
			}
			break;
		case augeFieldTypeFloat:
			{
				pValue = new GValue((float)atof(value));
			}
			break;
		case augeFieldTypeDouble:
			{
				pValue = new GValue((double)atof(value));
			}
			break;
		case augeFieldTypeChar:			 
			{
				pValue = new GValue(value[0]);
			}
			break;
		case augeFieldTypeString:
			{
				pValue = new GValue(value);
			}
			break;
		case augeFieldTypeTime:	
			{
				TIME_STRU tim;
				memset(&tim,0, sizeof(TIME_STRU));
				sscanf(value,"%d-%2d-%2d %2d:%2d:%2d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond));
				pValue = new GValue(&tim,true);
			}
			break;
		case augeFieldTypeBool:			 
			{

			}
			break;
		case augeFieldTypeBLOB:			 
			{

			}
			break;
		case augeFieldTypeGeometry:
			{
				size_t numBytes = 0;
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				g_uchar* pWKB = PQunescapeBytea((const g_uchar*)value, &numBytes);
				if(pWKB!=NULL)
				{
					//g_uchar* wkb = (g_uchar*)auge_malloc(sizeof(g_uchar), numBytes);
					//memcpy(wkb, pWKB, numBytes);
					Geometry* pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_uchar*)pWKB, false);
					if(pGeometry!=NULL)
					{
						pValue = new GValue(pGeometry);
					}
					PQfreemem(pWKB);
				}
			}
			break;
		}//switch

		return pValue;
	}

	bool FeaturePgs::GetBool(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		return GetBool(PQfnumber(m_pgResult, name));
	}

	char FeaturePgs::GetChar(const char* name) const
	{
		if(name==NULL)
		{
			return '0';
		}
		return GetChar(PQfnumber(m_pgResult, name));
	}

	short FeaturePgs::GetShort(const char* name) const
	{
		if(name==NULL)
		{
			return 0;
		}
		return GetShort(PQfnumber(m_pgResult, name));
	}

	int	FeaturePgs::GetInt(const char* name) const
	{
		if(name==NULL)
		{
			return 0;
		}
		return GetInt(PQfnumber(m_pgResult, name));
	}

	long FeaturePgs::GetLong(const char* name) const
	{
		if(name==NULL)
		{
			return 0;
		}
		return GetLong(PQfnumber(m_pgResult, name));
	}

	float FeaturePgs::GetFloat(const char* name) const
	{
		if(name==NULL)
		{
			return 0.0f;
		}
		return GetFloat(PQfnumber(m_pgResult, name));
	}

	double FeaturePgs::GetDouble(const char* name) const
	{
		if(name==NULL)
		{
			return 0.0f;
		}
		return GetDouble(PQfnumber(m_pgResult, name));
	}

	int64 FeaturePgs::GetInt64(const char* name)	const
	{
		if(name==NULL)
		{
			return 0;
		}
		return GetInt64(PQfnumber(m_pgResult, name));
	}

	const char*	FeaturePgs::GetString(const char* name)	const
	{
		if(name==NULL)
		{
			return NULL;
		}
		return GetString(PQfnumber(m_pgResult, name));
	}

	RESULTCODE FeaturePgs::SetValue(const char* name, GValue *pValue)
	{
		return AG_SUCCESS;
	}
}
