#ifndef __AUGE_CARTO_FACTORY_IMPL_H__
#define __AUGE_CARTO_FACTORY_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class CartoFactoryImpl : public CartoFactory
	{
	public:
		CartoFactoryImpl();
		virtual ~CartoFactoryImpl();
	public:
		virtual Map*			CreateMap();
		virtual FeatureLayer*	CreateFeatureLayer();
		virtual Canvas*			CreateCanvas2D(g_uint width, g_uint height);
	};
}

#endif //__AUGE_CARTO_FACTORY_IMPL_H__
