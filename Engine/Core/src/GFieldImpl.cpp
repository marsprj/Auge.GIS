#include "GFieldImpl.h"
#include "GeometryDefImpl.h"
#include "AugeCore.h"

namespace auge
{
	static const char *FieldTypeName[] = 
	{
		"none",
		"bool",
		"char",
		"short",
		"integer",
		"long",
		"integer64",
		"float",
		"double",
		"string",
		"time",
		"blob",
		"geometry",
		"clob",
		"nclob"
	};

	//const char* GField::FieldTypeAsString(augeFieldType type)
	//{
	//	const char* szType = "NONE";
	//	switch(type)
	//	{
	//	case augeFieldTypeBool:
	//		szType = "BOOL";		break;
	//	case augeFieldTypeChar:
	//		szType = "CHAR";		break;
	//	case augeFieldTypeShort:
	//		szType = "SHORT";		break;
	//	case augeFieldTypeInt:
	//		szType = "INT";			break;
	//	case augeFieldTypeLong:
	//		szType = "LONG";		break;
	//	case augeFieldTypeInt64:
	//		szType = "INT64";		break;
	//	case augeFieldTypeFloat:
	//		szType = "FLOAT";		break;
	//	case augeFieldTypeDouble:
	//		szType = "DOUBLE";		break;
	//	case augeFieldTypeString:
	//		szType = "STRING";		break;
	//	case augeFieldTypeTime:
	//		szType = "TIME";		break;
	//	case augeFieldTypeBLOB:
	//		szType = "BLOB";		break;
	//	case augeFieldTypeGeometry:
	//		szType = "GEOMETRY";	break;
	//	case augeFieldTypeCLOB:
	//		szType = "CLOB";		break;
	//	case augeFieldTypeNCLOB:
	//		szType = "NCLOB";		break;
	//	}

	//	return szType;
	//}
	//////////////////////////////////////////////////////////////////////////

	GFieldImpl::GFieldImpl():
	m_Type(augeFieldTypeNone),
	m_tag(augeFieldFlagUnknown),
	m_length(0),
	m_scale(1),
	m_precision(1),
	m_pGeometryDef(NULL),
	m_pValue(NULL)
	{

	}

	GFieldImpl::~GFieldImpl()
	{
		if (m_pGeometryDef != NULL)
		{
			m_pGeometryDef->Release();
		}
		if(m_pValue!=NULL)
		{
			m_pValue->Release();
			m_pValue = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	const char*	GFieldImpl::GetName()
	{
		return m_name.c_str();
	}

	void GFieldImpl::SetName(const char* szName)
	{
		if(szName!=NULL)
			m_name = szName;
	}

	const char* GFieldImpl::GetAlias()
	{
		return m_alias.c_str();
	}

	void GFieldImpl::SetAlias(const char* szAlias)
	{
		if(szAlias!=NULL)
			m_alias = szAlias;
	}

	void GFieldImpl::SetType(augeFieldType type)
	{
		m_Type = type;

		switch(m_Type)
		{
		case augeFieldTypeShort:
			m_length = sizeof(short);
			break;
		case augeFieldTypeInt:
			m_length = sizeof(int);
			break;
		case augeFieldTypeLong:
			m_length = sizeof(long);
			break;
		case augeFieldTypeInt64:
			m_length = sizeof(int64);
			break;
		case augeFieldTypeFloat:
			m_length = sizeof(float);
			break;
		case augeFieldTypeDouble:
			m_length = sizeof(double);
			break;
		}
	}

	augeFieldType GFieldImpl::GetType() const
	{
		return m_Type;
	}

	const char* GFieldImpl::GetTypeName() const
	{
		return FieldTypeName[m_Type];
	}

	long GFieldImpl::GetLength() const
	{
		return m_length;
	}

	void GFieldImpl::SetLength(long len)
	{
		m_length = len;
	}

	int	GFieldImpl::GetPrecision() const
	{
		return m_precision;
	}

	void GFieldImpl::SetPrecision(int nPrecision)
	{
		m_precision = nPrecision;
	}

	double GFieldImpl::GetScale() const
	{
		return m_scale;
	}

	void GFieldImpl::SetScale(double scale)
	{
		m_scale = scale;
	}

	bool GFieldImpl::IsGeometryField() const
	{
		return m_Type == augeFieldTypeGeometry;
	}

	GeometryDef* GFieldImpl::GetGeometryDef()
	{
		if(IsGeometryField())
		{
			if(m_pGeometryDef==NULL)
			{
				m_pGeometryDef = new GeometryDefImpl();
			}
		}
		else
		{
			if(m_pGeometryDef!=NULL)
			{
				m_pGeometryDef->Release();
				m_pGeometryDef = NULL;
			}
		}
		return m_pGeometryDef;
	}

	GField* GFieldImpl::Clone() const
	{
		GFieldImpl* pThis = (GFieldImpl*)this;
		GFieldImpl* pFieldNew = new GFieldImpl();
		pFieldNew->SetName(pThis->GetName());	
		pFieldNew->SetAlias(pThis->GetAlias());
		pFieldNew->SetType(pThis->GetType());
		pFieldNew->SetLength(pThis->GetLength());
		pFieldNew->SetPrecision(pThis->GetPrecision());
		pFieldNew->SetScale(pThis->GetScale());
		pFieldNew->SetNullable(pThis->IsNullable());
		pFieldNew->SetPrimary(pThis->IsPrimary());
		pFieldNew->SetSystem(pThis->IsSystem());

		if(GetType()==augeFieldTypeGeometry)
		{
			GeometryDef* pGeometryDef = pFieldNew->GetGeometryDef();
			GeometryDef_2* pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();

			GEnvelope extent;
			pThis->m_pGeometryDef->GetExtent(extent);

			pGeometryDef_2->SetDefault(pThis->m_pGeometryDef->IsDefault());
			pGeometryDef_2->SetExtent(extent);
			pGeometryDef_2->SetGeometryType(pThis->m_pGeometryDef->GeometryType());
			pGeometryDef_2->SetM(pThis->m_pGeometryDef->HasM());
			pGeometryDef_2->SetZ(pThis->m_pGeometryDef->HasZ());
			pGeometryDef_2->SetSRID(pThis->m_pGeometryDef->GetSRID());
		}

		return pFieldNew;
	}

	GField* GFieldImpl::Field()
	{
		return this;
	}

	GField_2* GFieldImpl::Field_2()
	{
		return this;
	}

	void GFieldImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	bool GFieldImpl::IsNullable()
	{
		return HasFlagX(m_tag,augeFieldFlagNullable);
	}

	void GFieldImpl::SetNullable(bool flag)
	{
		if (flag)
		{
			SetFlagX(m_tag,augeFieldFlagNullable);
		}
		else
		{
			CnlFlagX(m_tag,augeFieldFlagNullable);
		}
	}

	bool GFieldImpl::IsPrimary()
	{
		return HasFlagX(m_tag,augeFieldFlagPrimary);
	}

	void GFieldImpl::SetPrimary(bool flag)
	{
		if (flag)
		{
			SetFlagX(m_tag,augeFieldFlagPrimary);
		}
		else
		{
			CnlFlagX(m_tag,augeFieldFlagPrimary);
		}
	}

	bool GFieldImpl::IsSystem()
	{
		return HasFlagX(m_tag,augeFieldFlagSystem);
	}

	void GFieldImpl::SetSystem(bool flag)
	{
		if (flag)
		{
			SetFlagX(m_tag,augeFieldFlagSystem);
		}
		else
		{
			CnlFlagX(m_tag,augeFieldFlagSystem);
		}
	}

	GValue* GFieldImpl::GetDefaultValue()
	{
		return m_pValue;
	}

	bool GFieldImpl::SetDefaultValue(GValue* pValue)
	{
		if(m_pValue==pValue)
		{
			return true;
		}
		if(m_pValue!=NULL)
		{

			m_pValue->Release();
			m_pValue = NULL;
		}
		m_pValue = pValue;
		if(m_pValue!=NULL)
		{
			m_pValue->AddRef();
		}
		return true;
	}

}
