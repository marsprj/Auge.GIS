#include "RasterFolderImpl.h"
#include "EnumRasterFolderImpl.h"
#include "WorkspacePgs.h"

namespace auge
{
	RasterFolderImpl::RasterFolderImpl()
	{
		m_user = -1;
		m_name = "/";
		m_path = "/";
		m_pWorkspace = NULL;
		m_pRasterDataset = new RasterDatasetImpl();
	}

	RasterFolderImpl::~RasterFolderImpl()
	{
		m_pWorkspace = NULL;
		m_pRasterDataset->Release();
	}


	const char*	RasterFolderImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	RasterFolderImpl::GetPath()
	{
		return m_path.c_str();
	}

	const char*	RasterFolderImpl::GetLocalPath()
	{
		return m_local_path.c_str();
	}

	EnumRasterFolder* RasterFolderImpl::GetFolders()
	{
		const char* format = "select gid,name,alias,path,parent from %s where parent=%d and user_id=%d";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), GetID(), m_user);

		GResultSet* pResult = m_pWorkspace->m_pgConnection_r.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		EnumRasterFolderImpl* pFolders = new EnumRasterFolderImpl();

		g_uint count = pResult->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			g_uint id = pResult->GetInt(i, 0);
			const char* name = pResult->GetString(i,1);
			const char* alias= pResult->GetString(i, 2);
			const char* path = pResult->GetString(i, 3);
			g_uint parent = pResult->GetInt(i, 4);

			RasterFolderImpl *pFolder = new RasterFolderImpl();
			pFolder->Create(id, name, alias, path, m_pWorkspace, m_user);
			pFolders->Add(pFolder);
		}
		
		pResult->Release();

		return pFolders;
	}

	RasterFolder* RasterFolderImpl::GetFolder(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		const char* format = "select gid,name,alias,path,parent from %s where name='%s' and parent=%d and user_id";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), name, GetID(), m_user);

		GResultSet* pResult = m_pWorkspace->m_pgConnection_r.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		g_uint count = pResult->GetCount();
		if(count==0)
		{
			pResult->Release();
			return NULL;
		}

		g_uint id = pResult->GetInt(0, 0);
		//const char* name = pResult->GetString(0,1);
		const char* alias= pResult->GetString(0, 2);
		const char* path = pResult->GetString(0, 3);
		g_uint parent = pResult->GetInt(0, 4);

		RasterFolderImpl *pFolder = new RasterFolderImpl();
		pFolder->Create(id, name, alias, path, m_pWorkspace, m_user);
		pResult->Release();

		return pFolder;
	}

	RasterFolder* RasterFolderImpl::CreateFolder(const char* name)
	{
		if((name==NULL)||(strlen(name)==0))
		{
			return NULL;
		}

		if(HasFolder(name))
		{
			return NULL;
		}

		char folder_path[AUGE_PATH_MAX];
		memset(folder_path, 0, AUGE_PATH_MAX);
		auge_make_path(folder_path, NULL, m_path.c_str(), name, NULL);
		char local_folder_path[AUGE_PATH_MAX];
		memset(local_folder_path, 0, AUGE_PATH_MAX);
		auge_make_path(local_folder_path, NULL, m_pWorkspace->GetRepository(), folder_path+1, NULL);

		g_int id = RegisterFolder(name, name, folder_path);
		if(id<0)
		{
			return NULL;
		}

		auge_mkdir(local_folder_path);

		RasterFolderImpl* pFolder = new RasterFolderImpl();
		pFolder->Create(id, name, name, folder_path, m_pWorkspace, m_user);
		return pFolder;
	}

	RESULTCODE RasterFolderImpl::RemoveFolder(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		
		if(!IsEmpty())
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_sprintf(msg, "Folder [%s] is not empty", m_path.c_str());
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return AG_FAILURE;
		}

		const char* format = "delete from %s where name='%s' and parent=%d and user_id=%d";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), name, GetID(), m_user);

		GConnection* pgConnection = m_pWorkspace->GetConnectionW();
		if(pgConnection==NULL)
		{
			return AG_FAILURE;
		}

		//return m_pWorkspace->m_pgConnection_r.ExecuteSQL(sql);
		return pgConnection->ExecuteSQL(sql);
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

		const char* format = "select count(*) from %s where name='%s' and parent=%d and user_id=%d";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), name, GetID(),m_user);

		GResultSet* pResult = m_pWorkspace->m_pgConnection_r.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		int count = pResult->GetInt(0, 0);
		pResult->Release();

		return (count>0);
	}

	bool RasterFolderImpl::IsEmpty()
	{
		if(!m_pRasterDataset->IsEmpty())
		{
			return false;
		}

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);
		const char* format = "select count(*) from %s where parent=%d and user_id=%d";
		g_snprintf(sql, AUGE_PATH_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), GetID(), m_user);

		GResultSet* pResult = m_pWorkspace->m_pgConnection_r.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		int count = pResult->GetInt(0, 0);
		pResult->Release();

		return (count==0);
	}

	RasterDataset* RasterFolderImpl::GetRasterDataset()
	{
		return m_pRasterDataset;
	}
	
	void RasterFolderImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void RasterFolderImpl::Create(g_uint id, const char* name, const char* alias, const char* path, WorkspacePgs* pWorkspace, g_int user_id)
	{
		m_id = id;
		m_user = user_id;
		m_name = name;
		m_alias = alias==NULL ? name : alias;
		m_path = path;
		m_pWorkspace = pWorkspace;

		char local_path[AUGE_PATH_MAX];
		memset(local_path, 0, AUGE_PATH_MAX);
		auge_make_path(local_path, NULL, m_pWorkspace->GetRepository(), path+1, NULL);
		m_local_path = local_path;

		m_pRasterDataset->Create(name, this, m_pWorkspace);
	}

	g_int RasterFolderImpl::RegisterFolder(const char* name, const char* alias, const char* path)
	{
		const char* format = "insert into %s (name,alias,path,parent,user_id) values('%s','%s','%s',%d,%d) returning gid";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_raster_folder_table.c_str(), name, alias, path, GetID(),m_user);

		GConnection* pgConnection = m_pWorkspace->GetConnectionW();
		if(pgConnection==NULL)
		{
			return -1;
		}
		GResultSet* pResult = pgConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		g_int id = pResult->GetInt(0,0);
		pResult->Release();
		return id;
	}
}
