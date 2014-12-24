#include "WorkspaceShp.h"
#include "FeatureCursorShp.h"
#include "FeatureClassShp.h"

#include <AugeCore.h>

namespace auge
{
	FeatureClassShp::FeatureClassShp():
	m_pWorkspace(NULL),
	m_pSHPHandle(NULL),
	m_pDBFHandle(NULL)
	{

	}

	FeatureClassShp::~FeatureClassShp()
	{
		CloseSHPFile();
	}
	
	const char*	FeatureClassShp::GetName()
	{
		return m_name.c_str();
	}

	bool FeatureClassShp::Create(const char* szName, WorkspaceShp* pWorkspace)
	{
		if(szName==NULL||pWorkspace==NULL)
		{
			return false;
		}

		GError* pError = augeGetErrorInstance();

		char shp_path[AUGE_PATH_MAX];
		char dbf_path[AUGE_PATH_MAX];
		const char* dir = pWorkspace->m_path.c_str();
#ifdef WIN32
		_makepath(shp_path, NULL, dir, szName, "shp");
		_makepath(dbf_path, NULL, dir, szName, "dbf");
#else
		sprintf(szSHPPath, "%s/%s.%s", szDir, szName, szExt);
		sprintf(szDBFPath, "%s/%s.%s", szDir, szName, "DBF");
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

		m_pWorkspace = pWorkspace;
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
}