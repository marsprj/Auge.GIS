#ifndef __AUGE_MAP_IMPL_H__
#define __AUGE_MAP_IMPL_H__

#include "AugeCarto.h"
#include <string>
#include <map>
#include <vector>

namespace auge
{
	class MapImpl : public Map
	{
	public:
		MapImpl();
		virtual ~MapImpl();
	public:
		virtual const char*	GetName();
		virtual RESULTCODE	SetName(const char* szName);

		virtual const char*	GetURI();
		virtual RESULTCODE	SetURI(const char* uri);
		
		virtual GEnvelope&	GetExtent();
		virtual void		SetExtent(double xmin, double ymin, double xmax, double ymax);
		virtual g_int		GetSRID();
		virtual void		SetSRID(g_int srid);

		virtual g_uint		GetLayerCount();		
		virtual Layer*		GetLayer(g_uint i);
		virtual Layer*		GetLayer(const char* szName);

		virtual RESULTCODE	AddLayer(Layer* pLayer);

		

	private:
		void		Cleanup();

	private:
		std::string m_name;
		std::string m_uri;
		GEnvelope	m_exent;
		g_int		m_srid;
		//std::map<std::string, Layer*> m_layers;
		std::vector<Layer*> m_layers;
		
	};
}

#endif //__AUGE_MAP_IMPL_H__
