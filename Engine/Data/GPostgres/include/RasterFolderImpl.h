#ifndef __AUGE_RASTER_FOLDER_IMPL_H__
#define __AUGE_RASTER_FOLDER_IMPL_H__

#include "AugeData.h"
#include "RasterDatasetImpl.h"

namespace auge
{
	class Raster;
	class WorkspacePgs;

	class RasterFolderImpl : public RasterFolder
	{
	public:
		RasterFolderImpl();
		virtual ~RasterFolderImpl();
	public:
		virtual const char*			GetName();
		virtual EnumRasterFolder*	GetFolders();
		virtual RasterFolder*		GetFolder(const char* name);
		virtual RasterFolder*		CreateFolder(const char* name);
		virtual RESULTCODE			RemoveFolder(const char* name);
		virtual RESULTCODE			RenameFolder(const char* name);

		virtual bool				HasFolder(const char* name);
		virtual bool				IsEmpty();

		virtual RasterDataset*		GetRasterDataset();

		virtual const char*			GetPath();
		virtual const char*			GetLocalPath();

		virtual void				Release();

	public:
		
		void			Create(g_uint id, const char* name, const char* alias, const char* path, WorkspacePgs* pWorkspace);

	private:
		g_int			RegisterFolder(const char* name, const char* alias, const char* path);

	private:
		std::string		m_name;
		std::string		m_alias;
		std::string		m_path;
		std::string		m_local_path;

		WorkspacePgs*	m_pWorkspace;
		RasterDatasetImpl* m_pRasterDataset;
	};
}

#endif //__AUGE_RASTER_FOLDER_IMPL_H__
