#include "ProcessorFactoryImpl.h"
#include "FileSytemTileStoreGeneratorImpl.h"
#include "FeatureImportProcessorImpl.h"

#ifndef WIN32
#include "ArchiveProcessorImpl.h"
#endif

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
		return (new FileSystemileStoreGeneratorImpl());
	}

	ArchiveProcessor* GProcessorFactoryImpl::CreateArchiveProcessor()
	{
		ArchiveProcessor* procesor = NULL;
#ifdef WIN32
#else
		procesor = new ArchiveProcessorImpl();
#endif
		return procesor;
	}

	FeatureImportProcessor* GProcessorFactoryImpl::CreateFeatureImportProcessor()
	{
		return (new FeatureImportProcessorImpl());
	}
}
