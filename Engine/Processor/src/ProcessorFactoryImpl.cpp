#include "ProcessorFactoryImpl.h"
#include "FileSytemTileStoreGeneratorImpl.h"
#include "MongoTileStoreGenerator.h"
#include "FeatureImportProcessorImpl.h"
#include "CsvImportProcessorImpl.h"

#include "KmeanProcessorImpl.h"
#include "FeatureProjectProcessorImpl.h"

#include "RasterExtractByRectangleProcessorImpl.h"
#include "RasterStretchProcessorImpl.h"
#include "RasterEdgeDetectProcessorImpl.h"
#include "RasterGreylizeProcessorImpl.h"

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

	MapTileStoreGenerator* GProcessorFactoryImpl::CreateTileStoreGenerator()
	{
		return (new FileSystemTileStoreGeneratorImpl());
	}

	MapTileStoreGenerator* GProcessorFactoryImpl::CreateMongoTileStoreGenerator()
	{
		return (new MongoTileStoreGenerator());
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

	CsvImportProcessor*	GProcessorFactoryImpl::CreateCsvImportProcessor()
	{
		return (new CsvImportProcessorImpl());
	}

	KMeanProcessor*	GProcessorFactoryImpl::CreateKMeanProcessor()
	{
		return (new KMeanProcessorImpl());
	}

	FeatureProjectProcessor* GProcessorFactoryImpl::CreateFeatureProjectProcessor()
	{
		return (new FeatureProjectProcessorImpl());
	}

	RasterExtractByRectangleProcessor* GProcessorFactoryImpl::CreateRasterExtractByRectangleProcessor()
	{
		return (new RasterExtractByRectangleProcessorImpl());
	}

	RasterStretchProcessor*	GProcessorFactoryImpl::CreateRasterStretchProcessor()
	{
		return (new RasterStretchProcessorImpl());
	}

	RasterEdgeDetectProcessor* GProcessorFactoryImpl::CreateRasterEdgeDetectProcessor()
	{
		return (new RasterEdgeDetectProcessorImpl());
	}

	RasterGreylizeProcessor* GProcessorFactoryImpl::CreateRasterGreylizeProcessor()
	{
		return (new RasterGreylizeProcessorImpl());
	}
}
