#ifndef __AUGE_MAP_MANAGER_H__
#define __AUGE_MAP_MANAGER_H__

#include "Base.h"
#include <vector>

namespace auge
{
	class Map;
	class Layer;
	class FeatureLayer;
	class Connection;

	class AUGE_API MapIO
	{	
	public:
		~MapIO();

	private:
		MapIO();
	public:
		static MapIO*		GetInstance();
	private:
		static MapIO*		m_pInstance;

	public:
		/* Map IO */
		Map*				LoadMap(const char* szName);
		int					SaveMap(Map* pMap);
		int					RemoveMap(const char* szName);

		std::vector<Map*>&	GetMaps();		
		uint				GetMapCount();
		bool				HasMap(const char* szName);
		int					GetMapID(const char* szName);

		/* Layer IO */
		Layer*				LoadLayer(int mapID, const char* szLayerName);
		int					SaveLayer(Layer* pLayer);
		int					RemoveLayer(Layer* pLayer);
		int					RemoveLayer(int mapID, const char* szLayerName);
		int					RemoveLayer(const char* szMapName, const char* szLayerName);

	public:
		int					Initialize();
		void				SetConnection(Connection* pConnection);

	private:

		int					CreateMap(Map* pMap);
		Map*				GetMap2(const char* szName);

		int					SaveLayer(FeatureLayer* pLayer);
		int					WriteLayer(FeatureLayer* pLayer);
		FeatureLayer*		LoadFeatureLayer(int id, const char* name, const char* fname, int sid);
		
		int					LoadLayers(Map* pMap);
		int					RemoveLayers(int mapID);

		bool				Initialized();
		void				Cleanup();

	private:
		std::vector<Map*>	m_maps;

		Connection* m_pConnection;

	};
}

#endif //__AUGE_MAP_MANAGER_H__
