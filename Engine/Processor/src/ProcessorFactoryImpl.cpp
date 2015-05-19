#include "ProcessorFactoryImpl.h"
#include "FileSytemTileStoreGeneratorImpl.h"

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
#ifdef WIN32
		return NULL;
#else
		return (new ArchiveProcessorImpl());
#endif
	}
}