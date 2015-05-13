#include "ProcessorFactoryImpl.h"
#include "GoogleCRS84QuadTileStoreGeneratorImpl.h"

namespace auge
{
	GProcessorFactory*	augeGetGeoProcessorFactoryInstance()
	{
		static GProcessorFactoryImpl g_processor_factory;
		return &g_processor_factory;
	}

	GProcessorFactoryImpl::GProcessorFactoryImpl()
	{

	}

	GProcessorFactoryImpl::~GProcessorFactoryImpl()
	{

	}

	TileStoreGenerator* GProcessorFactoryImpl::CreateTileStoreGenerator()
	{
		return (new GoogleCRS84QuadTileStoreGeneratorImpl());
	}
}