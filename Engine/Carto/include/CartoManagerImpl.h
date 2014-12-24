#ifndef __AUGE_MAP_MANAGER_IMPL_H__
#define __AUGE_MAP_MANAGER_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class CartoManagerImpl : public CartoManager
	{
	public:
		CartoManagerImpl();
		virtual ~CartoManagerImpl();
	public:
		virtual g_uint			GetCount();
		virtual	Map*			GetMap(g_uint mid);
		virtual	Map*			GetMap(const char* name);
		virtual Map*			LoadMap(g_uint mid);
		virtual Map*			LoadMap(const char* name);

		virtual Map*			CreateMap(const char* name);
		virtual Map*			RemoveMap(const char* name);

		virtual Layer*			CreateLayer(const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_id, g_uint style_id);

		virtual bool			HasStyle(g_uint id);
		virtual bool			HasStyle(const char* name);
		virtual g_uint			GetStyleCount();
		virtual Style*			GetStyle(g_uint id);
		virtual Style*			GetStyle(const char* name);
		virtual g_int			CreateStyle(const char* name, Style* pStyle);
		virtual RESULTCODE		UpdateStyle(const char* name, Style* pStyle);
		virtual RESULTCODE		RemoveStyle(const char* name);
		
	public:
		virtual	RESULTCODE		Initialize(GConnection* pConnection);
		virtual void			Cleanup();

	private:
		Layer*					CreateLayer(int id, const char* name, augeLayerType type, const char* f_name, g_int source_id, g_int version, bool visible);
		FeatureLayer*			CreateFeatureLayer(const char* name, const char* f_name,g_uint source_id);

	private:
		RESULTCODE				CreateMapTable();
		RESULTCODE				CreateLayerTable();
		RESULTCODE				CreateStyleTable();

	private:
		GConnection	*m_pConnection;
	};
}

#endif //__AUGE_MAP_MANAGER_IMPL_H__
