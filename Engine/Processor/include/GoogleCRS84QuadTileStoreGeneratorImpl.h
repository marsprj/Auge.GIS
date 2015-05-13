#ifndef __AUGE_TILE_STORE_PROCESSOR_IMPL_H__
#define __AUGE_TILE_STORE_PROCESSOR_IMPL_H__

#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"

namespace auge
{
	class GoogleCRS84QuadTileStoreGeneratorImpl : public TileStoreGenerator
	{
	public:
		GoogleCRS84QuadTileStoreGeneratorImpl();
		virtual ~GoogleCRS84QuadTileStoreGeneratorImpl();
	public:
		virtual	void		SetMap(Map* pMap);
		virtual void		SetTileWorkspace(TileWorkspace* pWorkspace);
		virtual void		SetStartLevel(g_uint l);
		virtual void		SetEndLevel(g_uint l);
		virtual void		SetViewer(GEnvelope& viewer);
		virtual RESULTCODE	Execute();

	private:
		Map				*m_pMap;
		TileWorkspace	*m_pWorkspace;
		GEnvelope		m_viewer;
		g_uint			m_start_level;
		g_uint			m_end_level;
	};
}

#endif //__AUGE_TILE_STORE_PROCESSOR_IMPL_H__
