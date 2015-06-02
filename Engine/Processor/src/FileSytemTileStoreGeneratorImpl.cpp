#include "FileSytemTileStoreGeneratorImpl.h"
#include "AugeTile.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"

namespace auge
{
	FileSystemTileStoreGeneratorImpl::FileSystemTileStoreGeneratorImpl()
	{
		m_pMap = NULL;
		m_pTileStore = NULL;
		m_start_level = 1;
		m_end_level = 1;
	}

	FileSystemTileStoreGeneratorImpl::~FileSystemTileStoreGeneratorImpl()
	{

	}

	void FileSystemTileStoreGeneratorImpl::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	void FileSystemTileStoreGeneratorImpl::SetTileStore(TileStore* pTileStore)
	{
		m_pTileStore = pTileStore;
	}

	void FileSystemTileStoreGeneratorImpl::SetStartLevel(g_uint l)
	{
		m_start_level = l;
	}

	void FileSystemTileStoreGeneratorImpl::SetEndLevel(g_uint l)
	{
		m_end_level = l;
	}

	void FileSystemTileStoreGeneratorImpl::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;
	}

	RESULTCODE FileSystemTileStoreGeneratorImpl::Execute()
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
			g_uint64 row_blocks = m_pTileStore->GetBlockRows(l);
			g_uint64 col_blocks = m_pTileStore->GetBlockCols(l);

			for(g_uint r=0; r<row_blocks; r++)
			{
				for(g_uint c=0; c<col_blocks; c++)
				{
					TileBlock* pBlock = m_pTileStore->GetBlock(l, r, c);
					if(pBlock==NULL)
					{
						break;
					}

					BuildBlock(pBlock);

					pBlock->Release();
					//rc = m_pTileStore->GetTileExtent(viewer, l, r,c);
					//if(rc==AG_SUCCESS)
					//{
					//	printf("\r[%d]:%d-%d-%d",counter++,l,r,c);

					//	m_pTileStore->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

					//	pCanvas = pCartoFactory->CreateCanvas2D(256,256);
					//	pCanvas->SetViewer(viewer);
					//	////pCanvas->DrawBackground(bgColor);
					//	pCanvas->Draw(m_pMap);
					//	pCanvas->Save(t_path);
					//	pCanvas->Release();
					//}
				}
			}
		}

		return counter;
	}

	RESULTCODE FileSystemTileStoreGeneratorImpl::BuildBlock(TileBlock* pBlock)
	{
		g_uint64 nRow = pBlock->GetRows();
		g_uint64 nCol = pBlock->GetCols();

		GEnvelope& block_extent = pBlock->GetExtent();
		GEnvelope  view_extent  = block_extent;

		double tile_width = block_extent.GetWidth() / nCol;
		double tile_height= block_extent.GetHeight()/ nRow;

		view_extent.m_xmin -= tile_width;
		view_extent.m_xmax += tile_width;
		view_extent.m_ymin -= tile_height;
		view_extent.m_ymax += tile_height;

		g_int	canvas_width = AUGE_TILE_BLOCK_SIZE + (AUGE_TILE_SIZE << 1);
		g_int	canvas_height= AUGE_TILE_BLOCK_SIZE + (AUGE_TILE_SIZE << 1);

		RESULTCODE rc = AG_FAILURE;
		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
		
		pCanvas = pCartoFactory->CreateCanvas2D(canvas_width,canvas_height);
		pCanvas->SetViewer(view_extent);
		////pCanvas->DrawBackground(bgColor);
		pCanvas->Draw(m_pMap);
		
		char t_path[AUGE_PATH_MAX] = {0};
		for(g_uint64 r=0; r<nRow; r++)
		{
			for(g_uint64 c=0; c<nCol; c++)
			{

			}
		}
		pCanvas->Release();

		return AG_FAILURE;
	}

	//RESULTCODE FileSystemTileStoreGeneratorImpl::Execute()
	//{
	//	if(m_pMap==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	if(m_start_level<=0||m_end_level<=0)
	//	{
	//		return AG_FAILURE;
	//	}
	//	if(m_start_level>m_end_level)
	//	{
	//		return AG_FAILURE;
	//	}
	//	if(m_pTileStore==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	g_uint counter = 0;
	//	char t_path[AUGE_PATH_MAX] = {0};
	//	Canvas* pCanvas = NULL;
	//	CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
	//	GEnvelope viewer;
	//	RESULTCODE rc = AG_FAILURE;

	//	auge::GColor bgColor(255,255,255,255);		
	//	for(g_uint l=m_start_level; l<=m_end_level; l++)
	//	{
	//		g_uint rows = m_pTileStore->GetRows(l);
	//		g_uint cols = m_pTileStore->GetCols(l);

	//		for(g_uint r=0; r<rows; r++)
	//		{
	//			for(g_uint c=0; c<cols; c++)
	//			{
	//				rc = m_pTileStore->GetTileExtent(viewer, l, r,c);
	//				if(rc==AG_SUCCESS)
	//				{
	//					printf("\r[%d]:%d-%d-%d",counter++,l,r,c);

	//					m_pTileStore->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

	//					pCanvas = pCartoFactory->CreateCanvas2D(AUGE_TILE_SIZE,AUGE_TILE_SIZE);
	//					pCanvas->SetViewer(viewer);
	//					////pCanvas->DrawBackground(bgColor);
	//					pCanvas->Draw(m_pMap);
	//					pCanvas->Save(t_path);
	//					pCanvas->Release();
	//				}
	//			}
	//		}
	//	}

	//	return counter;
	//}
}