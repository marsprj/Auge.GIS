#include "AugeCore.h"
#include "AugeGeometry.h"

#include "time.h"
#include "stdlib.h"

#include "string.h"

namespace auge
{
	//==============================================================================
#define CHAR_NONE	 (-127 - 1)
#define SHROT_NONE   (-32768)
#define INT_NONE     (-2147483647 - 1)
#define LONG_NONE    (-2147483647L - 1)
	//#define INT64_NONE   (-9223372036854775807i64 - 1)
#define INT64_NONE   (-9223372036854775807 - 1)
#define FLT_NONE	 (-3.402823466e+38F)
#define DBL_NONE	 (-1.7976931348623158e+308)

	//各数值类型字段占用的二进制字节数
#define BOOL_LEN	sizeof(bool)
#define CHAR_LEN	sizeof(char)
#define SHORT_LEN	sizeof(short)
#define INT_LEN		sizeof(int)
#define LONG_LEN	sizeof(long)
#define INT64_LEN	sizeof(int64)
#define FLOAT_LEN	sizeof(float)
#define DOUBLE_LEN	sizeof(double)
#define TIME_LEN	sizeof(TIME_STRU)

	//==============================================================================
	//
	//   Class GValue : Begin
	//
	//==============================================================================
	void GValue::InitVariant(VARIANT_STRU& val)
{
	memset(&val,0,sizeof(VARIANT_STRU));
}

GValue::GValue()
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeNone;
}

GValue::GValue(augeValueType vt)
{
	InitVariant(m_Variant);
	m_Variant.vt = vt;
}

GValue::GValue(bool val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeBool;
	m_Variant.blrVal = val;
}


GValue::GValue(char val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeChar;
	m_Variant.chrVal = val;
}

GValue::GValue(short val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeShort;
	m_Variant.shtVal = val;
}

GValue::GValue(int val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeInt;
	m_Variant.intVal = val;
}

GValue::GValue(long val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeLong;
	m_Variant.lngVal = val;
}

GValue::GValue(int64 val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeInt64;
	m_Variant.i64Val = val;
}

GValue::GValue(float val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeFloat;
	m_Variant.fltVal = val;
}

GValue::GValue(double val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeDouble;
	m_Variant.dblVal = val;
}

GValue::GValue(const char* val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeString;
	if (val != 0)
	{
		if(m_Variant.strVal)
		{
			delete[] m_Variant.strVal;
		}
		size_t len = strlen(val);
		m_Variant.strVal = new char[len+1];
		memset(m_Variant.strVal,0,len+1);
		memcpy(m_Variant.strVal,val,len);
		//m_Variant.strVal = new AgString(val);
	}	
}

//GValue::GValue(AgString* val,bool copy )
//{
//	InitVariant(m_Variant);
//	m_Variant.vt = augeValueTypeString;
//	if (val != 0)
//	{
//		m_Variant.strVal = copy?(new AgString(*val)):val;
//	}
//}

GValue::GValue(GBlob* val,bool copy )
{
	//InitVariant(m_Variant);
	//m_Variant.vt = augeValueTypeBLOB;
	//if (val != 0)
	//{
	//	m_Variant.blbVal = copy?(new GBlob(*val)):val;
	//}
}

GValue::GValue(TIME_STRU* val,bool copy )
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeTime;
	if (val != 0)
	{
		if (copy)
		{
			m_Variant.timVal = new TIME_STRU;
			memcpy(m_Variant.timVal,val,sizeof(TIME_STRU));
		}
		else
		{
			m_Variant.timVal = val;
		}
	}
}

GValue::GValue(Geometry* val,bool copy )
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeGeometry;
	if (val != 0)
	{
		if(copy)
			m_Variant.gmtVal = val->Clone();
		else
			m_Variant.gmtVal = val;
		//m_Variant.gmtVal = copy?val->Clone():val;
	}
}

//GValue::GValue(const AgString* val)
//{
//	InitVariant(m_Variant);
//	m_Variant.vt = augeValueTypeString;
//	if (val != 0)
//	{
//		m_Variant.strVal = new AgString(*val);
//	}
//}

GValue::GValue(const GBlob* val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeBLOB;
	if (val != 0)
	{
		m_Variant.blbVal = new GBlob(*val);
	}
}

GValue::GValue(const TIME_STRU* val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeTime;
	if (val != 0)
	{
		m_Variant.timVal = new TIME_STRU;
		memcpy(m_Variant.timVal,val,sizeof(TIME_STRU));
	}
}

GValue::GValue(const Geometry* val)
{
	InitVariant(m_Variant);
	m_Variant.vt = augeValueTypeGeometry;
	if (val != 0)
	{
		m_Variant.gmtVal = val->Clone();
	}
}

GValue::GValue(const GValue& other)
{
	if (this != &other)
	{
		InitVariant(m_Variant);
		SetValue(other);
	}
}

GValue::GValue(const GValue* other)
{
	if (other != 0 && this != other)
	{
		InitVariant(m_Variant);
		SetValue(other);
	}	
}

GValue::~GValue()
{
	Clear();
}

long	GValue::SerializeSize() const
{
	long lSerializeSize = 0L;
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			return BOOL_LEN;
		}
		break;
	case augeValueTypeChar:
		{
			return CHAR_LEN;
		}
		break;
	case augeValueTypeShort:
		{
			return SHORT_LEN;
		}
		break;
	case augeValueTypeInt:
		{
			return INT_LEN;
		}
		break;
	case augeValueTypeLong:
		{
			return LONG_LEN;
		}
		break;
	case augeValueTypeInt64:
		{
			return INT64_LEN;
		}
		break;
	case augeValueTypeFloat:
		{
			return FLOAT_LEN;
		}
		break;
	case augeValueTypeDouble:
		{
			return DOUBLE_LEN;
		}
		break;
	case augeValueTypeString:
		if (m_Variant.shtVal != 0)
		{
			//return m_Variant.strVal->GetLength()+1;
			return 0;
		}
		break;
	case augeValueTypeTime:
		{
			return TIME_LEN;
		}
		break;
	case augeValueTypeBLOB:
		if (m_Variant.blbVal != 0)
		{
			return 0;
			//return m_Variant.blbVal->Length();
		}
		break;
	case augeValueTypeGeometry:
		if (m_Variant.gmtVal != 0)
		{
			long m_Length = 5;
			m_Length += 4; // if (TAG_HASSRID(tag))
			m_Length += 32;// if (TAG_HASBBOX(tag)) 
			m_Length += 4; // for WKB Size
			//return m_Length + m_Variant.gmtVal->GetWKBSize();
			return 0;
		}
		break;
	default:
		break;
	}

	return lSerializeSize;
}

void GValue::Serialize(char* pBytes,long lBytesSize) const
{
	if (!pBytes || lBytesSize <= 0) { return; }
	memset(pBytes,0,lBytesSize);
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			memcpy(pBytes,&m_Variant.blrVal,BOOL_LEN);
		}
		break;
	case augeValueTypeChar:
		{
			memcpy(pBytes,&m_Variant.chrVal,CHAR_LEN);
		}
		break;
	case augeValueTypeShort:
		{
			memcpy(pBytes,&m_Variant.shtVal,SHORT_LEN);
		}
		break;
	case augeValueTypeInt:
		{
			memcpy(pBytes,&m_Variant.intVal,INT_LEN);
		}
		break;
	case augeValueTypeLong:
		{
			memcpy(pBytes,&m_Variant.lngVal,LONG_LEN);
		}
		break;
	case augeValueTypeInt64:
		{
			memcpy(pBytes,&m_Variant.i64Val,INT64_LEN);
		}
		break;
	case augeValueTypeFloat:
		{
			memcpy(pBytes,&m_Variant.fltVal,FLOAT_LEN);
		}
		break;
	case augeValueTypeDouble:
		{
			memcpy(pBytes,&m_Variant.dblVal,DOUBLE_LEN);
		}
		break;
	//case augeValueTypeString:
	//	if (m_Variant.strVal)
	//	{
	//		memcpy(pBytes,m_Variant.strVal->GetBuffer(),m_Variant.strVal->GetLength());
	//	}	
	//	break;
	case augeValueTypeTime:
		if (m_Variant.timVal)
		{
			memcpy(pBytes,m_Variant.timVal,TIME_LEN);
		}
		break;
	//case augeValueTypeBLOB:
	//	if (m_Variant.blbVal)
	//	{
	//		memcpy(pBytes,m_Variant.blbVal->GetBuffer(),m_Variant.blbVal->Length());
	//	}
	//	break;
	case augeValueTypeGeometry:
		if (m_Variant.gmtVal)
		{
			unsigned char tag = 0;
			//TAG_SETBYTEODR(tag,1);
			// 			TAG_SETZM(tag,0,0);
			// 			TAG_SETHASBBOX(tag,1);
			// 			TAG_SETHASSRID(tag,1);
			// 			GeometryPackage gp(m_Variant.gmtVal,tag);
			// 			memcpy(pBytes,gp.GetBytesBuf(),lBytesSize);
		}
		break;
	default:
		break;
	}
}

bool GValue::Deserialize(const char* pBytes, long lByteSize)
{
	bool bResult=false;
	if (!pBytes || lByteSize < 0)
	{
		return bResult;
	}

	// First:caller must set the value type
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			memcpy(&m_Variant.blrVal,pBytes,BOOL_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeChar:
		{
			memcpy(&m_Variant.chrVal,pBytes,CHAR_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeShort:
		{
			memcpy(&m_Variant.shtVal,pBytes,SHORT_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeInt:
		{
			memcpy(&m_Variant.intVal,pBytes,INT_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeLong:
		{
			memcpy(&m_Variant.lngVal,pBytes,LONG_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeInt64:
		{
			memcpy(&m_Variant.i64Val,pBytes,INT64_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeFloat:
		{
			memcpy(&m_Variant.fltVal,pBytes,FLOAT_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeDouble:
		{
			memcpy(&m_Variant.dblVal,pBytes,DOUBLE_LEN);
			bResult = true;
		}
		break;
	case augeValueTypeString:
		{
			//if (!m_Variant.strVal)
			//{
			//	m_Variant.strVal = new AgString();	
			//}
			//m_Variant.strVal->Empty();
			//(*m_Variant.strVal) = pBytes;
			//bResult = true;
		}
		break;
	case augeValueTypeTime:
		{
			if (!m_Variant.timVal)
			{
				m_Variant.timVal = new TIME_STRU;
			}	
			memcpy(m_Variant.timVal,pBytes,TIME_LEN);
			bResult = true;
		}
		break;
	//case augeValueTypeBLOB:
	//	{
	//		if (!m_Variant.blbVal)
	//		{
	//			m_Variant.blbVal = new GBlob(lByteSize);
	//		}
	//		m_Variant.blbVal->Resize(lByteSize);
	//		m_Variant.blbVal->Load(pBytes,lByteSize);
	//		bResult = true;
	//	}
	//	break;
	case augeValueTypeGeometry:
		{
			// 			GeometryFactory* gf = GeometryFactory::GetInstance();
			// 			if (m_Variant.gmtVal) 
			// 			{ 
			// 				GeometryFactory::GetInstance()->DeleteGeometry(m_Variant.gmtVal);
			// 				m_Variant.gmtVal = 0;
			// 			}
			// 			GeometryPackage gp((void*)pBytes,lByteSize);
			// 			m_Variant.gmtVal = gp.GetGeometry();
			// 			bResult = m_Variant.gmtVal?true:false;
		}
		break;
	default:
		break;
	}

	return bResult;
}

void GValue::Release()
{
	if(ReleaseRef()<=0)
	{
		delete this;
	}
}

augeValueType GValue::GetType() const
{
	return (augeValueType)m_Variant.vt;
}

void GValue::Clear()
{
	switch(m_Variant.vt)
	{
	case augeValueTypeString:
		{
			if (m_Variant.strVal)
			{
				delete[] m_Variant.strVal;
			}
		}
		break;
	case augeValueTypeTime:
		{
			if (m_Variant.timVal)
			{
				delete m_Variant.timVal;
			}
		}
		break;
	//case augeValueTypeBLOB:
	//	{
	//		if (m_Variant.blbVal)
	//		{
	//			delete m_Variant.blbVal;
	//		}		
	//	}
	//	break;
	case augeValueTypeGeometry:
		{
			if(m_Variant.gmtVal!=NULL)
			{
				m_Variant.gmtVal->Release();
				m_Variant.gmtVal = NULL;
			}
		}
		break;
	default:
		break;
	}	
	InitVariant(m_Variant);
}

bool GValue::IsNull() const
{
	return m_Variant.vt == augeValueTypeNone;
}

void GValue::SetType(augeValueType vt)
{
	Clear();
	m_Variant.vt = vt;
}

bool GValue::GetBool() const
{
	if (m_Variant.vt == augeValueTypeBool)
	{
		return m_Variant.blrVal;
	}
	return false;
}

char GValue::GetChar() const
{
	if (m_Variant.vt == augeValueTypeChar)
	{
		return m_Variant.chrVal;
	}
	return CHAR_NONE;
}

short GValue::GetShort() const
{
	if (m_Variant.vt == augeValueTypeShort)
	{
		return m_Variant.shtVal;
	}
	return SHROT_NONE;
}

int GValue::GetInt() const
{
	if (m_Variant.vt == augeValueTypeInt)
	{
		return m_Variant.intVal;
	}
	return INT_NONE;
}

long GValue::GetLong() const
{
	if (m_Variant.vt == augeValueTypeLong)
	{
		return m_Variant.lngVal;
	}

	return LONG_NONE;
}

float GValue::GetFloat() const
{
	if (m_Variant.vt == augeValueTypeFloat)
	{
		return m_Variant.fltVal;
	}
	return FLT_NONE;
}

double GValue::GetDouble() const
{
	if (m_Variant.vt == augeValueTypeDouble)
	{
		return m_Variant.dblVal;
	}
	return DBL_NONE;
}

int64 GValue::GetInt64() const
{
	if (m_Variant.vt == augeValueTypeInt64)
	{
		return m_Variant.i64Val;
	}
	//return INT64_NONE;
	return INT_NONE;
}

const char*	GValue::GetString()	const
{
	if (m_Variant.vt == augeValueTypeString)
	{
		return m_Variant.strVal;
	}
	return NULL;
}

//const AgString*	GValue::GetString()	const
//{
//	if (m_Variant.vt == augeValueTypeString)
//	{
//		return m_Variant.strVal;
//	}
//	return NULL;
//}

const GBlob* GValue::GetBlob() const
{
	if (m_Variant.vt == augeValueTypeBLOB)
	{
		return m_Variant.blbVal;
	}
	return NULL;
}

const TIME_STRU*	GValue::GetTime()		const
{
	if (m_Variant.vt == augeValueTypeTime)
	{
		return m_Variant.timVal;
	}
	return NULL;
}

const Geometry*	GValue::GetGeometry()	const
{
	if (m_Variant.vt == augeValueTypeGeometry)
	{
		return m_Variant.gmtVal;
	}
	return NULL;
}

const char* GValue::GetString()
{
	if (m_Variant.vt == augeValueTypeString)
	{
		return m_Variant.strVal;
	}
	return NULL;
}

//AgString* GValue::GetString()
//{
//	if (m_Variant.vt == augeValueTypeString)
//	{
//		return m_Variant.strVal;
//	}
//	return NULL;
//}

GBlob* GValue::GetBlob()
{
	if (m_Variant.vt == augeValueTypeBLOB)
	{
		return m_Variant.blbVal;
	}
	return NULL;
}

TIME_STRU* GValue::GetTime()
{
	if (m_Variant.vt == augeValueTypeTime)
	{
		return m_Variant.timVal;
	}
	return NULL;
}

Geometry* GValue::GetGeometry()
{
	if (m_Variant.vt == augeValueTypeGeometry)
	{
		return m_Variant.gmtVal;
	}
	return NULL;
}

long GValue::SetBool(bool val)
{
	if (m_Variant.vt == augeValueTypeBool)
	{
		m_Variant.blrVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetChar(char val)
{
	if (m_Variant.vt == augeValueTypeChar)
	{
		m_Variant.chrVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetShort(short val)
{
	if (m_Variant.vt == augeValueTypeShort)
	{
		m_Variant.shtVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetInt(int val)
{
	if (m_Variant.vt == augeValueTypeInt)
	{
		m_Variant.intVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetLong(long val)
{
	if (m_Variant.vt == augeValueTypeLong)
	{
		m_Variant.lngVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetFloat(float val)
{
	if (m_Variant.vt == augeValueTypeFloat)
	{
		m_Variant.fltVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetDouble(double val)
{
	if (m_Variant.vt == augeValueTypeDouble)
	{
		m_Variant.dblVal = val;
		return TRUE;
	}
	return FALSE;
}

long GValue::SetInt64(int64 val)
{
	if (m_Variant.vt == augeValueTypeDouble)
	{
		m_Variant.i64Val = val;
		return TRUE;
	}
	return FALSE;
}

//long GValue::SetString(AgString* val,bool copy)
//{
//	if (!val) return FALSE;
//	if (m_Variant.vt == augeValueTypeString)
//	{
//		if (m_Variant.strVal) delete m_Variant.strVal;
//		if(copy)
//		{
//			m_Variant.strVal = new AgString(*val);
//		}
//		else
//		{
//			m_Variant.strVal = val;
//		}
//		return TRUE;
//	}
//	return FALSE;
//}

long GValue::SetBlob(GBlob* val,bool copy)
{
	if (!val) return FALSE;
	//if (m_Variant.vt == augeValueTypeBLOB)
	//{
	//	if (m_Variant.blbVal) delete m_Variant.blbVal;
	//	if(copy)
	//	{
	//		m_Variant.blbVal = new GBlob(*val);
	//	}
	//	else
	//	{
	//		m_Variant.blbVal = val;
	//	}
	//	return TRUE;
	//}
	return FALSE;
}

long GValue::SetTime(TIME_STRU* val,bool copy)
{
	if (!val) return FALSE;
	if (m_Variant.vt == augeValueTypeBLOB)
	{
		if (m_Variant.timVal) delete m_Variant.timVal;
		if(copy)
		{
			m_Variant.timVal = new TIME_STRU(*val);
		}
		else
		{
			m_Variant.timVal = val;
		}
		return TRUE;
	}
	return FALSE;
}

long GValue::SetGeometry(Geometry* val,bool copy)
{
	if (!val) return FALSE;
	if (m_Variant.vt == augeValueTypeBLOB)
	{
		// 		if (m_Variant.gmtVal) GeometryFactory::GetInstance()->DeleteGeometry(m_Variant.gmtVal);
		// 		if(copy)
		// 		{
		// 			m_Variant.gmtVal = val->Clone();
		// 		}
		// 		else
		// 		{
		// 			m_Variant.gmtVal = val;
		// 		}
		return TRUE;
	}
	return FALSE;
}

long GValue::SetValue(const GValue& other)
{
	Clear();
	m_Variant.vt = other.m_Variant.vt;
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			m_Variant.blrVal = other.m_Variant.blrVal;
		}
		break;
	case augeValueTypeChar:
		{
			m_Variant.chrVal = other.m_Variant.chrVal;
		}
		break;
	case augeValueTypeShort:
		{
			m_Variant.shtVal = other.m_Variant.shtVal;
		}
		break;
	case augeValueTypeInt:
		{
			m_Variant.intVal = other.m_Variant.intVal;
		}
		break;
	case augeValueTypeLong:
		{
			m_Variant.lngVal = other.m_Variant.lngVal;
		}
		break;
	case augeValueTypeInt64:
		{
			m_Variant.i64Val = other.m_Variant.i64Val;
		}
		break;
	case augeValueTypeFloat:
		{
			m_Variant.fltVal = other.m_Variant.fltVal;
		}
		break;
	case augeValueTypeDouble:
		{
			m_Variant.dblVal = other.m_Variant.dblVal;
		}
		break;
	case augeValueTypeString:
		{
			//if (other.m_Variant.strVal)
			//{
			//	m_Variant.strVal = new AgString(*other.m_Variant.strVal);
			//}
		}
		break;
	case augeValueTypeTime:
		{
			if (other.m_Variant.timVal)
			{
				m_Variant.timVal = new TIME_STRU;	
				memcpy(m_Variant.timVal,other.m_Variant.timVal,sizeof(TIME_STRU));
			}
		}
		break;
	case augeValueTypeBLOB:
		{
			if (other.m_Variant.blbVal)
			{
				m_Variant.blbVal = new GBlob(*other.m_Variant.blbVal);
			}
		}
		break;
	case augeValueTypeGeometry:
		{
			if (other.m_Variant.gmtVal)
			{
				m_Variant.gmtVal = other.m_Variant.gmtVal->Clone();
			}
		}
		break;
	default:
		break;
	}
	return (TRUE);
}

long GValue::SetValue(const GValue* rValue)
{
	return SetValue(*rValue);
}

bool GValue::Equals(const GValue* pValue) const
{
	return this->operator ==(*pValue);
}

bool GValue::Equals(const GValue& rValue) const
{
	return this->operator ==(rValue);
}

GValue& GValue::operator=(const GValue& other)
{
	if (this == &other) return *this;
	Clear();
	InitVariant(m_Variant);
	m_Variant.vt = other.m_Variant.vt;
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			m_Variant.blrVal = other.m_Variant.blrVal;
		}
		break;
	case augeValueTypeChar:
		{
			m_Variant.chrVal = other.m_Variant.chrVal;
		}
		break;
	case augeValueTypeShort:
		{
			m_Variant.shtVal = other.m_Variant.shtVal;
		}
		break;
	case augeValueTypeInt:
		{
			m_Variant.intVal = other.m_Variant.intVal;
		}
		break;
	case augeValueTypeLong:
		{
			m_Variant.lngVal = other.m_Variant.lngVal;
		}
		break;
	case augeValueTypeInt64:
		{
			m_Variant.i64Val = other.m_Variant.i64Val;
		}
		break;
	case augeValueTypeFloat:
		{
			m_Variant.fltVal = other.m_Variant.fltVal;
		}
		break;
	case augeValueTypeDouble:
		{
			m_Variant.dblVal = other.m_Variant.dblVal;
		}
		break;
	case augeValueTypeString:
		{
			//if (other.m_Variant.strVal)
			//{
			//	m_Variant.strVal = new AgString(*other.m_Variant.strVal);
			//}
		}
		break;
	case augeValueTypeTime:
		{
			if (other.m_Variant.timVal)
			{
				m_Variant.timVal = new TIME_STRU;	
				memcpy(m_Variant.timVal,other.m_Variant.timVal,sizeof(TIME_STRU));
			}
		}
		break;
	case augeValueTypeBLOB:
		{
			if (other.m_Variant.blbVal)
			{
				m_Variant.blbVal = new GBlob(*m_Variant.blbVal);
			}
		}
		break;
	case augeValueTypeGeometry:
		{
			if (other.m_Variant.gmtVal)
			{
				m_Variant.gmtVal = m_Variant.gmtVal->Clone();
			}
		}
		break;
	default:
		break;
	}

	return *this;
}

GValue& GValue::operator=(bool val)
{
	Clear();
	m_Variant.vt = augeValueTypeBool;
	m_Variant.blrVal = val;
	return *this;
}

GValue& GValue::operator=(char val)
{
	Clear();
	m_Variant.vt = augeValueTypeChar;
	m_Variant.chrVal = val;
	return *this;
}

GValue& GValue::operator=(short val)
{
	Clear();
	m_Variant.vt = augeValueTypeShort;
	m_Variant.shtVal = val;
	return *this;
}

GValue& GValue::operator=(int val)
{
	Clear();
	m_Variant.vt = augeValueTypeInt;
	m_Variant.intVal = val;
	return *this;
}

GValue& GValue::operator=(long val)
{
	Clear();
	m_Variant.vt = augeValueTypeLong;
	m_Variant.lngVal = val;
	return *this;
}

GValue& GValue::operator=(int64 val)
{
	Clear();
	m_Variant.vt = augeValueTypeInt64;
	m_Variant.i64Val = val;
	return *this;
}

GValue& GValue::operator=(float val)
{
	Clear();
	m_Variant.vt = augeValueTypeFloat;
	m_Variant.fltVal = val;
	return *this;
}

GValue& GValue::operator=(double val)
{
	Clear();
	m_Variant.vt = augeValueTypeDouble;
	m_Variant.dblVal = val;
	return *this;
}

GValue& GValue::operator=(const char* val)
{
	Clear();
	//m_Variant.vt = augeValueTypeString;
	//if (val != 0)
	//{
	//	m_Variant.strVal = new AgString(val);
	//}
	return *this;
}

//GValue& GValue::operator=(const AgString* val)
//{
//	Clear();
//	m_Variant.vt = augeValueTypeString;
//	if (val != 0)
//	{
//		//m_Variant.strVal = new AgString(*val);
//	}
//	return *this;
//}

GValue& GValue::operator=(const GBlob* val)
{
	Clear();
	m_Variant.vt = augeValueTypeBLOB;
	if (val != 0)
	{
		m_Variant.blbVal = new GBlob(*val);
	}
	return *this;
}

GValue& GValue::operator=(const TIME_STRU* val)
{
	Clear();
	m_Variant.vt = augeValueTypeTime;
	if (val != 0)
	{
		m_Variant.timVal = new TIME_STRU;
		memcpy(m_Variant.timVal,val,sizeof(TIME_STRU));
	}
	return *this;
}

GValue& GValue::operator=(const Geometry* val)
{
	Clear();
	m_Variant.vt = augeValueTypeGeometry;
	if (val != 0)
	{
		m_Variant.gmtVal = val->Clone();
	}
	return *this;
}


//GValue& GValue::operator=(AgString* val)
//{
//	Clear();
//	m_Variant.vt = augeValueTypeString;
//	if (val != 0)
//	{
//		m_Variant.strVal = val;
//	}
//	return *this;
//}

GValue& GValue::operator=(GBlob* val)
{
	Clear();
	m_Variant.vt = augeValueTypeBLOB;
	if (val != 0)
	{
		m_Variant.blbVal = val;
	}
	return *this;
}

GValue& GValue::operator=(TIME_STRU* val)
{
	Clear();
	m_Variant.vt = augeValueTypeTime;
	if (val != 0)
	{
		m_Variant.timVal = val;
	}
	return *this;
}

GValue& GValue::operator=(Geometry* val)
{
	Clear();
	m_Variant.vt = augeValueTypeGeometry;
	if (val != 0)
	{
		m_Variant.gmtVal = val;
	}
	return *this;
}

// Literal comparison. Types and values must match.
bool	GValue::operator==(const GValue& other)const
{
	switch(m_Variant.vt){
	case augeValueTypeBool:
		{
			bool thatValue = other;
			return m_Variant.blrVal == thatValue;
		}
		break;
	case augeValueTypeChar:
		{
			char thatValue = other;
			return m_Variant.chrVal == thatValue;
		}
		break;
	case augeValueTypeShort:
		{
			int thatValue = other;
			return m_Variant.shtVal == thatValue;
		}
		break;
	case augeValueTypeInt:
		{
			int thatValue = other;
			return m_Variant.intVal == thatValue;
		}
		break;
	case augeValueTypeLong:
		{
			int thatValue = other;
			return m_Variant.lngVal == thatValue;
		}
		break;
	case augeValueTypeInt64:
		{
			int64 thatValue = other;
			return m_Variant.i64Val == thatValue;
		}
		break;
	case augeValueTypeFloat:
		{
			float thatValue = other;
			return m_Variant.fltVal == thatValue;
		}
		break;
	case augeValueTypeDouble:
		{
			double thatValue = other;
			return m_Variant.dblVal == thatValue;
		}
		break;
	case augeValueTypeString:
		{
			//AgString thatValue = other;
			//if (m_Variant.strVal != 0)
			//{
			//	return  m_Variant.strVal->Compare(thatValue)==0?true:false;
			//}
			return false;
		}
		break;
	case augeValueTypeTime:
		{
			return memcmp(m_Variant.timVal,other.m_Variant.timVal,sizeof(TIME_STRU)) == 0;
		}
		break;
	//case augeValueTypeBLOB:
	//	{
	//		return ((*m_Variant.blbVal) == (*other.m_Variant.blbVal))?true:false;	
	//	}
	//	break;
	case augeValueTypeGeometry:
		{
			if (m_Variant.gmtVal != 0)
			{
				//return m_Variant.gmtVal->Equals(m_Variant.gmtVal)?true:false;
			}
			return false;
		}
		break;
	default:
		break;
	}
	return false;
}

bool	GValue::operator!=(const GValue& other)const
{
	return !((*this)==other);
}

bool	GValue::operator<(const GValue& other) const
{
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			bool thatValue = other;
			return m_Variant.blrVal < thatValue;
		}
		break;	
	case augeValueTypeChar:
		{
			char thatValue = other;
			return m_Variant.chrVal < thatValue;
		}
		break;
	case augeValueTypeShort:
		{
			int thatValue = other;
			return m_Variant.shtVal < thatValue;
		}
		break;
	case augeValueTypeInt:
		{
			int thatValue = other;
			return m_Variant.intVal < thatValue;
		}
		break;
	case augeValueTypeLong:
		{
			int thatValue = other;
			return m_Variant.lngVal < thatValue;
		}
		break;
	case augeValueTypeInt64:
		{
			int64 thatValue = other;
			return m_Variant.i64Val < thatValue;
		}
		break;
	case augeValueTypeFloat:
		{
			float thatValue = other;
			return m_Variant.fltVal < thatValue;
		}
		break;
	case augeValueTypeDouble:
		{
			double thatValue = other;
			return m_Variant.dblVal < thatValue;
		}
		break;
	case augeValueTypeString:
		{
			//AgString thatValue = other;
			//if (m_Variant.strVal != 0)
			//{
			//	return m_Variant.strVal->Compare(thatValue)<0?true:false;
			//}
			return false;
		}
		break;
	case augeValueTypeTime:
		{
			return memcmp(m_Variant.timVal,other.m_Variant.timVal,sizeof(TIME_STRU)) < 0;
		}
		break;
	case augeValueTypeBLOB:
		{
			return false;
			//return ((*m_Variant.blbVal) < (*other.m_Variant.blbVal))?true:false;	
		}
		break;
	default:
		break;
	}

	return false;
}

bool	GValue::operator<=(const GValue& other)const
{
	return !((*this)>other);
}

bool	GValue::operator>(const GValue& other)const
{
	switch(m_Variant.vt)
	{
	case augeValueTypeBool:
		{
			bool thatValue = other;
			return m_Variant.blrVal > thatValue;
		}
		break;	
	case augeValueTypeChar:
		{
			char thatValue = other;
			return m_Variant.chrVal > thatValue;
		}
		break;
	case augeValueTypeShort:
		{
			int thatValue = other;
			return m_Variant.shtVal > thatValue;
		}
		break;
	case augeValueTypeInt:
		{
			int thatValue = other;
			return m_Variant.intVal > thatValue;
		}
		break;
	case augeValueTypeLong:
		{
			int thatValue = other;
			return m_Variant.lngVal > thatValue;
		}
		break;
	case augeValueTypeInt64:
		{
			int64 thatValue = other;
			return m_Variant.i64Val > thatValue;
		}
		break;
	case augeValueTypeFloat:
		{
			float thatValue = other;
			return m_Variant.fltVal > thatValue;
		}
		break;
	case augeValueTypeDouble:
		{
			double thatValue = other;
			return m_Variant.dblVal > thatValue;
		}
		break;
	case augeValueTypeString:
		{
			//AgString thatValue = other;
			//if (m_Variant.strVal != 0)
			//{
			//	return  m_Variant.strVal->Compare(thatValue) > 0;
			//}
			return false;
		}
		break;
	case augeValueTypeTime:
		{
			return memcmp(m_Variant.timVal,other.m_Variant.timVal,sizeof(TIME_STRU)) > 0;
		}
		break;
	//case augeValueTypeBLOB:
	//	{
	//		return ((*m_Variant.blbVal) < (*other.m_Variant.blbVal))?true:false;	
	//	}
	//	break;
	default:
		break;
	}

	return false;
}

bool GValue::operator>=(const GValue& other)const
{
	return !((*this) < other);
}
//
//GValue::operator AgString () const
//{
//	switch(m_Variant.vt){
//	case augeValueTypeBool:
//		{
//			return AgString(m_Variant.blrVal?"TRUE":"FALSE");
//		}
//		break;
//	case augeValueTypeChar:
//		{
//			return AgString(m_Variant.chrVal);
//		}
//		break;
//	case augeValueTypeShort:
//		{
//			return AgString(m_Variant.shtVal);
//		}
//		break;
//	case augeValueTypeInt:
//		{
//			return AgString(m_Variant.intVal);
//		}
//		break;
//	case augeValueTypeLong:
//		{
//			return AgString(m_Variant.lngVal);
//		}
//		break;
//	case augeValueTypeInt64:
//		{
//			return AgString(m_Variant.i64Val);
//		}
//		break;
//	case augeValueTypeFloat:
//		{
//			return AgString(m_Variant.fltVal);
//		}
//		break;
//	case augeValueTypeDouble:
//		{
//			return AgString(m_Variant.dblVal);
//		}
//		break;
//	case augeValueTypeString:
//		{
//			return  AgString(*m_Variant.strVal);
//		}
//		break;
//	case augeValueTypeTime:
//		{
//			if (m_Variant.timVal)
//			{
//				tm tt;
//				tt.tm_sec = m_Variant.timVal->usSecond;
//				tt.tm_min = m_Variant.timVal->usMinute;
//				tt.tm_hour = m_Variant.timVal->usHour;
//				tt.tm_mday = m_Variant.timVal->usDay;
//				tt.tm_wday = m_Variant.timVal->usDayOfWeek;
//				tt.tm_mon = m_Variant.timVal->usMonth;
//				tt.tm_year = m_Variant.timVal->usYear;
//#ifndef _WIN32_WCE
//				return AgString(asctime(&tt));
//#endif
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}
//		}
//		break;
//	case augeValueTypeBLOB:
//		{
//			if (m_Variant.blbVal)
//			{
//				return AgString("BLOB");	
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}
//		}
//		break;
//	case augeValueTypeGeometry:
//		{
//			if (m_Variant.gmtVal)
//			{
//				return AgString("GEOMETRY");
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}	
//		}
//		break;
//	default:
//		break;
//	}
//	return AgString("Null");//错误值");
//}

GValue::operator bool () const
{
	switch(m_Variant.vt){
	case augeValueTypeBool:  
		{
			return m_Variant.blrVal;
		}
		break;
	case augeValueTypeChar:
		{
			return m_Variant.chrVal!=0;
		}
		break;
	case augeValueTypeShort:
		{
			return m_Variant.shtVal!=0;
		}
		break;
	case augeValueTypeInt:
		{
			return m_Variant.intVal!=0;
		}
		break;
	case augeValueTypeLong:
		{
			return m_Variant.lngVal!=0;
		}
		break;
	case augeValueTypeInt64:
		{
			return m_Variant.i64Val!=0;
		}
		break;
	case augeValueTypeFloat:
		{
			return m_Variant.fltVal!=0;
		}
		break;
	case augeValueTypeDouble:
		{
			return m_Variant.dblVal!=0;
		}
		break;
	case augeValueTypeString:
		{
			if (m_Variant.strVal)
			{
				//return  m_Variant.strVal->CompareNoCase("true")==0;
				return true;
			}
			return false;
		}
		break;
	case augeValueTypeTime:
		{
			return false;
		}
		break;
	case augeValueTypeBLOB:
		{
			return false;
		}
		break;
	case augeValueTypeGeometry:
		{
			return false;
		}
		break;
	default:
		break;
	}
	return false;
}

GValue::operator char () const
{
	int theValue;
	theValue = this->operator int();
	return theValue;
}

GValue::operator int () const
{
	int64 theValue;
	theValue = this->operator int64();
	return theValue;
}
GValue::operator int64 () const
{
	switch(m_Variant.vt){
	case augeValueTypeBool:  
		{
			return m_Variant.blrVal?1:0;
		}
		break;
	case augeValueTypeChar:
		{
			return m_Variant.chrVal;
		}
		break;
	case augeValueTypeShort:
		{
			return m_Variant.shtVal;
		}
		break;
	case augeValueTypeInt:
		{
			return m_Variant.intVal;
		}
		break;
	case augeValueTypeLong:
		{
			return m_Variant.lngVal;
		}
		break;
	case augeValueTypeInt64:
		{
			return m_Variant.i64Val;
		}
		break;
	case augeValueTypeFloat:
		{
			return m_Variant.fltVal;
		}
		break;
	case augeValueTypeDouble:
		{
			return m_Variant.dblVal;
		}
		break;
	case augeValueTypeString:
		{
			return atoi((const char*)(*m_Variant.strVal));
		}
		break;
	case augeValueTypeTime:
		{
			return 0;
		}
		break;
	case augeValueTypeBLOB:
		{
			return 0;
		}
		break;
	case augeValueTypeGeometry:
		{
			return 0;
		}
		break;
	default:
		break;
	}
	return 0;
}
GValue::operator float () const
{
	double theValue;
	theValue = this->operator double();
	return theValue;
}
GValue::operator double () const
{
	switch(m_Variant.vt){
	case augeValueTypeBool:  
		{
			return m_Variant.blrVal?1.0:0;
		}
		break;
	case augeValueTypeChar:
		{
			return m_Variant.chrVal;
		}
		break;
	case augeValueTypeShort:
		{
			return m_Variant.shtVal;
		}
		break;
	case augeValueTypeInt:
		{
			return m_Variant.intVal;
		}
		break;
	case augeValueTypeLong:
		{
			return m_Variant.lngVal;
		}
		break;
	case augeValueTypeInt64:
		{
			return m_Variant.i64Val;
		}
		break;
	case augeValueTypeFloat:
		{
			return m_Variant.fltVal;
		}
		break;
	case augeValueTypeDouble:
		{
			return m_Variant.dblVal;
		}
		break;
	case augeValueTypeString:
		{
			return atof((const char*)(*m_Variant.strVal));
		}
		break;
	case augeValueTypeTime:
		{
			return 0;
		}
		break;
	case augeValueTypeBLOB:
		{
			return 0;
		}
		break;
	case augeValueTypeGeometry:
		{
			return 0;
		}
		break;
	default:
		break;
	}
	return 0;
//}
//AgString	GValue::ToString() const
//{
//	switch(m_Variant.vt){
//	case augeValueTypeBool:
//		{
//			return AgString(m_Variant.blrVal?"TRUE":"FALSE");
//		}
//		break;
//	case augeValueTypeChar:
//		{
//			return AgString(m_Variant.chrVal);
//		}
//		break;
//	case augeValueTypeShort:
//		{
//			return AgString(m_Variant.shtVal);
//		}
//		break;
//	case augeValueTypeInt:
//		{
//			return AgString(m_Variant.intVal);
//		}
//		break;
//	case augeValueTypeLong:
//		{
//			return AgString(m_Variant.lngVal);
//		}
//		break;
//	case augeValueTypeInt64:
//		{
//			return AgString(m_Variant.i64Val);
//		}
//		break;
//	case augeValueTypeFloat:
//		{
//			return AgString(m_Variant.fltVal);
//		}
//		break;
//	case augeValueTypeDouble:
//		{
//			return AgString(m_Variant.dblVal);
//		}
//		break;
//	case augeValueTypeString:
//		{
//			return  AgString(*m_Variant.strVal);
//		}
//		break;
//	case augeValueTypeTime:
//		{
//			if (m_Variant.timVal)
//			{
//				tm tt;
//				tt.tm_sec = m_Variant.timVal->usSecond;
//				tt.tm_min = m_Variant.timVal->usMinute;
//				tt.tm_hour = m_Variant.timVal->usHour;
//				tt.tm_mday = m_Variant.timVal->usDay;
//				tt.tm_wday = m_Variant.timVal->usDayOfWeek;
//				tt.tm_mon = m_Variant.timVal->usMonth;
//				tt.tm_year = m_Variant.timVal->usYear;
//#ifndef _WIN32_WCE
//				return AgString(asctime(&tt));
//#endif
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}
//		}
//		break;
//	case augeValueTypeBLOB:
//		{
//			if (m_Variant.blbVal)
//			{
//				return AgString("BLOB");	
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}
//		}
//		break;
//	case augeValueTypeGeometry:
//		{
//			if(m_Variant.gmtVal)
//			{
//				return AgString("GEOMETRY");
//			}
//			else
//			{
//				return AgString("Null");//错误值");
//			}	
//		}
//		break;
//	default:
//		break;
//	}
//	return AgString("Null");//错误值");
//}

//void GValue::SetModified(bool modified)
//{
//	m_Variant.m_isModified = modified;
//}
//
//bool GValue::IsModified() const
//{
//	return m_Variant.m_isModified;
//}

}

}