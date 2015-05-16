#include "FileSytemTileStoreGeneratorImpl.h"
#include "AugeTile.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"

namespace auge
{
	FileSystemileStoreGeneratorImpl::FileSystemileStoreGeneratorImpl()
	{
		m_pMap = NULL;
		m_pTileStore = NULL;
		m_start_level = 1;
		m_end_level = 1;
	}

	FileSystemileStoreGeneratorImpl::~FileSystemileStoreGeneratorImpl()
	{

	}

	void FileSystemileStoreGeneratorImpl::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	void FileSystemileStoreGeneratorImpl::SetTileStore(TileStore* pTileStore)
	{
		m_pTileStore = pTileStore;
	}

	void FileSystemileStoreGeneratorImpl::SetStartLevel(g_uint l)
	{
		m_start_level = l;
	}

	void FileSystemileStoreGeneratorImpl::SetEndLevel(g_uint l)
	{
		m_end_level = l;
	}

	void FileSystemileStoreGeneratorImpl::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;
	}

	RESULTCODE FileSystemileStoreGeneratorImpl::Execute()
	{
		if(m_pMap==NULL)
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

		g_uint counter = 0;
		char t_path[AUGE_PATH_MAX] = {0};
		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
		GEnvelope viewer;
		RESULTCODE rc = AG_FAILURE;

		auge::GColor bgColor(255,255,255,255);		
		for(g_uint l=m_start_level; l<=m_end_level; l++)
		{
			g_uint rows = m_pTileStore->GetRows(l);
			g_uint cols = m_pTileStore->GetCols(l);

			for(g_uint r=0; r<rows; r++)
			{
				for(g_uint c=0; c<cols; c++)
				{
					rc = m_pTileStore->GetExtent(viewer, l, r,c);
					if(rc==AG_SUCCESS)
					{
						printf("\r[%d]:%d-%d-%d",counter++,l,r,c);

						m_pTileStore->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

						pCanvas = pCartoFactory->CreateCanvas2D(256,256);
						pCanvas->SetViewer(viewer);
						////pCanvas->DrawBackground(bgColor);
						pCanvas->Draw(m_pMap);
						pCanvas->Save(t_path);
						pCanvas->Release();
					}
				}
			}
		}

		return counter;
	}
}