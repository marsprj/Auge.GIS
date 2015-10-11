#include "FeatureByd.h"
#include "FeatureClassByd.h"
#include "AugeField.h"

namespace auge
{
	FeatureByd::FeatureByd()
	{
		m_fid = -1;
		m_field_count = 0;
		m_pFeatureClass = NULL;
		m_pbydResultSet = NULL;
		m_pbydRow = NULL;
		m_pbydFields = NULL;
		m_pGeometry = NULL;
		m_strs = NULL;
	}

	FeatureByd::~FeatureByd()
	{
		if(m_pGeometry!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pGeometry);
		}
		CleanupStrings();
	}

	FeatureClass* FeatureByd::GetFeatureClass() const
	{
		return m_pFeatureClass;
	}

	g_int FeatureByd::GetFID()
	{
		return m_fid;
	}

	Geometry* FeatureByd::GetGeometry()
	{
		if(m_pGeometry==NULL)
		{
			m_pGeometry = CreateGeometry();
		}

		return m_pGeometry;
	}

	GValue*	FeatureByd::GetValue(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}

		const char* fname = NULL;
		CPPIField* pbydField = m_pbydFields->GetField(i);
		fname = pbydField->GetName().c_str();

		GField* pField = m_pFeatureClass->GetField(fname);
		if(pField==NULL)
		{
			return NULL;
		}

		GValue* pValue = NULL;
		augeFieldType type = pField->GetType();
		switch(type)
		{
		case augeFieldTypeBool:
			pValue = new GValue(GetBool(i));
			break;
		case augeFieldTypeChar:
			pValue = new GValue(GetChar(i));
			break;
		case augeFieldTypeShort:
			pValue = new GValue(GetShort(i));
			break;
		case augeFieldTypeInt:
			pValue = new GValue(GetInt(i));
			break;
		case augeFieldTypeLong:
			pValue = new GValue(GetLong(i));
			break;
		case augeFieldTypeInt64:
			pValue = new GValue(GetInt64(i));
			break;
		case augeFieldTypeFloat:
			pValue = new GValue(GetFloat(i));
			break;
		case augeFieldTypeDouble:
			pValue = new GValue(GetDouble(i));
			break;
		case augeFieldTypeString:
			pValue = new GValue(GetString(i));
			break;
		case augeFieldTypeTime:
			pValue = new GValue(GetTime(i));
			break;
		case augeFieldTypeBLOB:
			//pValue = new GValue(GetBl(i));
			break;
		case augeFieldTypeGeometry:
			{
				FeatureByd* that = (FeatureByd*)this;
				pValue = new GValue(that->GetGeometry());
			}
			break;
		}

		return pValue;
	}

	bool FeatureByd::GetBool(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsBool();
	}

	char FeatureByd::GetChar(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsInt1();
	}

	short FeatureByd::GetShort(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsInt2();
	}

	int	FeatureByd::GetInt(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsInt4();
	}

	long FeatureByd::GetLong(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsUInt4();
	}

	float FeatureByd::GetFloat(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsFloat4();
	}

	double FeatureByd::GetDouble(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsFloat8();
	}

	int64 FeatureByd::GetInt64(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}
		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		return pbydItem->GetAsInt8();
	}

	const char*	FeatureByd::GetString(g_uint i) const
	{
		if(m_pbydRow==NULL)
		{
			return NULL;
		}
		if(i>=m_field_count)
		{
			return NULL;
		}

		FeatureByd* pThis = (FeatureByd*)this;
		const char* v = pThis->GetStringValue(i);
		if(v!=NULL)
		{
			return v;
		}

		CPPIDataItem* pbydItem = m_pbydRow->GetItem(i);
		if(pbydItem==NULL)
		{
			return NULL;
		}
		CPPIString str = pbydItem->GetAsString();
		v = str.c_str();
		if(v==NULL)
		{
			return NULL;
		}
		char* dv = strdup(v);
		pThis->SetStringValue(i, dv);

		return dv;
	}

	TIME_STRU* FeatureByd::GetTime(g_uint i) const
	{
		return NULL;
	}

	GValue*	FeatureByd::GetValue(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetValue(item);
	}

	bool FeatureByd::GetBool(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetBool(item);
	}

	char FeatureByd::GetChar(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetChar(item);
	}

	short FeatureByd::GetShort(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetShort(item);
	}

	int	FeatureByd::GetInt(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		CPPIField* f = m_pbydFields->GetField(item);
		CPPIDataType type = f->GetType();
		return GetInt(item);
	}

	long FeatureByd::GetLong(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetLong(item);
	}

	float FeatureByd::GetFloat(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetFloat(item);
	}

	double FeatureByd::GetDouble(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetDouble(item);
	}

	int64 FeatureByd::GetInt64(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		return GetInt64(item);
	}

	const char* FeatureByd::GetString(const char* name) const
	{
		if(name==NULL)
		{
			return NULL;
		}
		if(m_pbydFields==NULL)
		{
			return NULL;
		}
		CPPIUInt2 item = m_pbydFields->FindField(name);
		CPPIField* pbydField = m_pbydFields->GetField(item);
		CPPIDataType type = pbydField->GetType();
		CPPIString str = pbydField->GetName();
		const char* ss = str.c_str();
		return GetString(item);
	}

	RESULTCODE FeatureByd::SetValue(const char* name, GValue *pValue)
	{
		return AG_FAILURE;
	}

	bool FeatureByd::Create(CPPIResultSet* pbydResultSet, FeatureClassByd* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pbydResultSet = pbydResultSet;
		m_pbydRow = m_pbydResultSet->GetRow();
		m_pbydFields = m_pbydResultSet->GetFields();
		m_field_count = m_pbydFields->GetCount();
		
		CPPIUInt2 item = 0;
		CPPIFields* pbydFields = NULL;
		pbydFields = m_pbydResultSet->GetFields();
		item = pbydFields->FindField("fid");
		if(item>=0)
		{
			CPPIRow *pbydRow = NULL;
			CPPIDataItem* pbydItem = NULL;
			pbydRow = m_pbydResultSet->GetRow();
			pbydItem = pbydRow->GetItem(item);
			if(pbydItem==NULL)
			{
				return NULL;
			}
			if(pbydItem->IsNull())
			{
				return NULL;
			}
			m_fid = pbydItem->GetAsInt4();
		}
		InitStrings();
		return true;
	}

	Geometry* FeatureByd::CreateGeometry()
	{
		const char* geom_field = NULL;
		geom_field = m_pFeatureClass->m_geom_field.c_str();

		CPPIUInt2 item = 0;
		CPPIFields* pbydFields = NULL;
		pbydFields = m_pbydResultSet->GetFields();
		item = pbydFields->FindField(geom_field);
		if(item<0)
		{
			return NULL;
		}
		
		CPPIRow *pbydRow = NULL;
		CPPIDataItem* pbydItem = NULL;
		pbydRow = m_pbydResultSet->GetRow();
		pbydItem = pbydRow->GetItem(item);
		if(pbydItem==NULL)
		{
			return NULL;
		}
		if(pbydItem->IsNull())
		{
			return NULL;
		}
		CPPIBytes pbydBytes = pbydItem->GetAsBytes();
		CPPIByte* pBytes = pbydBytes.GetBytes();
		if(pBytes==NULL)
		{
			return NULL;
		}

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_uchar*)pBytes);

		return pGeometry;
	}

	void FeatureByd::InitStrings()
	{
		g_uint nField = m_pbydFields->GetCount();
		m_strs = (char**)malloc(sizeof(char*)*m_field_count);
		memset(m_strs, 0, sizeof(char*)*m_field_count);
	}

	void FeatureByd::CleanupStrings()
	{
		if(m_strs==NULL)
		{
			return;
		}
		for(g_uint i=0; i<m_field_count; i++)
		{
			if(m_strs[i]!=NULL)
			{
				free(m_strs[i]);
				m_strs[i] = NULL;
			}
		}
		free(m_strs);
	}

	const char*	FeatureByd::GetStringValue(g_uint i)
	{
		if(i>=m_field_count)
		{
			return NULL;
		}
		return m_strs[i];
	}

	void FeatureByd::SetStringValue(g_uint i, char* value)
	{
		if(i>=m_field_count)
		{
			return;
		}
		char* v = m_strs[i];
		if(v!=NULL)
		{
			free(v);
		}
		m_strs[i] = value;
	}
}