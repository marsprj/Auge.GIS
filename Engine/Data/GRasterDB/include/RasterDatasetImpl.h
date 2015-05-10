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
		virtual augeDataSetType	GetType();
		virtual Raster*			GetRaster();

	public:
		void	Create(Raster* pRaster);

	public:
		Raster*	m_pRaster;
	};
}

#endif //__AUGE_RASTER_DATASET_IMPL_H__
