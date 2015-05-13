#ifndef __AUGE_PROCESSPOR_FACTORY_IMPL_H__
#define __AUGE_PROCESSPOR_FACTORY_IMPL_H__

#include "AugeProcessor.h"

namespace auge
{
	class GProcessorFactoryImpl : public GProcessorFactory
	{
	public:
		GProcessorFactoryImpl();
		virtual ~GProcessorFactoryImpl();
	public:
		virtual TileStoreGenerator*	CreateTileStoreGenerator();
	};
}



#endif //__AUGE_PROCESSPOR_FACTORY_IMPL_H__
