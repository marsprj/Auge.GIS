#include "SQLBuilder.h"

namespace auge
{
	void SQLBuilder::BuildCreateTable(std::string& sql, const char* name, GFields* pFields)
	{
		g_int fcount = pFields->Count();

		sql = "create table ";
		sql.append(name);
		sql.append("(gid serial");

		GField* pField = NULL;
		for(g_int i=0; i<fcount; i++)
		{
			pField = pFields->GetField(i);
			
			if(!g_stricmp(pField->GetName(), "gid"))
			{
				continue;
			}
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				continue;
			}

			sql.append(",");
			sql.append(pField->GetName());
			sql.append(" ");
			if(pField->GetType()==augeFieldTypeString)
			{
				
			}
			else
			{
				sql.append(GetPgFieldType(type));
			}
		}

		sql.append(")");
	}

	const char* SQLBuilder::GetPgFieldType(augeFieldType type)
	{
		switch(type)
		{
		case augeFieldTypeBool:
			return "boolean";
		case augeFieldTypeChar:
			return "";
		case augeFieldTypeShort:
			return "smallint";
		case augeFieldTypeInt:
			return "int";
		case augeFieldTypeLong:
			return "int";
		case augeFieldTypeInt64:
			return "bigint";
		case augeFieldTypeFloat:
			return "double precision";
		case augeFieldTypeDouble:
			return "double precision";
		case augeFieldTypeString:
			return "character varying";
		case augeFieldTypeTime:
			return "date";
		case augeFieldTypeBLOB:
			return "blob";
		case augeFieldTypeGeometry:
			return "geometry";
		case augeFieldTypeCLOB:
			return "clob";
		case augeFieldTypeNCLOB:
			return "blob";
		case augeFieldTypeSerial:
			return "serial";
		}
		return "";
	}
}