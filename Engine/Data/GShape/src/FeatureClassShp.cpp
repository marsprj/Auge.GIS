#include "WorkspaceShp.h"
#include "FeatureCursorShp.h"
#include "FeatureClassShp.h"
#include "FeatureInsertCommandShp.h"

#include <AugeCore.h>

namespace auge
{
	FeatureClassShp::FeatureClassShp():
	m_pWorkspace(NULL),
	m_pshpHandle(NULL),
	m_pdbfHandle(NULL),
	m_srid(4326),
	m_feature_count(0)
	{
		FieldFactory* pFactory = augeGetFieldFactoryInstance();
		m_pFields = pFactory->CreateFields();
	}

	FeatureClassShp::~FeatureClassShp()
	{
		AUGE_SAFE_RELEASE(m_pFields);
		CloseSHPFile();
	}

	augeDataSetType FeatureClassShp::GetType()
	{
		return augeDataSetFeature;
	}
	
	const char*	FeatureClassShp::GetName()
	{
		return m_name.c_str();
	}

	const char*	FeatureClassShp::GetAlias()
	{
		return m_name.c_str();
	}

	const char* FeatureClassShp::GetUUID()
	{
		return "";
	}

	g_uint FeatureClassShp::GetSRID()
	{
		return m_srid;
	}

	GEnvelope& FeatureClassShp::GetExtent()
	{
		return m_extent;
	}

	g_uint FeatureClassShp::GetCount()
	{
		return m_pshpHandle->nRecords;
	}

	g_uint FeatureClassShp::GetCount(GEnvelope& extent)
	{
		return m_pshpHandle->nRecords;
	}

	g_uint FeatureClassShp::GetCount(GFilter* pFilter)
	{
		return m_pshpHandle->nRecords;
	}

	GFields* FeatureClassShp::GetFields()
	{
		return m_pFields;
	}

	GField* FeatureClassShp::GetField(const char* name)
	{
		return m_pFields->GetField(name);
	}

	RESULTCODE FeatureClassShp::AddField(const char* name, augeFieldType type, g_uint width)
	{
		return AG_FAILURE;
	}

	RESULTCODE FeatureClassShp::RemoveField(const char* name)
	{
		return AG_FAILURE;
	}

	bool FeatureClassShp::Create(const char* name, WorkspaceShp* pWorkspace)
	{
		if(!OpenSHPFile(name, pWorkspace))
		{
			return false;
		}

		m_name = name;
		m_pWorkspace = pWorkspace;

		if(!GetMetaData())
		{
			return false;
		}

		if(!CreateFields())
		{
			return false;
		}

		return true;
	}

	bool FeatureClassShp::Create(const char* name, WorkspaceShp* pWorkspace, SHPHandle pshpHandle, DBFHandle pdfHandle)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;
		m_pshpHandle = pshpHandle;
		m_pdbfHandle = pdfHandle;

		if(!GetMetaData())
		{
			return false;
		}

		if(!CreateFields())
		{
			return false;
		}

		return true;
	}

	bool FeatureClassShp::OpenSHPFile(const char* name, WorkspaceShp* pWorkspace)
	{
		GError* pError = augeGetErrorInstance();

		char shp_path[AUGE_PATH_MAX];
		char dbf_path[AUGE_PATH_MAX];
		const char* dir = pWorkspace->m_path.c_str();
#ifdef WIN32
		_makepath(shp_path, NULL, dir, name, "shp");
		_makepath(dbf_path, NULL, dir, name, "dbf");
#else
		sprintf(shp_path, "%s/%s.%s", dir, name, "shp");
		sprintf(dbf_path, "%s/%s.%s", dir, name, "dbf");
#endif

		CloseSHPFile();
		m_pshpHandle = SHPOpen(shp_path, "rb");
		if(m_pshpHandle==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Open SHP file [%s].", shp_path);
			pError->SetError(msg);
			return false;
		}
		m_pdbfHandle = DBFOpen(dbf_path, "rb");
		if(m_pdbfHandle==NULL)
		{
			SHPClose(m_pshpHandle);
			m_pshpHandle = NULL;

			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Open DBF file [%s].", dbf_path);
			pError->SetError(msg);
			
			return false;
		}

		return true;
	}

	void FeatureClassShp::CloseSHPFile()
	{
		if(m_pshpHandle!=NULL)
		{
			SHPClose(m_pshpHandle);
			m_pshpHandle = NULL;
		}
		if(m_pdbfHandle!=NULL)
		{
			DBFClose(m_pdbfHandle);
			m_pdbfHandle = NULL;
		}
	}

	bool FeatureClassShp::GetMetaData()
	{
		m_geom_type = GetGeometryType(m_pshpHandle->nShapeType);
		m_extent.Set(m_pshpHandle->adBoundsMin[0], m_pshpHandle->adBoundsMin[1], m_pshpHandle->adBoundsMax[0], m_pshpHandle->adBoundsMax[1]);
		m_feature_count = m_pshpHandle->nRecords;
		return true;
	}

	bool FeatureClassShp::CreateFields()
	{
		m_pFields->Clear();

		GField		*pField = NULL;
		GField_2	*pField_2 = NULL;
		FieldFactory* pFactory = augeGetFieldFactoryInstance();
		augeFieldType ftype = augeFieldTypeNone;


		DBFFieldType dbfType = FTInvalid;
		char name[AUGE_NAME_MAX] = {0};
		int width=0,decimal=0;

		int nfields = m_pdbfHandle->nFields;
		for(int i=0; i<nfields; i++)
		{
			dbfType = DBFGetFieldInfo(m_pdbfHandle, i, name, &width, &decimal);
			ftype = GetFieldType(dbfType);

			pField = pFactory->CreateField();
			pField_2 = pField->Field_2();
			pField_2->SetName(name);
			pField_2->SetAlias(name);
			pField_2->SetType(ftype);
			pField_2->SetLength(width);
			m_pFields->Add(pField);
		}

		pField = pFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("shape");
		pField_2->SetAlias("shape");
		pField_2->SetType(augeFieldTypeGeometry);

		GeometryDef	  *pGeometryDef = NULL;
		GeometryDef_2 *pGeometryDef_2 = NULL;
		pGeometryDef = pField->GetGeometryDef();
		pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetDefault(true);
		pGeometryDef_2->SetGeometryType(m_geom_type);
		pGeometryDef_2->SetM(false);
		pGeometryDef_2->SetZ(false);
		m_pFields->Add(pField);

		return true;
	}

	void FeatureClassShp::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	FeatureCursor* FeatureClassShp::Query(augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursorShp* pCursor = NULL;

		pCursor = new FeatureCursorShp();
		if(!pCursor->Create(this, m_pshpHandle, m_pdbfHandle))
		{
			pCursor->Release();
			pCursor = NULL;
		}
		return pCursor;
	}

	FeatureCursor* FeatureClassShp::Query(GEnvelope& extent, augeCursorType type/*=augeStaticCursor*/)
	{
		return Query();
	}

	FeatureCursor* FeatureClassShp::Query(GFilter* pFilter, augeCursorType type/*=augeStaticCursor*/)
	{
		return Query();
	}

	FeatureCursor* FeatureClassShp::Query(GQuery* pQuery, augeCursorType type/*=augeStaticCursor*/)
	{
		return Query();
	}

	//Cursor*	FeatureClassShp::GetRows()
	//{
	//	return NULL;
	//}

	augeGeometryType FeatureClassShp::GetGeometryType(int shpType)
	{
		switch(shpType)
		{
		case SHPT_POINT:
		case SHPT_POINTZ:
			return augeGTPoint;
		case SHPT_ARC:
		case SHPT_ARCZ:
			return augeGTMultiLineString;
		case SHPT_POLYGON:
		case SHPT_POLYGONZ:
			return augeGTMultiPolygon;
		case SHPT_MULTIPOINT:
			return augeGTMultiPoint;
		}

		return augeGTNull;
	}

	augeFieldType FeatureClassShp::GetFieldType(DBFFieldType dbfType)
	{
		switch(dbfType)
		{
		case FTString:
			return augeFieldTypeString;
		case FTInteger:
			return augeFieldTypeInt;
		case FTLong:
			return augeFieldTypeLong;
		case FTDouble:
			return augeFieldTypeDouble;
		case FTLogical:
			return augeFieldTypeBool;
		case FTDate:
			return augeFieldTypeTime;
		case FTInvalid:
			return augeFieldTypeNone;
		}

		return augeFieldTypeNone;
	}

	FeatureInsertCommand* FeatureClassShp::CreateInsertCommand()
	{
		return (new FeatureInsertCommandShp(this));
	}

	RESULTCODE FeatureClassShp::UpdateFeature(EnumString* pFieldNames, EnumValue* pValues, GFilter* pFilter)
	{
		return AG_FAILURE;
	}

	RESULTCODE FeatureClassShp::RemoveFeature(GFilter* pFilter)
	{
		return AG_FAILURE;
	}

	Feature* FeatureClassShp::NewFeature()
	{
		return NULL;
	}

	EnumValue* FeatureClassShp::GetUniqueValue(const char* filed, augeOrderType order/*=augeOrderAsc*/)
	{
		return NULL;
	}

	EnumValue* FeatureClassShp::GetMinMaxValue(const char* field)
	{
		return NULL;
	}

	RESULTCODE FeatureClassShp::Refresh()
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureClassShp::BuildSpatialIndex()
	{
		return AG_SUCCESS;
	}
}
