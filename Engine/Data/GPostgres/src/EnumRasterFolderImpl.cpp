#include "EnumRasterFolderImpl.h"

namespace auge
{
	EnumRasterFolderImpl::EnumRasterFolderImpl()
	{
		m_iter = m_RasterFolders.begin();
	}

	EnumRasterFolderImpl::~EnumRasterFolderImpl()
	{
		RasterFolder* pRasterFolder = NULL;
		std::vector<RasterFolder*>::iterator iter;
		for(iter=m_RasterFolders.begin(); iter!=m_RasterFolders.end(); iter++)
		{
			pRasterFolder = *iter;
			pRasterFolder->Release();
		}
		m_RasterFolders.clear();
	}

	void EnumRasterFolderImpl::Reset()
	{
		m_iter = m_RasterFolders.begin();
	}

	RasterFolder* EnumRasterFolderImpl::Next()
	{
		if(m_iter==m_RasterFolders.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	bool EnumRasterFolderImpl::Add(RasterFolder* pRasterFolder)
	{
		if(pRasterFolder)
		{
			m_RasterFolders.push_back(pRasterFolder);
		}
		return true;
	}

	void EnumRasterFolderImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}