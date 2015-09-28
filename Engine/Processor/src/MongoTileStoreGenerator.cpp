#include "MongoTileStoreGenerator.h"
#include "AugeTile.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	MongoTileStoreGenerator::MongoTileStoreGenerator()
	{
		m_pMap = NULL;
		m_pTileStore = NULL;
		m_start_level = 1;
		m_end_level = 1;

		m_user = 0;
	}

	MongoTileStoreGenerator::~MongoTileStoreGenerator()
	{

	}

	void MongoTileStoreGenerator::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	void MongoTileStoreGenerator::SetTileStore(TileStore* pTileStore)
	{
		m_pTileStore = pTileStore;
	}

	void MongoTileStoreGenerator::SetStartLevel(g_uint l)
	{
		m_start_level = l;
	}

	void MongoTileStoreGenerator::SetEndLevel(g_uint l)
	{
		m_end_level = l;
	}

	void MongoTileStoreGenerator::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;
	}

	RESULTCODE MongoTileStoreGenerator::Execute()
	{
		if(m_pMap==NULL)
		{
			return AG_FAILURE;
		}
		if(m_start_level<0||m_end_level<=0)
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
		char t_uuid[AUGE_PATH_MAX] = {0};
		char t_path[AUGE_PATH_MAX] = {0};
		char t_cwd[AUGE_PATH_MAX] = {0};
		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();		
		GEnvelope viewer;
		RESULTCODE rc = AG_FAILURE;

		auge_get_cwd(t_cwd, AUGE_PATH_MAX);
		auge_generate_uuid(t_uuid, AUGE_PATH_MAX);
		auge_make_path(t_path, NULL, t_cwd, t_uuid,"png");

		auge::GColor bgColor(255,255,255,255);		
		for(g_uint l=m_start_level; l<=m_end_level; l++)
		{
			g_uint rows = m_pTileStore->GetRows(l);
			g_uint cols = m_pTileStore->GetCols(l);

			for(g_uint r=0; r<rows; r++)
			{
				for(g_uint c=0; c<cols; c++)
				{
					rc = m_pTileStore->GetTileExtent(viewer, l, r,c);
					if(rc==AG_SUCCESS)
					{
						printf("\r[%d]:%d-%d-%d",counter++,l,r,c);

						pCanvas = pCartoFactory->CreateCanvas2D(256,256);
						pCanvas->SetViewer(viewer);
						//pCanvas->DrawBackground(bgColor);
						pCanvas->Draw(m_pMap);
						pCanvas->Save(t_path);
						pCanvas->Release();

						m_pTileStore->PutTile(l,r,c,t_path);
					}
				}
			}
		}

		return counter;
	}

	void MongoTileStoreGenerator::SetUser(g_uint user)
	{
		m_user = user;
	}
}