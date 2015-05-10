#ifndef __AUGE_RASTER_DATASET_IMPL_H__
#define __AUGE_RASTER_DATASET_IMPL_H__

#include "AugeData.h"

namespace auge
{
	class Raster;

	class RasterDatasetImpl : public RasterDataset
	{
	public:
		RasterDatasetImpl();
		virtual ~RasterDatasetImpl();
	public:
		virtual const char*		GetName();
		virtual const char*		GetAlias();
		virtual augeDataSetType	GetType();
		virtual g_uint			GetWidth();
		virtual g_uint			GetHeight();
		virtual g_uint			GetBandCount();
		virtual GEnvelope&		GetExtent();
		virtual g_int			GetSRID();
		virtual const char*		GetFormat();
		virtual const char*		GetPath();

		virtual Raster*			GetRaster();

	public:
		void	Create(Raster* pRaster);

	public:
		Raster*	m_pRaster;
	};
}

#endif //__AUGE_RASTER_DATASET_IMPL_H__
