#ifndef __AUGE_WMTS_LAYER_IMPL_H__
#define __AUGE_WMTS_LAYER_IMPL_H__

#include "AugeCarto.h"
#include <string>

namespace auge
{
	class WMTSLayerImpl : public WMTSLayer
	{
	public:
		WMTSLayerImpl();
		virtual ~WMTSLayerImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);
		virtual augeLayerType	GetType();

		virtual g_int			GetSRID();
		virtual GEnvelope&		GetExtent();

		virtual bool			IsQueryable();
		virtual void			SetQueryable(bool flag);

		virtual bool			IsVisiable();
		virtual bool			IsVisiable(double scale);
		virtual void			SetVisiable(bool flag);

		virtual	double			GetMinScale();
		virtual	void			SetMinScale(double scale);

		virtual	double			GetMaxScale();
		virtual	void			SetMaxScale(double scale);

		virtual const char*		GetURL();
		virtual RESULTCODE		SetURL(const char* url);

	private:
		std::string m_name;
		std::string m_url;

		GEnvelope	m_extent;
		g_int		m_srid;
		bool		m_visible;

		double			m_min_scale;
		double			m_max_scale;
	};
}

#endif //__AUGE_WMTS_LAYER_IMPL_H__
