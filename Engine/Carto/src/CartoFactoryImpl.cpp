#include "CartoFactoryImpl.h"
#include "MapImpl.h"
#include "FeatureLayerImpl.h"

#include "CanvasImpl.h"

namespace auge
{
	CartoFactory*	augeGetCartoFactoryInstance()
	{
		static CartoFactoryImpl g_cartoFactory;
		return &g_cartoFactory;
	}

	CartoFactoryImpl::CartoFactoryImpl()
	{

	}

	CartoFactoryImpl::~CartoFactoryImpl()
	{

	}

	Map* CartoFactoryImpl::CreateMap()
	{
		return (new MapImpl());
	}

	FeatureLayer* CartoFactoryImpl::CreateFeatureLayer()
	{
		return (new FeatureLayerImpl());
	}

	Canvas*	CartoFactoryImpl::CreateCanvas2D(g_uint width, g_uint height)
	{
		return (new CanvasImpl(width,height));
	}
}