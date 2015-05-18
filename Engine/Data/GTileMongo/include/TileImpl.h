#ifndef __AUGE_TILE_IMPL_H__
#define __AUGE_TILE_IMPL_H__

#include "AugeTile.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class TileWorkspaceFS;

	class TileImpl : public Tile
	{
	public:
		TileImpl();
		virtual ~TileImpl();
	public:
		virtual const char*		GetKey();
		virtual	g_uchar*		GetData();
		virtual g_uint			GetSize();
	public:
		void	SetData(g_uchar* data);
		void	SetSize(g_uint size);
		void	SetKey(const char* key);
		
	private:
		std::string		m_key;
		g_uchar*		m_data;
		g_uint			m_size;
	};
}

#endif //__AUGE_TILE_IMPL_H__
