#ifndef __AUGE_ENUM_RASTER_IMPL_H__
#define __AUGE_ENUM_RASTER_IMPL_H__

#include "AugeData.h"
#include <vector>

namespace auge
{
	class EnumRasterImpl : public EnumRaster
	{
	public:
		EnumRasterImpl();
		virtual ~EnumRasterImpl();
	public:
		virtual void			Reset();
		virtual Raster*			Next();
		virtual bool			Add(Raster* pRaster);
		virtual void			Release();

	private:
		std::vector<Raster*>	m_rasters;
		std::vector<Raster*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_RASTER_IMPL_H__
