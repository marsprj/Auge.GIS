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
		virtual MapTileStoreGenerator*		CreateTileStoreGenerator();
		virtual MapTileStoreGenerator*		CreateMongoTileStoreGenerator();
		virtual ArchiveProcessor*			CreateArchiveProcessor();
		virtual FeatureImportProcessor*		CreateFeatureImportProcessor();
		virtual FeatureExportProcessor*		CreateFeatureExportProcessor();
		virtual RandomPointsGenerator*		CreateRandomPointsGenerator();
		virtual RandomPointsInPolygonGenerator* CreateRandomPointsInPolygonGenerator();

		virtual CsvImportProcessor*			CreateCsvImportProcessor();
		virtual XYImportProcessor*			CreateXYImportProcessor();

		// Geometry
		virtual MultiPointToPointsProcessor* CreateMultiPointToPointsProcessor();
		virtual LineToPointsProcessor*		CreateLineToPointsProcessor();
		virtual PolygonToPointsProcessor*	CreatePolygonToPointsProcessor();
		virtual PolygonToLineProcessor*		CreatePolygonToLineProcessor();

		// Cluster 
		virtual KMeansProcessor*			CreateKMeansProcessor();
		virtual DBScanProcessor*			CreateDBScanProcessor();

		// Grid
		virtual GridPointGenerator*			CreateGridPointGenerator();

		// projection
		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor();

		// Interplate
		virtual FeatureIDWProcessor*		CreateFeatureIDWProcessor();
		virtual DelaunayProcessor*			CreateDelaunayProcessor();

		// Raster
		virtual RasterProjectProcessor*				CreateRasterProjectProcessor();
		virtual RasterGraylizeProcessor*			CreateRasterGraylizeProcessor();
		virtual RasterReverseProcessor*				CreateRasterReverseProcessor();
		virtual RasterThumbnailProcessor*			CreateRasterThumbnailProcessor();
		virtual RasterResampleProcessor*			CreateRasterResampleProcessor();

		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor();

		// ͼ����ǿ
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor();
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor();		
		virtual RasterSmoothProcessor*				CreateRasterSmoothProcessor();
		virtual RasterSubtractProcessor*			CreateRasterSubtractProcessor();
		virtual RasterHistogramEqualizationProcessor* CreateRasterHistogramEqualizationProcessor();

		// ͼ���ں�
		virtual RasterPixelBlendProcessor*			CreateRasterPixelBlendProcessor();
		// ͼ��ָ�
		virtual RasterThresholdProcessor*			CreateRasterThresholdProcessor();
		virtual RasterMosiacProcessor*				CreateRasterMosiacProcessor();

		virtual RasterFormatConvertToJPEGProcessor*	CreateRasterFormatConvertToJPEGProcessor();
		// ͼ����Ч
		virtual RasterSepiaToneEffectProcessor*		CreateRasterSepiaToneEffectProcessor();
		virtual RasterPenEffectProcessor*			CreateRasterPenEffectProcessor();

		// DEM
		virtual DemSlopeProcessor*					CreateDemSlopeProcessor();
		virtual DemAspectProcessor*					CreateDemAspectProcessor();
		virtual DemHillshadeProcessor*				CreateDemHillshadeProcessor();
		virtual DemInundationProcessor*				CreateDemInundationProcessor();
		
	};
}



#endif //__AUGE_PROCESSPOR_FACTORY_IMPL_H__
