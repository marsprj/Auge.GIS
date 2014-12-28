#include "SQLBuilder.h"

namespace auge
{
	void SQLBuilder::BuildCreateTable(std::string& sql, const char* name, GFields* pFields)
	{
		char str[AUGE_NAME_MAX];

		sql = "create table ";
		sql.append(name);
		sql.append("(gid serial");

		g_int fcount = pFields->Count();
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
				g_snprintf(str, AUGE_NAME_MAX, "%s(%d)", GetPgFieldType(pField->GetType()), pField->GetLength());
				sql.append(str);
			}
			else
			{
				sql.append(GetPgFieldType(pField->GetType()));
			}
		}

		sql.append(")");
	}

	void SQLBuilder::BuildQuery(std::string& sql, FeatureClass* pFeatureClass)
	{
		GField 	*pField = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();
		const char* fname  = NULL;

		bool first = true;

		sql = "select ";
		for(g_uint i=0; i<count; i++)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				sql.append(",");
			}

			pField = pFields->GetField(i);
			fname = pField->GetName();
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				sql.append("st_asbinary(");
				sql.append(fname);
				sql.append(") as ");
				sql.append(fname);
			}
			else
			{
				sql.append(fname);
			}
		}

		sql.append(" from ");
		sql.append(pFeatureClass->GetName());
	}

	void SQLBuilder::BuildInsertFeature(std::string& sql, const char* className, Feature* pFeature, g_uint srid)
	{
		std::string fields = "";
		std::string values = "";
		
		GField	*pField = NULL;
		GFields	*pFields= pFeature->GetFeatureClass()->GetFields();
		g_uint   fcount = pFields->Count();
		const char* fname= NULL;
		augeFieldType type = augeFieldTypeNone;
		char str[AUGE_BUFFER_MAX]; 

		bool first = true;		
		for(int i=0; i<fcount; i++)
		{
			pField = pFields->GetField(i);
			fname = pField->GetName();

			if(!g_stricmp(fname, "gid"))
			{
				continue;
			}


			if(first)
			{
				first = false;
			}
			else
			{
				fields.append(",");
				values.append(",");
			}

			type = pField->GetType();
			switch(type)
			{					 
			case augeFieldTypeShort:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetShort(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeInt:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeLong:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetLong(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeInt64:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt64(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeFloat:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetFloat(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeDouble:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetDouble(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeChar:			 
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pFeature->GetChar(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeString:
				{
					fields.append(fname);
					values.append("'");
					values.append(pFeature->GetString(i));
					values.append("'");
				}
				break;
			case augeFieldTypeTime:	
				{
					
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
					Geometry *pGeometry = pFeature->GetGeometry();
					if(pGeometry!=NULL)
					{
						const char* wkt = pGeometry->AsText(true);
						if(wkt!=NULL)
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%d",srid);

							fields.append(fname);
							values.append("st_geomfromtext(");
							values.append("'");
							values.append(wkt);
							values.append("',");
							values.append(str);
							values.append(")");
						}
					}
				}
				break;
			}//switch
		}

		// construct sdesql
		sql.append("insert into ");
		sql.append(className);
		sql.append(" (");
		sql.append(fields);
		sql.append(") VALUES (");
		sql.append(values);
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
			return "integer";
		case augeFieldTypeLong:
			return "integer";
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