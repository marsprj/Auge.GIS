#include "ProcessorFactoryImpl.h"
#include "FileSytemTileStoreGeneratorImpl.h"
#include "MongoTileStoreGenerator.h"
#include "FeatureImportProcessorImpl.h"
#include "FeatureExportProcessorImpl.h"
#include "CsvImportProcessorImpl.h"
#include "XYImportProcessorImpl.h"

#include "KmeanProcessorImpl.h"
#include "DBScanProcessorImpl.h"
#include "FeatureProjectProcessorImpl.h"
#include "RandomPointsGeneratorImpl.h"
#include "RandomPointsInPolygonGeneratorImpl.h"

#include "RasterExtractByRectangleProcessorImpl.h"
#include "RasterStretchProcessorImpl.h"
#include "RasterEdgeDetectProcessorImpl.h"
#include "RasterGraylizeProcessorImpl.h"
#include "RasterReverseProcessorImpl.h"
#include "RasterSmoothProcessorImpl.h"
#include "RasterSubtractProcessorImpl.h"
#include "RasterPixelBlendProcessorImpl.h"

#include "RasterThumbnailProcessorImpl.h"
#include "RasterThresholdProcessorImpl.h"

#include "RasterHistogramEqualizationProcessorImpl.h"
#include "RasterFormatConvertToJPEGProcessorImpl.h"

#include "RasterSepiaToneEffectProcessorImpl.h"
#include "RasterPenEffectProcessorImpl.h"

#include "DemSlopeProcessorImpl.h"
#include "DemAspectProcessorImpl.h"
#include "DemHillshadeProcessorImpl.h"

#include "MultiPointToPointsProcessorImpl.h"
#include "LineToPointsProcessorImpl.h"
#include "PolygonToPointsProcessorImpl.h"
#include "PolygonToLineProcessorImpl.h"

#include "GridPointGeneratorImpl.h"

#include "FeatureIDWProcessorImpl.h"
#include "DelaunayProcessorImpl.h"

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

	MultiPointToPointsProcessor* GProcessorFactoryImpl::CreateMultiPointToPointsProcessor()
	{
		return (new MultiPointToPointsProcessorImpl());
	}

	LineToPointsProcessor* GProcessorFactoryImpl::CreateLineToPointsProcessor()
	{
		return (new LineToPointsProcessorImpl());
	}

	PolygonToPointsProcessor* GProcessorFactoryImpl::CreatePolygonToPointsProcessor()
	{
		return (new PolygonToPointsProcessorImpl());
	}

	PolygonToLineProcessor*	GProcessorFactoryImpl::CreatePolygonToLineProcessor()
	{
		return (new PolygonToLineProcessorImpl());
	}

	FeatureImportProcessor* GProcessorFactoryImpl::CreateFeatureImportProcessor()
	{
		return (new FeatureImportProcessorImpl());
	}

	FeatureExportProcessor* GProcessorFactoryImpl::CreateFeatureExportProcessor()
	{
		return (new FeatureExportProcessorImpl());
	}

	CsvImportProcessor*	GProcessorFactoryImpl::CreateCsvImportProcessor()
	{
		return (new CsvImportProcessorImpl());
	}

	XYImportProcessor* GProcessorFactoryImpl::CreateXYImportProcessor()
	{
		return (new XYImportProcessorImpl());
	}

	KMeansProcessor* GProcessorFactoryImpl::CreateKMeansProcessor()
	{
		return (new KMeansProcessorImpl());
	}

	DBScanProcessor* GProcessorFactoryImpl::CreateDBScanProcessor()
	{
		return (new DBScanProcessorImpl());
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

	RasterGraylizeProcessor* GProcessorFactoryImpl::CreateRasterGraylizeProcessor()
	{
		return (new RasterGraylizeProcessorImpl());
	}

	RasterReverseProcessor*	GProcessorFactoryImpl::CreateRasterReverseProcessor()
	{
		return (new RasterReverseProcessorImpl());
	}

	RasterSmoothProcessor* GProcessorFactoryImpl::CreateRasterSmoothProcessor()
	{
		return (new RasterSmoothProcessorImpl());
	}

	RasterSubtractProcessor* GProcessorFactoryImpl::CreateRasterSubtractProcessor()
	{
		return (new RasterSubtractProcessorImpl());
	}

	RasterPixelBlendProcessor* GProcessorFactoryImpl::CreateRasterPixelBlendProcessor()
	{
		return (new RasterPixelBlendProcessorImpl());
	}

	RasterHistogramEqualizationProcessor* GProcessorFactoryImpl::CreateRasterHistogramEqualizationProcessor()
	{
		return (new RasterHistogramEqualizationProcessorImpl());
	}

	RasterFormatConvertToJPEGProcessor*	GProcessorFactoryImpl::CreateRasterFormatConvertToJPEGProcessor()
	{
		return (new RasterFormatConvertToJPEGProcessorImpl());
	}

	RasterSepiaToneEffectProcessor*	GProcessorFactoryImpl::CreateRasterSepiaToneEffectProcessor()
	{
		return new RasterSepiaToneEffectProcessorImpl();
	}

	RasterPenEffectProcessor* GProcessorFactoryImpl::CreateRasterPenEffectProcessor()
	{
		return (new RasterPenEffectProcessorImpl());
	}

	RasterThresholdProcessor* GProcessorFactoryImpl::CreateRasterThresholdProcessor()
	{
		return (new RasterThresholdProcessorImpl());
	}

	DemSlopeProcessor* GProcessorFactoryImpl::CreateDemSlopeProcessor()
	{
		return (new DemSlopeProcessorImpl());
	}

	DemAspectProcessor*	GProcessorFactoryImpl::CreateDemAspectProcessor()
	{
		return (new DemAspectProcessorImpl());
	}

	DemHillshadeProcessor* GProcessorFactoryImpl::CreateDemHillshadeProcessor()
	{
		return (new DemHillshadeProcessorImpl());
	}

	RandomPointsGenerator* GProcessorFactoryImpl::CreateRandomPointsGenerator()
	{
		return (new RandomPointsGeneratorImpl());
	}

	RandomPointsInPolygonGenerator* GProcessorFactoryImpl::CreateRandomPointsInPolygonGenerator()
	{
		return (new RandomPointsInPolygonGeneratorImpl());
	}

	GridPointGenerator*	GProcessorFactoryImpl::CreateGridPointGenerator()
	{
		return (new GridPointGeneratorImpl());
	}

	FeatureIDWProcessor* GProcessorFactoryImpl::CreateFeatureIDWProcessor()
	{
		return (new FeatureIDWProcessorImpl());
	}

	DelaunayProcessor* GProcessorFactoryImpl::CreateDelaunayProcessor()
	{
		return (new DelaunayProcessorImpl());
	}

	RasterThumbnailProcessor* GProcessorFactoryImpl::CreateRasterThumbnailProcessor()
	{
		return (new RasterThumbnailProcessorImpl());
	}
}
