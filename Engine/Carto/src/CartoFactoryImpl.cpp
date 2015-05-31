#include "CartoFactoryImpl.h"
#include "MapImpl.h"
#include "FeatureLayerImpl.h"
#include "RasterLayerImpl.h"
#include "QuadServerLayerImpl.h"

#include "CanvasImpl.h"
#include "RendererCairo.h"

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

	RasterLayer* CartoFactoryImpl::CreateRasterLayer()
	{
		return (new RasterLayerImpl());
	}

	QuadServerLayer* CartoFactoryImpl::CreateQuadServerLayer()
	{
		return (new QuadServerLayerImpl());
	}

	Canvas*	CartoFactoryImpl::CreateCanvas2D(g_uint width, g_uint height)
	{
		return (new CanvasImpl(width,height));
	}

	Renderer* CartoFactoryImpl::CreateRenderer2D(g_uint width, g_uint height)
	{
		return (new RendererCairo(width, height));
	}
}