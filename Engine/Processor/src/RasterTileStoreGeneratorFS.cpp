#include "RasterTileStoreGeneratorFS.h"
#include "AugeTile.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"
#include "AugeRaster.h"
#include "AugeUser.h"

namespace auge
{
	RasterTileStoreGeneratorFS::RasterTileStoreGeneratorFS()
	{
		m_pRaster = NULL;
		m_pTileStore = NULL;
		m_start_level = 1;
		m_end_level = 1;
		m_format = "png";

		m_pUser = NULL;
	}

	RasterTileStoreGeneratorFS::~RasterTileStoreGeneratorFS()
	{

	}

	void RasterTileStoreGeneratorFS::SetFormat(const char* format)
	{
		if(format)
		{
			m_format = format;
		}
		else
		{
			m_format.clear();
		}
	}

	void RasterTileStoreGeneratorFS::SetRaster(Raster* pRaster)
	{
		m_pRaster = pRaster;
	}

	void RasterTileStoreGeneratorFS::SetTileStore(TileStore* pTileStore)
	{
		m_pTileStore = pTileStore;
	}

	void RasterTileStoreGeneratorFS::SetStartLevel(g_uint l)
	{
		m_start_level = l;
	}

	void RasterTileStoreGeneratorFS::SetEndLevel(g_uint l)
	{
		m_end_level = l;
	}

	void RasterTileStoreGeneratorFS::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;
	}

	RESULTCODE RasterTileStoreGeneratorFS::Execute()
	{
		if(m_pRaster==NULL)
		{
			return AG_FAILURE;
		}
		if(m_start_level<=0||m_end_level<=0)
		{
			return AG_FAILURE;
		}
		if(m_start_level>m_end_level)
		{
			return AG_FAILURE;
		}
		if(m_pTileStore==NULL)
		{
			return AG_FAILURE;
		}

		g_uint o_level = m_pTileStore->GetOriginalLevel(m_pRaster->GetExtent(), m_pRaster->GetWidth(), m_pRaster->GetHeight());
		if(m_start_level>o_level)
		{
			return AG_FAILURE;
		}

		g_uint counter = 0;
		char t_path[AUGE_PATH_MAX] = {0};
		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
		GEnvelope viewer;
		RESULTCODE rc = AG_FAILURE;

		GEnvelope& r_extent = m_pRaster->GetExtent();
		GEnvelope  out_bound;
		
		for(g_uint l=m_start_level; l>=o_level; l--)
		{
			//g_uint rows = m_pTileStore->GetRows(l);
			//g_uint cols = m_pTileStore->GetCols(l);

			//for(g_uint r=0; r<rows; r++)
			//{
			//	for(g_uint c=0; c<cols; c++)
			//	{
			//		rc = m_pTileStore->GetExtent(viewer, l, r,c);
			//		if(rc==AG_SUCCESS)
			//		{
			//			printf("\r[%d]:%d-%d-%d",counter++,l,r,c);

			//			m_pTileStore->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

			//			pCanvas = pCartoFactory->CreateCanvas2D(256,256);
			//			pCanvas->SetViewer(viewer);
			//			////pCanvas->DrawBackground(bgColor);
			//			pCanvas->Draw(m_pMap);
			//			pCanvas->Save(t_path);
			//			pCanvas->Release();
			//		}
			//	}
			//}
		}

		return counter;
	}

	void RasterTileStoreGeneratorFS::SetUser(User* pUser)
	{
		m_pUser = pUser;
	}
}