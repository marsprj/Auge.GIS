#ifndef __AUGE_MAP_MANAGER_H__
#define __AUGE_MAP_MANAGER_H__

#include "Base.h"
#include <vector>

namespace auge
{
	class Map;
	class Layer;
	class FeatureLayer;
	class Workspace;

	class AUGE_API MapManager
	{	
	public:
		~MapManager();

	private:
		MapManager();
	public:
		static MapManager*	GetInstance();
	private:
		static MapManager*	m_pInstance;

	public:
		std::vector<Map*>&	GetMaps();
		Map*				GetMap(const char* szName);
		uint				GetMapCount();

		int					AddMap(Map* pMap);
		int					RemoveMap(const char* szName);
		bool				FindMap(const char* szName);
		int					GetMapID(const char* szName);

		int					AddLayer(Layer* pLayer);
		int					RemoveLayer(Layer* pLayer);
		int					RemoveLayer(int mapID, const char* szLayerName);
		int					RemoveLayer(const char* szMapName, const char* szLayerName);

		int					Initialize();
		void				SetConnection(Workspace* pConnection);

	private:

		int					CreateMap(Map* pMap);
		int					AddLayer(FeatureLayer* pLayer);
		int					WriteLayer(FeatureLayer* pLayer);
		int					RemoveLayers(int mapID);

		bool				Initialized();
		void				Cleanup();

	private:
		std::vector<Map*>	m_maps;

		Workspace* m_pConnection;

	};
}

#endif //__AUGE_MAP_MANAGER_H__
