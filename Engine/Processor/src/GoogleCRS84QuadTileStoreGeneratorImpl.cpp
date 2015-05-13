#include "GoogleCRS84QuadTileStoreGeneratorImpl.h"
#include "AugeTile.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"

namespace auge
{
	GoogleCRS84QuadTileStoreGeneratorImpl::GoogleCRS84QuadTileStoreGeneratorImpl()
	{
		m_pMap = NULL;
		m_pWorkspace = NULL;
		m_start_level = 1;
		m_end_level = 1;
	}

	GoogleCRS84QuadTileStoreGeneratorImpl::~GoogleCRS84QuadTileStoreGeneratorImpl()
	{

	}

	void GoogleCRS84QuadTileStoreGeneratorImpl::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	void GoogleCRS84QuadTileStoreGeneratorImpl::SetTileWorkspace(TileWorkspace* pWorkspace)
	{
		m_pWorkspace = pWorkspace;
	}

	void GoogleCRS84QuadTileStoreGeneratorImpl::SetStartLevel(g_uint l)
	{
		m_start_level = l;
	}

	void GoogleCRS84QuadTileStoreGeneratorImpl::SetEndLevel(g_uint l)
	{
		m_end_level = l;
	}

	void GoogleCRS84QuadTileStoreGeneratorImpl::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;
	}

	RESULTCODE GoogleCRS84QuadTileStoreGeneratorImpl::Execute()
	{
		g_uint counter = 0;
		char t_path[AUGE_PATH_MAX] = {0};
		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
		GEnvelope viewer;

		auge::GColor bgColor(255,255,255,255);		
		for(g_uint l=m_start_level; l<=m_end_level; l++)
		{
			g_uint rows = m_pWorkspace->GetRows(l);
			g_uint cols = m_pWorkspace->GetCols(l);

			for(g_uint r=0; r<rows; r++)
			{
				for(g_uint c=0; c<cols; c++)
				{
					counter++;
					//printf("\r[%d]:%d-%d-%d",counter++,l,r,c);
					
					m_pWorkspace->GetExtent(viewer, l, r,c);
					m_pWorkspace->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

					pCanvas = pCartoFactory->CreateCanvas2D(256,256);
					pCanvas->SetViewer(viewer);
					//pCanvas->DrawBackground(bgColor);
					pCanvas->Draw(m_pMap);
					pCanvas->Save(t_path);
					pCanvas->Release();
				}
			}
		}

		return counter;
	}
}