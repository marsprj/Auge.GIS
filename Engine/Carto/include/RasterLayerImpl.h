#ifndef __AUGE_RASTER_LAYER_IMPL_H__
#define __AUGE_RASTER_LAYER_IMPL_H__

#include "AugeCarto.h"
#include <string>

namespace auge
{
	class RasterLayerImpl : public RasterLayer
	{
	public:
		RasterLayerImpl();
		virtual ~RasterLayerImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);
		virtual augeLayerType	GetType();

		virtual g_int			GetSRID();
		virtual GEnvelope&		GetExtent();

		virtual bool			IsQueryable();
		virtual void			SetQueryable(bool flag);

		virtual bool			IsVisiable();
		virtual void			SetVisiable(bool flag);

		virtual RESULTCODE		SetStyle(Style* pStyle);
		virtual Style*			GetStyle();

		virtual void			Release();

	private:
		std::string	m_name;
		GEnvelope	m_extent;
		g_int		m_srid;
		bool		m_visiable;
		Style*		m_pStyle;
	};
}

#endif //__AUGE_RASTER_LAYER_IMPL_H__
