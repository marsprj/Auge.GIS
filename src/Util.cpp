#include "Util.h"

namespace auge
{
	Util::Util()
	{

	}
	Util::~Util()
	{

	}
	FieldType Util::ParseFieldType(Oid otype)
	{
		FieldType type = augeFieldTypeNone;
		switch(otype)
		{
		case 17:
			type = augeFieldTypeBLOB;
			break;
		case 20://int
		case 21://smallint
			type = augeFieldTypeInt;
			break;
		case 23:
			type = augeFieldTypeInt;
			break;
		case 701:
			type = augeFieldTypeDouble;
			break;
		case 1042:
		case 1043:
			type = augeFieldTypeString;
			break;
		case 1114:
			type = augeFieldTypeTime;
			break;
		case 16400:
		case 16408:
		case 16402:
		case 16416:
		case 16857:
		case 16890:
		case 17195:
		case 25031:
		case 93496:
			type = augeFieldTypeGeometry;
			break;
		default:
			type = augeFieldTypeGeometry;
		}
		return type;
	}

	GeometryType Util::ParseGeometryType(const char* stype)
	{
		if(stype==NULL)
			return wkbNull;

		GeometryType type = wkbNull;

		if(ag_stricmp(stype,		"POINT")		==0)
			type = wkbPoint;
		else if(ag_stricmp(stype,	"LINESTRING")	==0)
			type = wkbLineString;
		else if(ag_stricmp(stype,	"POLYGON")		==0)
			type = wkbPolygon;
		else if(ag_stricmp(stype,	"MULTIPOINT")	==0)
			type = wkbMultiPoint;
		else if(ag_stricmp(stype,	"MULTILINESTRING")==0)
			type = wkbMultiLineString;
		else if(ag_stricmp(stype,	"MULTIPOLYGON")==0)
			type = wkbMultiPolygon;

		return type;
	}
}
