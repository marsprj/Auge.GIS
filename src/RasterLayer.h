#ifndef __AUGE_RASTER_LAYER_H__
#define __AUGE_RASTER_LAYER_H__

#include "Base.h"

namespace auge
{
	class Layer;

	class AUGE_API RasterLayer : public Layer
	{
	public:
		RasterLayer();
		virtual ~RasterLayer();
	};

}//namespace

#endif //__AUGE_RASTER_LAYER_H__