#include "EnumTileStoreImpl.h"

namespace auge
{
	EnumTileStoreImpl::EnumTileStoreImpl()
	{
		m_iter = m_tile_stores.begin();
	}

	EnumTileStoreImpl::~EnumTileStoreImpl()
	{
		std::vector<TileStore*>::iterator iter;
		for(iter=m_tile_stores.begin(); iter!=m_tile_stores.end(); iter++)
		{
			(*iter)->Release();
		}
		m_tile_stores.clear();
	}

	void EnumTileStoreImpl::Reset()
	{
		m_iter = m_tile_stores.begin();
	}

	TileStore* EnumTileStoreImpl::Next()
	{
		if(m_iter==m_tile_stores.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumTileStoreImpl::Release()
	{
		delete this;
	}

	void EnumTileStoreImpl::Add(TileStore* pTileStore)
	{
		if(pTileStore!=NULL)
		{
			m_tile_stores.push_back(pTileStore);
		}
	}

}