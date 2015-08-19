#include "AugeCore.h"
#include "WorkspaceByd.h"
#include "DataEngineImpl.h"
//#include "FeatureClassShp.h"

namespace auge
{	
	WorkspaceByd::WorkspaceByd()
	{
		m_pbydEnvironment = NULL;
		m_pbydConnction = NULL;
	}

	WorkspaceByd::~WorkspaceByd()
	{
		Close();
		if(m_pbydEnvironment!=NULL)
		{
			CPPIEnvironment::TerminateEnvironment(m_pbydEnvironment);
			m_pbydEnvironment = NULL;
		}
	}

	DataEngine*	WorkspaceByd::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspaceByd::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceByd::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspaceByd::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char* WorkspaceByd::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE WorkspaceByd::Open()
	{
		GError* pError = augeGetErrorInstance();


		return AG_SUCCESS;
	}

	void WorkspaceByd::Close()
	{
		if(m_pbydConnction!=NULL)
		{
			m_pbydEnvironment->CloseConnection(m_pbydConnction);
			m_pbydConnction = NULL;
		}
	}

	bool WorkspaceByd::IsOpen()
	{
		return true;
	}

	GConnection* WorkspaceByd::GetConnection()
	{
		return NULL;
	}

	DataSet* WorkspaceByd::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceByd::RemoveDataSet(const char* name)
	{
		return RemoveFeatureClass(name);
	}

	FeatureClass* WorkspaceByd::OpenFeatureClass(const char* name)
	{
		//if(name==NULL)
		//{
		//	return NULL;
		//}

		//FeatureClassShp* pFeatureClass = new FeatureClassShp();
		//if(!pFeatureClass->Create(name, this))
		//{
		//	pFeatureClass->Release();
		//	pFeatureClass = NULL;
		//}

		//return pFeatureClass;

		return NULL;
	}

	FeatureClass* WorkspaceByd::CreateFeatureClass(const char* name, GFields* pFields)
	{
		if(name==NULL||pFields==NULL)
		{
			return NULL;
		}

		//GField* pField = pFields->GetGeometryField();
		//if(pField==NULL)
		//{
		//	return NULL;
		//}

		//char shp_path[AUGE_PATH_MAX];
		//char dbf_path[AUGE_PATH_MAX];
		//auge_make_path(shp_path, NULL, m_path.c_str(), name, "shp");
		//auge_make_path(dbf_path, NULL, m_path.c_str(), name, "dbf");

		//SHPHandle pshpHandle = NULL;
		//DBFHandle pdbfHandle = NULL;

		//GeometryDef* pGeometryDef = pField->GetGeometryDef();
		//g_uint shp_type = GetShpType(pGeometryDef->GeometryType());
		//
		//pshpHandle = ::SHPCreate(shp_path, shp_type);
		//if(pshpHandle==NULL)
		//{
		//	return NULL;
		//}
		////pshpHandle->adBoundsMin[0] = 115.472881;
		////pshpHandle->adBoundsMin[1] = 39.032912;
		////pshpHandle->adBoundsMin[2] = 0;
		////pshpHandle->adBoundsMin[3] = 0;
		////pshpHandle->adBoundsMax[0] = 117.614216;
		////pshpHandle->adBoundsMax[1] = 40.954599;
		////pshpHandle->adBoundsMax[2] = 0;
		////pshpHandle->adBoundsMax[3] = 0;

		//pdbfHandle = ::DBFCreate(dbf_path);
		//if(pdbfHandle==NULL)
		//{
		//	::SHPClose(pshpHandle);
		//	return NULL;
		//}

		//int ret = 0;
		//const char* fname = NULL;
		//augeFieldType ftype;
		//DBFFieldType  dbf_type;
		//g_int nfield = pFields->Count();
		//for(g_int i=0; i<nfield; i++)
		//{
		//	pField = pFields->GetField(i);
		//	if(pField->GetType()==augeFieldTypeGeometry)
		//	{
		//		continue;
		//	}
		//	fname = pField->GetName();
		//	if(!strcmp(fname, "gid"))
		//	{
		//		continue;
		//	}

		//	dbf_type = GetDbfType(pField->GetType());

		//	if(dbf_type==FTDouble)
		//		ret = ::DBFAddField(pdbfHandle, pField->GetName(), dbf_type, pField->GetLength(), pField->GetPrecision());
		//	else
		//		ret = ::DBFAddField(pdbfHandle, pField->GetName(), dbf_type, pField->GetLength(), 0);
		//}
		//::DBFFlush(pdbfHandle);

		//FeatureClassShp* pFeatureClass = new FeatureClassShp();
		//pFeatureClass->Create(name, this, pshpHandle, pdbfHandle);

		//return pFeatureClass;

		return NULL;
	}

	RESULTCODE WorkspaceByd::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	EnumDataSet* WorkspaceByd::GetDataSets()
	{
		return GetFeatureClasses();
	}

	EnumDataSet* WorkspaceByd::GetFeatureClasses()
	{
		return NULL;
	}
}
