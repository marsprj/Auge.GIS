#ifndef __AUGE_ENUM_TILESTORE_IMPL_H__
#define __AUGE_ENUM_TILESTORE_IMPL_H__

#include "AugeTile.h"
#include <vector>

namespace auge
{
	class EnumTileStoreImpl : public EnumTileStore
	{
	public:
		EnumTileStoreImpl();
		virtual ~EnumTileStoreImpl();
	public:
		virtual	void			Reset();
		virtual	TileStore*		Next();
		virtual void			Release();

	public:
		void	Add(TileStore* pTileStore);

	private:
		std::vector<TileStore*>				m_tile_stores;
		std::vector<TileStore*>::iterator	m_iter;
	};
}

#endif //__AUGE_ENUM_TILESTORE_IMPL_H__
