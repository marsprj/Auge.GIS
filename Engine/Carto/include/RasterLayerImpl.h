#ifndef __AUGE_RASTER_LAYER_IMPL_H__
#define __AUGE_RASTER_LAYER_IMPL_H__

#include "AugeCarto.h"
#include <string>

namespace auge
{
	class RasterDataset;

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
		virtual bool			IsVisiable(double scale);
		virtual void			SetVisiable(bool flag);

		virtual	double			GetMinScale();
		virtual	void			SetMinScale(double scale);

		virtual	double			GetMaxScale();
		virtual	void			SetMaxScale(double scale);

		virtual RESULTCODE		SetStyle(Style* pStyle);
		virtual Style*			GetStyle();

		virtual RESULTCODE		SetRaster(Raster* pRaster);
		virtual RESULTCODE		SetRaster(Raster* pRaster, RasterDataset* pRasterDataset);
		virtual Raster*			GetRaster();

		virtual RESULTCODE		SetRasterDataset(RasterDataset* pRasterDataset);
		virtual RasterDataset*	GetRasterDataset();

		virtual void			Release();

	private:
		std::string	m_name;
		GEnvelope	m_extent;
		g_int		m_srid;
		bool		m_visiable;
		Style*		m_pStyle;

		double			m_min_scale;
		double			m_max_scale;

		Raster*		m_pRaster;
		RasterDataset* m_pRasterDataset;
	};
}

#endif //__AUGE_RASTER_LAYER_IMPL_H__
