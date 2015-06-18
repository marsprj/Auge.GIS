#include "AugeCore.h"
#include "WorkspaceShp.h"
#include "DataEngineImpl.h"
#include "FeatureClassShp.h"

namespace auge
{	
	WorkspaceShp::WorkspaceShp()
	{

	}

	WorkspaceShp::~WorkspaceShp()
	{

	}

	DataEngine*	WorkspaceShp::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspaceShp::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceShp::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspaceShp::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		m_connstring = conn_string;
		size_t epos = m_connstring.find_first_of("=");
		if(epos==std::string::npos)
		{
			m_connstring.clear();
			return AG_FAILURE;
		}

		m_path = m_connstring.substr(epos+1, m_connstring.length()-epos-1);
		return AG_SUCCESS;
	}

	const char* WorkspaceShp::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE WorkspaceShp::Open()
	{
		GError* pError = augeGetErrorInstance();

		if(m_path.empty())
		{
			return AG_FAILURE;
		}

		if(g_access(m_path.c_str(), 4))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg,"Path [%s] doesn't exist.", m_path.c_str());
			pError->SetError(msg);

			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void WorkspaceShp::Close()
	{

	}

	bool WorkspaceShp::IsOpen()
	{
		return true;
	}

	DataSet* WorkspaceShp::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceShp::RemoveDataSet(const char* name)
	{
		return RemoveFeatureClass(name);
	}

	FeatureClass* WorkspaceShp::OpenFeatureClass(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		FeatureClassShp* pFeatureClass = new FeatureClassShp();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}

	FeatureClass* WorkspaceShp::CreateFeatureClass(const char* name, GFields* pFields)
	{
		if(name==NULL||pFields==NULL)
		{
			return NULL;
		}

		GField* pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			return NULL;
		}

		char shp_path[AUGE_PATH_MAX];
		char dbf_path[AUGE_PATH_MAX];
		auge_make_path(shp_path, NULL, m_path.c_str(), name, "shp");
		auge_make_path(dbf_path, NULL, m_path.c_str(), name, "dbf");

		SHPHandle pshpHandle = NULL;
		DBFHandle pdbfHandle = NULL;

		GeometryDef* pGeometryDef = pField->GetGeometryDef();
		g_uint shp_type = GetShpType(pGeometryDef->GeometryType());
		
		pshpHandle = ::SHPCreate(shp_path, shp_type);
		if(pshpHandle==NULL)
		{
			return NULL;
		}
		//pshpHandle->adBoundsMin[0] = 115.472881;
		//pshpHandle->adBoundsMin[1] = 39.032912;
		//pshpHandle->adBoundsMin[2] = 0;
		//pshpHandle->adBoundsMin[3] = 0;
		//pshpHandle->adBoundsMax[0] = 117.614216;
		//pshpHandle->adBoundsMax[1] = 40.954599;
		//pshpHandle->adBoundsMax[2] = 0;
		//pshpHandle->adBoundsMax[3] = 0;

		pdbfHandle = ::DBFCreate(dbf_path);
		if(pdbfHandle==NULL)
		{
			::SHPClose(pshpHandle);
			return NULL;
		}

		int ret = 0;
		const char* fname = NULL;
		augeFieldType ftype;
		DBFFieldType  dbf_type;
		g_int nfield = pFields->Count();
		for(g_int i=0; i<nfield; i++)
		{
			pField = pFields->GetField(i);
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				continue;
			}
			fname = pField->GetName();
			if(!strcmp(fname, "gid"))
			{
				continue;
			}

			dbf_type = GetDbfType(pField->GetType());

			if(dbf_type==FTDouble)
				ret = ::DBFAddField(pdbfHandle, pField->GetName(), dbf_type, pField->GetLength(), pField->GetPrecision());
			else
				ret = ::DBFAddField(pdbfHandle, pField->GetName(), dbf_type, pField->GetLength(), 0);
		}
		::DBFFlush(pdbfHandle);

		FeatureClassShp* pFeatureClass = new FeatureClassShp();
		pFeatureClass->Create(name, this, pshpHandle, pdbfHandle);

		return pFeatureClass;
	}

	RESULTCODE WorkspaceShp::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	//EnumDataSet* WorkspaceShp::GetDataSets()
	//{
	//	return NULL;
	//}

	EnumDataSet* WorkspaceShp::GetFeatureClasses()
	{
		return NULL;
	}

	g_uint WorkspaceShp::GetShpType(augeGeometryType type)
	{
		g_uint shp_type = 1;
		switch(type)
		{
		case augeGTPoint:
		case augeGTMultiPoint:
			shp_type = 1;
			break;
		case augeGTLineString:
		case augeGTMultiLineString:
			shp_type = 3;
			break;		
		case augeGTPolygon:
		case augeGTMultiPolygon:
			shp_type = 5;
			break;
		default:
			shp_type = 1;
			break;
		}
		return shp_type;
	}

	DBFFieldType WorkspaceShp::GetDbfType(augeFieldType type)
	{
		DBFFieldType dbf_type = FTInteger;
		switch(type)
		{
		case augeFieldTypeString:
			dbf_type = FTString;
			break;
		case augeFieldTypeInt:
			dbf_type = FTInteger;
			break;
		case augeFieldTypeLong:
			dbf_type = FTLong;
			break;
		case augeFieldTypeDouble:
			dbf_type = FTDouble;
			break;
		case augeFieldTypeBool:
			dbf_type = FTLogical;
			break;
		case augeFieldTypeTime:// add by tjz 201001121140
			dbf_type = FTDate;
			break;
		case augeFieldTypeNone:
			dbf_type = FTInvalid;
			break;
		case augeFieldTypeChar:
		case augeFieldTypeShort:
		case augeFieldTypeInt64:
		case augeFieldTypeFloat:
			//	case augeFieldTypeTime:
		case augeFieldTypeBLOB:
		case augeFieldTypeGeometry:
			break;
		}

		return dbf_type;
	}
}
