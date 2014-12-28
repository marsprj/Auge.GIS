#include "WorkspaceShp.h"
#include "FeatureCursorShp.h"
#include "FeatureClassShp.h"

#include <AugeCore.h>

namespace auge
{
	FeatureClassShp::FeatureClassShp():
	m_pWorkspace(NULL),
	m_pSHPHandle(NULL),
	m_pDBFHandle(NULL),
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
	
	const char*	FeatureClassShp::GetName()
	{
		return m_name.c_str();
	}

	g_uint FeatureClassShp::GetSRID()
	{
		return m_srid;
	}

	GEnvelope& FeatureClassShp::GetExtent()
	{
		return m_extent;
	}

	GFields* FeatureClassShp::GetFields()
	{
		return m_pFields;
	}

	GField* FeatureClassShp::GetField(const char* name)
	{
		return m_pFields->GetField(name);
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
		sprintf(szSHPPath, "%s/%s.%s", szDir, name, szExt);
		sprintf(szDBFPath, "%s/%s.%s", szDir, name, "DBF");
#endif

		CloseSHPFile();
		m_pSHPHandle = ::SHPOpen(shp_path, "rb");
		if(m_pSHPHandle==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Open SHP file [%s].", shp_path);
			pError->SetError(msg);
			return false;
		}
		m_pDBFHandle = ::DBFOpen(dbf_path, "rb");
		if(m_pSHPHandle==NULL)
		{
			::SHPClose(m_pSHPHandle);

			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Open DBF file [%s].", dbf_path);
			pError->SetError(msg);
			
			return false;
		}

		return true;
	}

	void FeatureClassShp::CloseSHPFile()
	{
		if(m_pSHPHandle!=NULL)
		{
			::SHPClose(m_pSHPHandle);
			m_pSHPHandle = NULL;
		}
		if(m_pDBFHandle!=NULL)
		{
			::DBFClose(m_pDBFHandle);
			m_pDBFHandle = NULL;
		}
	}

	bool FeatureClassShp::GetMetaData()
	{
		m_geom_type = GetGeometryType(m_pSHPHandle->nShapeType);
		m_extent.Set(m_pSHPHandle->adBoundsMin[0], m_pSHPHandle->adBoundsMin[1], m_pSHPHandle->adBoundsMax[0], m_pSHPHandle->adBoundsMax[1]);
		m_feature_count = m_pSHPHandle->nRecords;
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

		int nfields = m_pDBFHandle->nFields;
		for(int i=0; i<nfields; i++)
		{
			dbfType = ::DBFGetFieldInfo(m_pDBFHandle, i, name, &width, &decimal);
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
		if(!pCursor->Create(this, m_pSHPHandle, m_pDBFHandle))
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

	augeGeometryType FeatureClassShp::GetGeometryType(int shpType)
	{
		switch(shpType)
		{
		case 1:
			return augeGTPoint;
		case 3:
			return augeGTMultiLineString;
			break;		
		case 5:
			return augeGTMultiPolygon;
		case 8:
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
		return NULL;
	}
}