#include "FeatureObj.h"
#include "FeatureClassPgs.h"

namespace auge
{
	FeatureObj::FeatureObj(FeatureClassPgs* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();
		m_pFields = m_pFeatureClass->GetFields();
	}

	FeatureObj::~FeatureObj()
	{
		Cleanup();
		AUGE_SAFE_RELEASE(m_pFeatureClass);
	}

	FeatureClass* FeatureObj::GetFeatureClass() const
	{
		return m_pFeatureClass;
	}

	g_int FeatureObj::GetFID()
	{	
		return -1;
	}

	Geometry* FeatureObj::GetGeometry()
	{
		GValue* pValue = NULL;
		pValue = GetValue(m_pFields->GetGeometryField()->GetName());
		if(pValue==NULL)
		{
			return NULL;
		}
		return pValue->GetGeometry();
	}

	void FeatureObj::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
	
	GValue*	FeatureObj::GetValue(g_uint i) const
	{
		return GetValue(m_pFields->GetField(i)->GetName());
	}

	bool FeatureObj::GetBool(g_uint i) const
	{
		return GetBool(m_pFields->GetField(i)->GetName());
	}

	char FeatureObj::GetChar(g_uint i) const
	{
		return GetChar(m_pFields->GetField(i)->GetName());
	}

	short FeatureObj::GetShort(g_uint i) const
	{
		return GetShort(m_pFields->GetField(i)->GetName());
	}

	int	FeatureObj::GetInt(g_uint i) const
	{
		return GetInt(m_pFields->GetField(i)->GetName());
	}

	long FeatureObj::GetLong(g_uint i) const
	{
		return GetLong(m_pFields->GetField(i)->GetName());
	}

	float FeatureObj::GetFloat(g_uint i) const
	{
		return GetFloat(m_pFields->GetField(i)->GetName());
	}

	double FeatureObj::GetDouble(g_uint i) const
	{
		return GetDouble(m_pFields->GetField(i)->GetName());
	}

	int64 FeatureObj::GetInt64(g_uint i)	const
	{
		return GetInt64(m_pFields->GetField(i)->GetName());
	}

	const char*	FeatureObj::GetString(g_uint i)	const
	{
		return GetString(m_pFields->GetField(i)->GetName());
	}

	TIME_STRU* FeatureObj::GetTime(g_uint i)		const
	{
		return NULL;
	}

	GValue*	FeatureObj::GetValue(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return NULL;
		}
		GValue* pValue = iter->second;
		if(pValue!=NULL)
		{
			pValue->AddRef();
		}
		return pValue;
	}

	bool FeatureObj::GetBool(const char* name) const
	{
		if(name==NULL)
		{
			return false;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return false;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return false;
		}
		return pValue->GetBool();
	}

	char FeatureObj::GetChar(const char* name) const
	{
		if(name==NULL)
		{
			return '0';
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return '0';
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return '0';
		}
		return pValue->GetChar();
	}

	short FeatureObj::GetShort(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetShort();
	}

	int	FeatureObj::GetInt(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetInt();
	}

	long FeatureObj::GetLong(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetLong();
	}

	float FeatureObj::GetFloat(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetFloat();
	}

	double FeatureObj::GetDouble(const char* name) const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetDouble();
	}

	int64 FeatureObj::GetInt64(const char* name)	const
	{
		if(name==NULL)
		{
			return -1;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return -1;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return -1;
		}
		return pValue->GetInt64();
	}

	const char*	FeatureObj::GetString(const char* name)	const
	{
		if(name==NULL)
		{
			return NULL;
		}
		std::string strName = name;
		std::map<std::string, GValue*>::const_iterator iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			return NULL;
		}
		GValue* pValue = iter->second;
		if(pValue==NULL)
		{
			return NULL;
		}
		return pValue->GetString();
	}

	void FeatureObj::Cleanup()
	{
		GValue* pValue = NULL;
		std::map<std::string, GValue*>::iterator iter;
		for(iter=m_values.begin(); iter!=m_values.end(); iter++)
		{
			pValue = iter->second;
			if(pValue!=NULL)
			{
				pValue->Release();
			}
		}
		m_values.clear();
	}

	RESULTCODE FeatureObj::SetValue(const char* name, GValue *pValue)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		std::string strName = name;
		std::map<std::string, GValue*>::iterator iter;
		iter = m_values.find(strName);
		if(iter==m_values.end())
		{
			m_values[strName] = pValue;
		}
		else
		{
			GValue* pFValue = iter->second;
			if(pValue==NULL)
			{
				m_values.erase(iter);
			}
			else
			{
				m_values[strName] = pValue;
			}
			if(pFValue!=NULL)
			{
				pFValue->Release();
				pFValue = NULL;
			}
		}

		return AG_SUCCESS;
	}
}
