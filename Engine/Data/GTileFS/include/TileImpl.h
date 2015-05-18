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
		bool	Create(const char* path);
	private:
		std::string		m_key;
		std::string		m_path;
		g_uchar*		m_data;
		g_uint			m_size;
	};
}

#endif //__AUGE_TILE_IMPL_H__
