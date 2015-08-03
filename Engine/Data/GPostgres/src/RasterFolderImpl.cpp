#include "RasterFolderImpl.h"
#include "WorkspacePgs.h"

namespace auge
{
	RasterFolderImpl::RasterFolderImpl()
	{
		m_name = "/";
		m_path = "/";
		m_pWoskspace = NULL;
	}

	RasterFolderImpl::~RasterFolderImpl()
	{
		m_pWoskspace = NULL;
	}


	const char*	RasterFolderImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	RasterFolderImpl::GetPath()
	{
		return m_path.c_str();
	}

	EnumRasterFolder* RasterFolderImpl::GetFolders()
	{
		return NULL;
	}

	RasterFolder* RasterFolderImpl::CreateFolder(const char* name)
	{
		if((name==NULL)||(strlen(name)==0))
		{
			return NULL;
		}

		char folder_path[AUGE_PATH_MAX];
		memset(folder_path, 0, AUGE_PATH_MAX);
		auge_make_path(folder_path, NULL, m_path.c_str(), name+1, NULL);
		char local_folder_path[AUGE_PATH_MAX];
		memset(local_folder_path, 0, AUGE_PATH_MAX);
		auge_make_path(local_folder_path, NULL, m_pWoskspace->GetRepository(), folder_path+1, NULL);

		g_int id = RegisterFolder(name, name, folder_path);
		if(id<0)
		{
			return NULL;
		}

		auge_mkdir(local_folder_path);

		RasterFolderImpl* pFolder = new RasterFolderImpl();
		pFolder->Create(id, name, folder_path, m_pWoskspace);
		return pFolder;
	}

	RESULTCODE RasterFolderImpl::RemoveFolder(const char* name)
	{
		return AG_FAILURE;
	}

	RESULTCODE RasterFolderImpl::RenameFolder(const char* name)
	{
		return AG_FAILURE;
	}

	bool RasterFolderImpl::HasFolder(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		return false;
	}

	RasterDataset* RasterFolderImpl::GetRasterDataset()
	{
		return NULL;
	}

	void RasterFolderImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void RasterFolderImpl::Create(g_uint id, const char* name, const char* path, WorkspacePgs* pWorkspace)
	{
		m_id = id;
		m_name = name;
		m_path = path;
		m_pWoskspace = pWorkspace;
	}

	g_int RasterFolderImpl::RegisterFolder(const char* name, const char* alias, const char* path)
	{
		const char* format = "insert into %s (name,alias,path,parent) values('%s','%s','%s',%d) returning gid";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWoskspace->g_raster_folder_table.c_str(), name, alias, path, GetID());

		GResultSet* pResult = m_pWoskspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		g_int id = pResult->GetInt(0,0);
		pResult->Release();
		return id;
	}
}
