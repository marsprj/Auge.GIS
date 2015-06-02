#ifndef __AUGE_RASTER_TILE_STORE_PROCESSOR_IMPL_H__
#define __AUGE_RASTER_TILE_STORE_PROCESSOR_IMPL_H__

#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"

namespace auge
{
	class Raster;

	class RasterTileStoreGeneratorFS : public RasterTileStoreGenerator
	{
	public:
		RasterTileStoreGeneratorFS();
		virtual ~RasterTileStoreGeneratorFS();
	public:
		virtual	void		SetRaster(Raster* pRaster);
		virtual void		SetFormat(const char* format);

		virtual void		SetTileStore(TileStore* pTileStore);
		virtual void		SetStartLevel(g_uint l);
		virtual void		SetEndLevel(g_uint l);

		virtual void		SetViewer(GEnvelope& viewer);

		virtual RESULTCODE	Execute();

	private:
		Raster*			m_pRaster;
		TileStore*		m_pTileStore;
		GEnvelope		m_viewer;
		g_uint			m_start_level;
		g_uint			m_end_level;
		std::string		m_format;
	};
}

#endif //__AUGE_RASTER_TILE_STORE_PROCESSOR_IMPL_H__
