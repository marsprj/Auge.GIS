#include "GFieldsImpl.h"
#include "GFieldImpl.h"
#include "AugeType.h"

namespace auge
{
	// Fields* Fields::Create()
	// {
	// 	return (new FieldsImpl());
	// }

	//////////////////////////////////////////////////////////////////////////

	GFieldsImpl::GFieldsImpl()
	{

	}

	GFieldsImpl::~GFieldsImpl()
	{
		Clear();
	}

	int GFieldsImpl::Count() const
	{
		return m_fields.size();
	}

	GField* GFieldsImpl::GetField(int index)
	{
		if(!IsValidIndex(index))
			return NULL;
		return m_fields[index];
	}

	GField* GFieldsImpl::GetField(const char* szName)
	{
		int nIndex = FindField(szName);
		if(nIndex<0)
			return NULL;
		return m_fields[nIndex];
	}

	int	GFieldsImpl::FindField(const char* szName,int iFieldType/*=augeFieldTypeNone*/)
	{
		if(szName==NULL)
			return -1;
		int nIndex = -1;
		GField *pagField = NULL;
		FIELD_VECTOR::iterator iter;
		for(iter=m_fields.begin(), nIndex=0; iter!=m_fields.end(); iter++, nIndex++)
		{
			pagField = (*iter);
			if(pagField)
			{
				const char* szFieldName = pagField->GetName();

				char str[AUGE_MSG_MAX];
				g_sprintf(str, "%s : %s", szName, szFieldName);
				augeGetLoggerInstance()->Fatal(str, __FILE__, __LINE__);

				if(g_stricmp(szName, szFieldName)==0)
				{
					return nIndex;
				}
			}
		}
		return -1;
	}

	GField* GFieldsImpl::GetGeometryField()
	{
		GField *pagField = NULL;
		GField *pDefaultGeometryField = NULL;
		FIELD_VECTOR::iterator iter;
		for(iter=m_fields.begin(); iter!=m_fields.end(); iter++)
		{
			pagField = (*iter);
			if(pagField!=NULL)
			{
				if(pagField->GetType()==augeFieldTypeGeometry)
				{
					GeometryDef *pGeometryDef = pagField->GetGeometryDef();
					if(pGeometryDef!=NULL)
					{
						if(pGeometryDef->IsDefault())
						{
							return pagField;
						}
						else
						{
							pDefaultGeometryField = pagField;
						}
					}
				}
			}
		}
		return pDefaultGeometryField;
	}

	RESULTCODE GFieldsImpl::Add(GField* pField)
	{
		if(pField==NULL)
		{
			return AG_FAILURE;
		}
		int nIndex = FindField(pField->GetName());
		if(nIndex>0)
		{
			return AG_FAILURE;
		}
		m_fields.push_back(pField);
		return AG_SUCCESS;
	}

	RESULTCODE GFieldsImpl::AddRef(GField* pField)
	{
		if(pField==NULL)
		{
			return AG_FAILURE;
		}
		int nIndex = FindField(pField->GetName());
		if(nIndex>0)
		{
			return AG_FAILURE;
		}
		m_fields.push_back(pField);
		return AG_SUCCESS;
	}

	RESULTCODE GFieldsImpl::Remove(const char* szName)
	{
		int nIndex = FindField(szName);
		if(nIndex<0)
			return AG_FAILURE;
		m_fields.erase(m_fields.begin()+nIndex);
		return AG_SUCCESS;
	}

	void GFieldsImpl::Clear()
	{
		GField *pagField = NULL;
		FIELD_VECTOR::iterator iter=m_fields.begin();
		for(iter=m_fields.begin(); iter!=m_fields.end(); iter++)
		{
			pagField = (*iter);
			if(pagField != NULL)
			{
				pagField->Release();
			}
		}

		m_fields.clear();
	}

	bool GFieldsImpl::HasGeometryField() const
	{
		return true;
	}

	void GFieldsImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

	inline
		bool GFieldsImpl::IsValidIndex(int index)
	{
		return ((index>=0)&&index<(m_fields.size()));
	}
}
