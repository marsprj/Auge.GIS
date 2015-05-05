#ifndef __AUGE_MAP_MANAGER_IMPL_H__
#define __AUGE_MAP_MANAGER_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class GResultSet;

	class CartoManagerImpl : public CartoManager
	{
	public:
		CartoManagerImpl();
		virtual ~CartoManagerImpl();
	public:
		//------------------------------------------------------------------------
		// Map Methods
		//------------------------------------------------------------------------
		virtual g_uint			GetCount();
		virtual	Map*			GetMap(g_uint mid);
		virtual	Map*			GetMap(const char* name);
		virtual EnumMap*		GetMaps();
		virtual Map*			LoadMap(g_uint mid);
		virtual Map*			LoadMap(const char* name);

		virtual Map*			CreateMap(const char* name);
		virtual Map*			CreateMap(const char* name, g_uint srid, double xmin, double ymin, double xmax, double ymax);
		virtual RESULTCODE		RemoveMap(const char* name);
		virtual g_int			GetMapID(const char* name);

		//------------------------------------------------------------------------
		// Map User Methods
		//------------------------------------------------------------------------
		virtual g_uint			GetCount(g_uint user);
		virtual	Map*			GetMap(g_uint user, const char* mapName);
		virtual EnumMap*		GetMaps(g_uint user);
		virtual Map*			LoadMap(g_uint user, const char* mapName);

		virtual Map*			CreateMap(g_uint user, const char* name);
		virtual Map*			CreateMap(g_uint user, const char* name, g_uint srid, double xmin, double ymin, double xmax, double ymax);
		virtual RESULTCODE		RemoveMap(g_uint user, const char* name);
		virtual g_int			GetMapID(g_uint user, const char* name);
		
		virtual Layer*			CreateLayer(const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_id, g_uint style_id);
		virtual RESULTCODE		RemoveLayers(const char* mapName);
		virtual RESULTCODE		RemoveLayers(g_uint map_id);
		virtual RESULTCODE		RemoveLayer(const char* mapName, const char* layerName);
		virtual g_int			GetLayerID(const char* layerName, const char* mapName);

		virtual bool			HasStyle(g_uint id);
		virtual bool			HasStyle(const char* name);
		virtual g_uint			GetStyleCount();
		virtual Style*			GetStyle(g_uint id, FeatureClass* pFeatureClass);
		virtual Style*			GetStyle(const char* name, FeatureClass* pFeatureClass);
		virtual char*			GetStyleText(g_uint id);
		virtual char*			GetStyleText(const char* name);
		virtual EnumStyle*		GetStyles();
		virtual bool			IsStyleUpdated(Style* pStyle);
		
		virtual g_int			CreateStyle(const char* name, Style* pStyle,    augeGeometryType type);
		virtual RESULTCODE		CreateStyle(const char* name, const char* text, augeGeometryType type);

		virtual RESULTCODE		UpdateStyle(const char* name, Style* pStyle);
		virtual RESULTCODE		UpdateStyle(const char* name, const char* text);
		virtual RESULTCODE		RemoveStyle(const char* name);
		virtual g_int			GetStyleID(const char* name);

		virtual RESULTCODE		SetStyle(g_uint layerID, g_uint styleID);

		virtual	EnumColorMap*	GetColorMaps();
					
	public:
		virtual	RESULTCODE		Initialize(GConnection* pConnection);
		virtual void			Cleanup();

	private:
		Layer*					CreateLayer(int id, const char* name, augeLayerType type, const char* f_name, g_int source_id, g_int version, bool visible);
		FeatureLayer*			CreateFeatureLayer(const char* name, const char* f_name,g_uint source_id);
		bool					HasLayer(const char* layerName, int mapID);

		bool					IsValiad(const char* text);
		Map*					CreateMap(GResultSet* pResult, int row);

	private:
		RESULTCODE				CreateMapTable();
		RESULTCODE				CreateLayerTable();
		RESULTCODE				CreateStyleTable();
		RESULTCODE				CreateColorMapTable();
		RESULTCODE				InitColorMap();

	private:
		GConnection	*m_pConnection;
	};
}

#endif //__AUGE_MAP_MANAGER_IMPL_H__
