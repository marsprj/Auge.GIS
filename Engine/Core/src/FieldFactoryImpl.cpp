#include "FieldFactoryImpl.h"
#include "GFieldImpl.h"
#include "GFieldsImpl.h"

namespace auge
{
	FieldFactory* augeGetFieldFactoryInstance()
	{
		static FieldFactoryImpl g_field_factory;
		return &g_field_factory;
	}

	FieldFactoryImpl::FieldFactoryImpl()
	{

	}

	FieldFactoryImpl::~FieldFactoryImpl()
	{

	}

	GField*	FieldFactoryImpl::CreateField()
	{
		return (new GFieldImpl());
	}

	GFields* FieldFactoryImpl::CreateFields()
	{
		return (new GFieldsImpl());
	}

	const char*	FieldFactoryImpl::Encode(augeFieldType type)
	{
		switch(type)
		{
		case augeFieldTypeBool:
			return "boolean";
		case augeFieldTypeChar:
			return "character";
		case augeFieldTypeShort:
			return "short";
		case augeFieldTypeInt:
			return "int";
		case augeFieldTypeLong:
			return "long";
		case augeFieldTypeInt64:
			return "int64";
		case augeFieldTypeFloat:
			return "float";
		case augeFieldTypeDouble:
			return "double";
		case augeFieldTypeString:
			return "string";
		case augeFieldTypeTime:
			return "timestamp";
		case augeFieldTypeBLOB:
			return "blob";
		case augeFieldTypeGeometry:
			return "geometry";
		case augeFieldTypeCLOB:
			return "clob";
		case augeFieldTypeNCLOB:
			return "nclob";
		case augeFieldTypeSerial:
			return "serial";
		}
		return "";
	}

	augeFieldType FieldFactoryImpl::Decode(const char* type)
	{
		if(type==NULL)
		{
			return augeFieldTypeNone;
		}
		if(!g_stricmp(type, "boolean"))
		{
			return augeFieldTypeBool;
		}
		else if(!g_stricmp(type, "character"))
		{
			return augeFieldTypeString;
		}
		else if(!g_stricmp(type, "character"))
		{
			return augeFieldTypeChar;
		}
		else if(!g_stricmp(type, "short"))
		{
			return augeFieldTypeShort;
		}
		else if(!g_stricmp(type, "int"))
		{
			return augeFieldTypeInt;
		}
		else if(!g_stricmp(type, "long"))
		{
			return augeFieldTypeLong;
		}
		else if(!g_stricmp(type, "int64"))
		{
			return augeFieldTypeInt64;
		}
		else if(!g_stricmp(type, "float"))
		{
			return augeFieldTypeFloat;
		}
		else if(!g_stricmp(type, "double"))
		{
			return augeFieldTypeDouble;
		}
		else if(!g_stricmp(type, "string"))
		{
			return augeFieldTypeString;
		}
		else if(!g_stricmp(type, "timestamp"))
		{
			return augeFieldTypeTime;
		}
		else if(!g_stricmp(type, "blob"))
		{
			return augeFieldTypeBLOB;
		}
		else if(!g_stricmp(type,"geometry"))
		{
			return augeFieldTypeGeometry;
		}
		else if(!g_stricmp(type,"clob"))
		{
			return augeFieldTypeCLOB;
		}
		else if(!g_stricmp(type,"nclob"))
		{
			return augeFieldTypeNCLOB;
		}
		else if(!g_stricmp(type,"serial"))
		{
			return augeFieldTypeSerial;
		}
		return augeFieldTypeNone;
	}
}